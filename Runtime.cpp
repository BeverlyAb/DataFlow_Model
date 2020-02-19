#include "Runtime.h"

Runtime::Runtime(){


  for(int i = 0; i < SIZE; i++){
    TotalNodes[i].executionTime = 1;
    TotalNodes[i].endTime = i+2;       
  }

  for(int i = 0; i < SIZE; i++){
    for(int j = 0; j < SIZE; j++){
      Matrix[i][j].fwdCon = 0;
      Matrix[i][j].enabled = 0;

      if(i == 0 && j == 0)
        Matrix[i][j].enabled = 1;
      if( (i == 0 && j == 0) || 
          (i == 1 && j == 0) ||
          (i == 2 && j == 0) || 
          (i == 3 && j == 1)||
          (i == 3 && j == 2))
            Matrix[i][j].fwdCon = 1;
    }
  }
  globalClock = 0;
}

//doesn't exclude yet
void Runtime::CheckReadyToRun(){
  for(int i = 0; i < SIZE; i++){
    if(completedNodes.end() == find(completedNodes.begin(), completedNodes.end(), i)){
      bool allDependencyMet = true;
      int j = 0;
      printf("Checking Node %i\n", i);
      while (allDependencyMet && j < SIZE){
        if(Matrix[i][j].fwdCon && !Matrix[i][j].enabled)
          allDependencyMet = false;
        j++;
        tick();
      }
      if( allDependencyMet && 
          runningPool.end() == find(runningPool.begin(), runningPool.end(), i)){

        runningPool.push_back(i);
        printf("Node pushedback %i Total size %i\n",i, runningPool.size());
      }
    }
  }
}

void Runtime::ScanRunningPool(){
  for(int i = 0; i < runningPool.size(); i++){
    int nodeIndex = runningPool[i];
    printf("SCAN: Running Pool Size %i\n",runningPool.size());
    if(TotalNodes[nodeIndex].endTime <= globalClock){
      ReleaseData(nodeIndex);
      runningPool.erase(remove(runningPool.begin(), runningPool.end(), nodeIndex), runningPool.end());
    }
    printf("SCAN: Running Pool Size %i\n",runningPool.size());
    tick();
  }
}

void Runtime::ReleaseData(int index){
  for(int j = 0; j < SIZE; j++){
    if(Matrix[j][index].fwdCon == 1){
      Matrix[j][index].enabled = 1;
    
      printMatrix();
    }
    tick();
  }

  completedNodes.push_back(index);
}

void Runtime::Run(){
  while(completedNodes.size() < SIZE){
    CheckReadyToRun();
    ScanRunningPool();
  }
  printf("All done\n Nodes Completed %i\n Total Time %f\n", 
  completedNodes.size(), globalClock);
}

void Runtime::printTotalNodes(){
  for(int i = 0; i < SIZE; i++){
    printf("%f %f ", TotalNodes[i].executionTime, TotalNodes[i].endTime);       
  }
  printf("\n");
}

void Runtime::printMatrix(){
  for(int i = 0; i < SIZE; i++){
    for(int j = 0; j < SIZE; j++){
      printf("(%i %i)",Matrix[i][j].fwdCon, Matrix[i][j].enabled);
    }
    printf("\n");
  }
  printf("--------------------------\n");
}