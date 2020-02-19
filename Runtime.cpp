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
    
    bool allDependencyMet = true;
    int j = 0;
    while (allDependencyMet && j < SIZE){
      if(Matrix[i][j].fwdCon && !Matrix[i][j].enabled)
        allDependencyMet = false;
      j++;
      tick();
    }
    if(allDependencyMet)
      runningPool.push_back(j);
  }
}

void Runtime::ScanRunningPool(){
  for(int i = 0; i < runningPool.size(); i++){
    int nodeIndex = runningPool[i];
    if(TotalNodes[nodeIndex].endTime <= globalClock){
      ReleaseData(nodeIndex);
      runningPool.erase(remove(runningPool.begin(), runningPool.end(), nodeIndex), runningPool.end());
    }

    tick();
  }
}

void Runtime::ReleaseData(int index){
  for(int j = 0; j < SIZE; j++){
    Matrix[index][j].enabled = Matrix[index][j].fwdCon;
    tick();
  }

  completedNodes.push_back(index);
}

void Runtime::Run(){
  while(/*!runningPool.empty() || */completedNodes.size() < SIZE){
    CheckReadyToRun();
    ScanRunningPool();
  }

  printf("All done\n Nodes Completed %d\n Total Time %f\n", 
  completedNodes.size(), globalClock);
}
