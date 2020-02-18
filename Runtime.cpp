#include "Runtime.h"

Runtime::Runtime(){
TotalNodes[SIZE] =     
    {
      {1,2},
      {1,3},
      {1,4},
      {1,5}
    };

  Matrix[SIZE][SIZE] = 
  {
    { {1,1}, {0,0}, {0,0}, {0,0} },
    { {1,0}, {0,0}, {0,0}, {0,0} },
    { {1,1}, {0,0}, {0,0}, {0,0} },
    { {0,0}, {1,0}, {1,0}, {0,0} }
  };

  globalClock = 0;
}

//doesn't exclude yet
Runtime::CheckReadyToRun(){
  for(int i = 0; i < SIZE; i++){
    
    bool allDependencyMet = true;
    int j = 0;
    while (allDependecyMet && j < SIZE){
      if(Matrix[i][j].fwdCon && !Matrix[i][j].enabled)
        allDependecyMet = false;
      j++;
      tick();
    }
    if(allDependecyMet)
      runningPool.push_back(j);
  }
}

Runtime::ScanRunningPool(){
  for(int i = 0; i < runningPool.size(); i++){
    int nodeIndex = runningPool[i];
    if(TotalNodes[nodeIndex].endTime <= globalClock){
      ReleaseData(nodeIndex);
      runningPool.erase(nodeIndex);
    }

    tick();
  }
}

Runtime::ReleaseData(int index){
  for(int j = 0; j < size; j++){
    Matrix[index][j].enabled = Matrix[index][j].fwdCon;
    tick();
  }

  completedNodes.push_back(index);
}

Runtime::Run(){
  while(!runningPool.empty() && completedNodes.size() < SIZE){
    CheckReadyToRun();
    ScanRunningPool();
  }

  printf("All done\n Nodes Completed %d\n Total Time %d\n", 
  completedNodes.size(), globalClock);
}
