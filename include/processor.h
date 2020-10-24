#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
    
  
 private:
  long lastActive_{0};
  long lastTotal_{0};
  float lastLoad_{0};
};

#endif