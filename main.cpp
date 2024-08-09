//
//  main.cpp
//  Unscrambler
//  Course: CS 141
//  Author: Ricardo Gonzalez
//

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cassert>
#include <iomanip>

using namespace std;
class Node {
public:
    vector<char> boardState;
    Node* next;

    Node(vector<char> board, Node* nextNode = nullptr) : boardState(board), next(nextNode) {}
};

class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void pushState(vector<char> board) {
        Node* newNode = new Node(board, head);
        head = newNode;
    }

    vector<char> popState() {
        if (head == nullptr) {
            throw runtime_error("No more states to undo.");
        }
        Node* temp = head;
        vector<char> lastState = head->boardState;
        head = head->next;
        delete temp;
        return lastState;
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

class Board {
public:
    void setBoard(vector<char> theBoard, int theBoardSize) {
        board = theBoard;
        boardSize = theBoardSize;  // Ensure boardSize is set
    }
    
    void setRandomWords(vector<string> theRandomWords) {
        randomWords = theRandomWords;
    }
    
    void setCompletedBoard(vector<char> theCompletedBoard, int theBoardSize) {
        completedBoard = theCompletedBoard;
        boardSize = theBoardSize;  // Ensure boardSize is set
    }
    
    void setBoardSize(int theBoardSize) {
        boardSize = theBoardSize;
    }
    
    vector<char> *getBoard() {
        return &board;
    }
    
    vector<string> *getRandomWords() {
        return &randomWords;
    }
    
    vector<char> *getCompletedBoard() {
        return &completedBoard;
    }
    
    int getBoardsize() {
        return boardSize;
    }
    
private:
    vector<char> board;
    vector<string> randomWords;
    vector<char> completedBoard;
    int boardSize = 0; 
};


void readWordsIntoVector(vector <string> &myVector) {
    
    ifstream inStream;                          // Declare an input stream for reading
    
    inStream.open("dictionary-3to5-letters.txt");        // Open dictionary file
    assert( inStream.fail() == false );     // Ensure file open worked
    
    // Clear vector in case we have to use it again
    myVector.clear();
    
    // Keep repeating while input from the file yields a word
    string newWord;
    while( inStream >> newWord) {               // While there is another word to be read
        myVector.push_back(newWord);            // Push back the word into myVector
    }
    
    inStream.close();                           // Close the dictionary file
}

void displayMenu() {
    cout << "Enter one of the following: " << endl;
    cout << "   R to rotate a row or column, " << endl;
    cout << "   C to view what the completed board should look like, " << endl;
    cout << "   B to reset the board back to the beginning, " << endl;
    cout << "   G to generate a new board with new random words, " << endl;
    cout << "   S to have the computer automatically solve a board that you enter (5 points extra credit), " << endl;
    cout << "   U to undo the last move, " << endl; // Added undo option
    cout << "   Q to quit. " << endl;
}

void displayBoardInOneLine (vector<char> const &board) {    // Display a board in the manner of a single string
    
    cout << "Current board words: ";
    for (int i = 0; i < board.size(); i++) {
        cout << board.at(i);
    }
    cout << endl << endl;
}

void displayRandomWords(vector<string> const &randomWords) {    // Display the words in a single line
    
    cout << "These are the words that you should try to spell out using the board, in order: " << endl;
    for (int i = 0; i < randomWords.size(); i++) {
        
        cout << randomWords.at(i);
        
        if (i < randomWords.size() - 1) {   // If i is less than the last index
            cout << ", ";
        } else {                            // If i is equal to the last index
            cout << "." << endl;
        }
    }
    cout << endl;
}

void displayBoard(vector<char> const &board) {  // Display the board
    
    int nextIndex = 0;
    int boardSize = sqrt(board.size());
    int n = 0;
    
    cout << "       ";
    for (int i = 0; i < boardSize; i++) {
        cout << i << "   ";
    }
    cout << endl;
    
    cout << "      ";
    for (int i = 0; i < boardSize * 4 - 1; i++) {
        cout << "-";
    }
    cout << endl;
    
    for (int i = 0; i < boardSize; i++) {
        
        cout << setw(4) << n << " |";
        for (int j = 0; j < boardSize; j++) {
            cout << " " << board.at(nextIndex) << " |";
            nextIndex++;
        }
        cout << endl;
        
        cout << "      ";
        for (int k = 0; k < boardSize*4 - 1; k++) {
            cout << "-";
        }
        cout << endl;
        n++;
    }
    displayBoardInOneLine(board);
}

vector<char> createBoard(vector<string> const &words, vector<string> &randomWords, int boardSize) {
    
    vector<char> board(boardSize*boardSize);
    
    randomWords.clear();
    
    for (int i = 0; i < board.size(); i++) { // Fill up the board with whitespaces
        board.at(i) = ' ';
    }
    
    int roomLeft = board.size();            // To keep track of the number of spaces that are left
    int lastLetterIndex = 0;
    int randomIndex;
    string nextWord;
    
    while (roomLeft >= 3) {                      // Keep going as long as there is room for another word
        randomIndex = rand() % words.size();     // Generate random index between 0 and the size of the 'words' vector
        nextWord = words.at(randomIndex);        // Retrieve the word stored and randomIndex
        
        if (nextWord.size() > roomLeft) {        // If the word is larger than the space availabe, repeat the loop until we find a word
            continue;                            // of appropriate length
            
        } else {
            randomWords.push_back(nextWord);     // Store the word in randomWords
            for (int i = 0; i < nextWord.size(); i++) {         // Store each word character in the board of characters
                board.at(lastLetterIndex) = nextWord.at(i);
                lastLetterIndex++;
            }
            roomLeft = roomLeft - nextWord.size() - 1;      // Update roomLeft
            lastLetterIndex++;                              // Skip one whitespace
        }
    }
    return board;
}

void rotateRow(vector<char> &manipulatedBoard, int boardSize, int rowNum, int numOfShifts ) {
    
    int rowFirstIndex;
    char previousChar;
    char tempChar;
    
    int numOfShiftsAbs = abs(numOfShifts);
    
    if (numOfShifts >= 0) {                                 // If the number of shifts is positive, execute the appropriate algorithm
        
        for (int i = 0; i < numOfShiftsAbs; i++) {          // Outer loop determines the number of shifts
            
            rowFirstIndex = boardSize * rowNum;             // Store the first index of the chosen row
            previousChar = manipulatedBoard.at(rowFirstIndex + boardSize - 1);      // Previous character is the last character
            
            for (int j = rowFirstIndex; j < rowFirstIndex + boardSize; j++) {       // Inner loop makes one shift at a time
                
                tempChar = manipulatedBoard.at(j);          // tempChar is the character and the current index
                manipulatedBoard.at(j) = previousChar;      // store the previous character at the current index
                previousChar = tempChar;                    // Now  previousChar stores the character that was at the current index for the next iteration
            }
        }
    } else {                                                // This algorithm uses the same logic as the one above, but with different j values
        
        for (int i = 0; i < numOfShiftsAbs; i++) {
            
            rowFirstIndex = boardSize * rowNum;
            previousChar = manipulatedBoard.at(rowFirstIndex);
            
            for (int j = rowFirstIndex + boardSize - 1; j >= rowFirstIndex ; j--) {
                
                tempChar = manipulatedBoard.at(j);
                manipulatedBoard.at(j) = previousChar;
                previousChar = tempChar;
            }
        }
    }
}

void rotateCol(vector<char> &manipulatedBoard, int boardSize, int columnNum, int numOfShifts) {
    
    char previousChar;
    char tempChar;
    
    int numOfShiftsAbs = abs(numOfShifts);
    
    if (numOfShifts >= 0) {                 // If the number of shifts is positive, execute the appropriate algorithm
        
        for (int i = 0; i < numOfShiftsAbs; i++) {   // Outer loop determines the number of shifts
            
            previousChar = manipulatedBoard.at(columnNum + boardSize*(boardSize-1));    // Previous character is the last character
            
            for (int j = columnNum; j <= columnNum + boardSize*(boardSize-1); j = j + boardSize) {   // Inner loop makes one shift at a time
                
                tempChar = manipulatedBoard.at(j);       // tempChar is the character and the current index
                manipulatedBoard.at(j) = previousChar;   // store the previous character at the current index
                previousChar = tempChar;                 // Now  previousChar stores the character that was at the current index for the next iteration
            }
        }
    } else {                                // This algorithm uses the same logic as the one above, but with different j values
        
        for (int i = 0; i < numOfShiftsAbs; i++) {
            
            previousChar = manipulatedBoard.at(columnNum);
            
            for (int j = columnNum + boardSize*(boardSize - 1); j >= columnNum; j = j - boardSize ) {
                
                tempChar = manipulatedBoard.at(j);
                manipulatedBoard.at(j) = previousChar;
                previousChar = tempChar;
            }
        }
    }
}

vector<char> scrambleBoard(vector<char> const &unscrambledBoard, int boardSize, int numTimesToScramble) { // This functiion returns the scrambled board
    
    vector<char> scrambledBoard;
    
    for (int i = 0; i < unscrambledBoard.size(); i++) {             // Make a copy of unscrambledBoard
        scrambledBoard.push_back(unscrambledBoard.at(i));
    }
    
    // This loop generates random rows and columns, rotates them (in that order), and repeats numTimesToScramble times
    for (int i = 0; i < numTimesToScramble; i++) {
        
        int randomRow = rand() % boardSize;
        rotateRow(scrambledBoard, boardSize, randomRow, 1);
        
        int randomCol = rand() % boardSize;
        rotateCol(scrambledBoard, boardSize, randomCol, 1);
    }
    return scrambledBoard;
}


void rotateRowOrColum(vector<char> &manipulatedBoard, int boardSize, LinkedList &undoList) {

    // Store the current state before making changes
    undoList.pushState(manipulatedBoard);

    char rowOrColumn;
    int rowOrColumnNum;
    int numOfPositionsToRotate;
    
    do {
        cout << "   Enter the row/column you would like to rotate, and the number of positions to rotate by." << endl;
        cout << "   This should be in the format of <R or C> <row/column number> <number of positions to rotate>, " << endl;
        cout << "   where valid row and column numbers are between 0 and " << boardSize - 1 << ", " << endl;
        cout << "   E.g. R 0 1 would rotate the top row (row 0) of the board to the right once, " << endl;
        cout << "        c 1 -2 would rotate the second column (col 1) of the board upwards twice. " << endl;
        
        cin >> rowOrColumn;
        rowOrColumn = tolower(rowOrColumn);
        cin >> rowOrColumnNum;
        cin >> numOfPositionsToRotate;
        
        cout << "   Your choice: ";
        if (rowOrColumn != 'r' && rowOrColumn != 'c' ) {
            cout << "   First input must be 'R' or 'C'. Try again." << endl;
            cout << endl;
            continue;
        }
        
        if ( 0 > rowOrColumnNum || rowOrColumnNum > (boardSize - 1) ) {
            cout << "   Number must be between 0 and " << boardSize - 1 << ". Try again." << endl;
            cout << endl;
            continue;
        }
        
    } while((rowOrColumn != 'r' && rowOrColumn != 'c') || (0 > rowOrColumnNum || rowOrColumnNum > (boardSize - 1)));
    
    // If we pass the input filters, we can now rotate a row or a column
    if (rowOrColumn == 'r') {
        rotateRow(manipulatedBoard, boardSize, rowOrColumnNum,numOfPositionsToRotate);
    } else {            // rowOrColumn == 'c'
        rotateCol(manipulatedBoard, boardSize, rowOrColumnNum,numOfPositionsToRotate);
    }
}

bool checkWin (vector<char> const &manipulatedBoard, vector<char> const &unscrambledBoard) {
    
    for (int i = 0; i < manipulatedBoard.size(); i++) {
        if(manipulatedBoard.at(i) != unscrambledBoard.at(i)) {  // As soon as we find an mismatch, return false
            return false;
        }
    }
    return true;            // If we made it through the entire vector, then manipulatedBoard and unscrambledBoard are equal. Thus, return true
}
/*
bool solveBoardRecursive(vector<char>& currentBoard, const vector<char>& completedBoard, int boardSize, int maxDepth, int currentDepth = 0) {
    // Output the current depth and the current board state
    cout << "Current depth: " << currentDepth << ", Board: ";
    displayBoardInOneLine(currentBoard);

    // Base case: check if the current board matches the completed board
    if (currentBoard == completedBoard) {
        cout << "Solution found at depth " << currentDepth << endl;
        return true;
    }

    // Base case: if max depth is reached, stop recursion
    if (currentDepth >= maxDepth) {
        return false;
    }

    // Reverse the column rotations first (undo the downward rotation in scrambling)
    for (int col = 0; col < boardSize; col++) {
        vector<char> tempBoard = currentBoard;
        rotateCol(tempBoard, boardSize, col, -1); // Rotate up
        if (solveBoardRecursive(tempBoard, completedBoard, boardSize, maxDepth, currentDepth + 1)) {
            currentBoard = tempBoard; // Update current board with the solution
            return true;
        }
    }

    // Reverse the row rotations next (undo the right rotation in scrambling)
    for (int row = 0; row < boardSize; row++) {
        vector<char> tempBoard = currentBoard;
        rotateRow(tempBoard, boardSize, row, -1); // Rotate left
        if (solveBoardRecursive(tempBoard, completedBoard, boardSize, maxDepth, currentDepth + 1)) {
            currentBoard = tempBoard;
            return true;
        }
    }

    // If no solution found, return false
    return false;
}*/

bool solveBoardRecursive(vector<char>& currentBoard, const vector<char>& completedBoard, int boardSize, int maxDepth, int currentDepth, int& iterations) {
    iterations++;
    // Output the current depth and the current board state
    cout << "Current iteration: " << iterations << endl;
    cout << "Current depth: " << currentDepth << ", Board: ";
   
    displayBoardInOneLine(currentBoard);

    // Base case: check if the current board matches the completed board
    if (currentBoard == completedBoard) {
        cout << "Solution found at depth " << currentDepth << endl;
        return true;
    }

    // Base case: if max depth is reached, stop recursion
    if (currentDepth >= maxDepth) {
        return false;
    }

    // Additional Check: Try combining row and column rotations
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            vector<char> tempBoard = currentBoard;
            
            rotateCol(tempBoard, boardSize, col, -1); // Rotate up
            rotateRow(tempBoard, boardSize, row, -1); // Rotate left
            if (solveBoardRecursive(tempBoard, completedBoard, boardSize, maxDepth, currentDepth + 1, iterations)) {
                currentBoard = tempBoard;
                return true;
            }
        }
    }

    // If no solution found, return false
    return false;
}

