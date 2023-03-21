#include "Kernel.h"
#include <iostream>
#include <time.h>

Kernel::Kernel(int height, int width){
	srand(time(NULL));
	for (int j = 0; j < height; j++)
    {
        vector<float> row;
        for (int i = 0; i < width; i++)
        {
            row.push_back((float)rand()/RAND_MAX);
        }
        matrix.push_back(row);
    }    

}

void Kernel::resetWeights(bool random){
	for (int j = 0; j < matrix.size(); j++)
    {
        for (int i = 0; i < matrix[0].size(); i++)
        {
        	if (random)
        	{
            matrix[j][i] = (float)rand()/RAND_MAX;
        	}
        	else
        	{
        	matrix[j][i] = 0;
        	}
        }
    }    
}
