#include "Runtime.h"
//g++ -o main main.o Runtime.o
//g++ -c Runtime.cpp main.cpp
Runtime::Runtime(){
    globalClock = 0;
    percentageOfCon = 0; 
}

Runtime::Runtime( int percent){

  for(int i = 0; i < SIZE; i++){
    TotalNodes[i].executionTime = rand() % 10; 
    TotalNodes[i].startedRunning = 0;
    TotalNodes[i].endTime = 0;   
    TotalNodes[i].expirationTime = rand() % 11;  
  }
  percentageOfCon = percent;
  setRandMatrix();
  //printMatrix();
  globalClock = 0;
}

//excludes completedNodes. note that nodes without any fwdCon are "ready to run"
void Runtime::CheckReadyToRun(){
  for(int i = 0; i < SIZE; i++){
    if(completedNodes.end() == find(completedNodes.begin(), completedNodes.end(), i)){
      bool allDependencyMet = true;
      int j = 0;

      while (allDependencyMet && j < SIZE){
        if(Matrix[i][j].fwdCon && !Matrix[i][j].enabled)
          allDependencyMet = false;
        j++;
        //tick();
      }
      if( allDependencyMet && 
          runningPool.end() == find(runningPool.begin(), runningPool.end(), i)){

        runningPool.push_back(i);
        
        //printf("Time %f : ", globalClock);
      //  printTotalNodes();
        setEndTime(i);
        //printf("Node pushedback %i Total size %lu\n",i, runningPool.size());
      }
    }
  }
}

void Runtime::ScanRunningPool(){
  for(int i = 0; i < runningPool.size(); i++){
    int nodeIndex = runningPool[i];

    if(TotalNodes[nodeIndex].endTime <= globalClock){
      if(!reFire(nodeIndex)){
        ReleaseData(nodeIndex);
        runningPool.erase(remove(runningPool.begin(), runningPool.end(), nodeIndex), runningPool.end());
      
        //printf("Time %f : ", globalClock);
      //  printTotalNodes();
      }
    }
    tick();
  }
}

void Runtime::ReleaseData(int index){
  for(int i = 0; i < SIZE; i++){
    if(Matrix[i][index].fwdCon == 1){
      Matrix[i][index].enabled = 1;
    
    //  printMatrix();
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
  //printTotalNodes();
  exportToCSV();
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
for(int i = 0; i < N; i++) {
    for (int j = i+1; j < N; j++) {
      int random = rand() % 100;
       if(random < percentageOfCon){
        Matrix[j][i].fwdCon = 1; //inverted, so that lower triangle is populated
       }
    }
  } 
  //Starting Node is always enabled and ready to start
  Matrix[0][0].enabled = 1;
  Matrix[0][0].fwdCon = 1;  
}

bool Runtime::reFire(int index){
  if(TotalNodes[index].endTime > TotalNodes[index].expirationTime){
    TotalNodes[index].expirationTime = globalClock + 2*TotalNodes[index].expirationTime;
    printf("REFIRED %i\n", index);
    return true;
  } else
    return false; 
}

/*
 ---------- Print Methods ------------------------------------------------------------------------------------
*/
void Runtime::printTotalNodes(){
  for(int i = 0; i < SIZE; i++){
    printf("%f,\t%f,\t%f\n", TotalNodes[i].executionTime, TotalNodes[i].startedRunning,TotalNodes[i].endTime);       
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

void Runtime::exportToCSV()
{
  ofstream myfile;
  myfile.open ("TEST2.csv");
  myfile << "NodeID, Execution Time, Start Time, End Time\n";
  for(int i=0; i <SIZE; i++)
    myfile << i << "," << TotalNodes[i].executionTime << "," << TotalNodes[i].startedRunning <<"," <<TotalNodes[i].endTime << "\n";
  myfile.close(); 
}