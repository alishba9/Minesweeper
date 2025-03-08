#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QSize>

class Game : public QObject
{
    Q_OBJECT

public:
    // Constructor
    Game();

    // Method to initialize the board, place bombs, etc.
    void init();

    // Checks if a given cell is a mine
    bool checkMine(int x, int y);

    // Constants / member variables
    static const QSize RESOLUTION;
    const int w;
    const QString PATH_TO_TILES;
    const QString PATH_TO_FACE;
    int grid[31][17];    // Actual board: 9 = mine, 0..8 = # of adjacent mines
    int sgrid[31][17];   // State board: 10 = covered, 11 = flagged, etc.
    const float ITERATION_VALUE;

signals:
    // Emitted when a mine is clicked
    void mineClicked();
};

#endif // GAME_H

