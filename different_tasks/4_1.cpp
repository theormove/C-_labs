#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <iostream>
using namespace std;

int main(int argc, const char* argv[]){
	cout << "Input Matrix Size: ";
	int dim;
	cin >> dim;

	printf("Integergroesse: %lu\n", sizeof(int));
	printf("Matrixgroesse: %d x %d\n", dim, dim);
	printf("Die Matrizen belegen jeweils %lu bytes an Speicher.\n", dim * dim * sizeof(int));
	printf("Insgesamt also %lu bytes.\n", 3 * dim * dim * sizeof(int));

	int * matrixA = new int [dim*dim];
	int * matrixB = new int [dim*dim];
	int * matrixC = new int [dim*dim];
	//int matrixA [9] = { 1,1,1,1,1,1,1,1,1 };
	//int matrixB [9] = { 1,1,1,1,1,1,1,1,1 };

	// Messinstrumente
	struct timeval start, end;
	long mtime, seconds, useconds;

	// Initialisiere Random Number Generator
	srand((unsigned)time(NULL));

	for (int i = 0; i < dim*dim; i++){
	matrixA[i] = rand();
	matrixB[i] = rand();
	matrixC[i] = 0; 
	}
	
	
	
	// omp parallel for should be used for a first cycle to avoid race conditions (multiple threads trying to change the same matrix elem in our case at the same time )
	// for example if in the first type of multiplying we move omp for to the last cycle all threads would be modyfying matrix[i*dim + j] causing race conditions 
	// the other minor reason for the placement is less administrative work for OpenMP (dim times fewer)
	// also there is no need in specifying clauses it only harms optimization 

	// Waehle eine Variante fuer Matrizenmultiplikation
	cout << "Choose type of mutliplying: 1/2/3 ? ";
	int t;
	cin >> t;
	cout << "\n";

	// starte die Zeitnahme
	gettimeofday(&start, NULL);

	if (t == 1)
	{
        #pragma omp parallel for 
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				for (int k = 0; k < dim; k++) {
					matrixC[i*dim + j] += matrixA[i*dim + k] * matrixB[k*dim + j];

				}
			}
		}
	}
	
	else if (t == 2)
	{
        #pragma omp parallel for
		for (int k = 0; k < dim; k++) {
			for (int i = 0; i < dim; i++) {
				for (int j = 0; j < dim; j++) {
					matrixC[i*dim + j] += matrixA[i*dim + k] * matrixB[k*dim + j];

				}
			}
		}
	}

	else if (t == 3)
	{
	    #pragma omp parallel for
	    for(int j = 0; j < dim; j++ ){
		    for(int k = 0; k < dim; k++){	
			    for(int i = 0; i < dim; i++){
				    matrixC[i*dim + j] += matrixA[i*dim + k] * matrixB[k*dim + j];

			    }
		    }
	    }
	
    }

	// beende die Zeitnahme und gib die Dauer der Berechnung aus
	gettimeofday(&end, NULL);
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
	printf("Die Ausfuehrung dauerte %ld ms.\n", mtime);

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
	
	//delete[] matrixA;
	//delete[] matrixB;
	//delete[] matrixC;

	return 0 ;
}
















