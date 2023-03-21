#include <stdlib.h>

#include <sys/time.h>

#include <time.h>

#include <stdio.h>

#include <math.h>

#include <omp.h>

#include <iostream>

#include <omp.h>

using namespace std; 

timeval st,en;

float randomFloatStart(){
  return ((float) rand()) / ((float) RAND_MAX);
}
//using rand_r() with multithreading decreases time by 10 times !!! 
float randomFloat(unsigned int seed){
  return ((float) rand_r(&seed)) / ((float) RAND_MAX);
}
void squareStep(int centerRow, int centerColumn, int gridSize, int halfSize,  float * punkte, float variatonRange) {
  	int centerIndex = centerRow * gridSize +
    centerColumn;
  float upperLeft = punkte[centerIndex -
    (halfSize * gridSize) - halfSize];

  float upperRight = punkte[centerIndex -
    (halfSize * gridSize) + halfSize];
 
  float bottomLeft = punkte[centerIndex +
    (halfSize * gridSize) - halfSize];
  float bottomRight = punkte[centerIndex +
    (halfSize * gridSize) + halfSize];
  unsigned int seed = time(NULL);
  float variation = (randomFloat(seed) * variatonRange * 2) - variatonRange;

  punkte[centerIndex] = ((upperLeft +
      upperRight + bottomLeft + bottomRight) / 4.0) +
    variation;
}

void diamondStep(int centerRow, int centerColumn, int gridSize, int halfSize,
  float * punkte, float variatonRange) {
  int centerIndex = centerRow * gridSize +
    centerColumn;
  int centerToCenter = 2 * halfSize;
  unsigned int seed = time(NULL);
  float variation = (randomFloat(seed) *variatonRange * 2) - variatonRange;
  float upperLeft = punkte[centerIndex -
    (halfSize * gridSize) - halfSize];
  float upperRight = punkte[centerIndex -
    (halfSize * gridSize) + halfSize];
  float bottomLeft = punkte[centerIndex +
    (halfSize * gridSize) - halfSize];
  float bottomRight = punkte[centerIndex +
    (halfSize * gridSize) + halfSize];
  float center = punkte[centerIndex];
  bool inTopRange = (centerIndex -
    (centerToCenter * gridSize)) >= 0;
  bool inBottomRange = (centerIndex +
    (centerToCenter * gridSize)) < (gridSize *
    gridSize);
  bool inLeftRange = ((centerIndex %
    gridSize) - centerToCenter) >= 0;
  bool inRightRange = ((centerIndex %
    gridSize) + centerToCenter) < gridSize;
  float upper = inTopRange ?
    punkte[centerIndex - (centerToCenter * gridSize)] : center;
  float right = inRightRange ?
    punkte[centerIndex + centerToCenter] : center;
  float bottom = inBottomRange ?
  punkte[centerIndex + (centerToCenter * gridSize)]: center;
  float left = inLeftRange ?
    punkte[centerIndex - centerToCenter] : center; 

  
  punkte[centerIndex - (halfSize *
    gridSize)] = ((upper + upperRight + center +
    upperLeft) / 4.0) + variation;
  punkte[centerIndex + halfSize] =
    ((upperRight + right + bottomRight + center) /
      4.0) + variation;
  punkte[centerIndex + (halfSize *
    gridSize)] = ((center + bottomRight + bottom +
    bottomLeft) / 4.0) + variation;
  punkte[centerIndex - halfSize] =
    ((upperLeft + center + bottomLeft + left) /
      4.0) + variation;
}
int main() {
  srand((unsigned) time(NULL));
  int n = 12;
  int gridSize = pow(2, n) + 1;
  float hoehe = 100.0;
  float variatonRange = hoehe * 0.5;
  float rauheit = 0.8;
  float * punkte = new float[gridSize *
    gridSize];
  for (int i = 0; i < gridSize * gridSize; i++)
    punkte[i] = 0.0;
  punkte[0] = (randomFloatStart() *
    variatonRange * 2) - variatonRange;
  punkte[gridSize - 1] = (randomFloatStart() *
    variatonRange * 2) - variatonRange;
  punkte[(gridSize - 1) * gridSize] =
    (randomFloatStart() * variatonRange * 2) -
    variatonRange;
  punkte[gridSize * gridSize - 1] =
    (randomFloatStart() * variatonRange * 2) -
    variatonRange;

  gettimeofday(&st, NULL);
	  
  for (int subdevisions = 1; subdevisions <=
    (gridSize / 2); subdevisions *= 2) {
    variatonRange *= rauheit;
    int halfSize = gridSize / (2 *
      subdevisions);
    // printf("%d\t%d\n",subdevisions, halfSize);
    int
      subCenterRow;
      int subCenterColumn;
  if (halfSize < 1) {
    break;
  }
  
  for (int subDivisionX = 0; subDivisionX < subdevisions; subDivisionX++) {
    int subCenterColumn =
      (subDivisionX * 2 * halfSize) + halfSize;
    #pragma omp parallel for  // the placement is motivated by chunk sizes, race conditions couldn't have appeared anyway
    for (int subDivisionY = 0; subDivisionY < subdevisions; subDivisionY++) {
      int
      subCenterRow = (subDivisionY * 2 * halfSize) +
        halfSize;
      squareStep(subCenterRow, subCenterColumn,
        gridSize, halfSize, punkte, variatonRange);
    }
  }

  /* printf("Square\n");
  for (int i = 0; i < gridSize;
  i++)
  {
  for (int column = 0;
  column < gridSize; column++)
  {	
  printf("%.1f\t", punkte[i * gridSize +
  column]);
  }
  printf("\n");
  }
  printf("\n\n");
  **/
  for (int subDivisionX = 0; subDivisionX < subdevisions; subDivisionX++) {
    int subCenterColumn =
      (subDivisionX * 2 * halfSize) + halfSize; 
     
  	#pragma omp parallel for 
    for (int subDivisionY = 0; subDivisionY < subdevisions; subDivisionY++) {
      int
      subCenterRow = (subDivisionY * 2 * halfSize) +
        halfSize;
      diamondStep(subCenterRow, subCenterColumn,
        gridSize, halfSize, punkte, variatonRange);
    }
  }

  /* printf("Diamond\n");
  for (int i = 0; i < gridSize;
  i++)
  {
  for (int column = 0;
  column < gridSize; column++)
  {
  printf("%.1f\t", punkte[i * gridSize +
  column]);
  }
  printf("\n");
  }
  printf("\n\n"); */
}
gettimeofday(&en, NULL);
/*
for (int i = 0; i < gridSize; i++){
  for (int j = 0; j < gridSize; j++){
      cout << punkte[i * gridSize + j] << " ";   
  }
  cout << endl;
}*/
float seconds = en.tv_sec - st.tv_sec;
float useconds = en.tv_usec - st.tv_usec;
cout << seconds*1000 + useconds/1000 << endl;
delete[] punkte;
return 0;
}