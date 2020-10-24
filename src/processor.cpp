#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
    auto newAktive = LinuxParser::ActiveJiffies();
    auto newTotal = LinuxParser::Jiffies();
    auto deltaAktive = newAktive-lastActive_;
    auto deltaTotal = newTotal-lastTotal_;
    
    if   (deltaTotal != 0  )  {
        lastTotal_ = newTotal;
        lastActive_ = newAktive;
        lastLoad_ = static_cast<float>(deltaAktive)/static_cast<float>(deltaTotal);
    }
    return lastLoad_; 

 }