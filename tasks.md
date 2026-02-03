# Session 2

Create a Google Colab shared in groups of 3 (100 points).

Each student is responsible for at least one C++ program and a third of the shell commands.

Use Markdown boxes to make your interactive guide attractive and provide instructions.

Build an interactive tutorial that teaches all the code and commands, and in the process, learn them yourselves.

Groups of 4 to 5 will be asked to merge results afterward.

Rubric:

- Poor (result is no better than the best individual group) (5)
- Meets requirements (results combined, the combined group demonstrates how the combined demo is better than any single
  group) (10)
- Exceeds (new features added which significantly improve the tutorial) + (5-10, depending on quality)

## Tasks

1. Tutorial on C++ data types:
	- `char`
	- `short` / `short int`
	- `int`
	- `long` / `long int`
	- `long long int`
	- `unsigned` qualifier
	- `signed` qualifier
	- `float`
	- `double`
	- `long double` (this is not quad precision; it is not specified how big it is)

   Show examples:
	- Min/max value for each data type
	- Overflow for each data type
	- Portable integer data types:
		- `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`
		- `int8_t`, `int16_t`, `int32_t`, `int64_t`

2. Calculate ${n\choose r} = \frac{n!}{r!(n - r)!}$
	- This will overflow. Demonstrate overflow for `uint64_t` for ${52\choose 6}$.
	- Then write one that will not overflow because you cancel the numbers:
	  $\frac{52\times51\times50\times49\times48\times47}{6\times5\times4\times3\times2\times1}$
	- Even this one will blow up for bigger numbers like ${200\choose 20}$, so do another version in double precision
	  that can give an approximate answer.

3. Scope and lifetime:
	- Write a program demonstrating how variable values live and die.
	- Auto variables (uninitialized, on the stack, or in registers)
	- Global (pre-initialized to zero, born before and after main)
	- Static (same as global, but only visible within the function they are declared in)

4. Write a program to:
	1. Write to a file.
	2. Write to standard out and standard error and demonstrate redirecting those to files.
	3. Modify my low-performance level 2 I/O to write to a file and make it more efficient.
	4. Look up how to find out what the block size of your drive is (optimal performance should use a buffer that is an
	   integer multiple of block size, and probably a power of 2).

5. Demonstrate:
	- How C++ generates assembly code. Show the compiled code for `+`, `*`, `/` for:
		1. Unoptimized and optimized code
		2. 32-bit integers
		3. 64-bit integers
		4. Double precision floating point
	- How programs are constructed using a linker:
		- Global and extern variables
		- Function prototypes
	- Show an error when a function is called in one file that:
		- Does not exist
		- Exists but has the wrong types or number of parameters

6. Pointer math:
	- Demonstrate
		- How arrays in C/C++ work
		- How you can calculate the size of variables using `sizeof()` and how this works for arrays declared in your
		  function, but it does not work for parameters
		- How arrays are passed to functions
		- How to pass variables by reference in C and C++:
			1. Using pointers (traditional, the Google style)
			2. Using reference (the new C++ way, not really better)
		- Pointer errors:
			1. Printing the value at a pointer that has never been set (crash)
			2. Writing a value to a pointer that has never been set (crash)
			3. Assuming that variables declared next to each other are next to each other (not guaranteed)
			4. Allocating memory and never freeing it
			5. Freeing memory twice
			6. Allocating a pointer, changing where it points to, and freeing it (crash)
			7. Using `sizeof()` on an array or pointer parameter: it is always the size of a pointer, which is 8 bytes
			   on our machine
			8. Accidentally writing just before a dynamically allocated array, so it thinks that there is a different
			   amount of memory; when you free it (crash)

7. Setup:
	- Logins on Google Cloud server (Nikhil shared server)
	- Create a GitHub account if you do not have one
	- Put your GitHub account in a shared file (Nikhil)

8. Tutorial on shell commands:
	- `cp`, `mv`, `ln`, `ln -s`
	- `ps`, `top`
	- `kill`, `pkill`
	- `ssh-keygen`
	- `ssh`
	- `file`
	- Redirecting output:
		- `./prog > myfile`
		- `./prog 2> myotherfile`
		- `./prog > myfile2 2> myfile3`
		- `./prog < myinputfile`
	- Build commands:
		- `g++ prog.cpp`
		- `g++ prog.cpp -o myprog`
		- `g++` (options for debugging, optimization)
		- `make`
		- `rclone` to copy to Google Drive

## Next time

- `g++` linking libraries
- `ldd` to find out which shared libraries a program uses

## Additional tasks

1. Rename the file using the `mv` command.
2. Renumber all lecture files, so they sort in order. Add a leading zero to all single-digit files. To do this, use:

   ```
   for f in lecture\ [1-9]\ *; do; your command goes here; done
   ```
