// MADE BY SAMOT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "game.h"

// set Global variables
int gridsize_x = 10;
int gridsize_y = 21;
int startx = 4;
int starty = 10;
int debug = 0;
// int portalpositioncheck = 1;
int currentlevel = 1;
time_t t;

int main(int argc, char *argv[])
{
    srand((unsigned)time(&t));
    if (argc == 2 && strcmp(argv[1], "debug") == 0)
    {
        debug = debug + 1;
    }
    // start sequence
    // player input y or Y to start
    if (make_start() == 0)
    {
        printf("byebye\n");
        return 0;
    }
    Point random;
    random.x = 0;
    random.y = 0;
    int switchportalposition = 1;
    // Generate a random x and y coordinate within the grid
    if (random.x == 0 && random.y == 0)
    {
        random = random_grid_point(gridsize_x, gridsize_y);
    }
    World *worldarray = collision_array();
    Point *tree = new_tree();
    tree = make_tree(random, worldarray, tree);
    Checks newcheck;
    newcheck.spawntree = true;
    newcheck.spawnenemy = true;
    newcheck.spawn = true;
    static_collision_update(tree, worldarray, newcheck, switchportalposition);
    // load grid and player start xy
    grid_render(make_player(startx, starty), tree, worldarray, newcheck, switchportalposition);
    // check end_state if end_state = 1 break
    while (end_state(0) != 1)
    {
        Point input = get_input(worldarray);
        if (change_world(0))
        {
            //portalpositioncheck = switch_position(0);
            worldarray = new_world(worldarray);
            leveldata level = levels();
            newcheck.spawntree = level.tree.spawn;
            newcheck.spawnenemy = level.spawnenemy;
            newcheck.spawn = level.spawn;
            newcheck.spawnstart = false;
            // if (newcheck.spawntree){tree = make_tree(random, worldarray);}
            if (newcheck.spawntree)
            {
                newcheck.newworld = true;
                if (level.tree.random)
                {
                    tree = make_tree(random_grid_point(gridsize_x, gridsize_y), worldarray, tree);
                }
                else if(level.tree.x != 0 | level.tree.y != 0)
                {
                    Point newtree_xy;
                    newtree_xy.x = level.tree.x;
                    newtree_xy.y = level.tree.y;
                    tree = make_tree(newtree_xy, worldarray, tree);
                }
                else
                {
                    printf("error: game.c main(): No tree \n");
                }
            }
            static_collision_update(tree, worldarray, newcheck, switchportalposition);
            change_world(2);
        }
        // make world and move player
        Point player = make_player(input.x, input.y);
        grid_render(player, tree, worldarray, newcheck, switchportalposition);
        //dynamic_collision_update(player, worldarray, newcheck);
        newcheck.newworld = false;
    }
    // ending sequence
    make_ending();
    if (worldarray != NULL)
    {
        // done using worldarray
        free(worldarray);
        worldarray = NULL;
    }
    // done using the tree
    if (tree != NULL)
    {
        free(tree);
        tree = NULL;
    }
    return 0;
}
void grid_render(Point player, Point *tree, World *worldarray, Checks newcheck, int switchportalposition)
{
    char cell_buffer[50];
    char line_buffer[1000];
    static int enemycounter = 0;
    static Point enemy;
    char *cell_icon;
    int cell_colour;
    if (newcheck.newworld)
    {
        enemycounter = 0;
        enemy.x = 7;
        enemy.y = 7;
    }
    enemy = enemy_movement(player, enemy, worldarray);
    // for x in the grid
    for (int x = 0; x < gridsize_x; x++)
    {
        strcpy(&line_buffer[0], "");
        strcat(&line_buffer[0], "|");
        // for y in the grid
        for (int y = 0; y < gridsize_y; y++)
        {
            if (is_Tree_Point(x, y, worldarray))
            {
                // Render the tree
                cell_icon = "+";
                cell_colour = 57;
            }
            else
            {
                // Print a grass symbol with a specific color
                // If debug mode is on and the point is not a tree
                cell_colour = grass_colour;
                if (debug != 0)
                {
                    cell_icon = ".";
                }
                else
                {
                    cell_icon = " ";
                }
            }
            if (x == player.x && y == player.y)
            {
                // print player *
                cell_icon = "*";
                cell_colour = grass_colour;
                // if player is on # then stop #
                if (player.x == enemy.x && player.y == enemy.y && enemycounter < 1)
                {
                    enemycounter = enemycounter + 1;
                }
            }
            else if (x == enemy.x && y == enemy.y && enemycounter < 1 && newcheck.spawnenemy)
            {
                cell_icon = "#";
                cell_colour = grass_colour;
            }
            else if (switch_portal_position(x, y, switchportalposition) && newcheck.spawn)
            {
                cell_icon = "@";
                cell_colour = portal_colour;
            }
            print_colour_buffer(cell_icon, cell_colour, &cell_buffer[0]);
            strcat(&line_buffer[0], &cell_buffer[0]);
        }
        strcat(&line_buffer[0], "|");
        strcat(&line_buffer[0], "\n");
        // print_colour(cell_icon,cell_colour);
        printf("%s", &line_buffer[0]);
    }
    // debug view
    if (debug != 0)
    {
        printf("x%i by y%i\n", gridsize_x, gridsize_y);
        printf("player: x%i by y%i\n", player.x, player.y);
    }
}
Point make_player(int x, int y)
{
    // return a Player object with the given x and y coordinates
    Point player;
    player.x = x;
    player.y = y;
    return player;
}
Point get_input(World *worldarray)
{
    //--This function gets and checks the player input--
    static int x;
    static int y;
    // load start state
    static int enemycounter = 0;
    if (enemycounter != 1)
    {
        /*
        x = startx;
        y = starty;
        why do I need this still?
        */
        enemycounter++;
    }
    // save xy for collision
    int y_save = y;
    int x_save = x;
    // get input wasd
    char wasd;
    do
    {
        printf("wasd: ");
        wasd = getchar();
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        switch (wasd)
        {
        case 'w':
            x = x - 1;
            break;
        case 'a':
            y = y - 1;
            break;
        case 's':
            x = x + 1;
            break;
        case 'd':
            y = y + 1;
            break;
        case 'e':
            // set end state to 1
            end_state(1);
            Point endInput;
            endInput.x = -1;
            endInput.y = -1;
            printf("\033[2J");
            return endInput;
        }
    } while (wasd != 'w' && wasd != 'a' && wasd != 's' && wasd != 'd' && wasd != 'e');
    if (is_colliding(x, y, worldarray, 3))
    {
        // THIS FUCKING CODE IS SO FUCKING STUPID WHY DID I MAKE THIS
        change_world(1);
        /*if (switch_position(0) == 1)
        {
            switch_position(2);
        }
        else if (switch_position(0) == 2)
        {
            switch_position(1);
        }*/
        // load xy in to input
        Point input;
        x = 5;
        y = 1;
        input.x = x; // flag
        input.y = y;
        // return x and y of player
        printf("\033[2J");
        return input;
    }
    else if (x != -1 && x != gridsize_x && y != -1 && y != gridsize_y && !is_colliding(x, y, worldarray, 2))
    {
        // if x and y != object in an array else load xy_save's
        //  load xy in to input
        Point input;
        input.x = x;
        input.y = y;
        // return x and y of player
        printf("\033[2J");
        return input;
    }
    else
    {
        // reset XY to before input
        y = y_save;
        x = x_save;
        // load save xy in to Input
        Point input;
        input.x = x_save;
        input.y = y_save;
        // return x and y in of player in Input
        printf("\033[2J");
        return input;
    }
}
int end_state(int n)
{
    static int x = 0;
    if (n != 0)
    {
        x = n;
    }
    return x;
}
void print_colour(char *text, int colour)
{
    printf("\e[48;5;%dm%s\e[0m", colour, text);
}
Point *make_tree(Point random, World *worldarray, Point *tree)
{
    int x = random.x;
    int y = random.y;
    // Function to create a tree structure in the worldarray
    tree[0].x = x;     tree[0].y = y;     tree[1].x = x - 1; tree[1].y = y;     tree[2].x = x - 2; 
    tree[2].y = y;     tree[3].x = x - 1; tree[3].y = y - 1; tree[4].x = x - 1; tree[4].y = y + 1;
    tree[5].x = x - 3; tree[5].y = y;     tree[6].x = x - 1; tree[6].y = y - 2; tree[7].x = x - 1;
    tree[7].y = y + 2; tree[8].x = x - 2; tree[8].y = y + 1; tree[9].x = x - 2; tree[9].y = y - 1;
    // Loop through the tree array and set each point in the worldarray
    for (int i = 0; i <= (tree_num * 10) - 1; i++)
    {
        set_Tree_array(tree[i].x, tree[i].y, worldarray);
    }
    return tree;
}
World *new_world(World *worldarray)
{
    // set worldarray to 0
    memset(worldarray, 0, gridsize_y * gridsize_x * sizeof(World));
    return worldarray;
}
bool switch_portal_position(int x, int y, int i)
{
    switch (i)
    {
    case 1:
        if (x == (gridsize_x / 2) && y == (gridsize_y - 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    case 2:
        if (x == (gridsize_x / 2) && y == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    case 3:
        if (x == 0 && y == (gridsize_y / 2))
        {
            return true;
        }
        else
        {
            return false;
        }
    case 4:
        if (x == gridsize_x && y == (gridsize_y / 2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
void static_collision_update(Point *tree, World *worldarray, Checks newcheck, int switchportalposition)
{
    // for x in the grid
    for (int x = 0; x < gridsize_x; x++)
    {
        // for y in the grid
        for (int y = 0; y < gridsize_y; y++)
        {
            if (is_Tree_Point(x, y, worldarray))
            {
                set_collision_array(x, y, 2, worldarray);
            }
            else if (switch_portal_position(x, y, switchportalposition) && newcheck.spawn)
            {
                set_collision_array(x, y, 3, worldarray);
            }
            else
            {
                bool not_tree = true;
                if (newcheck.spawntree)
                {
                    not_tree = !is_Tree_Point(x, y, worldarray);
                }
                if (debug != 0 && not_tree)
                {
                    set_collision_array(x, y, 1, worldarray);
                }
                else if (not_tree)
                {
                    set_collision_array(x, y, 1, worldarray);
                }
            }
        }
    }
}
void dynamic_collision_update(Point player, World *worldarray, Checks newcheck)
{
    static int enemycounter = 0;
    // for x in the grid
    for (int x = 0; x < gridsize_x; x++)
    {
        // for y in the grid
        for (int y = 0; y < gridsize_y; y++)
        {
            if (player.x == 7 && player.y == 7 && enemycounter < 1)
            {
                enemycounter = enemycounter + 1;
            }
            if (x == 7 && y == 7 && enemycounter != 1 && newcheck.spawnenemy)
            {
                set_collision_array(x, y, 1, worldarray);
            }
        }
    }
}
void print_colour_buffer(char *text, int colour, char *cellbuffer)
{
    sprintf(cellbuffer, "\e[48;5;%dm%s\e[0m", colour, text);
}
Point *new_tree(void)
{
    // Allocate memory for the tree
    Point *tree = malloc((tree_num * 10) * sizeof(Point));
    // set treearray to 0
    memset(tree, 0, (tree_num * 10) * sizeof(Point));
    return tree;
}
Point enemy_movement(Point player, Point enemy, World *worldarray)
{
    Point save_enemy = enemy;
    double bearing_to_player = (atan2(enemy.y - player.y, enemy.x - player.x) * (180.0 / 3.14159265358979323846));
    if (bearing_to_player < 0) 
    {
    bearing_to_player += 360;
    }
    if (bearing_to_player >= 0 &&bearing_to_player <= 90)
    {
        enemy.x = enemy.x - 1;
    }
    else if (bearing_to_player >= 90 &&bearing_to_player <= 180)
    {
        enemy.y = enemy.y - 1;
    }
    else if (bearing_to_player >= 180 &&bearing_to_player <= 240)
    {
        enemy.x = enemy.x + 1;
    }
    else if (bearing_to_player >= 240 &&bearing_to_player <= 360)
    {
        enemy.y = enemy.y + 1;
    }
    else
    {
        printf("error: enemy_movement(): calculation error\n");
    }
    if(is_colliding(enemy.x, enemy.y, worldarray, 2))
    {
        if(!is_colliding((save_enemy.x - 1), save_enemy.y, worldarray, 2))
        {
            save_enemy.x = save_enemy.x - 1;
            return save_enemy;
        }
        else if(!is_colliding((save_enemy.x + 1), save_enemy.y, worldarray, 2))
        {
            save_enemy.x = save_enemy.x + 1;
            return save_enemy;
        }
        else
        {
            return save_enemy;
        }
    }
    else 
        return enemy;
}
leveldata levels(void)
{
    leveldata level;
    if (currentlevel < 6)
    {
        currentlevel++;
    }
    level.tree.spawn = false;
    level.spawnenemy = false;
    level.spawn = true;
    level.tree.x = 0;
    level.tree.y = 0;
    level.tree.random = false;
    switch (currentlevel)
    {
        case 1:
            printf("error: game.c levels(): how did you get here? ");
            break;
        case 2:
            level.tree.spawn = true;
            level.spawnenemy = true;
            level.tree.x = 9;
            level.tree.y = 5;
            break;
        case 3:
            level.tree.spawn = true;
            level.spawnenemy = true;
            level.tree.x = 8;
            level.tree.y = 5;
            break;
        case 4:
            level.tree.spawn = false;
            level.spawnenemy = true;
            level.tree.x = 7;
            level.tree.y = 5;
            break;
        case 5:
            level.tree.spawn = true;
            level.spawnenemy = false;
            level.tree.random = true;
            break;
        case 6:
            level.tree.spawn = false;
            level.spawnenemy = true;
            level.spawn = false;
            break;
        default:
            printf("error: game.c levels(): No level ");
            break;
    }
    return level;
}
