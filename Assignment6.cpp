/*
Program Name: EECS 348 Assignment 6
Description: Brute force Puzzle Solving Program
Inputs: Test files labled puzzleX.txt where every X is a number in [1, 5]
Output: Solved puzzles (All solutions)

Collaborators: Code generated by Deepseek AI, Annotated and Dubugged by Author

Author: Aiman Boullaouz
Creation Date: 4/9/2025
*/

#include <iostream>    // standard input / output
#include <fstream>     // used for file interaction
#include <sstream>     // used for manipulating strings
#include <vector>      // imports vectors (dynamic array)
#include <string>      // More string operations
using namespace std;   // Setting the standard namespace for the rest of the file

// Struct to represent a Sudoku grid
struct SudokuGrid {
    int cells[9][9];   // nested array (9 x 9) to store Sudoku grid cells
};

// Class to solve Sudoku puzzles
class SudokuSolver {
private:
    int grid[9][9];           // 2D array to store the Sudoku grid
    vector<SudokuGrid> solutions;  // Vector to store solutions found

    // Defining a helper function that finds the next empty cell in the sudoku grid
    bool findEmptyCell(int &row, int &col) {
        for (row = 0; row < 9; row++) {             // Iterate over the rows
            for (col = 0; col < 9; col++) {         // Iterate over the columns
                if (grid[row][col] == 0) {          // Look at the element at the specific index
                    return true;  // Return True when an empty cell is found
                }
            }
        }
        return false;  // No empty cell found
    }

    // Helper function to check if placing a number at a cell is valid
    bool isValid(int row, int col, int num) {
        // Checking the row
        for (int c = 0; c < 9; c++) {        // Iterate over the columns
            if (grid[row][c] == num && c != col) {    
                return false;  // the number already exists in the same row
            }
        }
        // Check column
        for (int r = 0; r < 9; r++) {         //Iterate over the rows
            if (grid[r][col] == num && r != row) {
                return false;  // num already exists in the same column
            }
        }

        // Calculate the starting row of the 3x3 box
        int startRow = row - row % 3;
        // Calculate the starting column of the 3x3 box
        int startCol = col - col % 3;

        // Loop through all 9 cells of the 3x3 box
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                // Compute the actual row and column in the grid being checked
                int currentRow = startRow + r;
                int currentCol = startCol + c;

                // If the number already exists in this cell and it's not the original (row, col) cell
                if (grid[currentRow][currentCol] == num && (currentRow != row || currentCol != col)) {
                    return false;  // Placement of 'num' is invalid because it's already in the same 3x3 box
                }
            }
        }
        return true;  // Placement of 'num' is valid
    }

public:
    // Constructor to initialize the SudokuSolver with an initial grid
    SudokuSolver(int initialGrid[9][9]) {
        // Copy initial grid into the solver's grid
        for (int i = 0; i < 9; i++) {            // Iterate over rows
            for (int j = 0; j < 9; j++) {        // Iterate over columns
                grid[i][j] = initialGrid[i][j];  //Copy one cell over to the other
            }
        }
    }

    // Function to solve the Sudoku puzzle recursively
    void solve() {
        int row, col; 
        // Find the next empty cell in the grid
        if (!findEmptyCell(row, col)) {
            // If no empty cell is found, a solution is found
            SudokuGrid solution;
            // Copy the grid into the solution structure
            for (int i = 0; i < 9; i++) {               //Iterate over rows
                for (int j = 0; j < 9; j++) {           //Iterate over columns
                    solution.cells[i][j] = grid[i][j];  //Copy one cell over to the other
                }
            }
            solutions.push_back(solution);  // Store the solution
            return;  // Return to find other solutions if they exist
        }

        // Try placing numbers 1-9 in the empty cell
        for (int num = 1; num <= 9; num++) {
            if (isValid(row, col, num)) {
                grid[row][col] = num;  // Place num in the empty cell
                solve();               // Recursively solve the remaining grid
                grid[row][col] = 0;    // Backtrack: reset the cell for future calls
            }
        }
    }

    // Function to retrieve solutions found
    const vector<SudokuGrid>& getSolutions() const {
        return solutions;
    }

    // Function to print the original grid
    void printOriginal() const {
        printGrid(grid);  // Call static function to print grid
    }

    // Static function to print any given Sudoku grid
    static void printGrid(const int grid[9][9]) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (grid[i][j] == 0) {
                    cout << "_ ";   // Print '_ 'for empty cells
                } else {
                    cout << grid[i][j] << " ";  // Print the number
                }
            }
            cout << endl;  // Move to the next line after each row
        }
    }
};

