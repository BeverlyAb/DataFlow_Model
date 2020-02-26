#include "Runtime.h"
//g++ -o main main.o Runtime.o
//g++ -c Runtime.cpp main.cpp
Runtime::Runtime(){
    globalClock = 0;
    percentageOfCon = 0;
}

Runtime::Runtime( int percent){

  //identical execution time for now
  for(int i = 0; i < SIZE; i++){
    TotalNodes[i].executionTime = rand() % 100; 
    TotalNodes[i].endTime = 0;     
  }
  percentageOfCon = percent;
  printf("PercentageOfCon %d ",percentageOfCon);
  setRandMatrix();
  printMatrix();
  globalClock = 0;


}

//excludes completedNodes. note that nodes without any fwdCon are "ready to run"
void Runtime::CheckReadyToRun(){
  for(int i = 0; i < SIZE; i++){
    if(completedNodes.end() == find(completedNodes.begin(), completedNodes.end(), i)){
      bool allDependencyMet = true;
      int j = 0;
     //  printf("Checking Node %i\n", i);
      while (allDependencyMet && j < SIZE){
        if(Matrix[i][j].fwdCon && !Matrix[i][j].enabled)
          allDependencyMet = false;
        j++;
        tick();
      }
      if( allDependencyMet && 
          runningPool.end() == find(runningPool.begin(), runningPool.end(), i)){

        runningPool.push_back(i);
        printf("Time %f : ", globalClock);
        printTotalNodes();
        setEndTime(i);
        printf("Node pushedback %i Total size %lu\n",i, runningPool.size());
      }
    }
  }
}

void Runtime::ScanRunningPool(){
  for(int i = 0; i < runningPool.size(); i++){
    int nodeIndex = runningPool[i];
    printf("SCAN: Running Pool Size %lu\n",runningPool.size());
    if(TotalNodes[nodeIndex].endTime <= globalClock){
      ReleaseData(nodeIndex);
      runningPool.erase(remove(runningPool.begin(), runningPool.end(), nodeIndex), runningPool.end());
    
      printf("Time %f : ", globalClock);
      printTotalNodes();
    }
    printf("SCAN: Running Pool Size %lu\n",runningPool.size());
   // tick();
  }
}

void Runtime::ReleaseData(int index){
  for(int i = 0; i < SIZE; i++){
    if(Matrix[i][index].fwdCon == 1){
      Matrix[i][index].enabled = 1;
    
      printMatrix();
    }
   // tick();
  }

  completedNodes.push_back(index);
}

void Runtime::Run(){
  while(completedNodes.size() < SIZE){
    CheckReadyToRun();
    ScanRunningPool();
    CheckReadyToRun();
    if(!isReachable())
      break;
  }
  printf("All done\n Nodes Completed %lu\n Total Time %f\n", 
  completedNodes.size(), globalClock);
}

void Runtime::setRandMatrix()
{
  for(int i = 0; i < SIZE; i++){
    for(int j = 0; j < SIZE; j++){
      //all disabled except for 1st node  
      Matrix[i][j].enabled = 0;
      Matrix[i][j].fwdCon = 0;
      //randomly assign forward con
      //No deadlocks
      //No self loop (only starting loop has self loop)
      //First node has only 1 dep. which is itself
      
      //if(rand() % 100 > percentageOfCon && Matrix[j][i].fwdCon != 1 && i!=j && i>0){
      //  Matrix[i][j].fwdCon = 1;
    //  }
    }
  }
  int N = SIZE;
  //debug online
  for(int i = N; i > 0; i--) {
    for (int j = N; j > i+ 1; j--) {
      int percent = rand() % 100;
       if(percent < percentageOfCon){
        Matrix[i][j].fwdCon = 1;
       }
    }
  } 


  //Starting Node is always enabled and ready to start
  Matrix[0][0].enabled = 1;
  Matrix[0][0].fwdCon = 1;  
}
/*
  Print Methods
*/
void Runtime::printTotalNodes(){
  for(int i = 0; i < SIZE; i++){
    printf("(%f %f )", TotalNodes[i].executionTime, TotalNodes[i].endTime);       
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

bool Runtime::isReachable(){
  if(runningPool.size()==0 && completedNodes.size() != SIZE){
    for(int i = 0; i < SIZE; i++){
      if(completedNodes.end() == find(completedNodes.begin(), completedNodes.end(), i)){
        
        for(int j = 0; j < SIZE; j++){
          if( Matrix[i][j].fwdCon && !Matrix[i][j].enabled &&
              find(runningPool.begin(), runningPool.end(), j) != runningPool.end() )
              return true;
        }
      }
    }
    printf("Unreachable\n");
    return false;
  } else
  return true;
}