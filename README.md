Logging-System
==============

This is a project in the "Algorithms &amp; Data Structures" course in the department of Electrical Engineering and Computer Science at the University of Patras, for the academic year 2010 - 2011.

<img src="http://i76.photobucket.com/albums/j16/paign10/DataStructure_zpsb134390a.png" alt="data-structure" width="800" align="center"/>

It involves parsing some purchase records, structuring those records in memory, and asking questions about the people who made those purchases. You can find more info at [paign10.me](http://paign10.me/projects/2011-08-31-logging-system.html).


Compilation
===========

On windows, you can use CMake GUI to create a solution for Visual Studio.
- - - 
If you are on Linux, be aware that you will have to find (a replacement for) `conio.h`.
```
# cd into the project's folder
mkdir build
cd build
cmake ..
make
./loggingSystem
```
