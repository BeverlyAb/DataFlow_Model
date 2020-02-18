#DEFINE size 10

struct timeTuples{
  double executionTime;
  double endTime;
};

struct boolTuples{
  bool fwdCon;
  bool enabled;
};

Class Runtime{
  private:
    timeTuples TotalNodes[size];
    boolTuples Matrix[size][size];
    vector<int> runningPool;
    vector<int> completedNodes; //nodes that finished running 
    double globalClock;

  public:
    //Constructor
    Runtime();

    /* checks Matrix if all fwdCon are enabled (while excluding
   completedNodes).
    adds indices to running pool */
    void CheckReadyToRun();

    /* checks if any of the running nodes finished executing. 
    Calls release data and updates runningPool
    */ 
    void ScanRunningPool();

    /* enables the dependencies along the col. of the index.
    add index to completedNodes
    */
    void ReleaseData(int index;

    /*keep iterating until # of completedNodes == size 
    & runningPool is empty.
    Keep running globalClock;
    */
    void Run();
}

Runtime::Runtime(){}

//doesn't exclude yet
Runtime::CheckReadyToRun(){
  for(int i = 0; i < size; i++){
    
    bool allDependencyMet = true;
    int j = 0;
    while (allDependecyMet && j < size){
      if(Matrix[i][j].fwdCon && !Matrix[i][j].enabled)
        allDependecyMet = false;
      j++;
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
  }
}

Runtime::ReleaseData(int index){
  for(int j = 0; j < size; j++){
    Matrix[index][j].enabled = Matrix[index][j].fwdCon;
  }

  completedNodes.push_back(index);
}

Runtime::Run(){
  while(!runningPool.empty() && completedNodes.size() < size){
    CheckReadyToRun();
    ScanRunningPool();
  }

  printf("All done\n Nodes Completed %d\n Total Time %d\n", 
  completedNodes.size(), globalClock);
}
