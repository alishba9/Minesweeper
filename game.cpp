#include "game.h"
#include <cstdlib>
#include <ctime>

// Define the static member for resolution
const QSize Game::RESOLUTION = QSize(1000, 650);

Game::Game()
    : w(32),
      PATH_TO_TILES(":/images/tiles.png"),
      PATH_TO_FACE(":/images/face.png"),
      ITERATION_VALUE(1000.0f/60.0f)
{
    // Call init() in the constructor to set up the grid
    init();
}

void Game::init()
{
    // Seed our random number generator
    srand(static_cast<unsigned>(time(0)));

    // Step 1: Clear both grids (state grid and actual grid)
    for (int i = 1; i <= 30; i++) {
        for (int j = 1; j <= 16; j++) {
            sgrid[i][j] = 10; // 10 might represent a covered cell
            grid[i][j] = 0;   // 0 means no bomb
        }
    }

    // Step 2: Place 99 bombs randomly
    int bombsPlaced = 0;
    while (bombsPlaced < 99) {
        int i = rand() % 30 + 1; // row in [1..30]
        int j = rand() % 16 + 1; // column in [1..16]

        // If not already a bomb, place one
        if (grid[i][j] != 9) {
            grid[i][j] = 9; // 9 represents a bomb
            bombsPlaced++;
        }
    }

    // Step 3: Calculate adjacent bomb counts for each cell
    for (int i = 1; i <= 30; i++) {
        for (int j = 1; j <= 16; j++) {
            if (grid[i][j] == 9) {
                // It's a bomb; skip
                continue;
            }
            int count = 0; // number of adjacent bombs
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;
                    // Check bounds
                    if (nx > 0 && nx <= 30 && ny > 0 && ny <= 16) {
                        if (grid[nx][ny] == 9) {
                            count++;
                        }
                    }
                }
            }
            grid[i][j] = count;
        }
    }
}

// Checks if the cell at (x, y) is a mine.
// If it is, emit the mineClicked() signal.
bool Game::checkMine(int x, int y)
{
    if (grid[x][y] == 9) {
        emit mineClicked();
        return true;
    }
    return false;
}

