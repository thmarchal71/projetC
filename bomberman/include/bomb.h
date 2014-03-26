#ifndef BOMB_H_
#define BOMB_H_

#include <player.h>
#include <map.h>
#include <constant.h>

struct bomb;

int bomb_get_x(struct bomb* bomb);

int bomb_get_y(struct bomb* bomb);

int bomb_get_timer(struct bomb* bomb);

void bomb_set_current_state(struct bomb* bomb, enum state state);

struct bomb* bomb_find(struct map* map, int x, int y);

struct bomb* bomb_init(struct player* player);

void bomb_install(struct player* player, struct map* map);

enum state bomb_get_current_state(struct bomb* bomb);

void bomb_display(struct map* map, struct player* player);

void bomb_flame_display(struct map* map, struct player* player,struct bomb* bomb);

void bomb_destruct(struct map* map, struct player* player,struct bomb* bomb);

#endif /* BOMB_H_ */
