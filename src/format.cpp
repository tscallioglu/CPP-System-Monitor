#include <string>

#include "format.h"

using std::string;

// Helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    long h = (seconds / 3600);
    long m = (seconds % 3600) / 60;
    long s = (seconds % 3600) % 60 ;
    
    string hh = ((h<10)? ("0" + std::to_string(h)) : (std::to_string(h)));
    string mm = ((m<10)?  ("0" + std::to_string(m)):(std::to_string(m)));
    string ss = ((s<10)?  ("0" + std::to_string(s)):(std::to_string(s)));

    return hh + ":" + mm + ":" + ss;

    }