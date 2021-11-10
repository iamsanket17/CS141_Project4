//  vexed.cpp

//     Move pieces to be adjacent.  Once adjacent pieces match, they are cleared off the board.  Once the entire
//     board is clear, play advances to the next level.
     
//  Prog 4: Vexed
//  Author: Dale Reed
//  Using Codio, for UIC CS 141, Fall 2019
//  Sanket Patel

#include <iostream>   // For cin, cout
#include <fstream>    // For file input
#include <chrono>     // Used in pausing for some milliseconds using sleep_for(...)
#include <thread>     // Used in pausing for some milliseconds using sleep_for(...)
#include <vector>
using namespace std;


// Global constants
const int MaxNumberOfBoards = 118;
const int BoardRows = 8;
const int BoardColumns = 10;
const int NumberOfPresetBoardParValues = 60;    // After the first 60 par values, the default par value is 15
const char FileName[] = "boards.txt";
const int SleepAmount = 150;                    // Length of time to pause, in milliseconds
const char change_Array[10] = {' ', '&', '@', '+', '%', '^', '#', '=', '*', '.'};
const char char_Array[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
const int movecount = 0;




//------------------------------------------------------------------------------------------------------------------
// Class used to store a game board
class Board
{
    public:
        // Constructor declarations 
        Board();
        Board(int level, int name[], int par);
        void displayBoard();
        void swaps(int name[]);
        void gravitySpecificChar(int pos, int name[]);
        void storeVector(int name[]);
        int setLevel();
        
        
    private:
           int level;        //Counts the level of board
           int name[BoardRows * BoardColumns];  //1D Array for displaying the board (Single Board)
           int par;          //Number of moves in which the level must be completed
           vector <int> storeIndex;
           vector <int> storeCharacters;
};  // end class Board


//Constructors
Board :: Board() : level(-1), par(-1) { //default constructor set to -1.
    for( int i = 0; i < name[BoardRows * BoardColumns]; i++) {
        name[i] = -1;
    }
}


Board :: Board(int level_p, int name_p[BoardRows * BoardColumns], int par_p) : level(level_p), par(par_p) {    //fully qualified constructor.
    for( int i = 0; i < BoardRows * BoardColumns; i++) {
        name[i] = name_p[i];
    }
}


void Board :: displayBoard() {
    int j = 0;
    cout << "\n    Board " << level << " par " << par << endl;
    cout << "      0 1 2 3 4 5 6 7 8 9  " << endl;
    cout << "    -----------------------  " << endl;
    for( int i = 1; i <= 80; i++) {
        if(i % 10 == 1) {
            cout << "  " << char_Array[j] << " | ";
        }
        cout << change_Array[name[i - 1]] << " ";  
        if( i % 10 == 0) {
            cout << "| " << char_Array[j] << endl;
            j++;
        }    
    }
    cout << "    -----------------------  " << endl;
    cout << "      0 1 2 3 4 5 6 7 8 9  " << endl;
}


void Board :: swaps(int name[]) {
    int temp;
    for(int k = 0; k <= 7; k++) {
        for(int i = 0; i <= 7; i++) {
            for(int j = 0; j <= 9; j++) {
                if(change_Array[name[i * 10 + j]] != '.' && change_Array[name[i * 10 + j]] != ' ') {
                    if(change_Array[name[i * 10 + j + 10]] == ' ') {
                        temp = name[i * 10 + j + 10];
                        name[i * 10 + j + 10] = name[i * 10 + j];
                        name[i * 10 + j] = temp;
                        displayBoard();
                    }
                }
            }
        }
    }
}


void Board :: gravitySpecificChar(int pos, int name[]) {
    int temps, counter = 10, counter2 = 0;
    
    while(change_Array[name[pos + counter]] == ' ') {
        temps = name[pos + 10];
        name[pos + 10] = name[pos + counter2];
        name[pos + counter2] = temps;
        displayBoard();
        temps = name[pos + 20];
        name[pos + 20] = name[pos + 10];
        name[pos + 10] = temps;
        counter = counter + 10;
        displayBoard();
    }
}


void Board :: storeVector(int name[]) {
    for(int i = 11; i <= 69; i++) {
        if((name[i] == 0) || (name[i] == 9)) {
            continue;
        }
        else if ((name[i] == name[i - 1]) || 
                 (name[i] == name[i - 10])|| 
                 (name[i] == name[i + 1]) || 
                 (name[i] == name[i + 10])) 
        {
            storeIndex.push_back(i);
            storeCharacters.push_back(i);
        }
    }
}


int Board :: setLevel() {
    int column = 0, tf = 1, tf2 = 1, temp;
    char row, col, rorl;
    
    
    cout << "1. Your move: ";
    cin >> row;


    row = toupper(row);
    
    if(row == 'X') {
        return 1;
    }
    else if(row == 'R') {
        return 2;
    }
    else if(row == 'S') {
        return 3;
    }
    else {
        cin >> col >> rorl;
        rorl = toupper(rorl);
        
        column = col - 48;
        
        while(row < 65 || row > 72 || column < 0 || column > 9) {
            cout << "Move value is out of bounds. Please retry." << endl;
            tf = 0;
            break;
            setLevel();
        }


        while(tf == 1 && (change_Array[name[(column + 1) + ((row - 'A') * 10) - 1]] == ' ' || change_Array[name[(column + 1) + ((row - 'A') * 10) - 1]] == '.')) {
            cout << "Attempting to move an invalid character.  Please retry." << endl;
            tf = 0;
            break;
            setLevel();
        }
            
        while(tf == 1 && rorl != 'L' && rorl != 'R') {
            cout << "Invalid move direction.  Please retry." << endl;
            tf = 0;
            break;
            setLevel();
        }


        while(tf == 1 && rorl == 'L' && change_Array[name[(column + 1) + ((row - 'A') * 10) - 2]] != ' ') {
            cout << "Attempting to move into a non-empty space.  Please retry." << endl;
            tf = 0;
            break;
            setLevel();
        }
        
        while(tf == 1 && rorl == 'R' && change_Array[name[(column + 1) + ((row - 'A') * 10)]] != ' ') {
            cout << "Attempting to move into a non-empty space.  Please retry." << endl;
            tf = 0;
            break;
            setLevel();
        }
        
        while(tf == 1) {
            if(rorl == 'L') {
                temp = name[(column + 1) + ((row - 'A') * 10) - 1];
                name[(column + 1) + ((row - 'A') * 10) - 1] = name[(column + 1) + ((row - 'A') * 10) - 2];
                name[(column + 1) + ((row - 'A') * 10) - 2] = temp;
                displayBoard();
                
                gravitySpecificChar((column + 1) + ((row - 'A') * 10), name);
                
                for(int i = 0; i < 80; i++) {
                    swaps(name);
                    storeVector(name);
                    for(int i = 0; i < storeIndex.size(); i++) {
                        //if(name[storeCharacters.at(i)] == '+') {
                            name[storeIndex.at(i)] = 0;
                            tf2 = 2;
                        //}
                    }
                    if (tf2 == 2) {
                       displayBoard();
                    }
                    
                    storeIndex.clear();
                    storeCharacters.clear();
                    tf2 = 1;
                }
                return 4;
            }


            else {
                temp = name[(column + 1) + ((row - 'A') * 10) - 1];
                name[(column + 1) + ((row - 'A') * 10) - 1] = name[(column + 1) + ((row - 'A') * 10)];
                name[(column + 1) + ((row - 'A') * 10)] = temp;
                displayBoard();
                
                gravitySpecificChar((column + 1) + ((row - 'A') * 10), name);
                
                for(int i = 0; i < 80; i++) {
                    swaps(name);
                    storeVector(name);
                    for(int i = 0; i < storeIndex.size(); i++) {
                        //if(name[storeCharacters.at(i)] == '+') {
                            name[storeIndex.at(i)] = 0;
                            tf2 = 2;
                        //}
                    }
                    if (tf2 == 2) {
                       displayBoard();
                    }
                    
                    storeIndex.clear();
                    storeCharacters.clear();
                    tf2 = 1;
                }
                return 4;
            }
            exit (-1);
        }
    }
}




//------------------------------------------------------------------------------------------------------------------
// Class used to read in and store all game boards
class AllBoards
{
    public:
        //------------------------------------------------------------------------------------------------------------------
        // Constructor that reads in data from the data file.
        AllBoards();    // Declaration.  See the definition outside the class, below.
    
        // Get and Set member functions.
    
        //------------------------------------------------------------------------------------------------------------------
        // Find and return a particular board.
        Board getBoard( int boardIndex)
        {
            // Validate board index number
            if( boardIndex < 0 || boardIndex > MaxNumberOfBoards) {
                cout << "In getBoard() inside AllBoards class, boardIndex "
                     << boardIndex << " is out of range. Exiting..." << endl;
                exit( -1);
            }
            
            // Construct a Board from one of all the boards
            Board newBoard( boardIndex, allBoards[ boardIndex], levelParValues[ boardIndex]);
            return newBoard;
        }
    
        //------------------------------------------------------------------------------------------------------------------
        // Retrieve the par value
        int getParValue( int boardIndex) {
            int theParValue = 15;   // default
            if( boardIndex < NumberOfPresetBoardParValues) {
                theParValue = levelParValues[ boardIndex];
            }
            return theParValue;
        } // end getParValue()
    
    private:
        int par = 0;             // The number of moves it should take to solve this level
        int currentLevel = -1;   // Which board we're on
        int allBoards[ 118][ BoardRows * BoardColumns];   // Array to store all 118 8x10 boards
    
        // Par values for levels 0 through 59.  Default is 15 after that.
        const int levelParValues[ NumberOfPresetBoardParValues] =
            // 0   1   2   3   4   5   6   7   8   9
            { 4,  3, 14,  5,  6,  4,  4, 14,  5, 25,  //  0 -  9
             12, 17, 17,  8,  4, 12,  9,  8, 12,  8,  // 10 - 19
             10, 10, 16, 13, 20, 14, 10,  9, 12, 14,  // 20 - 29
             15, 13, 20,  8, 15, 10, 10, 11,  7, 23,  // 30 - 39
              8, 11, 16, 14, 12, 13, 13,  3, 35, 18,  // 40 - 49
             26, 10, 13, 18, 26, 12, 15,  5, 22, 15}; // 50 - 59
}; //end class allBoards




//------------------------------------------------------------------------------------------------------------------
// AllBoards constructor
AllBoards::AllBoards()
{
    // Read in all the boards from the data file
    ifstream inputFileStream;  // declare the input file stream


    // Open input file and verify file open worked.
    inputFileStream.open( FileName);
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file. " << FileName << ".  Exiting..." << endl;
        exit( -1);
    }


    // Read the five lines of comments at the top of the datafile.  Datafile structure is:
    //    // Originally from Vexed v2.0 - globals.c "Global variable declarations"
    //    // Copyright (C) 1999 James McCombe (cybertube@earthling.net)
    //    // September 1,2001 - Version 2.0 changes by Mark Ingebretson (ingebret@yahoo.com) and others.
    //    // Oct 13, 2019  Format changed by Dale Reed (reed @ uic.edu)
    //    // Covered by the GNU General Public License https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
    //    // Level 000
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 2 1 0 0 9 9 9
    //    9 9 9 9 9 0 0 9 9 9
    //    9 9 9 0 0 0 0 9 9 9
    //    9 9 9 1 0 0 2 9 9 9
    //    9 9 9 9 1 2 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    //    // Level 001
    //    9 9 9 9 9 9 9 9 9 9
    //    9 9 9 9 9 9 9 9 9 9
    // ...
    
    // Read and discard the five lines of comments at the beginning of the file.
    string inputLine;
    for( int i=0; i<5; i++) {
        getline(inputFileStream, inputLine);
    }
    
    // Now read each of the 118 (MaxNumberOfBoards) boards.  Each one starts with a comment that has the board number.
    for( int boardIndex = 0; boardIndex < MaxNumberOfBoards; boardIndex++) {
        // Read the comment with the board number
        getline(inputFileStream, inputLine);
        
        // For each board now read the BoardRows * BoardColumns number of board values, as integers
        for( int row = 0; row < BoardRows; row++) {
            for( int col = 0; col < BoardColumns; col++) {
                inputFileStream >> allBoards[ boardIndex][ row * BoardColumns + col];
            }
            // Get rid of end-of-line character at the end of each board, so we can get a clean start on reading the
            //   first line of the next board, on the line with the comment and board number.
            getline(inputFileStream, inputLine);
        }
    }//end for( int boardIndex=0...


    // Close the input file.
    inputFileStream.close();
}//end AllBoards() constructor




