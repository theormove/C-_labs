#include <iostream>
#include <cmath>
#include <ctime>
#include <tuple>
#include <fstream>
using namespace std;

class CellAut {
    public:

        // "Erstellt eine Klasse für den Zellulären Automaten, welcher Cornway’s Game of Life umsetzt.
        // Dabei soll die Feldgröße 30x30 Zellen betragen, jedoch vom Benutzer des Programmes verändert werden können."
        // Set default size of the field to 30x30.
        int fieldRows = 30;
        int fieldColumns = 30;



        // "Es sollen im Wesentlichen nur zwei Arrays verwendet werden, eines für den alten Zustand und eines für den neuen Zustand.""
        // Create two 2d dynamic arrays to store the current and the previous state of the field.
        char **currState = new char* [fieldRows];
        char **prevState = new char* [fieldRows];
        void createStateArrays() {
            for (int row = 0; row < fieldRows; row++) {
                currState[row] = new char[fieldColumns];
                prevState[row] = new char[fieldColumns];
            }
        }



        // "Der Benutzer soll den Zustand der Zellen über ihre Koordinaten einzeln setzen (lebend, tot), aber auch ändern 
        // können (lebend zu tot, tot zu lebend)."
        // l rowOfCell columnOfCell makes a cell come alive, for example l 4 4.
        // t rowOfCell columnOfCell kills a cell, for example t 4 4.
        // c rowOfCell columnOfCell changes the state of a cell, for example c 4 4.
        // A single user input can contain multiple commands, but they have to be separated by commas, for example l 0 0, l 1 1, t 3 4, c 3 4.

        // Take user input that contains commands and separate individual commands.
        void readCommand(string userInp) {

            // Save current state before executing changes.
            currToPrev();

            // Find the position of the first comma, separating the first command from the rest.
            int commaPos;
            commaPos = userInp.find(',');

            // If there is no such comma, treat user input as a single command and interpret it.
            if (commaPos < 0) {
                obeyUserCommand(userInp);

            // If there is a comma (meaning there is at least 2 individual commands in the user input), interpret the first command.
            // Then delete it from the original user input, and repeat the same process with what is left after deleting the first command.
            } else {
                obeyUserCommand(userInp.substr(0,commaPos));
                userInp = userInp.substr(commaPos + 2);
                readCommand(userInp);
            }
        }


        // Execute a single user command.
        void obeyUserCommand(string userCommand) {

            // If the user command starts with an l or L, extract the numbers from the command and mark the cell they reference as alive.
            if ((userCommand[0] == 'l') || (userCommand[0] == 'L')) {
                int rowOfCell, colOfCell;
                tie(rowOfCell, colOfCell) = extractIntsFromString(userCommand);
                currState[rowOfCell][colOfCell] = '*';
            }

            // If the user command starts with a t or T, extract the numbers from the command and mark the cell they reference as dead.
            if ((userCommand[0] == 't') || (userCommand[0] == 'L')) {
                int rowOfCell, colOfCell;
                tie(rowOfCell, colOfCell) = extractIntsFromString(userCommand);
                currState[rowOfCell][colOfCell] = 'o';
            }

            // If the user command starts with a c or C, extract the numbers from the command and change the state of the cell.
            if ((userCommand[0] == 'c') || (userCommand[0] == 'L')) {
                int rowOfCell, colOfCell;
                tie(rowOfCell, colOfCell) = extractIntsFromString(userCommand);
                if (prevState[rowOfCell][colOfCell] == 'o') {
                    currState[rowOfCell][colOfCell] = '*';
                } else {
                    currState[rowOfCell][colOfCell] = 'o';
                }
            }
        }


        // Take a user command and extract the numbers from it.
        // The first number will be the row of the cell that the command references, the second number will be its column.
        tuple <int, int> extractIntsFromString(string toBeRead) {
            int rowOfCell;
            int colOfCell;

            // Delete the first 2 characters from the command, which can be "l ", "t ", or "c ".
            toBeRead.erase(0,2);

            // Find the position of the space character separating the two numbers.
            int spacePos;
            spacePos = toBeRead.find(' ');

            // Separate the two numbers (still as string) and store them into different variables.
            string rowCoord = toBeRead.substr(0,spacePos);
            string colCoord = toBeRead.substr(spacePos + 1);

            // Take the numbers in the string and turn it into an integer.
            rowOfCell = stoi(rowCoord, nullptr, 10);
            colOfCell = stoi(colCoord, nullptr, 10);

            return make_tuple(rowOfCell, colOfCell);
    }



