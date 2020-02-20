//MatrixGen.cpp

#include "MatrixGen.h"

MatrixGen::MatrixGen(int s){
  size = s;
}

MatrixGen::boolTuples ** MatrixGen::getMatrix(){
  struct boolTuples ** Matrix = new struct boolTuples * [size];
  return Matrix;
}