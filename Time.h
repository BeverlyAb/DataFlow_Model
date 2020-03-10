//Time.h
#ifndef TIME_H
#define TIME_H
#include <string>
using namespace std;

class Time{
  private:
    struct tm * timeinfo;

  public:
  Time(){
    time_t rawtime;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
  }
  string getTime(){return asctime(timeinfo);}
};
#endif