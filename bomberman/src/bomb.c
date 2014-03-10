#include <SDL/SDL_image.h>
#include <assert.h>

#include <map.h>
#include <bomb.h>
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
};

struct bomb* bomb_init(struct player* player) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->current_state = STATE1;
	bomb->range = player_get_range(player);
	bomb->timer_bomb = SDL_GetTicks();

	return bomb;
}

void bomb_free(struct bomb* bomb) {
	assert(bomb);
	free(bomb);
}

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

void bomb_set_timer(struct bomb* bomb, int timer) {
	assert(bomb);
	bomb->timer_bomb = timer;
}

void bomb_set_range(struct bomb* bomb, int range) {
	assert(bomb);
	bomb->range = range;
}



void plant_bomb(struct player* player, struct map* map) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
			error("Memory error");
	bomb = bomb_init(player);
	map_set_bomb(map,bomb);
	map_set_cell_type(map, bomb_get_x(bomb), bomb_get_y(bomb), CELL_BOMB);
}

void display_bomb(struct bomb* bomb, struct map* map) {
	if ( SDL_GetTicks() - bomb_get_timer(bomb) < 1000 ) {
		bomb_set_current_state(bomb, STATE1);
		window_display_image(sprite_get_bomb(bomb->current_state),
					bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
	} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 2000 ) {
		bomb_set_current_state(bomb, STATE2);
		window_display_image(sprite_get_bomb(bomb->current_state),
					bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
	} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 3000 ) {
		bomb_set_current_state(bomb, STATE3);
		window_display_image(sprite_get_bomb(bomb->current_state),
					bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
	} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 4000 ) {
		bomb_set_current_state(bomb, STATE4);
		window_display_image(sprite_get_bomb(bomb->current_state),
					bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);

	} else if ( SDL_GetTicks() - bomb_get_timer(bomb) < 5000 ) {
		map_set_cell_type(map, bomb_get_x(bomb), bomb_get_y(bomb), CELL_EMPTY);

	}
}
