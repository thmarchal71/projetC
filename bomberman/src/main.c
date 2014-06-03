#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <constant.h>
#include <game.h>
#include <player.h>
#include <window.h>
#include <menu.h>

int main(int argc, char *argv[]) {
	struct game* game = game_new(); // Create new game

	window_create(SIZE_BLOC * MAP_WIDTH,
			SIZE_BLOC * MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) // Open music player
		printf("%s", Mix_GetError());

	Mix_AllocateChannels(32); // Number of channels
	Mix_VolumeMusic(MIX_MAX_VOLUME); // Volume of the music
	Mix_Music *musique;
	musique = Mix_LoadMUS("music/MarioTheme.mp3"); // Load music
	Mix_PlayMusic(musique, -1); // Play music with repeat option

	Mix_Chunk *explosion;
	explosion = Mix_LoadWAV("music/bomb.wav"); // Load sound
	Mix_VolumeChunk(explosion, MIX_MAX_VOLUME);
	// game loop
	// fixed time rate implementation
	int done = menu_display(game); // Return 1 if you want to exit during menu
	while (!done) {
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game,explosion);

		if ( player_get_dead(game_get_player(game))) {		// Reset the game if the player is dead
			game_free(game);
			game=game_new();
		}

		if(game_get_win(game) == 1){
			done=1;
		}

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.

	}
	window_free();
	game_free(game);
	Mix_FreeChunk(explosion);
	Mix_FreeMusic(musique);
	Mix_CloseAudio();
	SDL_Quit();

	return EXIT_SUCCESS;
}
