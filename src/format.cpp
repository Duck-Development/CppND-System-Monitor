#include "format.h"
#include <iomanip>
#include <string>
#include <sstream>
using std::string;
using std::ostringstream;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] onceoo you define the function
string Format::ElapsedTime(long seconds) { 
    auto hour = seconds / (60*60);
    seconds -= hour*(60*60);
    auto minute = seconds / 60;
    seconds -= minute*60;
    ostringstream os;
    os << std::setfill('0') << std::setw(2) << hour << ":";
    os << std::setfill('0') << std::setw(2) << minute << ":";
    os << std::setfill('0') << std::setw(2) << seconds;

    return os.str();
 }