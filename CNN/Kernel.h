
#include <vector>
using namespace std;

class Kernel
{
    public:
        vector <vector <float>> matrix;
        //initializing with random weights
        Kernel(int height = 3, int width = 3);
        //Reset weights if false to 0 
        void resetWeights(bool random = true);

};
