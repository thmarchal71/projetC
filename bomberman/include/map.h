#ifndef MAP_H_
#define MAP_H_

typedef enum cell_type {
	CELL_EMPTY=0,
	CELL_GOAL,  // 1
	CELL_SCENERY, // 2
	CELL_PLAYER, // 3
	CELL_CASE,   // 4
	CELL_BONUS, // 5
	CELL_MONSTER, // 6
	CELL_BOMB, // 7
	CELL_KEY, // 8
	CELL_DOOR, // 9
} cell_type_t;

enum bonus_type {
	RANGE_INC=1,
	RANGE_DEC, // 1
	NB_INC, // 2
	NB_DEC, // 3
	LIFE, // 4
	MONSTER_INC, // 5
};

enum goal_type {
	FLAG,
	PEACH,
};

enum scenery_type {
	SCENERY_STONE, // 0
	SCENERY_TREE, // 1
};

enum door_type {
	CLOSED_DOOR, // 0
	OPENED_DOOR, // 1
};

enum door_number {
	DOOR_1,
	DOOR_2,
	DOOR_3,
	DOOR_4,
	DOOR_5,
	DOOR_6,
	DOOR_7,
};

enum compose_type {
	CELL_STONE = CELL_SCENERY | (SCENERY_STONE << 4), 				// 0000 0010 -> 2
	CELL_TREE = CELL_SCENERY | (SCENERY_TREE << 4), 				// 0001 0010 -> 18
	CELL_CASE_BOMBINC = CELL_CASE | (NB_INC << 4), 		// 0011 0100 -> 52
    CELL_CASE_BOMBDEC = CELL_CASE | (NB_DEC << 4), 		// 0100 0100 -> 68
    CELL_CASE_RANGEINC = CELL_CASE | (RANGE_INC << 4), 	// 0001 0100 -> 20
    CELL_CASE_RANGEDEC = CELL_CASE | (RANGE_DEC << 4), 	// 0010 0100 -> 36
    CELL_CASE_LIFE = CELL_CASE | (LIFE << 4), 				// 0101 0100 -> 84
    CELL_CASE_MONSTER = CELL_CASE | (MONSTER_INC << 4),			// 0110 0100 -> 100
    CELL_BONUS_BOMB_RANGE_INC = CELL_BONUS | (RANGE_INC << 4),
    CELL_BONUS_BOMB_RANGE_DEC = CELL_BONUS | (RANGE_DEC << 4),
    CELL_BONUS_BOMB_NB_INC = CELL_BONUS | (NB_INC << 4),
    CELL_BONUS_BOMB_NB_DEC = CELL_BONUS | (NB_DEC << 4),
    CELL_BONUS_LIFE = CELL_BONUS | (LIFE << 4),
    CELL_DOOR_CLOSED_1 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_1 << 4),
    CELL_DOOR_OPENED_1 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_1 << 4),
    CELL_DOOR_CLOSED_2 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_2 << 4),
    CELL_DOOR_OPENED_2 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_2 << 4),
    CELL_DOOR_CLOSED_3 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_3 << 4),
    CELL_DOOR_OPENED_3 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_3 << 4),
    CELL_DOOR_CLOSED_4 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_4 << 4),
    CELL_DOOR_OPENED_4 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_4 << 4),
    CELL_DOOR_CLOSED_5 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_5 << 4),
    CELL_DOOR_OPENED_5 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_5 << 4),
    CELL_DOOR_CLOSED_6 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_6 << 4),
    CELL_DOOR_OPENED_6 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_6 << 4),
    CELL_DOOR_CLOSED_7 = CELL_DOOR | (CLOSED_DOOR << 7) | (DOOR_7 << 4),
    CELL_DOOR_OPENED_7 = CELL_DOOR | (OPENED_DOOR << 7) | (DOOR_7 << 4),
    CELL_GOAL_FLAG = CELL_GOAL | (FLAG << 4),
    CELL_GOAL_PEACH = CELL_GOAL | (PEACH << 4),
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
cell_type_t map_get_cell_type(struct map* map, int x, int y);
cell_type_t map_get_cell(struct map* map, int x, int y);
cell_type_t map_get_cell_bonus_type(struct map* map, int x, int y);
cell_type_t map_get_cell_door_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, cell_type_t type);

void map_set_opened_door(struct map* map, int x, int y);

int map_get_goal_type(struct map* map, int x, int y);

int map_get_door_number(struct map* map, int x, int y);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default 12x12 static map
struct map* map_get_default();

struct map* map_get_nb();

// Return the bomb list of the map
struct list* map_get_bombs(struct map* map);

// Change the bomb list of the map
void map_set_bombs(struct map* map, struct list* bomb);

// Insert a bomb in the map bomb list
void map_insert_bomb(struct map* map, int x, int y, void* data);

struct list* map_get_monsters(struct map* map);
void map_set_monsters(struct map* map, struct list* monster);
void map_insert_monster(struct map* map, int x, int y, void* data);

// Display a bonus after the explosion of a case
void map_case_explosion(struct map* map, int x, int y);

// Display the map on the screen
void map_display(struct map* map);

struct map* map_load_from_file(char* data);

#endif /* MAP_H_ */
