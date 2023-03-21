// Grundlagen der Bioinformatik (Praktikum)
// Yelyzaveta Kornieieva: 6980513
// Aufgabenblatt 03
// Aufgabe 3.2

#include <iostream>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;

// Constants
const unsigned int N =  4;
const int X = pow(2, N) + 1;

const float MIN = 0;
const float MAX = 20;


// Variables
float x[X];
float y[X][X];


// This function is used to init random number by some seed
// every time random numbers will be different
void initRandomFunction (int seed=-1)
{
	if (seed == -1)
		srand(time(NULL));
	else
		srand(seed);
}

// Call of this function returns random float number in range from min to max
// for example "randomFloat(0, 20);"
float randomFloat (float min, float max)
{
    float r = (float) rand() / RAND_MAX;
    return min + r * (max - min);
}


// This function prints==shows all values in array "x" and "y"
void printAllArrays (string text="")
{
    //cout << "\n";
    if (text != "")
        cout << text << "\n";
    for (int i = 0; i < X; i++)
    {
     for(int j = 0; j < X; j++){   
        cout <<" "<< y[i][j];}
        cout << "\n";
    }
    for (int i = 0; i < X; i++)
    {
     for(int j = 0; j < X; j++){   
        cout <<" "<< i+1 << " " << j+1 <<" "<<  y[i][j] << "\n";}
    }
    //cout << "\n";
}


// This function changes y[i] on some random value between 0 and 20 and multiplies it by "k"
// "k" is ñoefficient 
// For example:
// y[5] = 10;
// varyWithId(5, 0.25); // this chages y[5] by some random value, scatter==spread == 0.25*randomFloat(0, 20)
// cout << y[5] << "\n"; // may be from 10-0.25*(20/2) to 10+0.25*(20/2)
void varyWithId (int i,int j, float k)
{
    float tmpRand = k*randomFloat(-MAX/2, MAX/2);
    float tmpY = y[i][j] + tmpRand;
    if (MIN < tmpY && tmpY < MAX)
    {
        y[i][j] = tmpY;
    }
    else
    {
        y[i][j] = y[i][j] - tmpRand;
    }
}
void squareStep(int x1, int y1, int half, float k){
        int count = 0;
        float avg = 0.0;
        if (x1 - half >= 0)
        {
         avg += y[x1-half][y1];
         count++;
         }
        if (x1 + half < X)
        {
        avg += y[x1+half][y1];
        count++;
        }
        if (y1 - half >= 0)
         {
         avg += y[x1][y1-half];
         count++;
        }
        if (y1 + half < X)
        {
         avg += y[x1][y1+half];
         count++;
        } 
         avg /= count;
         y[x1][y1] = avg;
         varyWithId(x1,y1,k);

}


// Vary values in range [from;to)
// [from;to) means {from, from+1, from+2, ..., to-2, to-1}
// Last element isn't included, because array is indexed from zero
// If range to small: finish this sub branch
// If range==2or3: change values in this range and finish this sub branch
// If range bigger then 3: divide it by two sub branches and start them (where we get recursion)
void varyInRange (int size = X, int depth = 0)
{
    // Åhis can be used to see all arrays on each step
    // string text =
    // "depth = " + to_string(depth) +
    // "\nfrom = " + to_string(from) +
    // "\nto = " + to_string(to);
    // printAllArrays(text);

    if (size < 2)
    {
        // If range to small, then return==finish
        return;
    }

    float k = 0;
    switch (depth) {
        case 0:
            k = 1.0;
            break;
        case 1:
            k = 0.5;
        default:    // means else
            //k = 1./4;
            k = pow(0.5, depth);
            break;
    }
    int half = size/2;
    //diamond step
     
    for(int x1 = half; x1 < X; x1 += size){
        for(int y1 = half; y1 < X; y1 += size){
            y[x1][y1] = (y[x1-half][y1+half] + y[x1+half][y1-half] + y[x1-half][y1-half] + y[x1+half][y1+half])/4.0;
            varyWithId(x1,y1,k);
     }
     }       
    //square step
    int col = 0;
    for (int x1 = 0; x1 < X; x1 += half)
    {
        col++;
    //If this is an odd column.
    if (col % 2 == 1){
        for (int y1 = half; y1 < X; y1 += size){
            squareStep(x1, y1, half,k);
        }
    }
    else{
        for (int y1 = 0; y1 < X; y1 += size){
            squareStep(x1, y1, half,k);
        }
    }
 }
        varyInRange(size/2, depth+1);
}



int main() {

	// Init random numbers by system time, so they will be different every time program is launced
    initRandomFunction();

	// Edge elements set as random float numbers in range 0..20
    y[0][0] = randomFloat(MIN, MAX);
    y[X-1][0] = randomFloat(MIN, MAX);
    y[0][X-1] = randomFloat(MIN, MAX);
    y[X-1][X-1] = randomFloat(MIN, MAX);

    // Vary all values recursively
    varyInRange(X, 0);
 
    // Print all values
    //printAllArrays("finally: ");
    printAllArrays();
    system("PAUSE");
    return 0;
}
