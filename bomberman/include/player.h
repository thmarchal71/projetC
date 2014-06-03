#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <game.h>
#include <level.h>
#include <constant.h>

struct player;

// Creates a new player with given numbers of bombs, lifes, range
struct player* player_init(int bomb_number, int life_number, int range_number);

// Free player struct
void player_free(struct player* player);

// Return life state
int player_get_dead(struct player* player);

// Set player death
void player_set_dead(struct player* player);

// Return, Set 1=key, 0=no key
int player_get_key(struct player* player);
void player_set_key(struct player* player,int k);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Return, Decrease, Set the invincible timer of the player
int player_get_invincibleTimer(struct player* player);
void player_dec_invincibleTimer(struct player* player);
void player_set_invincible(struct player* player, int n);

// Set the direction of the next move of the player
void player_set_current_way(struct player* player, way_t way);

// Set, Increase, Decrease the number of bomb that player can put
int player_get_nb_bomb(struct player* player);
void player_inc_nb_bomb(struct player* player);
void player_dec_nb_bomb(struct player* player);
void player_set_nb_bomb(struct player* player,int b);

// Set, Increase, Decrease the number of life that player has
int player_get_nb_life(struct player* player);
void player_inc_nb_life(struct player* player);
void player_dec_nb_life(struct player* player);
void player_set_nb_life(struct player* player, int l);

// Set, Increase, Decrease the range that player has
int player_get_range(struct player* player);
void player_inc_range(struct player* player);
void player_dec_range(struct player* player);
void player_set_range(struct player* player, int r);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
int player_move(struct game* game);

// Display the player on the screen
void player_display(struct player* player);

#endif /* PLAYER_H_ */
