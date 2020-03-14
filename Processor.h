//Processor.h

#ifndef PROCESSOR_H
#define PROCESSOR_H

#define UNAVAILABLE 0
#define AVAILABLE 1
#define DEAD

class Processor{

  private:
    map<int, vector<int> >procTask; //key : pID, values : tasks
    map<int, int> procAvailability; //key : pID, values : availbility

  public:
    Processor();
    Processor(int taskSize, int procSize);
    // void setStatus(int stat) { Status = stat;}
    // int getID(){ return ProcessorId; }
    // int getStatus(){ return Status; }
    // inline bool operator< (const Processor rhs) const {return ProcessorId < rhs.ProcessorId; }
  };
#endif