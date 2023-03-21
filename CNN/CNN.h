#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Channel.h"
#include "Kernel.h"
using namespace std;

class ConvNetwork
{
    public:
        // Initialize the neural network by feeding it a vector of matrices (input channels).
        ConvNetwork(vector <vector <vector <float>>> input);

        vector <vector <vector <float>>> inputMatrices;
        int convolvedFeatureStride = 1;
        int maxPoolStride = 1;

        vector <Channel> inputChannels;
        vector <Kernel> kernels;
        vector <vector <vector <float>>> convolvedFeatures;
        vector <vector <vector <float>>> maxPoolChannels;
        vector <vector <float>> output;

       
        void initializeInputChannels(); // Turn input matrices into channels.
        void initializeKernels(); // Initialize kernels with random values. Default size 3x3.
        void initializeConvolvedFeatures(); // Initialize convolved features using channels and their kernels.
        void initializeMaxPoolLayer(int maxPoolMatrixHeight=2, int maxPoolMatrixWidth=2); // Initialize maxPool layer with default channel size 2x2.
        vector <vector <float>> findOutput(); // Find sum of all maxPool Channels.
        
        void clearAll(); // Clears
        void clearInputChannels(); // Clears input channels.
        void clearKernels(); // Clears kernels.
        void clearConvolvedFeatures(); // Clears convolved features.
        void clearMaxPoolLayer(); // Clears maxPool Channels.


        // Other functions:
        // For initializeConvolvedFeatures() and initializeMaxPoolLayer():
        int findNumOfDividends(int start, int end, int stride);
        vector <vector <vector <float>>> extractPartialMatrices(Channel inputChannel, int partialMatrixHeight, int partialMatrixWidth, int stride);

        // For initializeConvolvedFeatures():
        float KernelxPartialMatrix(vector <vector <float>> partialMatrix, Kernel kernel);
        vector <vector <float>> initConvolvedFeature(Channel inputChannel, Kernel kernel);

        // For initializeMaxPoolLayer():
        vector <vector <float>> initMaxPool(vector <vector <float>> convFeatMatrix, int maxPoolMatrixHeight, int maxPoolMatrixWidth);
        float findVectorMax(vector <float> vectorx);
        float findMatrixMax(vector <vector <float>> matrix);

        void printMatrix(vector <vector <float>> matrix);
        vector <vector <vector <float>>> readInputFromFile(string fileName);
        


};