void solve(vector<char> manipulatedBoard, vector<char> unscrambledBoard, vector<string> randomWords, int boardSize) {
    cout << "Enter the board to be automatically solved, as a single connected string: " << endl;

    cin.ignore();       // Ignore the /n in the stream
    string str;
    getline(cin,str);

    // Convert the input string to a vector of chars
    vector<char> inputBoard(str.begin(), str.end());

    // Set a maximum depth for the recursion to avoid infinite loops
    int maxDepth = 4; // As we assume the scramble was with a value of 1, 2, 3, or 4

    cout << "Checking for a solution..." << endl;

    int iterations = 0;
    if (solveBoardRecursive(inputBoard, unscrambledBoard, boardSize, maxDepth, 0,iterations )) {
        cout << "Solution found! The complete board is: " << endl;
        displayBoard(inputBoard);
        displayRandomWords(randomWords);
        cout << "Current board: " << endl;
        displayBoard(manipulatedBoard);
    } else {
        cout << "No solution found within the allowed depth." << endl;
    }
}

int main(int argc, const char * argv[]) {
    
    char choice;
    vector<string> words;                   // To store all the words from the dictionary
    int boardSize;                          // Stores the length of one dimension of the board
    int numTimesToScramble;                 // Number of times the users wants to scramble the board at the beginning
    vector<char> unscrambledBoard;          // This vector stores the character in the right order. We need also take as a reference to determine the win
    vector<char> firstScrambledBoard;            // This vector stores the scrambled board. We might need it again if we want to reset the board.
    vector<char> tempBoard;
    vector<char> manipulatedBoard;          // This vector is the one we will manipulate to play the game
    vector<string> randomWords;             // To store the randomly generated words
    
    Board board;
    LinkedList undoList;
    
    cout << "Program 5: Unscrambler        " << endl;
    cout << "CS 141, Fall 2021, UIC        " << endl;
    cout << endl;
    
    cout << "You will be presented with a board that is filled  " << endl;
    cout << "with scrambled words. Rotate the rows and columns  " << endl;
    cout << "to get the words displayed in the right order.     " << endl << endl;
    
    readWordsIntoVector(words);
    
    cout << words.size() << " words have been read in from the dictionary." << endl << endl;
    
    do {                // This loop serves as a filter if the user wants to quit right away at the beginning
        cout << "Enter Q to quit or B to begin: ";
        
        cin >> choice;
        
        choice = tolower(choice);
        
        if (choice == 'q') {
            
            cout << "Thank you for playing! " << endl;
            cout << "Exiting program..." << endl;
            exit(0);
        }
        
    } while(choice != 'b');
    
    do {                // This loop gathers the appropriate board size which must be greater than 3 in length, and creates the unscrambled board
        cout << "Choose your board size (must be a number greater than or equal to 4): ";
        cin >> boardSize;
        
        if (boardSize < 4) {
            cout << "The board size must be at least 4. Retry." << endl;
            continue;
        }
        board.setCompletedBoard(createBoard(words,randomWords,boardSize), boardSize);
        //unscrambledBoard = createBoard(words,randomWords,boardSize);    // The board size is correct, we can now create a board
        board.setRandomWords(randomWords);
        
    } while (boardSize < 4);
    
    do {            // This loop first checks for correct number of scrambles which must be greater than 0, and srambles the board
        cout << "Choose the number of times you would like the board to be scrambled (must be a single number >= 1): ";
        cin >> numTimesToScramble;
        
        if (numTimesToScramble < 1) {
            cout << "The number of times you select must be at least 1. Retry." << endl;
            continue;
        }
        
        board.setBoard(scrambleBoard(*board.getCompletedBoard(), boardSize, numTimesToScramble), boardSize);
        //scrambledBoard = scrambleBoard(unscrambledBoard,boardSize,numTimesToScramble); // The number of times to scramble is correct, now scramble board
        
       firstScrambledBoard = *board.getBoard();
        
        displayRandomWords(*board.getRandomWords());
        cout << "Current board: " << endl;          // Display board
        displayBoard(*board.getBoard());
        
    } while(numTimesToScramble < 1);
    
    do {   // After getting through the first filters, we can now start the game
        displayMenu();
        cin >> choice;
        choice = tolower(choice);
        
        if (choice != 'q') {
            cout << "Your choice: " << endl;
        }
        
        switch (choice) {
            case 'r': {
                
                rotateRowOrColum(*board.getBoard(), boardSize, undoList); // Pass the undo list, and make rotations
         
                if (checkWin(*board.getBoard(), *board.getCompletedBoard())) {      // Check for win
                    
                    displayBoard(*board.getBoard());
                    cout << "Congratulations, you won! Thank you for playing!" << endl;
                    cout << "Exiting program..." << endl;
                    exit(0);
                    
                } else {
                    
                    displayRandomWords(*board.getRandomWords());
                    cout << "Current board: " << endl;
                    displayBoard(*board.getBoard());
                }
                break;
            }
            case 'c': {                                               // Display the current state of the board
                
                cout << "The completed board should look like:" << endl;
                displayBoard(*board.getCompletedBoard());
                displayRandomWords(*board.getRandomWords());
                
                cout << "Current board: " << endl;
                displayBoard(*board.getBoard());
                
                break;
            }
            case 'b': {
                
                board.setBoard(firstScrambledBoard, boardSize);
                //manipulatedBoard = firstScrambledBoard;      // Set the manipulatedBoard equal to the one we started with at the beginning
                undoList = LinkedList();
                displayRandomWords(*board.getRandomWords());
                
                cout << "Current board: " << endl;
                displayBoard(*board.getBoard());
                
                break;
            }
            case 'g': {
                
                board.setCompletedBoard(createBoard(words, randomWords, boardSize), boardSize);
                //unscrambledBoard = createBoard(words,randomWords,boardSize);                    // Create a new unscrambledBoard
                
                board.setBoard(scrambleBoard(*board.getCompletedBoard(),boardSize,numTimesToScramble), boardSize);
               // firstScrambledBoard = scrambleBoard(unscrambledBoard,boardSize,numTimesToScramble);  //  Create a new scrambledBoard
                
                firstScrambledBoard.clear();                               // Clear the previous manipulatedBoard to use it again
                
                for (int i = 0; i < firstScrambledBoard.size(); i++) {
                    firstScrambledBoard.push_back((*board.getBoard()).at(i));       // Make a copy of the scrambled board. manipulatedBoarad is the one we use for the game
                }
                displayRandomWords(*board.getRandomWords());
                
                cout << "Current board: " << endl;
                displayBoard(*board.getBoard());
                
                break;
            }
            case 's': {
                
                  solve(*board.getBoard(), *board.getCompletedBoard(), *board.getRandomWords(), boardSize);    // Automatically solve the board
                  break;
            }

            case 'u': { // Undo last move
                if (!undoList.isEmpty()) {
                    *board.getBoard() = undoList.popState();
                    displayBoard(*board.getBoard());
                } else {
                    cout << "No moves to undo!" << endl;
                }
                break;
            }
                
            default:
                break;
        }
    } while (choice != 'q');
    
    cout << "Your choice: ";
    cout << "Thank you for playing!" << endl;
    cout << "Exiting program..." << endl;
    
}

