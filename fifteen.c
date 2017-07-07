/**
 * Implement the Game of Fifteen, per the below (generalized to d x d).
 *
 *   $ ./fifteen 3
 *   WELCOME TO GAME OF FIFTEEN
 *
 *   8  7  6
 *
 *   5  4  3
 *
 *   2  1  _
 *
 *   Tile to move:
 *
 *
 * Usage: fifteen d
 * 
 * Author: Julie Huang, julie@juliehuang.co.nz (implementation of init, draw, move, won)
 * 
 * 
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * This file is a solution to the Game of Fifteen problem from
 * pset3 of CS50x.
 */
 
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cs50.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

int board[DIM_MAX][DIM_MAX]; // board
int d;  // dimensions
int x, y;  // EMPTY TILE: x = row, y = column | board[x][y]
int a, b;  // TILE: a = row, y = column | board[a][b]
int counter;  // tracks first move

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int largest_tile = (d * d) - 1; // declare & initialize largest tile value
    
    // print tiles in descending order
    for (int i = 0; i < d; i++)
    {   
        for(int j = 0; j < d; j++)
        {
            board[i][j] = largest_tile; // board[i][j] (starting at board[0][0]) inherits value of largest tile
            largest_tile--; // decrement value of largest_tile for next iteration
            
            // initializes global co-ordinates for empty tile 
            if (board[i][j] == 0)
            {
                x = i;
                y = j;
            }
        }
    }
        // swap tile 1 & 2 if number of tiles is even
        if(d % 2 == 0)
        {
            int temp = board[d - 1][d - 2];
            board[d - 1][d - 2] = board[d - 1][d - 3];
            board[d - 1][d - 3] = temp;
        }
    
    counter = -1;   // initialize counter
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int line_counter = 0;  // line break/formatting
    
    // "i" = row, "j" = column of board[][]
    for(int i = 0; i < d; i++)
    {   
        for(int j = 0; j < d; j++)
        {
            // handles placeholder (represented by 0) for empty tile. Display as "__"
            if (board[i][j] == 0)
            {
                printf(" __ ");
                line_counter++; // increment line counter
                
                // if row has been printed, reset line counter
                if (line_counter == d)
                {
                    printf("\n\n");
                    line_counter = 0;
                }
            }
            else
            {
                // print tile value stored in board[i][j]
                printf(" %2i ", board[i][j]);
                line_counter++;
                
                // if row has been printed, reset line counter
                if (line_counter == d)
                {
                    printf("\n\n");
                    line_counter = 0; 
                }
            }
        }
    }
    
    // debugging
    //eprintf ("[a] is %i, [b] is %i\n", a, b);
    //eprintf ("[x] is %i, [y] is %i\n", x, y);
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int tile_max = (d * d) - 1; // declare & initialize largest tile value
    
    // finds first position of tile
    if (counter == -1)
    {
        a = (tile_max - tile) % d;
        b = d - ((tile / d) + 1);
        counter ++;
    }
    
    // checks tile can be swapped & changes co-ordinates if true
    if(tile == board[x - 1][y])
    {
        // empty tile moves up
        a = x;
        b = y;
        x--;
    }
    else if(tile == board[x + 1][y])
    {
        // empty tile moves down
        a = x;
        b = y;
        x++;
    }
    else if(tile == board[x][y - 1])
    {
        // empty tile moves left
        a = x;
        b = y;
        y--;
    }
    else if(tile == board[x][y + 1])
    {
        // empty tile moves right
        a = x;
        b = y;
        y++;
    }
    else
    {
        return false;
    }
    
    // new co-ordinates re-initialized to finish swap
    board[a][b] = tile;
    board[x][y] = 0;
    
    return true;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int x = 1; // tracks value of tile
    int empty_tile = d * d; // represents value of the empty tile
    
    // checks tile values against winning position, except for n
    for (int i = 0; i < d; i++)
    {   
        for(int j = 0; j < d; j++)
        {
            if (x < empty_tile && board[i][j] != x)
                return false;
                
            x++;
        }
    }
    
    // checks if empty tile (0 placeholder) is in correct position
    if(board[d - 1][d - 1] != 0)
    return false;
 
    // printf("win\n");
    return true;
}
