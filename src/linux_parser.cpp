#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, tmp ;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> tmp >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 

  string line , tmp ;
  float res = 0;
  long total, free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream totalStream(line);
    totalStream >> tmp  >> total;
    std::getline(stream, line);
    std::istringstream freeStream(line);
    freeStream >> tmp  >> free;

    res = static_cast<float>(total-free)/static_cast<float>(total);

  }

return res; 
}

long LinuxParser::UpTime() { 

  float uptime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return (long)uptime;
  
}

long LinuxParser::Jiffies()  { 
  //cpu  35901 95 12512 570306 585 0 4211 0 0 0
   string line;
  string cpu;
  long  user, nice, system, idle, iowait, irq, soft_irq,steal,guest,guest_nice;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user>> nice>> system>> idle>> iowait>> irq>> soft_irq>>steal>>guest>>guest_nice;
  }
  return user+ nice+ system+ idle+ irq+ soft_irq+steal+guest+guest_nice;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }


long LinuxParser::ActiveJiffies()  { 
  //cpu  35901 95 12512 570306 585 0 4211 0 0 0
   string line;
  string cpu;
  long  user, nice, system, idle, iowait, irq, soft_irq,steal,guest,guest_nice;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user>> nice>> system>> idle>> iowait>> irq>> soft_irq>>steal>>guest>>guest_nice;
  }
  return user+ nice+ system+  irq+ soft_irq+steal+guest+guest_nice;
 }


long LinuxParser::IdleJiffies()  { 
  //cpu  35901 95 12512 570306 585 0 4211 0 0 0
   string line;
  string cpu;
  long  user, nice, system, idle, iowait, irq, soft_irq,steal,guest,guest_nice;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user>> nice>> system>> idle>> iowait>> irq>> soft_irq>>steal>>guest>>guest_nice;
  }
  return idle;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() { 
   string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >>value;
        if (key == "processes") {
          break;
      } else
      {
        value = 0;
      }
      
    }
  }
  return value;
 }

int LinuxParser::RunningProcesses() { 
   string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >>value;
        if (key == "procs_running") {
          break;
      } else
      {
        value = 0;
      }
      
    }
  }
  return value;
 }

string LinuxParser::Command(int pid ) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory +to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >>value;
        if (key == "Name:") {
          break;
      } 
    }
  }
  return value;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid ) { 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory +to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >>value;
        if (key == "VmSize:") {
          break;
      } 
    }
  }
  return to_string(value/1000);
 }


string LinuxParser::Uid(int pid){ 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory +to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >>value;
        if (key == "Uid:") {
          break;
      } 
    }
  }
  return value;
 }

string LinuxParser::User(int pid) {
  auto uid = Uid(pid);
  string line, tmp;
  string key;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >>value >> tmp >> key;
        if (key == uid) {
          break;
      } 
    }
  }
  return value;

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }