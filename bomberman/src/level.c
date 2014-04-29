#include <assert.h>
#include <stdlib.h>
#include <stddef.h> // NULL (macos)
#include <level.h>
#include <monster.h>
#include <game.h>
#include <map.h>

struct level {
	struct map** maps; // array of the level's maps
	short nb_maps; // nb maps of the level
	short cur_map; // the current map
};

struct level* level_get_level(int num) {
	struct level* level = malloc(sizeof(*level));
	switch (num) {
	case 0:
		level->nb_maps = 3;
		level->cur_map = 0;
		level->maps = malloc(sizeof(*level->maps));
		level->maps[0] = map_load_from_file(MAP_1_1);
		level->maps[1] = map_load_from_file(MAP_1_2);
		level->maps[2] = map_load_from_file(MAP_1_3);

		return level;
		break;
	case 1:
		level->nb_maps = 1;
		level->cur_map = 0;
		level->maps = malloc(sizeof(*level->maps));
		level->maps[0] = map_load_from_file(MAP_2_1);

		return level;
		break;
	}
	return NULL ;
}

struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}

void level_set_cur_map(struct level* level, int i){
	level->cur_map = i;
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level) {
	for (int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);

	free(level->maps);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map]);
}
