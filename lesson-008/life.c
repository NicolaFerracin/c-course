#include <stdio.h>
#include <unistd.h>

#define GRID_COLS 30
#define GRID_ROWS 30
#define GRID_CELLS (GRID_COLS*GRID_ROWS)
#define ALIVE '#'
#define DEAD '.'


/** Normalizes x and y so they are never out of bounds and they wrap around to the other side */
int coords_to_index(int x, int y) {
    if (x >= GRID_COLS) x = x % GRID_COLS;
    else if (x < 0) x = GRID_COLS - (-x % GRID_COLS);

    if (y >= GRID_ROWS)  y = y % GRID_ROWS;
    else if (y < 0)  y = GRID_ROWS - (-y % GRID_ROWS);

    return y * GRID_COLS + x;
} 

/** 
* Sets the given cell to the desired state.
* Internally, it makes sure to wrap any cell overflowing the grid to the other side
*/
void set_cell(char *grid, int x, int y, char state) {
    grid[coords_to_index(x, y)] = state;
}

/** 
* Returns the given cell state.
* Internally, it makes sure to wrap any cell overflowing the grid to the other side
*/
int get_cell(char *grid, int x, int y) {
    return grid[coords_to_index(x, y)];
}

/** Sets all cells in the grid to the desired state */
void set_grid(char *grid, char state) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            set_cell(grid, x, y, state);
        }
    }
}

/** Clears the screen and prints the current grid content to the terminal */
void print_grid(char *grid) {
    printf("\x1b[3J\x1b[H\x1b[2J");

    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            printf("%c", get_cell(grid, x, y));
        }
        printf("\n");
    }
}

/** Gets the number of alive neighbours for the given cell */
int get_alive_neighbours(char *grid, int x, int y) {
    int n_alive = 0;
    for (int yo = -1; yo <= 1; yo++) {
        for (int xo = -1; xo <= 1; xo++) {
            if (yo == 0 && xo == 0) continue;

            if (get_cell(grid, x + xo, y + yo) == ALIVE) n_alive++;
        }
    }

    return n_alive;
}

/** Plays the current generation and get the state for the next one */
/** Rules:
    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void play_generation(char *grid, char *new_grid) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            int new_state = DEAD;
            int state = get_cell(grid, x, y);
            int n_alive = get_alive_neighbours(grid, x, y);

            if (state == ALIVE && (n_alive == 2 || n_alive == 3)) new_state = ALIVE;
            if (state == DEAD && n_alive == 3) new_state = ALIVE;

            set_cell(new_grid, x, y, new_state);
        }
    }
}

int main(void) {
    // We need two grids so that we can populate one while we check the other, and viceversa
    char grid_a[GRID_CELLS];
    char grid_b[GRID_CELLS];

    set_grid(grid_a, DEAD);

    // Initialize a few formations
    set_cell(grid_a, 21, 21, ALIVE);
    set_cell(grid_a, 21, 22, ALIVE);
    set_cell(grid_a, 21, 23, ALIVE);
    set_cell(grid_a, 20, 23, ALIVE);
    set_cell(grid_a, 19, 22, ALIVE);
    
    set_cell(grid_a, 10, 21, ALIVE);
    set_cell(grid_a, 10, 22, ALIVE);
    set_cell(grid_a, 10, 23, ALIVE);

    set_cell(grid_a, 90, 21, ALIVE);
    set_cell(grid_a, 90, 22, ALIVE);
    set_cell(grid_a, 90, 23, ALIVE);

    set_cell(grid_a, 17, 21, ALIVE);
    set_cell(grid_a, 17, 22, ALIVE);
    set_cell(grid_a, 17, 23, ALIVE);

    set_cell(grid_a, 76, 21, ALIVE);
    set_cell(grid_a, 76, 22, ALIVE);
    set_cell(grid_a, 76, 23, ALIVE);
    

    while (1) {
        // print A and generate next generation onto B
        print_grid(grid_a);
        usleep(100000);
        play_generation(grid_a, grid_b);
        
        // print B and generate next generation onto A
        print_grid(grid_b);
        usleep(100000);
        play_generation(grid_b, grid_a);
    }

    return 0;
}