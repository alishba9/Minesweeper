# Minesweeper

**A classic Minesweeper game** built with C++ and Qt. Use left-click to reveal cells, right-click to place flags, and reset the board by clicking the smiley face.

**Features**
- Grid: 30 columns × 16 rows, with 99 mines hidden randomly.
- Graphics: Uses Qt for both rendering (tiles, face icon) and event handling.
- Random Seed: Each run places bombs in different positions, making every game unique.
- Reset: Click the smiley face at the top (positions x=5 or x=6, y=0) to restart the game.


  
**Requirements**

Qt (version 5 or higher)
C++11 or newer compiler
CMake or qmake (depending on your build preference)

**Compilation**

1. Install Qt and Qt Creator or ensure qmake is available in your system PATH.
2. Open a terminal in the project folder, then run:

                                                   qmake && make 
This will generate an executable named **Minesweeper** (or similar, depending on your OS).

**Running the Game**

                                                  ./Minesweeper

**Gameplay**

1. **Left-Click** on a cell to reveal it:
      - If you click on a mine, the game ends.
      - If you click a safe cell, you see a number indicating adjacent mines.
2. **Right-Click** on a cell to toggle a flag (helpful to mark suspected mines).
3. **Smiley Face Reset:**
      - If you click on the smiley face at the top row, you can reset the entire board.

      
**File Structure**

      Minesweeper/
      ├── game.cpp/.h        # Core logic for board initialization, bomb placement, adjacency checks
      ├── gamescene.cpp/.h   # Qt scene that handles rendering, input, & communication with `Game`
      ├── view.cpp/.h        # Qt view that sets up the scene
      ├── main.cpp           # Application entry point
      ├── resource.qrc       # Qt resource file for images (tiles, face icon)
      ├── images/            # PNGs for tiles & face
      ├── Minesweeper.pro    # Qt project file
      └── README.md          # This file
