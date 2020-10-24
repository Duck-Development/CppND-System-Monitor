#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>
using std::ostringstream;
using std::string;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  auto hour = seconds / (60 * 60);
  seconds -= hour * (60 * 60);
  auto minute = seconds / 60;
  seconds -= minute * 60;
  ostringstream os;
  os << std::setfill('0') << std::setw(2) << hour << ":";
  os << std::setfill('0') << std::setw(2) << minute << ":";
  os << std::setfill('0') << std::setw(2) << seconds;

  return os.str();
}