//------------------------------------------------------------------------------------------------------------------
void displayDirections()
{
    cout << "\nWelcome to Vexed!  \n"
         << "The objective is to place identical pieces next to each other, so that they vanish,   \n"
         << "clearing the board completely within the indicated number of moves. On each move      \n"
         << "enter the row, column, and direction (L or R) to move, in either upper or lower       \n"
         << "case. You may also enter 'x' to exit the program, or 'r' to reset the current level. \n"
         << endl;
}




//------------------------------------------------------------------------------------------------------------------
// Driver for the program, using the classes and functions declared above
int main()
{
    AllBoards allTheBoards;     // Reads in and stores all the boards from a data file
    int currentBoardIndex = 0;  // Starting board index
    Board theBoard;             // The board instance, that is set to hold the values for each level
    int score = 0;              // Score accumulates par points for each level finished
    displayDirections();
    int selection;
    int truefalse = 1;
    int movecount = 1;
    
    theBoard = allTheBoards.getBoard( currentBoardIndex);
    theBoard.displayBoard();
    
    while(truefalse = 1) {
        switch(selection) {
            case 1:
                exit(-1);
                truefalse = 0;
            break;
            case 2:
                theBoard = allTheBoards.getBoard(currentBoardIndex);
                theBoard.displayBoard();
                movecount = 1;
            break;
            case 3:
                cin >> currentBoardIndex;
                theBoard = allTheBoards.getBoard(currentBoardIndex);
                theBoard.displayBoard();
                movecount = 1;
            break;
            case 4:
            break;
        }
        //cout << movecount++;
        selection = theBoard.setLevel();
    }
    return 0;
}