        // "Erstellt eine Funktion, welche die Evolution für genau einen Zeitschritt durchführt."
        // Calculate the number of neighbours a cell has.
        int howManyNeighbours(int y, int x) {
            int numOfNeighbours = 0;

            // Check north cell.
            if (prevState[moduloRedifined((y-1), fieldRows)][x] == '*') {
                numOfNeighbours++;
            }

            // Check south cell.
            if (prevState[moduloRedifined((y+1), fieldRows)][x] == '*') {
                numOfNeighbours++;
            }

            // Check west cell.
            if (prevState[y][moduloRedifined((x-1), fieldColumns)] == '*') {
                numOfNeighbours++;
            }

            // Check east cell.
            if (prevState[y][moduloRedifined((x+1), fieldColumns)] == '*') {
                numOfNeighbours++;
            }

            // Check northwest cell.
            if (prevState[moduloRedifined((y-1), fieldRows)][moduloRedifined((x-1), fieldColumns)] == '*') {
                numOfNeighbours++;
            }

            // Check northeast cell.
            if (prevState[moduloRedifined((y-1), fieldRows)][moduloRedifined((x+1), fieldColumns)] == '*') {
                numOfNeighbours++;
            }

            // Check southwest cell.
            if (prevState[moduloRedifined((y+1), fieldRows)][moduloRedifined((x-1), fieldColumns)] == '*') {
                numOfNeighbours++;
            }

            // Check southeast cell.
            if (prevState[moduloRedifined((y+1), fieldRows)][moduloRedifined((x+1), fieldColumns)] == '*') {
                numOfNeighbours++;
            }

            return numOfNeighbours;
        }

        // Apply the rules of the Game of Life for one evoluitonary step.
        void oneEvolStep() {
            currToPrev();
            for (int row = 0; row < fieldRows; row++)
                for (int col = 0; col < fieldColumns; col++) {

                    // Vereinsamung.
                    if ((howManyNeighbours(row,col) <= 1) && prevState[row][col] == '*') {
                        currState[row][col] = 'o';
                    }

                    // Verhungern.
                    if ((howManyNeighbours(row,col) > 3) && prevState[row][col] == '*') {
                        currState[row][col] = 'o';
                    }

                    // Überleben
                    if (((howManyNeighbours(row,col) == 2) || (howManyNeighbours(row,col) == 2)) && prevState[row][col] == '*') {
                        currState[row][col] = '*';
                    }

                    // Geburt.
                    if ((howManyNeighbours(row,col) == 3) && prevState[row][col] == 'o') {
                        currState[row][col] = '*';
                    }

            }
        }

        // Create a function that calculates modulo differenty, so that 
        // a cell with negative (not valid) coordinates references back to an existing cell with positive coordinates.
        int moduloRedifined(int a, int b) {
            if (a >= 0) {
                return (a % b);
            } else {
                return (a + b);
            }
        }



        // "Erstellt eine Funktion, welche einen String erstellt, der den Zustand des Automaten in Form einer Matrix repräsentiert."
        // Turn the current state of the field and turn it into a string.
        string currStateToStr() {

            // Create an empty string.
            string currStateStr;

            // Create a variable that stores the row of the cell previous to the one that is currently being added to the string.
            int prevRow = 0;

            // Iterate over every single cell in the current field.
            for (int row = 0; row < fieldRows; row++)
                for (int col = 0; col < fieldColumns; col++) {

                    // If the row of the cell isn't the same as the row of the previous cell, start a new line.
                    if (row != prevRow) {
                    currStateStr.insert(currStateStr.length(), "\n");
                    }

                    // Add the state of existence of the current cell to the string currStateStr.
                    currStateStr.push_back(currState[row][col]);

                    // Save the row of the current cell as previous before going to the next cell.
                    prevRow = row;
                }
            return currStateStr;
        }


        // Kill off all the cells.
        void genocide() {
            for (int row = 0; row < fieldRows; row++) 
                for (int col = 0; col < fieldColumns; col++) {
                    currState[row][col] = 'o';
                }   
        }

        // Create a random field of alive and dead cells.
        void randomizeField() {
            srand(time(NULL));
            for (int row = 0; row < fieldRows; row++) 
                for (int col = 0; col < fieldColumns; col++) {
                    int aliveOrDeadRandom = rand() % 2;
                    if (aliveOrDeadRandom == 0) {
                        currState[row][col] = 'o';
                    } else {
                        currState[row][col] = '*';
                    }  
                }
        };


