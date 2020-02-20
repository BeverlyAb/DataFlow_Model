#include "Runtime.h"
// #include "MatrixGen.h"
#include <stdlib.h>
#include <time.h>
// #define SIZE 4
int main(){
  srand(time(NULL));
  Runtime myRunner = Runtime(rand());
  myRunner.Run();

  // MatrixGen myMat = MatrixGen(SIZE);
  // struct boolTuples Matrix[SIZE][SIZE] = myMat.getMatrix();
  // for(int i = 0; i < SIZE; i++){
  //   for(int j = 0; j < SIZE; j++){
  //     printf("(%i %i)",Matrix[i][j].fwdCon, Matrix[i][j].enabled);
  //   }
  //   printf("\n");
  // }
  // printf("--------------------------\n");


  return 0;
}