// Function to read a Sudoku grid from a file
bool readSudokuGridFromFile(const string& filename, int grid[9][9]) {
    ifstream file(filename);  // Try to open file for reading
    if (!file.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return false;  // Return false if file cannot be opened
    }

    string line;    //Creating a buffer to store chars in from file
    int row = 0;
    // Read each line of the file
    while (getline(file, line) && row < 9) {
        istringstream iss(line);  // Use stringstream to parse the line
        vector<string> tokens;
        string token;
        // Split line into tokens (numbers or '_')
        while (iss >> token) {
            tokens.push_back(token);
        }
        // Each line should have exactly 9 elements (digits or '_')
        if (tokens.size() != 9) {
            cerr << "Error: Line " << row + 1 << " in file '" << filename 
                 << "' does not contain 9 elements." << endl;
            return false;  // Return false for invalid line format
        }
        // Process each token in the line
        for (int col = 0; col < 9; col++) {
            string& token = tokens[col];
            if (token.size() != 1) {
                cerr << "Error: Invalid element '" << token << "' in file '" << filename 
                     << "' at line " << row + 1 << ", column " << col + 1 << endl;
                return false;  // Return false for invalid token format
            }
            char c = token[0];
            if (c == '_') {
                grid[row][col] = 0;  // Convert '_' to 0 in the grid
            } else if (isdigit(c)) {
                int num = c - '0';    // Convert char to integer
                if (num < 1 || num > 9) {
                    cerr << "Error: Invalid digit '" << c << "' in file '" << filename 
                         << "' at line " << row + 1 << ", column " << col + 1 << endl;
                    return false;  // Return false for out-of-range digit
                }
                grid[row][col] = num;  // Assign valid number to the grid
            } else {
                cerr << "Error: Invalid character '" << c << "' in file '" << filename 
                     << "' at line " << row + 1 << ", column " << col + 1 << endl;
                return false;  // Return false for invalid character
            }
        }
        row++;  // Move to the next row in the grid
    }

    // Check if exactly 9 lines are read from the file
    if (row != 9) {
        cerr << "Error: File '" << filename << "' contains " << row << " lines, but 9 are required." << endl;
        return false;  // Return false for incorrect number of lines
    }

    return true;  // Sudoku grid successfully read from file
}

// Main function to execute the Sudoku solver
int main() {
    // Loop through the 5 puzzle files
    for (int i = 1; i <= 5; ++i) {
        string filename = "puzzle" + to_string(i) + ".txt";  // Constructing the filename iteratively
        int grid[9][9] = {0};  // Initialize grid with zeros

        cout << "Processing " << filename << ":" << endl;  // Print processing message
        // Attempt to read Sudoku grid from file
        if (!readSudokuGridFromFile(filename, grid)) {
            cout << "Skipping due to errors." << endl << endl;  // Skip if errors occur
            continue;  // Continue to the next puzzle file
        }

        SudokuSolver solver(grid);  // Create instance of SudokuSolver with the grid
        cout << "Original puzzle:" << endl;
        solver.printOriginal();  // Print the original Sudoku grid
        cout << endl;

        solver.solve();  // Solve the Sudoku puzzle
        vector<SudokuGrid> solutions = solver.getSolutions();  // Get number of solutions found

        // Print solutions if found, otherwise print no solution
        if (solutions.empty()) {
            cout << "No solution found." << endl;
        } else {
            cout << "Solutions found: " << solutions.size() << endl;
            // Print each solution
            for (size_t j = 0; j < solutions.size(); ++j) {
                cout << "Solution " << j + 1 << ":" << endl;
                SudokuSolver::printGrid(solutions[j].cells);
                cout << endl;
            }
        }

        cout << "------------------------------------------" << endl << endl;
    }

    return 0;  // Exit program
}
