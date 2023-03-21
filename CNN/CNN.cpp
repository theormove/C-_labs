#include "CNN.h"

ConvNetwork::ConvNetwork(vector <vector <vector <float>>> input)
{
    inputMatrices = input;
    initializeInputChannels();
    initializeKernels();
    initializeConvolvedFeatures();
    initializeMaxPoolLayer();
    output = findOutput();

    cout << "Input Channels: " << endl;
    for (int n = 0; n < inputChannels.size(); n++)
    {
        cout << "Channel #" << (n + 1) << endl;
        printMatrix(input[n]);
        cout << endl;
        cout << endl;
    }

    cout << endl;

    cout << "Kernels: " << endl;
    for (int n = 0; n < kernels.size(); n++)
    {
        cout << "Kernel #" << (n + 1) << endl;
        printMatrix(kernels[n].getMatrix());
        cout << endl;
        cout << endl;
    }

    cout << endl;

    cout << "Convolved Features: " << endl;
    for (int n = 0; n < kernels.size(); n++)
    {
        cout << "Convolved Feature #" << (n + 1) << endl;
        printMatrix(convolvedFeatures[n]);
        cout << endl;
        cout << endl;
    }

    cout << endl;

    cout << "MaxPool Matrices: " << endl;
    for (int n = 0; n < maxPoolChannels.size(); n++)
    {
        cout << "MaxPool Matrix #" << (n + 1) << endl;
        printMatrix(maxPoolChannels[n]);
        cout << endl;
        cout << endl;
    }

    cout << endl;

    cout << "Final Output: " << endl;
    printMatrix(output);

    cout << endl;
    cout << endl;

}

void ConvNetwork::initializeInputChannels()
{
    int numOfInputMatrices = inputMatrices.size();
    for (int n = 0; n < numOfInputMatrices; n++)
    {
        Channel newChannel = Channel(inputMatrices[n]);
        inputChannels.push_back(newChannel);

    }
}

void ConvNetwork::initializeKernels()
{
    int numOfInputMatrices = inputMatrices.size();
    for (int n = 0; n < numOfInputMatrices; n++)
    {
        Kernel newKernel = Kernel();
        kernels.push_back(newKernel);
    }
}

void ConvNetwork::initializeConvolvedFeatures()
{
    for (int n = 0; n < inputChannels.size(); n++)
    {
        convolvedFeatures.push_back(initConvolvedFeature(inputChannels[n], kernels[n]));
    }
}

void ConvNetwork::initializeMaxPoolLayer(int maxPoolMatrixHeight, int maxPoolMatrixWidth)
{
    for (int n = 0; n < convolvedFeatures.size(); n++)
    {
        maxPoolChannels.push_back(initMaxPool(convolvedFeatures[n], maxPoolMatrixHeight, maxPoolMatrixWidth));
    }
}

vector <vector <float>> ConvNetwork::findOutput()
{
    vector <vector <float>> output;
    vector <float> matrixSumRow;
    float matrixSumValue = 0;
    for (int j = 0; j < maxPoolChannels[0].size(); j++)
    {
        for (int i = 0; i < maxPoolChannels[0][0].size(); i++)
        {
            for (int n = 0; n < maxPoolChannels.size(); n++)
            {
                matrixSumValue += maxPoolChannels[n][j][i];
            }
            matrixSumRow.push_back(matrixSumValue);
            matrixSumValue = 0;
        }
        output.push_back(matrixSumRow);
        matrixSumRow.clear();
    }

    return output;
}

float ConvNetwork::findVectorMax(vector<float> vectorx)
{
    float maxValue;
    maxValue = vectorx[0];
    for (int n = 1; n < vectorx.size(); n++)
    {
        if (vectorx[n] > maxValue)
        {
            maxValue = vectorx[n];
        }
    }

    return maxValue;
}

float ConvNetwork::findMatrixMax(vector<vector<float> > matrix)
{
    float maxValue = findVectorMax(matrix[0]);
    for (int n = 1; n < matrix.size(); n++)
    {
        if (findVectorMax(matrix[n]) > maxValue)
        {
            maxValue = findVectorMax(matrix[n]);
        }
    }

    return maxValue;
}

int ConvNetwork::findNumOfDividends(int start, int end, int stride)
{
    int numOfDividends = 0;
    for (int n = start; n < end; n += stride)
    {
        if (n % stride == 0)
        {
            numOfDividends++;
        }
    }

    return numOfDividends;
}

vector <vector <vector <float>>> ConvNetwork::extractPartialMatrices(Channel inputChannel, int partialMatrixHeight, int partialMatrixWidth, int stride)
{
    vector <vector <float>> channelMatrix = inputChannel.getMatrix();

    vector <vector <vector <float>>> allPartialMatrices;
    vector <vector <float>> partialChannelMatrix;
    vector <float> partialChannelMatrixrow;

    for (int j = 0; j < (channelMatrix.size() - partialMatrixHeight + 1); j += stride)
        for (int i = 0; i < (channelMatrix[0].size() - partialMatrixWidth + 1); i += stride)
        {
            for (int n = j; n < (partialMatrixHeight + j); n++)
            {
                for (int m = i; m < (partialMatrixWidth + i); m++)
                {
                    partialChannelMatrixrow.push_back(channelMatrix[n][m]);
                }
                partialChannelMatrix.push_back(partialChannelMatrixrow);
                partialChannelMatrixrow.clear();
            }
            allPartialMatrices.push_back(partialChannelMatrix);
            partialChannelMatrix.clear();
        }
    return allPartialMatrices;
}

