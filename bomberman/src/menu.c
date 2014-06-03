#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <map.h>
#include <list.h>
#include <monster.h>
#include <level.h>
#include <window.h>
#include <sprite.h>
#include <menu.h>


void menu_load_game(struct game* game){
	FILE *file = fopen("save/p_data.txt", "r");
	int nb_life,nb_bomb,range,key,lvl_nb,map_nb;
	fscanf(file, "%d %d %d %d %d %d", &nb_life, &nb_bomb, &range, &key, &lvl_nb, &map_nb);
	fclose(file);
	player_set_nb_life(game_get_player(game),nb_life);
	player_set_nb_bomb(game_get_player(game),nb_bomb);
	player_set_range(game_get_player(game),range);
	player_set_key(game_get_player(game),key);
	game_load_level(game, lvl_nb, map_nb);
	int type=0;
	int height=12;
	int width=12;
	if ( lvl_nb==0 ){
		file = fopen("save/s_map_1_1.lvl", "r");
		for (int i = 0; i < height-1; i++) {
			for (int j = 0; j < width-1; j++) {
				fscanf(file, "%d ", &type);
				map_set_cell_type(level_get_map(game_get_curr_level(game), 0), j, i, type);
			}
			fscanf(file, "%d\n", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 0), width-1, i, type);
		}

		for(int j = 0; j < width-1; j++){
			fscanf(file, "%d ", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 0), j, height-1, type);
		}
		fscanf(file, "%d", &type);
		map_set_cell_type(level_get_map(game_get_curr_level(game), 0), width-1, height-1, type);
		fclose(file);

		map_free_monster(level_get_map(game_get_curr_level(game), 0));
		monster_from_map(level_get_map(game_get_curr_level(game), 0));

		file = fopen("save/s_map_1_2.lvl", "r");
		for (int i = 0; i < height-1; i++) {
			for (int j = 0; j < width-1; j++) {
				fscanf(file, "%d ", &type);
				map_set_cell_type(level_get_map(game_get_curr_level(game), 1), j, i, type);
			}
			fscanf(file, "%d\n", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 1), width-1, i, type);
		}

		for(int j = 0; j < width-1; j++){
			fscanf(file, "%d ", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 1), j, height-1, type);
		}
		fscanf(file, "%d", &type);
		map_set_cell_type(level_get_map(game_get_curr_level(game), 1), width-1, height-1, type);
		fclose(file);

		map_free_monster(level_get_map(game_get_curr_level(game), 1));
		monster_from_map(level_get_map(game_get_curr_level(game), 1));

		file = fopen("save/s_map_1_3.lvl", "r");
		for (int i = 0; i < height-1; i++) {
			for (int j = 0; j < width-1; j++) {
				fscanf(file, "%d ", &type);
				map_set_cell_type(level_get_map(game_get_curr_level(game), 2), j, i, type);
			}
			fscanf(file, "%d\n", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 2), width-1, i, type);
		}

		for(int j = 0; j < width-1; j++){
			fscanf(file, "%d ", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 2), j, height-1, type);
		}
		fscanf(file, "%d", &type);
		map_set_cell_type(level_get_map(game_get_curr_level(game), 2), width-1, height-1, type);
		fclose(file);

		map_free_monster(level_get_map(game_get_curr_level(game), 2));
		monster_from_map(level_get_map(game_get_curr_level(game), 2));
	}
	if ( lvl_nb==1 ){
		file = fopen("save/s_map_2_1.lvl", "r");
		for (int i = 0; i < height-1; i++) {
			for (int j = 0; j < width-1; j++) {
				fscanf(file, "%d ", &type);
				map_set_cell_type(level_get_map(game_get_curr_level(game), 0), j, i, type);
			}
			fscanf(file, "%d\n", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 0), width-1, i, type);
		}

		for(int j = 0; j < width-1; j++){
			fscanf(file, "%d ", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 0), j, height-1, type);
		}
		fscanf(file, "%d", &type);
		map_set_cell_type(level_get_map(game_get_curr_level(game), 0), width-1, height-1, type);
		fclose(file);

		map_free_monster(level_get_map(game_get_curr_level(game), 0));
		monster_from_map(level_get_map(game_get_curr_level(game), 0));

		file = fopen("save/s_map_2_2.lvl", "r");
		for (int i = 0; i < height-1; i++) {
			for (int j = 0; j < width-1; j++) {
				fscanf(file, "%d ", &type);
				map_set_cell_type(level_get_map(game_get_curr_level(game), 1), j, i, type);
			}
			fscanf(file, "%d\n", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 1), width-1, i, type);
		}

		for(int j = 0; j < width-1; j++){
			fscanf(file, "%d ", &type);
			map_set_cell_type(level_get_map(game_get_curr_level(game), 1), j, height-1, type);
		}
		fscanf(file, "%d", &type);
		map_set_cell_type(level_get_map(game_get_curr_level(game), 1), width-1, height-1, type);
		fclose(file);

		map_free_monster(level_get_map(game_get_curr_level(game), 1));
		monster_from_map(level_get_map(game_get_curr_level(game), 1));
		}
	player_from_map(game_get_player(game), level_get_map(game_get_curr_level(game), map_nb));

}

int menu_display(struct game* game){
	int i = 0;
	int opt=0;
	SDL_Event event;
	while( i == 0 ){
		window_display_image(sprite_get_menu(opt),0,0);
		window_refresh();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:	// Mouse position
				if(event.motion.x>107 && event.motion.x<392 && event.motion.y>62 && event.motion.y<100)
					opt=0;
				if(event.motion.x>104 && event.motion.x<315 && event.motion.y>132 && event.motion.y<161)
					opt=1;
				break;
			case SDL_MOUSEBUTTONDOWN: // Mouse click
				if(event.button.x>107 && event.button.x<392 && event.button.y>62 && event.button.y<100 && event.button.button==1){
					opt=0;
					i=1;
				}
				if(event.button.x>104 && event.button.x<315 && event.button.y>132 && event.button.y<161 && event.button.button==1){
					opt=1;
					i=1;
				}
				break;
				break;
			case SDL_QUIT:
				return 1;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					return 1;
				case SDLK_UP:
					opt=0;
					break;
				case SDLK_DOWN:
					opt=1;
					break;
				case SDLK_RETURN: // Enter in normal keyboard
					i=1;
					break;
				case SDLK_KP_ENTER: // Enter in pad keyboard
					i=1;
					break;
				default :
					break;
				}
				break;
			}
		}
	}
	if(opt==1)
		menu_load_game(game);
	return 0;
}
