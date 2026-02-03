Session 2:

HW
  Create a google colab shared in groups of 3 (100 points)
  Each student is responsible for at least 1 C++ program, and 1/3 of the shell commands
  Use Markdown boxes to make your interactive guide attractive and provide instructions

  build an interactive tutorial teaching all the code and commands and in the process learn it yourselves.
  groups of 4-5 groups will be asked to merge results afterward.
  Second homework
  Rubric:
  poor (result is no better than the best individual group) 5
  meets requirements (results combined, combined group demonstrates how the combined demo is better than any single group, 10)
  exceeds: (new features added which significantly improves tutorial) +5-10 depending on quality

1. Tutorial on C++ data types:
   char
   short /  short int
   int
   long / long int
   long long int
   unsigned qualifier
   signed qualifer

   Show examples
     min/max value for each data type
     overflow for each data type.

  portable integer data types
    uint8_t, uint16_t, uint32_t, uint64_t
    int8_t, int16_t, int32_t, int64_t 

  float
  double
  long double (this is not quad precision, it's not specified how big it is)

2. Calculate choose(n, r) = n!/(r!(n-r)!)
  this will overflow. demonstrate overflow for uint64_t for choose(52,6) then wirite one that won't overflow because you cancel the numbers:
   $ 52*51*50*49*48*47 / (6*5*4*3*2*1)$
  Even this one will blow up for bigger numbers like: choose(200, 20) so do another version in double precision that can give an approximate answer

3. scope and lifetime
  write a program demonstrating how variable values live and die
  auto variables (uninitialized, on the stack, or in registers)
  global (pre-initialized to zero, born before and after main
  static (same as global, but only visible within the function they are declared in)

4. Write a program to
  a. write to a file
  b. write to standard out and standard error and demonstrate redirecting those to files
  c. Modify my low-performance level 2 I/O to write to file and make it more efficient.
  d. look up how to find out what the block size of your drive is (optimal performance should use a buffer that is an integer multiple of block size, and probably a power of 2).

4. Demonstrate
   a. how C++ generates assembly code. Show the compiled code for
     +   * /  for
       1. unoptimized and optimized code
       2. 32-bit integers
       3. 64-bit integers
       4. double precision floating point
  How programs are constructed using a linker
    global and extern variables
    function prototypes

  show an error when a function is called in one file that:
    a. does not exist
    b. exists but has the wrong types or number of parameters

3. pointer math
  demonstrate
  a. how arrays in C/C++ work
  b. how you can calculate the size of variables using sizeof() and that this works for arrays declared in your function, but does not work for parameters
  c. how arrays are passed to functions
  d. Howto pass variables by reference in C and C++
    1. using pointers (tradition, the google style)
    2. using reference (the new C++ way, not really better)
  e. Pointer errors
    1. Printing the value at a pointer that has never been set (crash)
    2. writing a value to a pointer that has never been set (crash)
    3. Assuming that variables declared next to each other are next to each other (not guaranteed)
    4. allocating memory and never freeing it
    5. freeing memory twice
    6. allocating a pointer, changing where it points to, and freeing it (crash)
    7. using sizeof() on an array or pointer parameter it's always the size of a pointer, which is 8 bytes on our machine
    8. accidentally writing just before a dynamically allocated array, so it thinks that there is a different amount of memory, so when you free it (crash)

4. Setup
  a. logins on Google Cloud server (Nikhil shared server)
  b. create a github account if you do not have one
  c. Put your github account in a shared file (Nikhil)

5. Tutorial on shell commands
   cp, mv, ln, ln -s
   ps, top
   kill, pkill
   ssh-keygen
   ssh
   file

   redirecting output:
      ./prog >myfile
      ./prog 2>myotherfile
      ./prog >myfile2 2>myfile3
      ./prog <myinputfile
      

   g++ prog.cpp
   g++ prog.cpp -o myprog
   g++  (options for debugging, optimization)
   make
   rclone to copy to google drive



next time
   g++ linking libraries
   ldd to find out which shared libraries a program uses



1. rename file using the mv command
1. 
1. Renumber all lecture files so they sort in order. Add a leading zero to all single digit files. To do this, use:

for f in lecture\ [1-9]\ *; do;  your command goes here ; done

