#include "Channel.h"
#include <iostream>

Channel::Channel(int height, int width)
{
    for (int j = 0; j < height; j++)
    {
        vector<float> row;
        for (int i = 0; i < width; i++)
        {
            row.push_back(0);
        }
        matrix.push_back(row);
    }    
}

Channel::Channel(vector <vector <float>> inputMatrix)
{
    matrix = inputMatrix;
}

vector <vector <float>> Channel::getMatrix()
{
    return matrix;
}

void Channel::setMatrix(vector<vector<float> > newMatrix)
{
    matrix = newMatrix;
}

void Channel::addToMatrix(vector<float> newRow)
{
    matrix.push_back(newRow);
}

void Channel::clearChannel()
{
    int height = matrix.size();
    for (int j = 0; j < height; j++)
        matrix[j].clear();
}

void Channel::printMatrix()
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

void Channel::setName(string name)
{
    channelName = name;
}

string Channel::getName()
{
    return channelName;
}

void Channel::setNumber(int number)
{
    channelNumber = number;
}

int Channel::getNumber()
{
    return channelNumber;
}

float Channel::activationFunc(float value)
{
    if (value > 0) {
        return value;
    }
    else {
        return 0.01 * value;
    }
}
