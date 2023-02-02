#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <format.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Functions reads the system data in files in Linux system

// Read and return  Operating System Name from the filesystem
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



// Read and return  Kernel Version from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Read and return  Process IDs from the filesystem
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






// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value, kb;
  string line;
  float memTotal;
  float memFree;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value>>kb) {
        if (key == "MemTotal:") {
          memTotal = (std::stof(value));
        }
        else if (key == "MemFree:"){
          memFree = (std::stof(value));
        }
      }
   }
  }
  return (memTotal - memFree)/memTotal;
}


// Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}






// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return (LinuxParser::UpTime() * sysconf (_SC_CLK_TCK));
 }



// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  string pidNum = to_string(pid);
  long ajiffies{0};

  std::ifstream stream(kProcDirectory + pidNum + kStatFilename);
  if (stream.is_open()) { 
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i=0; i<= 16; ++i){
        linestream >> value;

        if (i>12){ 
          if (value.empty()) { continue;}
          ajiffies += std::stol(value); }
      }
      return ajiffies;
    }
  }
  return ajiffies;
 }


// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long ajiffies{0};
  string value;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i<=10; i++){
          linestream >> value;
          if (i>0) { 
            if (value.empty()) { continue;}
            ajiffies += std::stol(value);}
      }
      return ajiffies;
    }
  }
  return ajiffies;
}


// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long ijiffies{0};
  string value;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i<=5; i++){
          linestream >> value;
          if (i>3) { 
            if (value.empty()) { continue;}
            ijiffies += std::stol(value);}
        }
      return ijiffies;
      }
    }
    return ijiffies;
  }



// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, value;
  vector<string> cpu_utilisation;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> value) {
      cpu_utilisation.emplace_back(value);
    } 
  }
  return cpu_utilisation; 
}



// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  string line;
  int processes;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value ) {
        if (key == "processes") {
          processes = (std::stoi(value));
          return processes;
        }
      }
   }
  }
  return processes;
 }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  int processes;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "procs_running") {
          processes = (std::stoi(value));
          return processes;
        }
      }
    }
  }
  return processes;
}


// --------------------------------------------------------------------------->

// Read and return Cpu Utilization associated with a process
float LinuxParser::CpuUtilization(int pid) { 
  string line, value;
  int i = 0;
  float total_time;
  float seconds;

  vector<string> cpu_util;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value && i<23) {
      cpu_util.emplace_back(value);
      i +=1;
    }
  }
  total_time = ( stof(cpu_util[13]) + stof(cpu_util[14]) + stof(cpu_util[15]) + stof(cpu_util[16]) );
  seconds = LinuxParser::UpTime() - (stof(cpu_util[21]) / sysconf(_SC_CLK_TCK));
  return (total_time/sysconf(_SC_CLK_TCK)/seconds);
}



// Read and return the Command associated with a process
string LinuxParser::Command(int pid) { 
  string value, line;
  string pidNum = to_string(pid);

  std::ifstream stream(kProcDirectory + pidNum + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value.substr(0, 20);
}



// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value;
  string pidNum = to_string(pid);

  std::ifstream stream(kProcDirectory + pidNum + kStatusFilename);
  if (stream.is_open()) { 
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value ) {
        if (key == "VmSize:") {
          return std::to_string(std::stol(value)/1024);
        }
      }
    }
  }
  return "";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key, value;
  string pidNum = to_string(pid);

  std::ifstream stream(kProcDirectory + pidNum + kStatusFilename);
  if (stream.is_open()) { 
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  } 
  return "";
}

// Read and return the  user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string key, value, uid;
  string pidNum = to_string(pid);

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) { 
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return key;
        }
      }
    }
  }
  return "";
}



// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  string pidNum = to_string(pid);
  string uptime;

  std::ifstream stream(kProcDirectory + pidNum + kStatFilename);
  if (stream.is_open()) { 
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0; i<22; ++i){
      linestream >> uptime;
    }
    return (std::stol(uptime) / sysconf(_SC_CLK_TCK));
  }
  return 0;
}
















