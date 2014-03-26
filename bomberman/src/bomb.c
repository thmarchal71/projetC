#include <SDL/SDL_image.h>
#include <assert.h>

#include <map.h>
#include <bomb.h>
#include <list.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bomb {
	int x, y;
	enum state current_state;
	int range;
	float timer_bomb;
	int explode;
};


int bomb_get_x(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->y;
}

int bomb_get_timer(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->timer_bomb;
}

void bomb_set_current_state(struct bomb* bomb, enum state state) {
	assert(bomb);
	bomb->current_state = state;
}

enum state bomb_get_current_state(struct bomb* bomb) {
	assert(bomb);
	return bomb->current_state;
}

void bomb_set_range(struct bomb* bomb, int range) {
	assert(bomb);
	bomb->range = range;
}

struct bomb* bomb_init(struct player* player) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->current_state = STATE1;
	bomb->range = player_get_range(player);
	bomb->timer_bomb = SDL_GetTicks();
	bomb->explode = 0;

	return bomb;
}

void bomb_install(struct player* player, struct map* map) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
			error("Memory error");
	bomb = bomb_init(player);
	map_insert_bomb(map, bomb_get_x(bomb), bomb_get_y(bomb), bomb);
	map_set_cell_type(map, bomb_get_x(bomb), bomb_get_y(bomb), CELL_BOMB);
}

void display_bomb(struct map* map)
{
	if(map_get_bombs(map) != NULL){
		struct list* bomb_list = map_get_bombs(map);
		struct bomb* bomb = NULL;
		while (bomb_list != NULL){
			bomb = bomb_list->data;
			if ( SDL_GetTicks() - bomb_get_timer(bomb) < 1000 ) {
				bomb_set_current_state(bomb, STATE1);
				window_display_image(sprite_get_bomb(bomb_get_current_state(bomb)),
						bomb_get_x(bomb) * SIZE_BLOC, bomb_get_y(bomb) * SIZE_BLOC);
			} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 2000 ) {
				bomb_set_current_state(bomb, STATE2);
				window_display_image(sprite_get_bomb(bomb_get_current_state(bomb)),
										bomb_get_x(bomb) * SIZE_BLOC, bomb_get_y(bomb) * SIZE_BLOC);
			} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 3000 ) {
				bomb_set_current_state(bomb, STATE3);
				window_display_image(sprite_get_bomb(bomb_get_current_state(bomb)),
										bomb_get_x(bomb) * SIZE_BLOC, bomb_get_y(bomb) * SIZE_BLOC);
			} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 4000 ) {
				bomb_set_current_state(bomb, STATE4);
				window_display_image(sprite_get_bomb(bomb_get_current_state(bomb)),
										bomb_get_x(bomb) * SIZE_BLOC, bomb_get_y(bomb) * SIZE_BLOC);
			} else {
				map_set_cell_type(map, bomb_get_x(bomb), bomb_get_y(bomb), CELL_EMPTY);
				map_set_bombs(map, list_find_delete(map_get_bombs(map),bomb_get_x(bomb), bomb_get_y(bomb)));
				bomb_list = map_get_bombs(map);
			}
			if (map_get_bombs(map) != NULL)
				bomb_list = bomb_list->next;
		}
	}
}
