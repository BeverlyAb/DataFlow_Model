//Runtime.h
#ifndef RUNTIME_H
#define RUNTIME_H

#define SIZE 4
#include <stdlib.h>
#include <vector>
#include <fstream>
using namespace std;

class Runtime{

  struct timeTuples{
    double executionTime;
    double startedRunning;
    double endTime;
    //
    double expirationTime;
  };

  struct boolTuples{
    bool fwdCon;
    bool enabled;
  };

  private:
    struct timeTuples TotalNodes[SIZE];
    struct boolTuples Matrix[SIZE][SIZE];

    int percentageOfCon;
    vector<int> runningPool;
    vector<int> completedNodes; //nodes that finished running 
    double globalClock;

  public:
    Runtime();
    //Constructor
    Runtime(int percent);

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
    //sets Time to end and expiration and triggers when it began running
    void setEndTime(int index){
      TotalNodes[index].startedRunning = globalClock;
      TotalNodes[index].endTime = globalClock + TotalNodes[index].executionTime;
      TotalNodes[index].expirationTime = globalClock + TotalNodes[index].expirationTime;
    }

    /* generates DAG with randomly assigned connections */
    void setRandMatrix();
    /* checks if DAG can run to completion given random dependencies */
    bool isReachable();
    
    /*node refires when it expires before it completes it execution. Expiration time gets doubled*/
    bool reFire(int index);

    void exportToCSV();
    //debugging methods
    void printTotalNodes();
    void printMatrix();

};
#endif
