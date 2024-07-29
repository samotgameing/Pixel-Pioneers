//MADE BY SAMOT
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

void clear_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
int make_start(void)
{
    printf("hello and welcome\ndo you want to play\n");
    // get y or Y
    int startstate = 0;
    // prompt user to agree
    char input;
    printf("y/n ");
    scanf("%c", &input);
    // Check whether user agreed
    if (input == 'y' || input == 'Y')
    {
        printf("\033[2J");
        printf("\n  _____ _          _   _____ _                                "
               "\n |  __ (_)        | | |  __ (_)                               "
               "\n | |__) |__  _____| | | |__) |  ___  _ __   ___  ___ _ __ ___ "
               "\n |  ___/ \\ \\/ / _ \\ | |  ___/ |/ _ \\| '_ \\ / _ \\/ _ \\ '__/ __|"
               "\n | |   | |>  <  __/ | | |   | | (_) | | | |  __/  __/ |  \\__ \\"
               "\n |_|   |_/_/\\_\\___|_| |_|   |_|\\___/|_| |_|\\___|\\___|_|  |___/\n");
        printf(" Loading.");
        /*printf("Press ENTER to start the game...\n");
        char buf[128];
        fgets (buf, sizeof buf, stdin);*/
        time_t seconds;
        seconds = time(NULL);
        if (seconds == -1)
        {
            printf("AAAAaa FUCK error: subgame.c make_start(): Time error ");
            return startstate;
        }
        time_t timesave = seconds;
        long wait_time = 2;
        while (seconds < (timesave + wait_time))
        {
            printf(".");
            seconds = time(NULL);
            //printf("%lo and %lo",timesave,seconds);
        }
        printf("\n");
        printf("\033[2J");
        return startstate + 1;
    }
    else
    {
        return startstate;
    }
}
void make_ending(void)
{
    printf("\033[2J");
    printf(" _                    __   ___              ___   __  \n"
           "| |                  / /  / _ \\            / _ \\  \\ \\ \n"
           "| |__  _   _  ___   | |  | | | |          | | | |  | |\n"
           "| '_ \\| | | |/ _ \\  | |  | | | |          | | | |  | |\n"
           "| |_) | |_| |  __/  | |  | |_| |          | |_| |  | |\n"
           "|_.__/ \\__, |\\___|  | |   \\___/            \\___/   | |\n"
           "        __/ |        \\_\\          ______          /_/ \n"
           "       |___/                     |______|             \n");
    printf("\n");
}
Point random_grid_point(int gridsize_x, int gridsize_y)
{
    int random_x;
    int random_y;
    // end of random was a +3 don't know why? ¯\_(ツ)_/¯
    random_x = (rand() % (gridsize_x - 3)) + 3;
    random_y = (rand() % (gridsize_y - 5)) + 3;
    Point random;
    random.x = random_x;
    random.y = random_y;
    return random;
}
/*int switch_position(int n)
{
    static int x = 1;
    if (n == 1)
    {
        x = 1;
    }
    else if (n == 2)
    {
        x = 2;
    }
    return x;
}*/
/*bool collision_update_rate(bool i)
{
    // Function to control an update rate
    static int n = 5;
    static bool update = false;
    if (i)
    {
        if (n == 5)
        {
            update = true;
            n = 0;
        }
        else
        {
            update = false;
            n++;
        }
    }
    return update;
}*/
bool change_world(int n)
{
    static bool x = false;
    if (n == 1)
    {
        x = true;
    }
    else if (n == 2)
    {
        x = false;
    }
    return x;
}
bool is_Tree_Point(int x, int y, World *worldarray)
{
    //              ****Legacy****
    // --This function checks if a point (x, y) collides with
    // any point in an array of points.--

    // Loop through each point in the array
    // for (int i = 0; i < size; i++)
    //{
    // If the current point in the array is equal to the point we're checking
    // if (tree[i].x == x && tree[i].y == y)
    //  {
    //      // Return true because a collision with tree has occurred
    //       return true;
    //    }
    // }
    // If no collision was found, return false
    // return false;
    //              ^^^^Legacy^^^^
    int arraycheck = gridsize_y * gridsize_x * sizeof(World);
    // --This function checks if a point (x, y) x and y coordinates--
    int i;
    // Calculating the index based on x, y, gridsize_y, gridsize_x, and sizeof(int)
    i = y * gridsize_x + x;
    // Checking if the calculated index is less than 210
    if (i < arraycheck)
    {
        // return the address of the cell in the worldarray
        return worldarray[i].istree;
    }
    printf("error: subgame.c is_Tree_Point(): arraymax_I: %i\n", i);
    return false;
}
bool is_colliding(int x, int y, World *worldarray, int check)
{
    int arraycheck = gridsize_y * gridsize_x;
    // --This function checks if a point (x, y) x and y coordinates--
    int i;
    // Calculating the index based on x, y, gridsize_y, gridsize_x, and sizeof(int)
    i = y * gridsize_x + x;
    // Checking if the calculated index is less than 210
    if (i < arraycheck && x < gridsize_x && y < gridsize_y)
    {
        // Getting the address of the cell in the worldarray
        if (worldarray[i].collision == check)
        {
            return true;
        }
        return false;
    }
    printf("error: subgame.c is_colliding(): arraymax_I: %i\n", i);
    return false;
}
//              ****Legacy****
// Point *makecollision(int x, int y)
//{
// --This function creates a collision at a given point (x, y)--
// Keep track of the current index in the collision array
// static int I = 0;
// Set the x and y coordinates of the collision
// collision[I].y = y;
// collision[I].x = x;
// if (I != collisionmax ){i++}
// Return the updated collision array
// return collision;
//}
//              ^^^^Legacy^^^^
World *collision_array(void)
{
    // --Function to allocate memory for the worldarray--
    // Allocating memory for gridsize_y * gridsize_x, size of struct integers and returning the pointer
    World *worldarray = malloc(gridsize_y * gridsize_x * sizeof(World));
    if (worldarray != NULL)
    {
        memset(worldarray, 0, gridsize_y * gridsize_x * sizeof(World));
    }
    return worldarray;
}
int set_collision_array(int x, int y, int setarray, World *worldarray)
{
    if (worldarray == NULL)
    {
        printf(" error: subgame.c set_collision_array(): array is NULL \n");
        return -1;
    }
    int arraycheck = gridsize_y * gridsize_x;
    // --Function to set a value in the worldarray based on x and y coordinates--
    int i;
    // Calculating the index based on x, y and gridsize_y.
    i = y * gridsize_x + x;
    // Checking if the calculated index is less than 210
    if (i < arraycheck && x < gridsize_x && y < gridsize_y)
    {
        // Set the value of the worldarray[i] to setarray
        worldarray[i].collision = setarray;
        // Returning the value just set
        return worldarray[i].collision;
    }
    else
    {
        printf("error: subgame.c set_collision_array(): arraymax_I: %i\n", i);
        return -1;
    }
}
void set_Tree_array(int x, int y, World *worldarray)
{
    if (worldarray == NULL)
    {
        printf(" error: subgame.c set_Tree_array(): array is NULL \n");
        return;
    }
    int arraycheck = gridsize_y * gridsize_x;
    // --Function to set a value in the worldarray based on x and y coordinates--
    int i;
    // Calculating the index based on x, y and gridsize_y.
    i = y * gridsize_x + x;
    // Checking if the calculated index is less than 210
    if (i < arraycheck)
    {
        // Getting the address of the cell in the worldarray
        worldarray[i].istree = true;
    }
    else
    {
        printf("error: subgame.c set_Tree_array(): arraymax_I: %i\n", i);
    }
}
