//Runtime.h
#ifndef RUNTIME_H
#define RUNTIME_H

#define SIZE 6
#include <vector>
#include "MatrixGen.h"

using namespace std;

class Runtime{

  struct timeTuples{
    double executionTime;
    double endTime;
  };

  struct boolTuples{
    bool fwdCon;
    bool enabled;
  };

  private:
    struct timeTuples TotalNodes[SIZE];
    struct boolTuples Matrix[SIZE][SIZE];
    int seed;
    int percentageOfCon;

    vector<int> runningPool;
    vector<int> completedNodes; //nodes that finished running 
    double globalClock;

  public:
    //Constructor
    Runtime(int seed, int percent);

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
    void ReleaseData(int index);

    /*keep iterating until # of completedNodes == size 
    & runningPool is empty.
    Keep running globalClock;
    */
    void Run();

    void tick(){
      globalClock++;
    }

    void setEndTime(int index){
      TotalNodes[index].endTime = globalClock + TotalNodes[index].executionTime;
    }

    void setRandMatrix();
    bool isUnReachable();
    //debugging methods
    void printTotalNodes();
    void printMatrix();

};
#endif
