//MADE BY SAMOT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"

// set Global variables
int gridsize_x = 10;
int gridsize_y = 21;
int startx = 4;
int starty = 10;
int debug = 0;
int portalpositioncheck = 1;

int main(int argc, char *argv[])
{
    time_t t;
    srand((unsigned) time(&t));
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
        random = make_random(gridsize_x, gridsize_y);
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
            portalpositioncheck = switch_position(0);
            worldarray = new_world(worldarray);
            newcheck.spawntree = true;
            newcheck.spawnenemy = true;
            newcheck.spawn = true;
            // if (newcheck.spawntree){tree = make_tree(random, worldarray);}
            if (newcheck.spawntree)
            {
                tree = make_tree(make_random(gridsize_x, gridsize_y), worldarray, tree);
            }
            static_collision_update(tree, worldarray, newcheck, switchportalposition);
            change_world(2);
        }
        // make world and move player
        Point player = make_player(input.x, input.y);
        grid_render(player, tree, worldarray, newcheck, switchportalposition);
        dynamic_collision_update(player, worldarray, newcheck);
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
    static int q = 0;
    char* cell_icon;
    int cell_colour;
    // for x in the grid
    for (int x = 0; x < gridsize_x; x++)
    {
        strcpy(&line_buffer[0], "");
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
                if (debug != 0 )
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
                if (player.x == 7 && player.y == 7 && q < 1)
                {
                    q = q + 1;
                }
            }
            else if (x == 7 && y == 7 && q != 1 && newcheck.spawnenemy)
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
        strcat(&line_buffer[0], "\n");
        //print_colour(cell_icon,cell_colour);
        printf(&line_buffer[0]);
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
    static int q = 0;
    if (q != 1)
    {
        x = startx;
        y = starty;
        q++;
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
    }
    while (wasd != 'w' && wasd != 'a' && wasd != 's' && wasd != 'd' && wasd != 'e');
    if (is_colliding(x, y, worldarray, 3))
    {
        change_world(switch_position(0));
        if (switch_position(0) == 1)
        {
            switch_position(2);
        }
        else if (switch_position(0) == 2)
        {
            switch_position(1);
        }
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
    tree[0].x = x; // root
    tree[0].y = y;

    tree[1].x = x - 1; // trunk 1
    tree[1].y = y;

    tree[2].x = x - 2; // trunk 2
    tree[2].y = y;

    tree[3].x = x - 1; // branch 1
    tree[3].y = y - 1;

    tree[4].x = x - 1; // branch 2
    tree[4].y = y + 1;

    tree[5].x = x - 3; // trunk 3
    tree[5].y = y;

    tree[6].x = x - 1;
    tree[6].y = y - 2;

    tree[7].x = x - 1;
    tree[7].y = y + 2;

    tree[8].x = x - 2;
    tree[8].y = y + 1;

    tree[9].x = x - 2;
    tree[9].y = y - 1;
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
        default:
            return false;
    }
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
            else if (switch_portal_position(x, y, switchportalposition) && newcheck.spawn )
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
                if (debug != 0 && not_tree )
                {
                    set_collision_array(x, y, 1, worldarray);
                }
                else if (not_tree )
                {
                    set_collision_array(x, y, 1, worldarray);
                }
            }
        }
    }
}
void dynamic_collision_update(Point player, World *worldarray, Checks newcheck)
{
    static int q = 0;
    // for x in the grid
    for (int x = 0; x < gridsize_x; x++)
    {
        // for y in the grid
        for (int y = 0; y < gridsize_y; y++)
        {
            if (player.x == 7 && player.y == 7 && q < 1)
            {
                q = q + 1;
            }
            if (x == 7 && y == 7 && q != 1 &&newcheck.spawnenemy )
            {
                set_collision_array(x, y, 1, worldarray);
            }
        }
    }
}
void print_colour_buffer(char *text, int colour, char* cellbuffer)
{
    sprintf(cellbuffer, "\e[48;5;%dm%s\e[0m", colour, text);
}
Point *new_tree()
{
    // Allocate memory for the tree
    Point *tree = malloc((tree_num * 10) * sizeof(Point));
    // set treearray to 0
    memset(tree, 0, (tree_num * 10) * sizeof(Point));
    return tree;
}
