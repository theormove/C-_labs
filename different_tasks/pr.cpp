#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <iostream>
using namespace std;

int main(int argc, const char *argv[]){
	/*if (argc < 2){
		return 1;
	}*/	
	int dim = 500;//atoi(argv[1]);

	int * matrixA = new int [dim*dim];
	int * matrixB = new int [dim*dim];
	int * matrixC = new int [dim*dim];
	//int matrixA [9] = { 1,1,1,1,1,1,1,1,1 };
	//int matrixB [9] = { 1,1,1,1,1,1,1,1,1 };

	timeval st,en;

	srand((unsigned) time(NULL));

	for (int i = 0; i < dim*dim; i++){
	//matrixA[i] = rand();
	//matrixB[i] = rand();
	matrixC[i] = 0; 
	}
	char sw = 'a';
	// omp parallel for should be used for a first cycle to avoid race conditions (multiple threads trying to change the same matrix elem in our case at the same time )
	// also there is no need in specifying clauses it only harms optimization  
	gettimeofday(&st,NULL);
	switch(sw){
	case 'a':	
	#pragma omp parallel for 
	for(int i = 0; i < dim; i++ ){
		for(int j = 0; j < dim; j++){	
			for(int k = 0; k < dim; k++){
				matrixC[i*dim + j] += matrixA[i*dim + k] * matrixB[k*dim + j];

			}		
		}
	}
	break;
	case 'b':	
	#pragma omp parallel for
	for(int k = 0; k < dim; k++ ){
		for(int i = 0; i < dim; i++){	
			for(int j = 0; j < dim; j++){
				matrixC[i*dim + j] += matrixA[i*dim + k] * matrixB[k*dim + j];

			}
		}
	}
	break;
	case 'c':	
	#pragma omp parallel for
	for(int j = 0; j < dim; j++ ){
		for(int k = 0; k < dim; k++){	
			for(int i = 0; i < dim; i++){
				matrixC[i*dim + j] += matrixA[i*dim + k] * matrixB[k*dim + j];

			}
		}
	}
	break;
}


	gettimeofday(&en,NULL);
	float seconds = en.tv_sec - st.tv_sec;
	float useconds = en.tv_usec - st.tv_usec;
	cout << seconds*1000 + useconds/1000 << endl;
	//printing matrices to check
	/*for(int i = 0; i < dim; i++ ){
		for(int j = 0; j < dim; j++){
			printf(" %d ", matrixA[i*dim + j]);
		}
		printf("\n");
	}
	printf("\n");
	for(int i = 0; i < dim; i++ ){
		for(int j = 0; j < dim; j++){
			printf(" %d ", matrixB[i*dim + j]);
		}
		printf("\n");
	}
	printf("\n");
	for(int i = 0; i < dim; i++ ){
		for(int j = 0; j < dim; j++){
			printf(" %d ", matrixC[i*dim + j]);
		}
		printf("\n");
	}*/
	//printf("Die Ausfuehrung dauerte  ms.\n");
	//delete[] matrixA;
	//delete[] matrixB;
	//delete[] matrixC;

	return 0 ;
}
















