#include <assert.h>
#include <time.h>
#include <SDL/SDL_mixer.h>

#include <list.h>
#include <bomb.h>
#include <monster.h>
#include <game.h>
#include <misc.h>
#include <map.h>
#include <level.h>
#include <window.h>
#include <sprite.h>

struct game {
	struct level* curr_level; // current level
	struct player* player;
	int win;

};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->curr_level = level_get_level(0); // get maps of the first level

	game->player = player_init(1,2,1);
	player_from_map(game->player, level_get_map(game->curr_level, 0)); // get x,y of the player on the first map

	game->win=0;

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	level_free(game->curr_level);
}

int game_get_win(struct game* game) {
	assert(game);
	return game->win;
}

void game_set_win(struct game* game) {
	game->win=1;
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_change_level(struct game* game, int i){
	level_free(game->curr_level);
	game->curr_level = level_get_level(i);
	player_from_map(game->player, level_get_map(game->curr_level, 0));
}

void game_load_level(struct game* game, int lvl_nb, int map_nb){
	level_free(game->curr_level);
	game->curr_level = level_get_level(lvl_nb);
	level_set_cur_map(game_get_curr_level(game), map_nb);
	player_from_map(game->player, level_get_map(game->curr_level, map_nb));
}

struct level* game_get_curr_level(struct game* game) {
	return game->curr_level;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = level_get_curr_map(game_get_curr_level(game));
	struct player* player = game_get_player(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), 0, y);

	x = SIZE_BLOC;
	window_display_image(
				sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x =3* SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x =4 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_range(game_get_player(game))), x, y);

	if(player_get_key(player)==1){
		x = 9 * SIZE_BLOC-20;
		window_display_image(sprite_get_key(), x, y);
	}

	x = 10 * SIZE_BLOC;
	window_display_image(sprite_get_level(level_get_lvl_nb(game_get_curr_level(game))), x, y);

}

void game_display(struct game* game,Mix_Chunk *explosion) {
	assert(game);

	window_clear();

	window_display_image(sprite_get_ground(),0,0);
	game_banner_display(game);
	level_display(game_get_curr_level(game));
	player_display(game->player);
	bomb_display(level_get_curr_map(game_get_curr_level(game)),game->player,explosion);
	monster_display(level_get_curr_map(game_get_curr_level(game)),game->player);

	window_refresh();
}

void game_pause_display(struct game* game){
	int delay;
	int timer;
	int pause=0;
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));
	pause = 1;
	timer=SDL_GetTicks();
	while(pause == 1){

		window_display_image(sprite_get_pause(),((map_get_width(map) / 2 )-1)* SIZE_BLOC,(( map_get_height(map) / 2) -1)* SIZE_BLOC);
		window_refresh();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: // keyboard : esc
				game->win=1;
				pause=0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					game->win=1;
					pause=0;
					break;
				case SDLK_s:
					game_save(game);
					break;
				case SDLK_p:
					Mix_Resume(-1); // Resume sound
					Mix_ResumeMusic(); // Resume music
					pause=0; // Exit the loop
					break;
				default :
					break;
				}
				break;
			}
		}
	}
	delay=SDL_GetTicks()-timer;
	bomb_delay_timer(map,player, delay); // Delay the timer with the time of the pause
	monster_delay_timer(map,player);
}

short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: // keyboard : esc
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_p:
				Mix_Pause(-1); // Pause sound
				Mix_PauseMusic(); // Pause music
				game_pause_display(game);
				break;
			case SDLK_r:
				player_set_dead(player); // Restart the game
				break;
			case SDLK_s:
				game_save(game); // Save instantly the game
				break;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(game);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(game);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(game);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(game);
				break;
			case SDLK_SPACE:
				if (player_get_nb_bomb(player) > 0 && map_get_cell_type(map, player_get_x(player), player_get_y(player)) != CELL_BOMB) {
					bomb_install(player,map);
					player_dec_nb_bomb(player);
				}
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}


int game_update(struct game* game) {
	if (input_keyboard(game) == 1)
		return 1; // exit game
	return 0;
}

