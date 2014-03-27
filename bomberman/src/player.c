#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct player {
	int x, y;
	enum way current_way;
	int nb_bomb;
	int nb_life;
	int range;
	int invincibleTimer;
	int dead;
};

struct player* player_init(int bomb_number, int life_number, int range_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_way = SOUTH;
	player->nb_bomb = bomb_number;
	player->nb_life = life_number;
	player->range = range_number;
	player->invincibleTimer = 0;
	player->dead = 0;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_dead(struct player* player) {
	assert(player != NULL);
	return player->dead;
}

void player_set_dead(struct player* player) {
	assert(player != NULL);
	player->dead = 1;
}

int player_get_invincibleTimer(struct player* player) {
	assert(player != NULL);
	return player->invincibleTimer;
}

void player_dec_invincibleTimer(struct player* player) {
	assert(player);
	player->invincibleTimer -= 1;
}

void player_set_invincible(struct player* player, int n) {
	assert(player);
	player->invincibleTimer = n;
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum way way) {
	assert(player);
	player->current_way = way;
}


int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bomb;
}

int player_get_nb_life(struct player* player) {
	assert(player);
	return player->nb_life;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb += 1;
}

void player_inc_nb_life(struct player* player) {
	assert(player);
	player->nb_life += 1;
}

int player_get_range(struct player* player) {
	assert(player);
	return player->range;
}

void player_inc_range(struct player* player) {
	assert(player);
	player->range += 1;
}

void player_dec_range(struct player* player) {
	assert(player);
	player->range -= 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb -= 1;
}

void player_dec_nb_life(struct player* player) {
	assert(player);
	player->nb_life -= 1;
}

void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
			}
		}
	}
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_CASE:
		if(player->current_way == SOUTH && map_is_inside(map, x, y + 1) && map_get_cell_type(map, x, y + 1)==CELL_EMPTY) {
			map_set_cell_type(map, x, y + 1, map_get_cell(map, x, y));
			map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
		}
		if(player->current_way == NORTH && map_is_inside(map, x, y - 1) && map_get_cell_type(map, x, y - 1)==CELL_EMPTY) {
			map_set_cell_type(map, x, y - 1, map_get_cell(map, x, y));
			map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
		}
		if(player->current_way == EAST && map_is_inside(map, x + 1, y) && map_get_cell_type(map, x + 1, y)==CELL_EMPTY) {
			map_set_cell_type(map, x + 1, y, map_get_cell(map, x, y));
			map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
		}
		if(player->current_way == WEST && map_is_inside(map, x - 1, y) && map_get_cell_type(map, x - 1, y)==CELL_EMPTY) {
			map_set_cell_type(map, x - 1, y, map_get_cell(map, x, y));
			map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
		}
		return 0;
		break;

	case CELL_BONUS:
		switch (map_get_cell_bonus_type(map, x,  y)) {
			case BONUS_BOMB_NB_INC:
				player_inc_nb_bomb(player);
				return 1;
				break;
			case BONUS_BOMB_NB_DEC:
				if (player_get_nb_bomb(player) > 0)
					player_dec_nb_bomb(player);
				return 1;
				break;
			case BONUS_BOMB_RANGE_INC:
				player_inc_range(player);
				return 1;
				break;
			case BONUS_BOMB_RANGE_DEC:
				if (player_get_range(player) > 1)
					player_dec_range(player);
				return 1;
				break;
			case BONUS_LIFE:
				player_inc_nb_life(player);
				return 1;
				break;
			default:
				break;
		}
		return 0;
		break;

	case CELL_GOAL:
		break;

	case CELL_MONSTER:
		if (player->invincibleTimer == 0) {
			if (player_get_nb_life(player) == 0 ) {
				player->dead = 1;
				return 0;
			}
			player_dec_nb_life(player);
			player->invincibleTimer = 60;
		}
		return 0;
		break;

	case CELL_PLAYER:
		break;

	case CELL_BOMB:
		return 0;
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_way) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		if ( map_get_cell_type(map, x, y) != CELL_BOMB )
			map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	if (player_get_invincibleTimer(player)>0){
		window_display_image(sprite_get_inv_player(player->current_way),
				player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}
	if (player_get_invincibleTimer(player)==0){
		window_display_image(sprite_get_player(player->current_way),
				player->x * SIZE_BLOC, player->y * SIZE_BLOC);
	}
	if (player_get_invincibleTimer(player) > 0)
			player_dec_invincibleTimer(player);
}

