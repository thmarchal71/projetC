#ifndef BOMB_H_
#define BOMB_H_

#include <player.h>
#include <map.h>
#include <constant.h>

struct bomb;

struct bomb* bomb_init(struct player* player);

void bomb_free(struct bomb* bomb);

int bomb_get_x(struct bomb* bomb);

int bomb_get_y(struct bomb* bomb);

int bomb_get_timer(struct bomb* bomb);

void bomb_set_current_state(struct bomb* bomb, enum state state);

void bomb_set_timer(struct bomb* bomb, int timer);

void bomb_set_range(struct bomb* bomb, int range);

void plant_bomb(struct player* player, struct map* map);

void display_bomb(struct bomb* bomb, struct map* map);


#endif /* BOMB_H_ */
