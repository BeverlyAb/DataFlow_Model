//Processor.cpp

#include "Processor.h"

Processor::Processor(){
}


Processor::Processor(int taskSize, int procSize){
    //randomly assign Processor to task or node
    for(int i = 0; i < SIZE; i++){
      int pID = rand() % procSize;
      vector<int> v;
      if(!procTask.empty() && !procTask.find(pID)->second.empty()){
        vector<int> v = procTask.find(pID)->second;
      }

      v.push_back(i);
      procTask.insert(pair<int, vector<int> >(pID,v)); 
      
      //set all Processor to available
      procAvailability.insert(pair<int,int> (pID, AVAILABLE));
    }
}