//Processor.cpp

#include "Processor.h"

Processor::Processor(){
  ProcessorId = 0;
  Status = 0;
}


Processor::Processor(int id, int stat){
  ProcessorId = id;
  Status = stat;
}