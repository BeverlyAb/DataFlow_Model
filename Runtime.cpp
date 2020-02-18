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
    void ReleaseData();

    /*keep iterating until # of completedNodes == size 
    & runningPool is empty.
    */
    void Run();
}