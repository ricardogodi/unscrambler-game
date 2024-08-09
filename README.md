# Unscrambler Game

## Project Overview
The Unscrambler Game is a C++ word puzzle game that challenges users to rearrange scrambled words on a board back to their original order. This project includes both a manual solving mode, where users rotate rows and columns to achieve the correct arrangement, and an automatic solving mode, which utilizes advanced recursion to reverse the scrambling process. The project also features an undo functionality, allowing users to revert their moves, implemented through a custom linked list.

### Key Features
- **Custom Board Generation**: Randomly selects words from a dictionary and arranges them on a board of user-specified size, then scrambles the board through systematic rotations.
- **Manual Solving Mode**: Users can manually rotate rows and columns to unscramble the board.
- **Automatic Solving Mode**: A recursive algorithm that automatically solves the scrambled board, reversing the scrambling steps.
- **Undo Functionality**: Tracks and stores each board state in a custom linked list, allowing for undo operations.


## Files Included

- **main.cpp**: The core implementation of the Unscrambler Game, containing the logic for board creation, scrambling, manual solving, automatic solving with recursion, and undo functionality.
- **dictionary-3to5-letters.txt**: A dictionary file containing words of 3 to 5 letters, used to populate the game board with random words.
- **Makefile**: A simple makefile for compiling the project and cleaning up the build files.

## Technical Description

### Board Representation and Scrambling
The game board is represented as a vector of characters, where words are placed sequentially. The board's size is dynamically determined by user input. Scrambling is performed by rotating rows and columns by a specified number of times, ensuring that the scrambled board is sufficiently mixed.

### Solving Algorithm
The automatic solving feature uses a recursive depth-first search algorithm to explore possible board configurations. By systematically reversing the row and column rotations, the algorithm attempts to restore the board to its original state. The recursion is controlled by a maximum depth parameter, ensuring that the search space remains manageable.

### Linked List Implementation
A custom linked list is used to implement the undo functionality. Each node in the list stores a snapshot of the board's state, allowing the user to revert back to any previous state in the game.

## Instructions for Running

### Compiling the Project
To compile the Unscrambler Game, simply run the following command in the terminal:

```bash
make
```

### Running the Game
After compiling, you can run the game by executing the following command:

```bash
./unscrambler
```

### Game Instructions

1. **Choose Board Size:**
   - You will be prompted to enter the board size.
   - Enter a number greater than or equal to 4.

2. **Choose Scramble Level:**
   - You will be asked to select the number of times to scramble the board.
   - Enter a number greater than or equal to 1.

3. **Menu Options:**
   - **R**: Rotate a row or column.
     - You will be prompted to specify the row or column number and the number of positions to rotate.
   - **C**: View the completed (unscrambled) board.
   - **B**: Reset the board back to its initial scrambled state.
   - **G**: Generate a new board with different random words.
   - **S**: Automatically solve the board.
   - **U**: Undo the last move.
   - **Q**: Quit the game.













