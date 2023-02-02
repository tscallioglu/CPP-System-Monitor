# Cpp-System-Monitor

This code is the  System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program]. It is a system monitor program written in C++ that resembles the program htop, which is a standart program for monitoring Linux systems, similar to the Process Explorer or Process Monitor applications in Windows. 
The top half the screen is monitoring the CPU Usage , RAM Usage and some system-wide data. The bottom half of the screen, there are processes sorted by the Cpu usage. 

![System Monitor](images/monitor.png)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

To install ncurses within Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/tscallioglu/CppND-System-Monitor.git`

2. Build the project from the project folder: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)
