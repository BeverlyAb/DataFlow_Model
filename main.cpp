#include "Runtime.h"
#include <stdlib.h>
#include <time.h>

int main(){
  srand(time(NULL));
  Runtime myRunner = Runtime(rand());
  myRunner.Run();
  return 0;
}