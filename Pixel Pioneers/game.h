//MADE BY SAMOT
#ifndef game_h
#define game_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
// structs
typedef struct
{
	int x;
	int y;
} Point;
typedef struct
{
	int collision;
	bool istree;
} World;
typedef struct
{
	bool spawn;
	bool spawnenemy;
	bool spawntree;
	bool spawnstart;
	bool newworld;
} Checks;
typedef struct
{
	char* cell_icon;
	int cell_colour;
} cell;
typedef struct
{
	int x;
	int y;
	bool spawn;
	bool random;
} Tree;
typedef struct
{
	bool spawnenemy;
	bool spawn;
	Tree tree;
} leveldata;
typedef struct {
    double x, y;
} double_Point;

// defines
#define grass_colour 28
#define tree_num 1
#define portal_colour 39

// Global variables
extern int gridsize_x;
extern int gridsize_y;
extern int startx;
extern int starty;
extern int debug;
extern int portalpositioncheck;

// !!! all functions !!!

//---subgame.c---
void clear_buffer(void);
// clear the input buffer

int make_start(void);
// prompt user to start and prints startscreen

void make_ending(void);
// prints endscreen

Point random_grid_point(int gridsize_x, int gridsize_y);
// returns a random x,y based of gridsize

//int switch_position(int n);
// global toggle switch for switching of the portal position
//        ****Legacy****

//bool collision_update_rate(bool i)
//        ****Legacy****

bool change_world(int n);
// global toggle switch for changing the world

bool is_Tree_Point(int x, int y, World* worldarray);
// returns worldarray[].istree based of x and y coordinate

bool is_colliding(int x, int y, World* worldarray, int check);
// checks worldarray[].collision if it equals check based of x and y coordinate

//Point *makecollision(int x, int y);
//        ****Legacy****

World* collision_array(void);
// makes a malloc array for the world and memsets the malloc to 0

int set_collision_array(int x, int y, int setarray, World* worldarray);
// Function to set a value in the worldarray.collision based of x and y coordinates

void set_Tree_array(int x, int y, World* worldarray);
// Function to set a value in the worldarray.istree based of x and y coordinates

//^^^subgame.c^^^

//---game.c---
void grid_render(Point player, Point* tree, World* worldarray, Checks newcheck, int switchportalposition);
// renders a frames based of gridsize (note: handles all rendering)

Point make_player(int x, int y);
// loads x,y in to and point and returns it

Point get_input(World* worldarray);
// prompt user for input and handles new player position

int end_state(int n);
// checks if to stop rendering new frames

void print_colour(char* text, int colour);
// prints char* with and colour

Point* make_tree(Point random, World* worldarray, Point* tree);
// returns array of tree points based of random x,y points

World* new_world(World* worldarray);
// memsets worldarray to 0

bool switch_portal_position(int x, int y, int i);
// returns value of switch case i

void static_collision_update(Point* tree, World* worldarray, Checks newcheck, int switchportalposition);
// sets world.collision with all static collision objects

//void dynamic_collision_update(Point player,World *worldarray, Checks newcheck);
// sets world.collision with all dynamic collision objects
//            ***Needs updating***

void print_colour_buffer(char* text, int colour, char* cellbuffer);
// a printf for a char* and in a set colour

Point* new_tree(void);
// makes a malloc array for the trees and memsets the malloc to 0

Point enemy_movement(Point player, Point enemy, World* worldarray);
// The function enemy_movement calculates the bearing from the enemy to the player and moves the enemy one step closer to the player based on that bearing.

leveldata levels(void);
// handles level logic

int Distance(Point player, Point enemy);
// distance from Player to enemy

bool castRay(Point grid, Point player, Point enemy);
// raycast from enemy to player

bool castRay_v2(int gridsize_x, int gridsize_y, Point player, Point enemy, World* worldarray);
// raycast from enemy to player

//^^^game.c^^^
#endif
