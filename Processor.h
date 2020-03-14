//Processor.h

#ifndef PROCESSOR_H
#define PROCESSOR_H

#define UNAVAILABLE 0
#define AVAILABLE 1
#define DEAD

class Processor{

  private:
    int ProcessorId;
    int Status;

  public:
    Processor();
    Processor(int id, int stat);
    void setStatus(int stat) { Status = stat;}
    int getID(){ return ProcessorId; }
    int getStatus(){ return Status; }
    bool operator()(Processor & left, Processor & right)  {
        return left.getID() < right.getID();
    }
  };
#endif