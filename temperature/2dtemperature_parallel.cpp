// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <omp.h>
#include <iostream>
using namespace std;

int main() {
	float TEMP = 100;
	int x, t;
	cout << "Print size of the array:" << "\n";
	cin >> x;
	cout << "Print time to pass:" << "\n";
	cin >> t;
	float arry[x][x];
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < x; ++j) {
			arry[i][j] = 0;
		}
	}
	arry[x / 2][x / 2] = TEMP;
	float arry2[x][x];
	for(int tim = 0; tim < t; ++tim){
	    cout << "Time: ";
	    cout << tim;
	    cout << "\n";
	    for (int i = 0; i < x; ++i) {
		    for (int j = 0; j < x; ++j) {
			    cout << arry[i][j];
			    cout << " ";
		    }
		cout << "\n";    
	    }
	    #pragma omp parallel for
	    for (int i = 0; i < x; ++i) {
		    for (int j = 0; j < x; ++j) {
			    arry2[i][j] = arry[i][j];
		    }
	    }
	    #pragma omp parallel for
	    for (int i = 0; i < x; ++i) {
		    for (int j = 0; j < x; ++j) {
		        if (i != 0 && j !=0 && i != x-1 && j != x-1 ){
			        arry[i][j] = arry2[i][j] + 0.25*(arry2[i-1][j]+arry2[i][j-1]-4.0*arry2[i][j]+arry2[i+1][j]+arry2[i][j+1]);
		            }
		    }
	    }
	    arry[x / 2][x / 2] = TEMP;    
	}    	
	return(0);
}