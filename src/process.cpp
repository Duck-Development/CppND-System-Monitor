#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const {
  auto activ = LinuxParser::ActiveJiffies(pid_);
  auto time = LinuxParser::UpTime() - static_cast<long>(LinuxParser::UpTime(pid_));
  auto load =
      (static_cast<float>(activ) / static_cast<float>(sysconf(_SC_CLK_TCK))) /
      static_cast<float>(time);
  return load;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() const { return LinuxParser::UpTime() -  LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}