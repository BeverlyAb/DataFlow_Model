//Runtime.h
#ifndef RUNTIME_H
#define RUNTIME_H

#define DEBUG_TEST 0
#define SIZE 80
#define PROC_SIZE 2

#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>
#include "Processor.h"
#include "Time.h"
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

    Processor assignedProc[PROC_SIZE]; // Procesor Objects
    map<int, Processor *>procList; //key : task, value: ProcessorID, availability
    map<int, vector<int> > taskList; // key : PID, value: list of assigned tasks

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
    void setEndTime(int index);

    /* generates DAG with randomly assigned connections */
    void setRandMatrix();
    /* checks if DAG can run to completion given random dependencies */
    bool isReachable();
    
    /*node refires when it expires before it completes it execution. Expiration time gets doubled*/
    bool reFire(int index);

    /*returns true if the current node passes information to another node under the same Processor*/
    bool isContinuingNode(int pID, int nextTask);

    void exportToCSV();
    //debugging methods
    void printTotalNodes();
    void printMatrix();

};
#endif