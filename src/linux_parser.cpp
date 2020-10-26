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
  auto res = CpuUtilization();
  long sum = 0;
  for(long unsigned int i = 0 ; i < res.size() ; i++)
  {
    if ( i==kIOwait_){
      continue;
    }
    sum +=res[i];
  }
  return sum;
 }

long LinuxParser::ActiveJiffies(int pid )
{ 
  string line;
  string tmp;
  long utime , stime , cutime , cstime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (auto i = 0 ; i < 18 ; i++)
    {
      switch (i)
      {
      case 14:
        linestream >> utime;
        break;
      case 15:
        linestream >> stime;
        break;
      case 16:
        linestream >> cutime;
        break;
        case 17:
        linestream >> cstime;
        break;
      
      default:
      linestream >> tmp;
        break;
      }
    }
  }
  return utime+stime+cutime+cstime; 
}


long LinuxParser::ActiveJiffies()  { 
  auto res = CpuUtilization();
  long sum = 0;
  for(long unsigned int i = 0 ; i < res.size() ; i++)
  {
    if (i == kIdle_  || i==kIOwait_){
      continue;
    }
    sum +=res[i];
  }
  return sum;
 }


long LinuxParser::IdleJiffies()  { 
  
  auto res = CpuUtilization();
  return res[kIdle_];
 }


vector<long> LinuxParser::CpuUtilization() { 
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
  vector<long> res(kCPUSize);
  res[kUser_] = user;
  res[kNice_] = nice;
  res[kSystem_] = system;
  res[kIdle_] = idle;
  res[kIOwait_] = iowait;
  res[kIRQ_] = irq;
  res[kSoftIRQ_] = soft_irq;
  res[kSteal_] = steal;
  res[kGuest_] = guest;
  res[kGuestNice_] = guest_nice;
  
  return res;
 }

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
  string filename = kProcDirectory +to_string(pid) + kCmdlineFilename;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    std::getline(filestream, value);
  } else
  {
    value = "no file: " +filename;
  }
  
  return value;
 }

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

long LinuxParser::UpTime(int pid ){ 
  string line , tmp;
  string key;
  long uptime = 0;
  string filename = kProcDirectory +to_string(pid) + kStatFilename;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (auto i = 1 ; i < 22 ; i++)
    {
      linestream >>  tmp;
    }
      linestream >>  uptime; 
      uptime= uptime / sysconf(_SC_CLK_TCK);
  }
  return uptime;
 }