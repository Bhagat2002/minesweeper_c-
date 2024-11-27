#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 9;  // Rows in the grid
const int COLS = 9;  // Columns in the grid
const int MINES = 10; // Number of mines in the grid

char displayGrid[ROWS][COLS];  // Grid shown to the player
char actualGrid[ROWS][COLS];   // Grid with mines and numbers

// Directions for the 8 surrounding cells (for checking neighbors)
int dx[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
int dy[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

// Function to check if a given cell is within the grid
bool isValid(int row, int col) {
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

// Function to print the current state of the game (displayGrid)
void printGrid() {
    cout << "    ";
    for (int i = 0; i < COLS; i++) {
        cout << i << " ";
    }
    cout << endl;
    cout << "   -------------------" << endl;
    for (int i = 0; i < ROWS; i++) {
        cout << i << " | ";
        for (int j = 0; j < COLS; j++) {
            cout << displayGrid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Function to place mines randomly in the grid
void placeMines() {
    int placedMines = 0;
    while (placedMines < MINES) {
        int row = rand() % ROWS;
        int col = rand() % COLS;
        if (actualGrid[row][col] != '*') {
            actualGrid[row][col] = '*';
            placedMines++;
        }
    }
}

// Function to calculate the number of mines surrounding each cell
void calculateNumbers() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (actualGrid[row][col] == '*') {
                continue;
            }
            int minesCount = 0;
            for (int i = 0; i < 8; i++) {
                int newRow = row + dx[i];
                int newCol = col + dy[i];
                if (isValid(newRow, newCol) && actualGrid[newRow][newCol] == '*') {
                    minesCount++;
                }
            }
            actualGrid[row][col] = minesCount + '0'; // Store the number as a character
        }
    }
}

// Function to reveal cells recursively (if the cell has 0 neighboring mines)
void reveal(int row, int col) {
    if (!isValid(row, col) || displayGrid[row][col] != '-') {
        return;
    }

    displayGrid[row][col] = actualGrid[row][col];

    if (actualGrid[row][col] == '0') {
        for (int i = 0; i < 8; i++) {
            reveal(row + dx[i], col + dy[i]);
        }
    }
}

// Function to check if the player has won (all non-mine cells are revealed)
bool checkWin() {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (displayGrid[row][col] == '-' && actualGrid[row][col] != '*') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    srand(time(0));

    // Initialize display grid and actual grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            displayGrid[i][j] = '-';
            actualGrid[i][j] = '0';
        }
    }

    placeMines();        // Place mines on the actual grid
    calculateNumbers();   // Calculate the numbers (neighboring mines) for each cell

    bool gameOver = false;
    while (!gameOver) {
        printGrid();
        int row, col;
        char action;

        cout << "Enter row and column (e.g. 3 4), then action ('r' to reveal, 'f' to flag/unflag): ";
        cin >> row >> col >> action;

        if (!isValid(row, col)) {
            cout << "Invalid position! Try again.\n";
            continue;
        }

        if (action == 'r') {  // Reveal action
            if (actualGrid[row][col] == '*') {
                cout << "Game Over! You hit a mine!\n";
                printGrid(); // Show final grid
                break;
            } else {
                reveal(row, col);
                if (checkWin()) {
                    cout << "Congratulations! You won the game!\n";
                    gameOver = true;
                }
            }
        } else if (action == 'f') {  // Flag or unflag action
            if (displayGrid[row][col] == 'F') {
                displayGrid[row][col] = '-';
            } else {
                displayGrid[row][col] = 'F';
            }
        }
    }

    return 0;
}