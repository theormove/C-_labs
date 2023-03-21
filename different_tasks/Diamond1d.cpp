/**
 * Diamond1d
 *
 */



#include <iostream>
#include <string>
#include <cmath>

using namespace std;



// Constants
const unsigned int N = 10;
const int X = pow(2, N) + 1;

const float MIN = 0;
const float MAX = 20;



// Variables
float x[X];
float y[X];



// this function is used to init random by some seed
// for example "initRandomFunction();" means
// init random numbers by your system time miliseconds
// so every time random numbers will be different
void initRandomFunction (int seed=-1)
{
	if (seed == -1)
		srand(time(NULL));
	else
		srand(seed);
}



// calling this function returns random float number in range from min to max
// for example "randomFloat(0, 20);"
// will give you some random number between 0 and 20
float randomFloat (float min, float max)
{
    float r = (float) rand() / RAND_MAX;
    return min + r * (max - min);
}



// this function prints==shows all values in array "x" and "y"
// parameter text isn't necessary to give to function
void printAllArrays (string text="")
{
    //cout << "\n";
    if (text != "")
        cout << text << "\n";
    for (int i = 0; i < X; i++)
    {
        cout << x[i] << " " << y[i] << "\n";
    }
    //cout << "\n";
}



// this function changing y[i] on some random value between 0 and 20 and multiplying by "k"
// "k" is koeficient
// for example:
//
// y[5] = 10;
// varyWithId(5, 0.25); // this is chaging y[5] by some random value, scatter==spread == 0.25*randomFloat(0, 20)
// cout << y[5] << "\n"; // may be from 10-0.25*(20/2) to 10+0.25*(20/2)
void varyWithId (int i, float k)
{
    float tmpRand = k*randomFloat(-MAX/2, MAX/2);
    float tmpY = y[i] + tmpRand;
    if (MIN < tmpY && tmpY < MAX)
    {
        y[i] = tmpY;
    }
    else
    {
        y[i] = y[i] - tmpRand;
    }
}



// vary values in range [from;to)
// [from;to) means {from, from+1, from+2, ..., to-2, to-1}
// last element isn't included, because array are indexed from zero
//
// this function is works recusively like that:
// if range to small: finish this sub branch
// if range==2or3: change values in this range and finish this sub branch
// if range bigger then 3: divide it by two sub branches and start them (this where we get recursion)
void varyInRange (int from, int to, int depth)
{
    // this can be used to see all arrays on each depth==step
    // string text =
    // "depth = " + to_string(depth) +
    // "\nfrom = " + to_string(from) +
    // "\nto = " + to_string(to);
    // printAllArrays(text);

    if (to - from < 2)
    {
        // if range to small, then return==finish
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

    if (to - from == 2)
    {
        y[from+1] = 0.5*y[from] + 0.5*y[from+2];
        varyWithId(from+1, k);
    }
    else if (to - from == 3)
    {
        y[from+1] = 0.666*y[from] + 0.333*y[from+3];
        varyWithId(from+1, k);

        y[from+2] = 0.333*y[from] + 0.666*y[from+3];
        varyWithId(from+2, k);
    }
    else //if (to - from > 3)
    {
        int middle = (from + to) / 2;

        y[middle] = 0.5*y[from] + 0.5*y[to];
        varyWithId(middle, k);

        varyInRange(from, middle, depth+1);
        varyInRange(middle, to, depth+1);
    }

}



// this is main function ;)
// it's called when program starts
int main() {

    // init random numbers by system time, so they will different every time program is launced
    initRandomFunction();

    // initializing arrays
    // x = {0, 1, 2, 3, 4, ..., X-1}
    // y = {0, 0, 0, 0, 0, ..., 0}
    for (int i = 0; i < X; i++)
    {
        x[i] = i;
        y[i] = 0;
    }

    // edge elements set as random float numbers in range 0..20
    y[0] = randomFloat(MIN, MAX);
    y[X-1] = randomFloat(MIN, MAX);

    // vary all values recursively
    varyInRange(0, X, 0);

    // print all values
    printAllArrays();

    // finish ;)
    return 0;
}