void game_save(struct game* game) {
	struct player* player = game_get_player(game);
	FILE* f = NULL;
	f=fopen("save/p_data.txt","w"); // Open a file in write mode
	int k,b,l,r,n,m;
	k=player_get_key(player);
	b=player_get_nb_bomb(player);
	l=player_get_nb_life(player);
	r=player_get_range(player);
	n=level_get_lvl_nb(game_get_curr_level(game));
	m=level_get_map_nb(game_get_curr_level(game)); // Get all the player characteristics
	fprintf(f,"%d %d %d %d %d %d", l, b, r, k, n, m); // Write them
	fclose(f);
	if ( level_get_lvl_nb(game_get_curr_level(game)) == 0 ){
		f=fopen("save/s_map_1_1.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if( player_get_x(player)==j && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==0)
					fprintf(f, "%u ", CELL_PLAYER);
				else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, i)==CELL_BOMB)
					fprintf(f, "%u ", CELL_EMPTY);
				else
					fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, i));
			}
			if( player_get_x(player)==11 && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==0)
				fprintf(f, "%u\n", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, i)==CELL_BOMB)
				fprintf(f, "%u\n", CELL_EMPTY);
			else
				fprintf(f, "%u\n", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, i));
		}
		for(int j = 0; j < 11; j++){
			if( player_get_x(player)==j && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==0)
				fprintf(f, "%u ", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, 11)==CELL_BOMB)
				fprintf(f, "%u ", CELL_EMPTY);
			else
				fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, 11));
		}
		if( player_get_x(player)==11 && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==0)
			fprintf(f, "%u", CELL_PLAYER);
		else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, 11)==CELL_BOMB)
			fprintf(f, "%u", CELL_EMPTY);
		else
			fprintf(f, "%u", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, 11));
		fclose(f);
		f=fopen("save/s_map_1_2.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if( player_get_x(player)==j && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==1)
					fprintf(f, "%u ", CELL_PLAYER);
				else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, i)==CELL_BOMB)
					fprintf(f, "%u ", CELL_EMPTY);
				else
					fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, i));
			}
			if( player_get_x(player)==11 && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==1)
				fprintf(f, "%u\n", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, i)==CELL_BOMB)
				fprintf(f, "%u\n", CELL_EMPTY);
			else
				fprintf(f, "%u\n", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, i));
		}
		for(int j = 0; j < 11; j++){
			if( player_get_x(player)==j && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==1)
				fprintf(f, "%u ", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, 11)==CELL_BOMB)
				fprintf(f, "%u ", CELL_EMPTY);
			else
				fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, 11));
		}
		if( player_get_x(player)==11 && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==1)
			fprintf(f, "%u", CELL_PLAYER);
		else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, 11)==CELL_BOMB)
			fprintf(f, "%u", CELL_EMPTY);
		else
			fprintf(f, "%u", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, 11));
		fclose(f);
		f=fopen("save/s_map_1_3.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if( player_get_x(player)==j && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==2)
					fprintf(f, "%u ", CELL_PLAYER);
				else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 2), j, i)==CELL_BOMB)
					fprintf(f, "%u ", CELL_EMPTY);
				else
					fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 2), j, i));
			}
			if( player_get_x(player)==11 && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==2)
				fprintf(f, "%u\n", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 2), 11, i)==CELL_BOMB)
				fprintf(f, "%u\n", CELL_EMPTY);
			else
				fprintf(f, "%u\n", map_get_true_cell(level_get_map(game_get_curr_level(game), 2), 11, i));
		}
		for(int j = 0; j < 11; j++){
			if( player_get_x(player)==j && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==2)
				fprintf(f, "%u ", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 2), j, 11)==CELL_BOMB)
				fprintf(f, "%u ", CELL_EMPTY);
			else
				fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 2), j, 11));
		}
		if( player_get_x(player)==11 && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==2)
			fprintf(f, "%u", CELL_PLAYER);
		else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 2), 11, 11)==CELL_BOMB)
			fprintf(f, "%u", CELL_EMPTY);
		else
			fprintf(f, "%u", map_get_true_cell(level_get_map(game_get_curr_level(game), 2), 11, 11));
		fclose(f);
		struct map* map=map_load_from_file(MAP_2_1);
		f=fopen("save/s_map_2_1.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fprintf(f, "%u ", map_get_true_cell(map, j, i));
			}
			fprintf(f, "%u\n", map_get_true_cell(map, 11, i));
		}
		for(int j = 0; j < 11; j++){
			fprintf(f, "%u ", map_get_true_cell(map, j, 11));
		}
		fprintf(f, "%u", map_get_true_cell(map, 11, 11));
		fclose(f);
		map=map_load_from_file(MAP_2_2);
		f=fopen("save/s_map_2_2.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fprintf(f, "%u ", map_get_true_cell(map, j, i));
			}
			fprintf(f, "%u\n", map_get_true_cell(map, 11, i));
		}
		for(int j = 0; j < 11; j++){
			fprintf(f, "%u ", map_get_true_cell(map, j, 11));
		}
		fprintf(f, "%u", map_get_true_cell(map, 11, 11));
		fclose(f);
	}
	if ( level_get_lvl_nb(game_get_curr_level(game)) == 1 ){
		f=fopen("save/s_map_2_1.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if( player_get_x(player)==j && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==0)
					fprintf(f, "%u ", CELL_PLAYER);
				else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, i)==CELL_BOMB)
					fprintf(f, "%u ", CELL_EMPTY);
				else
					fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, i));
			}
			if( player_get_x(player)==11 && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==0)
				fprintf(f, "%u\n", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, i)==CELL_BOMB)
				fprintf(f, "%u\n", CELL_EMPTY);
			else
				fprintf(f, "%u\n", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, i));
		}
		for(int j = 0; j < 11; j++){
			if( player_get_x(player)==j && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==0)
				fprintf(f, "%u ", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, 11)==CELL_BOMB)
				fprintf(f, "%u ", CELL_EMPTY);
			else
				fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), j, 11));
		}
		if( player_get_x(player)==11 && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==0)
			fprintf(f, "%u", CELL_PLAYER);
		else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, 11)==CELL_BOMB)
			fprintf(f, "%u", CELL_EMPTY);
		else
			fprintf(f, "%u", map_get_true_cell(level_get_map(game_get_curr_level(game), 0), 11, 11));
		fclose(f);
		f=fopen("save/s_map_2_2.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				if( player_get_x(player)==j && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==1)
					fprintf(f, "%u ", CELL_PLAYER);
				else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, i)==CELL_BOMB)
					fprintf(f, "%u ", CELL_EMPTY);
				else
					fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, i));
			}
			if( player_get_x(player)==11 && player_get_y(player)==i && level_get_map_nb(game_get_curr_level(game))==1)
				fprintf(f, "%u\n", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, i)==CELL_BOMB)
				fprintf(f, "%u\n", CELL_EMPTY);
			else
				fprintf(f, "%u\n", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, i));
		}
		for(int j = 0; j < 11; j++){
			if( player_get_x(player)==j && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==1)
				fprintf(f, "%u ", CELL_PLAYER);
			else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, 11)==CELL_BOMB)
				fprintf(f, "%u ", CELL_EMPTY);
			else
				fprintf(f, "%u ", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), j, 11));
		}
		if( player_get_x(player)==11 && player_get_y(player)==11 && level_get_map_nb(game_get_curr_level(game))==1)
			fprintf(f, "%u", CELL_PLAYER);
		else if (map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, 11)==CELL_BOMB)
			fprintf(f, "%u", CELL_EMPTY);
		else
			fprintf(f, "%u", map_get_true_cell(level_get_map(game_get_curr_level(game), 1), 11, 11));
		fclose(f);
		struct map* map=map_load_from_file(MAP_1_1);
		f=fopen("save/s_map_1_1.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fprintf(f, "%u ", map_get_true_cell(map, j, i));
			}
			fprintf(f, "%u\n", map_get_true_cell(map, 11, i));
		}
		for(int j = 0; j < 11; j++){
			fprintf(f, "%u ", map_get_true_cell(map, j, 11));
		}
		fprintf(f, "%u", map_get_true_cell(map, 11, 11));
		fclose(f);
		map=map_load_from_file(MAP_1_2);
		f=fopen("save/s_map_1_2.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fprintf(f, "%u ", map_get_true_cell(map, j, i));
			}
			fprintf(f, "%u\n", map_get_true_cell(map, 11, i));
		}
		for(int j = 0; j < 11; j++){
			fprintf(f, "%u ", map_get_true_cell(map, j, 11));
		}
		fprintf(f, "%u", map_get_true_cell(map, 11, 11));
		fclose(f);
		map=map_load_from_file(MAP_1_3);
		f=fopen("save/s_map_1_3.lvl","w");
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {
				fprintf(f, "%u ", map_get_true_cell(map, j, i));
			}
			fprintf(f, "%u\n", map_get_true_cell(map, 11, i));
		}
		for(int j = 0; j < 11; j++){
			fprintf(f, "%u ", map_get_true_cell(map, j, 11));
		}
		fprintf(f, "%u", map_get_true_cell(map, 11, 11));
		fclose(f);
	}
}
