#include <string>
#include <vector>
using namespace std;

class Channel
{
    public:

        vector <vector <float>> matrix;

        // Verschiedene Initialisierungen:
        Channel(int height, int width); // Initialisiert einen Channel, in dem alle Werte gleich 0 sind.
        Channel(vector <vector <float>> inputMatrix); // Initialisiert einen Channel mit einer Input-Matrix.

        // Der Channel kann einen Namen (z.B. "red", "green", "blue") oder eine Nummer haben.
        string channelName;
        int channelNumber;

        // Teilaufgabe 1b)
        vector <vector <float>> getMatrix();
        void setMatrix(vector <vector <float>> newMatrix);
        void addToMatrix(vector <float> newRow);
        void clearChannel();

        void printMatrix();
        void setName(string name);
        string getName();
        void setNumber(int number);
        int getNumber();

        // Teilaufgabe 1c)
        float activationFunc(float value);


};