float ConvNetwork::KernelxPartialMatrix(vector<vector<float> > partialMatrix, Kernel kernel)
{
    vector <vector <float>> kernelMatrix = kernel.getMatrix();

    float newNumber;
    float sum;

    for (int n = 0; n < partialMatrix.size(); n++)
    {
        for (int m = 0; m < partialMatrix[0].size(); m++)
        {
            newNumber = partialMatrix[n][m] * kernelMatrix[n][m];
            sum += newNumber;
        }
    }

    return sum;
}

vector <vector <float>> ConvNetwork::initConvolvedFeature(Channel inputChannel, Kernel kernel)
{
    vector <vector <float>> channelMatrix = inputChannel.getMatrix();
    vector <vector <float>> kernelMatrix = kernel.getMatrix();

    int partialMatrixHeight = kernelMatrix.size();
    int partialMatrixWidth = kernelMatrix[0].size();

    vector <vector <vector <float>>> allPartialMatrices;
    allPartialMatrices = extractPartialMatrices(channelMatrix, partialMatrixHeight, partialMatrixWidth, convolvedFeatureStride);

    vector <float> allNumbers;
    float newNum;
    for (int n = 0; n < allPartialMatrices.size(); n++)
    {
        newNum = KernelxPartialMatrix(allPartialMatrices[n], kernel);
        allNumbers.push_back(newNum);
    }

    vector <vector <float>> convFeatMatrix;
    vector <float> convFeatMatrixRow;

    for (int j = 0; j < findNumOfDividends(0, (channelMatrix.size() - kernelMatrix.size() + 1), convolvedFeatureStride); j++)
    {
        for (int i = 0; i < findNumOfDividends(0, (channelMatrix[0].size() - kernelMatrix[0].size() + 1), convolvedFeatureStride); i++)
        {
            convFeatMatrixRow.push_back(allNumbers[0]);
            allNumbers.erase(allNumbers.begin());
        }
        convFeatMatrix.push_back(convFeatMatrixRow);
        convFeatMatrixRow.clear();
    }

    return convFeatMatrix;
}

vector <vector <float>> ConvNetwork::initMaxPool(vector <vector <float>> convFeatMatrix, int maxPoolMatrixHeight, int maxPoolMatrixWidth)
{
    Kernel newKernel = Kernel(maxPoolMatrixHeight, maxPoolMatrixWidth);

    vector <vector <vector <float>>> partialMatrices;
    partialMatrices = extractPartialMatrices(convFeatMatrix, maxPoolMatrixHeight, maxPoolMatrixWidth, maxPoolStride);

    vector <float> allMaxValues;
    float newValue;
    for (int n = 0; n < partialMatrices.size(); n++)
    {
        newValue = findMatrixMax(partialMatrices[n]);
        allMaxValues.push_back(newValue);
    }

    vector <vector <float>> maxPoolMatrix;
    vector <float> maxPoolMatrixRow;

    for (int j = 0; j < findNumOfDividends(0, (convFeatMatrix.size() - maxPoolMatrixHeight + 1), maxPoolStride); j++)
    {
        for (int i = 0; i < findNumOfDividends(0, (convFeatMatrix[0].size() - maxPoolMatrixWidth + 1), maxPoolStride); i++)
        {
            maxPoolMatrixRow.push_back(allMaxValues[0]);
            allMaxValues.erase(allMaxValues.begin());
        }
        maxPoolMatrix.push_back(maxPoolMatrixRow);
        maxPoolMatrixRow.clear();
    }

    return maxPoolMatrix;
}

void ConvNetwork::printMatrix(vector <vector <float>> matrix)
{
    int height = matrix.size();
    int width = matrix[0].size();
    int prevRow = 0;
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
        {
            if (j != prevRow)
            {
                cout << endl;
            }

            cout << matrix[j][i] << " ";
            prevRow = j;
        }
}

vector <vector <vector <float>>> ConvNetwork::readInputFromFile(string fileName)
{
    ifstream file;
    file.open(fileName);

    vector <string> allLines;
    string line;
    while (getline(file, line))
    {
        allLines.push_back(line);
    }

    vector <vector <vector <float>>> allInputChannels;
    vector <vector <float>> inputChannel;
    vector <float> inputChannelVector;

    for (int n = 0; n < allLines.size(); n++)
    {
        if (!allLines[n].empty())
        {
            stringstream stringStream(allLines[n]);
            for (float k; stringStream >> k;)
            {
                inputChannelVector.push_back(k);
            }
            inputChannel.push_back(inputChannelVector);
            inputChannelVector.clear();
        } else
        {
            allInputChannels.push_back(inputChannel);
            inputChannel.clear();
        }
    }
    allInputChannels.push_back(inputChannel);

    return allInputChannels;
}
void ConvNetwork::clearAll()
{
	inputChannels.clear();
    kernels.clear();
    convolvedFeatures.clear();
    maxPoolChannels.clear();
}
void ConvNetwork::clearMaxPoolLayer()
{
    maxPoolChannels.clear();
}
void ConvNetwork::clearConvolvedFeatures()
{
    convolvedFeatures.clear();
}
void ConvNetwork::clearKernels()
{
    kernels.clear();
}
void ConvNetwork::clearInputChannels()
{
	inputChannels.clear();
}
