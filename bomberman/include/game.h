#ifndef GAME_H_
#define GAME_H_

#include <level.h>
#include <map.h>
#include <SDL/SDL_mixer.h>

struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return, Set the end of the game
int game_get_win(struct game* game);
void game_set_win(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current level
struct level* game_get_curr_level(struct game* game);

// Display the game on the screen
void game_display(struct game* game,Mix_Chunk *explosion);

// Display the pause on the screen
void game_pause_display(struct game* game);

// Change of game level
void game_change_level(struct game* game, int i);

// Load the level and the map
void game_load_level(struct game* game, int lvl_nb, int map_nb);

// Update
int game_update(struct game* game);

// Save changes
void game_save(struct game* game);

#endif /* GAME_H_ */