        // Save the current state of the field as previous (used before changing the current state).
        void currToPrev() {

        // Create a (temporary) variable, whose value will change with every iteration.
        char tempVar;
            for (int row = 0; row < fieldRows; row++)
                for (int col = 0; col < fieldColumns; col++) {

                    // The temporary variable takes on the value of a cell in the current state array.
                    tempVar = currState[row][col];

                    // The temporary variable passes its newly acquired value to the equivalent cell in the previous state array.
                    prevState[row][col] = tempVar;
                }
        }
        //Save field to file
        void saveToFile(string file){
            ofstream o(file);
            o << currStateToStr();
        }
        //Take field from file 
        void stateFromFile(string file){
            fieldRows = 0;
            string matrix;
            string line;
            ifstream fil;
            fil.open(file);
            while (! fil.eof()){
                getline(fil, line);
                matrix += line;
                fieldRows++;
                fieldColumns = line.length();
            }
            fil.close();
            createStateArrays();
            for (int row = 0; row < fieldRows; row++){
                for (int col = 0; col < fieldColumns; col++) {
                    currState[row][col] = matrix[row*fieldColumns + col];
                    currState[row][col] = matrix[row*fieldColumns + col];
                }
                }
        }   


};




int main() {
    CellAut myAut;

    // Determening size of the field (asking user wether or not they want to continue with the default size of 30x30).
    char yesOrNo;
    int numOfRows;
    int numOfCols;

    cout << "The default size of the field is 30x30." << endl;
    cout << "       PRESS Y if you want to change it." << endl;
    cout << "       PRESS ANY OTHER KEY if you want to keep the default size." << endl;
    
    cout << "\n";

    cin >> yesOrNo;
    cout << "\n";
    if ((yesOrNo == 'y') || (yesOrNo == 'Y')) {

        cout << "Type in the number of rows: ";
        cin >> numOfRows;
        myAut.fieldRows = numOfRows;

        cout << "Type in the number of columns: ";
        cin >> numOfCols;
        cout << "\n";
        myAut.fieldColumns = numOfCols;
    }
    

    // Create field.
    myAut.createStateArrays();



    // Determening initial state of the field - empty or random.
    char initState;

    cout << "\n";
    cout << "Do you want your field empty or randomized?" << endl;
    cout << "       PRESS R for randomized." << endl;
    cout << "       PRESS U if you want to upload state from file"<<endl;
    cout << "       PRESS ANY OTHER KEY for empty." << endl;
    cout << "\n";

    cin >> initState;
    cout << "\n";
    cout << "\n";
    

    if ((initState == 'r') || (initState == 'R')) {
        myAut.randomizeField();
    }else{
        myAut.genocide();
    }
    if ((initState == 'u') || (initState == 'U')) {
        cout << "Type whereabouts of the file or just the name if in current directory: ";
        string file;
        cin >> file;
        myAut.stateFromFile(file);
    }

    // Show initial state.
    cout << "Here's what your field looks like: " << endl;
    cout << myAut.currStateToStr();
    cout << "\n";
    cout << "\n";
    cout << "\n";
    cout << "\n";
    cout << "Time to play the Game of Life!" << endl;


    // Show options.
        cout << "\n";
        cout << "       PRESS E to see the next evolutionary step of the current field." << endl;
        cout << "       TYPE L ROW_NUMBER COLUMN_NUMBER to make a cell come alive, for example: l 2 2" << endl;
        cout << "       TYPE T ROW_NUMBER COLUMN_NUMBER to kill a cell, for example: t 1 44" << endl;
        cout << "       TYPE col ROW_NUMBER COLUMN_NUMBER to kill an alive cell or ressurect a dead one, for example: col 9 1" << endl;
        cout << "       YOU CAN ALSO COMBINE THESE FUNCTIONS - just separate them with commas, for example: l 2 2, t 1 44, col 9 1, t 5 5;" << endl;
        cout << "       TYPE S TO SAVE STATE";
        cout << "\n";
        cout << "\n";

    // Let the user change the field and ask to see the next evolutionary step.
    while (1) {
        string nextAction;
        getline(cin, nextAction);

        if ((nextAction == "E") || (nextAction == "e")) {
            cout << "\n";
            cout << "\n";
            myAut.oneEvolStep();
            cout << "\n";
            cout << "\n";
            cout << myAut.currStateToStr();
            cout << "\n";
            cout << "\n";
        }
        if ((nextAction == "S") || (nextAction == "s")) {
            cout << "Type whereabouts of the file or just the name if in current directory: ";
            string file;
            cin >> file;
            myAut.saveToFile(file);
            cout << "File was saved successfully!";     

        }


         else {
            myAut.readCommand(nextAction);
            cout << "\n";
            cout << "\n";
            cout << myAut.currStateToStr();
            cout << "\n";
            cout << "\n";
        }


    }

    return 0;

}