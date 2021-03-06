#include "Runtime.h"
//g++ -c Runtime.cpp main.cpp Processor.cpp
//g++ -o main main.o Runtime.o Processor.o

Runtime::Runtime(){
    globalClock = 0;
    percentageOfCon = 0; 
}

Runtime::Runtime( int percent){

  int setExpireTime = rand()%9;
  
  for(int i = 0; i < PROC_SIZE; i++)
    assignedProc[i] = Processor(i,AVAILABLE);
  
  for(int i = 0; i < SIZE; i++){
    TotalNodes[i].executionTime = 3;//rand() % 10; 
    TotalNodes[i].startedRunning = 0;
    TotalNodes[i].endTime = 0;   
    TotalNodes[i].expirationTime = setExpireTime; 

    //randomly assign Processor to task or node
    int pID = rand() % PROC_SIZE;
    procList.insert(pair<int,Processor *>(i, &assignedProc[pID])); 

    vector<int> v;
    if(taskList.find(pID) == taskList.end()){
      v.push_back(i);
      taskList.insert(pair<int,vector<int> >(pID,v));
    } else{
      taskList.find(pID)->second.push_back(i);
    }
 }

 if(DEBUG_TEST && 0){
  for(map<int, Processor *>::iterator it = procList.begin(); it != procList.end(); it++)
    printf("TASK %i PID %i Availability %i\n", it->first, it->second->getID(), it->second->getStatus());

  for(map<int, vector<int> >::iterator it = taskList.begin(); it != taskList.end(); it++){
    printf("pid %i\n", it->first);
    for(int i = 0; i < it->second.size(); i++){
      printf("task %i ",it->second[i]);
    }
    printf("\n");
  }
 }
  percentageOfCon = percent;
  setRandMatrix();
  printMatrix();
  globalClock = 0;
}

//excludes completedNodes. note that nodes without any fwdCon are "ready to run"
void Runtime::CheckReadyToRun(){
  for(int i = 0; i < SIZE; i++){

    //hasn't completed and its Proc is ready to run
    if(completedNodes.end() == find(completedNodes.begin(), completedNodes.end(), i)
    && procList.find(i)->second->getStatus() == AVAILABLE){
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
        procList.find(i)->second->setStatus(UNAVAILABLE);


        if (DEBUG_TEST){
          printf("Time %f : \n", globalClock);
          printTotalNodes();
        }
        setEndTime(i);
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
        // printf("Freed Node %i, Proc %i\n",i,find.(i)->second.getStatus());
        if (DEBUG_TEST){
          printf("Time %f : \n", globalClock);
          printf("Node: %i , size = %lu\n", nodeIndex, runningPool.size());
        }
        
      }
    }
    // tick();
  }
  if(DEBUG_TEST){
    for(map<int, Processor *>::iterator it = procList.begin(); it != procList.end(); it++)
      printf("TASK %i PID %i Availability %i\n", it->first, it->second->getID(), it->second->getStatus());
    printf("procList size %lu\n", procList.size());
    }
}

void Runtime::ReleaseData(int index){
  for(int i = 0; i < SIZE; i++){
    if(Matrix[i][index].fwdCon == 1){
      Matrix[i][index].enabled = 1;
    
      if (DEBUG_TEST)
        printMatrix();
    }
   // tick();
  }
  procList.find(index)->second->setStatus(AVAILABLE);
  completedNodes.push_back(index);
}

void Runtime::Run(){
  while(completedNodes.size() < SIZE){
    CheckReadyToRun();
    ScanRunningPool();
    CheckReadyToRun();
    if(!isReachable())
      break;
    tick();
  }
  printf("All done\n Nodes Completed %lu\n Total Time %f\n", 
  completedNodes.size(), globalClock);
  
  if(DEBUG_TEST)
    printTotalNodes();
  else
    exportToCSV();
}

void Runtime::setEndTime(int index)
{
  TotalNodes[index].startedRunning = globalClock;

  if(TotalNodes[index].executionTime <= TotalNodes[index].expirationTime)
    TotalNodes[index].endTime = TotalNodes[index].startedRunning + TotalNodes[index].executionTime;
  else
  {
    TotalNodes[index].endTime = TotalNodes[index].startedRunning + TotalNodes[index].expirationTime;
  }
  
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
  if(TotalNodes[index].executionTime > TotalNodes[index].expirationTime){
    TotalNodes[index].expirationTime *= 2;
    TotalNodes[index].startedRunning = globalClock;
    
    setEndTime(index);
    if(DEBUG_TEST) {    
      printf("REFIRED %i\n", index);
      printTotalNodes();
    }
    return true;
  } else
    return false; 
}
bool Runtime::isContinuingNode(int pID, int nextTask){
  
  for(int i = 0; i < taskList.find(pID)->second.size();i++){
  //  printf("pID %i Tasks %i\n", pID, taskList.find(pID)->second[i]);
    if(nextTask == taskList.find(pID)->second[i]) {
      
      return true;
    }
  }
  return false;
}
/*
 ---------- Print Methods ------------------------------------------------------------------------------------
*/
void Runtime::printTotalNodes(){
  printf("Time %f : \n", globalClock);
  printf("NodeID\tEnd Time\tProcID\tExpire\t\tExec. Time\tStart Time\n");
  for(int i = 0; i < SIZE; i++){
    printf("%i:\t%f,\t%i\t%f,\t%f,\t%f\n", i,TotalNodes[i].endTime, procList.find(i)->second->getID(), TotalNodes[i].expirationTime, TotalNodes[i].executionTime, TotalNodes[i].startedRunning);       
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
              find(runningPool.begin(), runningPool.end(), j) != runningPool.end() ){
              return true;
          }
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
  Time curTime = Time();
  string fileName = "N" + to_string(SIZE) + "-" + curTime.getTime() + ".csv";
  myfile.open (fileName);
  myfile << "NodeID,  End Time, ProcID, Expiration Time, Execution Time, Start Time\n";

  for(int i=0; i <SIZE; i++){
    string line = 
    to_string(i) + "," +
    to_string(TotalNodes[i].endTime) + "," +
    to_string(procList.find(i)->second->getID()) + "," +
    to_string(TotalNodes[i].expirationTime) + "," +
    to_string(TotalNodes[i].executionTime) + "," + 
    to_string(TotalNodes[i].startedRunning) + "\n";

    myfile << line;
  }
  myfile.close(); 
}