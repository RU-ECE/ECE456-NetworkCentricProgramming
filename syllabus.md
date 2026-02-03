# ECE 456: Network Centric Computing Syllabus

## Expectations

- Everyone is expected to come to class every time
- If you do not come to class, find out what you missed from a friend. Do not expect a private recap of a 3-hour class.
- DO NOT
	- Ask about grades unless there is a mistake.
		- Asking about getting your grade changed because you need it will reduce your grade.
		- Tell me about your medical conditions. If you have a legitimate excuse for a long absence, contact the dean of
		  students, who will then contact all the professors. You do not have to share private health information with
		  us.
	- Send private messages or emails about grades. If you have to ask, see me before class or during office hours.
	- Ask me for a recommendation if all you did was take the class. You must have interacted with me significantly.
	- Assume that I check your messages on Saturday night and send multiple messages when I do not answer the first
	  within 12 hours.
	- Ask me if you can come to my office hours. That is what they are for.
	- Let an LLM think for you. If you do not learn to code, you will be worthless in the job market. AI may eventually
	  replace coding. However, for the moment, the people who get jobs have to know how code works. The AI can do 90% of
	  the work, but the remaining 10% includes understanding how it works. The AI is not yet intelligent. It does not
	  know what it is doing. It has just memorized a lot.
- DO
	- Ask about the material during office hours
	- Make an appointment (on Discord, on Zoom, or in person) if you cannot attend office hours.
	- Do your work on time each week.
	- Do the readings
	- Write a lot of code, so you learn to program well. You cannot learn by having ChatGPT do it for you.
	- Use ChatGPT or an AI-enabled editor to check your work
		- Ask “What is wrong with this code?” or “Why am I getting this error?”
		- Ask “Is there anything I could do to improve this code?”

## Topics

1. Setup
	- Do you have a laptop? If not, please contact me. I will try to arrange a loaner. You can live without one, but it
	  is nice to have.
	- Do you want to buy a laptop? Prices are rising sharply on RAM. If you want to lock
	  in, [Costco currently has the one I am using for $379, and several others are slightly more](https://www.costco.com/s?keyword=laptop).
	  Since the price of RAM is now higher than that, I suspect this will change very fast.

2. Learning Unix Shell commands for development
	- A curated set of the most important Unix commands.
	- You will not be tested on them, but to be a competent developer, you have to be able to get around.
	- We do not have time to lecture on all these. Self-tests will be provided. You will see me using many of these
	  throughout the course.
		- Developer tools
			- `gcc`, `g++`, `ld`, `as`
			- `gdb`, `gprof`
			- `clang`, `clang++`, `lldb`
			- `objdump`, `readelf`, `nm`
			- `ldd`
			- `xxd`, `hexdump`, `od`
			- `make`, `cmake`
			- `ar`, `ranlib` (static libraries)
			- `strings`
			- `strace`, `ltrace` (system/library call tracing)
			- `valgrind` (memory debugging)
			- `perf` (performance profiling)
			- `man` (manual pages)
		- File
			- `ls`, `pwd`, `cp`, `mv`, `ln`, `ln -s`
			- `find`, `grep`, `locate`
			- `tree`
			- `chmod`, `chown`, `chgrp` (permissions)
			- `stat` (file information)
			- `file` (file type)
			- `touch`, `mkdir`, `rmdir`, `rm`
			- `tar`, `gzip`, `gunzip`, `zip`, `unzip`
			- `diff`, `patch`
			- `head`, `tail`, `less`, `more`, `cat`
			- `wc` (word count)
		- Pipeline
			- `date`
			- `cut`, `awk`, `sed`
			- `sort`, `uniq`
			- `tr` (translate characters)
			- `tee` (split output)
			- `xargs`
		- Process management
			- `which`, `kill`, `pkill`, `killall`
			- `top`, `htop`, `ps`, `pgrep`, `pstree`
			- `jobs`, `fg`, `bg`, `nohup`
			- `ulimit` (resource limits)
			- `nice`, `renice` (priority)
			- `wait`, `sleep`
		- Benchmarking
			- `time`, `/usr/bin/time` (execution time measurement)
			- `times` (shell builtin)
			- `hyperfine` (modern benchmarking tool)
			- `strace -c` (system call timing)
			- `perf stat` (hardware performance counters)
		- Commands
			- `history`
		- Networking
			- `netstat`, `ss` (socket statistics)
			- `ip`, `ifconfig`, `route`
			- `nslookup`, `host`, `dig`
			- `ping`, `traceroute`, `tracepath`
			- `nc` (netcat — network testing)
			- `telnet` (nearly obsolete, testing connections)
			- `curl`, `wget` (HTTP testing)
			- `tcpdump`, `wireshark` (packet capture/analysis)
			- `iptables`, `ufw` (firewalls)
			- `iostat`, `netstat -i` (network interface stats)
			- `ssh`, `scp`, `ssh-copy-id`, `ssh-keygen`, `sftp`, `sshd`
		- System discovery
			- `uname`, `lsb_release`
			- `lsblk`, `lspci`, `lscpu`
			- `df`, `du` (disk usage)
			- `mount`, `umount`
			- `free`, `vmstat` (memory stats)
			- `dmesg` (kernel messages)
			- requiring sudo (admin rights)
				- `sudo`, `su`
				- `systemctl`, `service` (service management)
		- Miscellaneous topics
			- `apt install`, `dpkg -i`, `apt update`, `apt upgrade`
			- Editing in shell, `emacs` commands
			- Piping commands
			- Redirecting `stdin`, `stdout`, and `stderr`
			- Editors
				- `vi`, `vim`
				- `emacs`
				- `vscode`
				- `cursor`
			- Shell variables
				- `PATH`
				- `LD_LIBRARY_PATH`
				- `C_INCLUDE_PATH`, `CPLUS_INCLUDE_PATH`
				- `PKG_CONFIG_PATH`
			- Environment
				- `env`, `export`, `set`, `unset`
				- `alias`, `unalias`
				- `source`
			- Text processing
				- `sed`, `awk` (already in pipeline)
				- `tr`, `rev`
				- `comm`, `join`
			- Version control
				- `git` (basic commands)
3. Using virtual machines in the cloud
	- **Google Colab**: We will be doing some labs/homework using Google Colab to give interactive access to a Unix
	  machine in Google Cloud.
	- **Google GCP**: Colab remembers nothing. Each time, you have to run scripts to load what you need. So you can spin
	  up a Google Cloud server, and that gives you a virtual machine in the cloud. Students can run one GCP micro host
	  (0.6 CPUs, 1 GB RAM) for free. For this course, we have a $50 credit per student, so you can do more without
	  paying. You will have to sign up with your Rutgers email address for Google Cloud.
	- **Amazon Web Services (AWS)**: I have not yet learned this, but you are welcome to try it on your own. AWS is the
	  leading provider of cloud computing worldwide.
	- **Homework**: You will learn a subset of the Linux commands listed above, following instructions in a Colab step
	  by step.
4. I/O
	- In class, we will cover C I/O (`fopen`, `fprintf`, `printf`, `scanf`, etc).
	- C++ I/O
	- In-class exercises:
		- Given a prime number program, modify it only to print prime numbers that have sevens in them.
		- Read in lines, find lines containing a word, and count the number of occurrences of that word.
		- Read in lines, break them into tokens, store all tokens in a dictionary, and print the most common k tokens.
	- **Homework**: Follow instructions and set up your Google GCP server and SSH on your computer so you can remotely
	  log in.
5. High-performance coding
	- Understanding how C++ generates machine language code
	- Compiler optimizations
	- Tuning a program
	- In-class
		- Writing a file using C (we provide the code)
		- Writing a file in C++ (you write the code)
		- Writing high-performance binary files
6. Client/Server
	- Networks
	- IP addresses
	- DNS
	- SSH
	- Configuring SSH for automatic login
	- In class
		- Write a simple client/server in Colab
		- Debug a simple client/server using `gdb`
	- Homework
		- Configure your GCP server for remote access

## Books

## Schedule

| Week | Date | Topic                                                                                                                                             | Description                                                          | Readings | Homework                                                                                                                                                                                         |
|------|------|---------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------|----------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1    | 1/26 | Introduction                                                                                                                                      | Syllabus, developing in C/C++, hands-on with Colab programming C/C++ |          | Sign up for Colab Pro, GCP, install WSL on Windows, or create your VM. You may use VLab if you do not have resources at home. If you have a computer, it is better to use Colab and a Google VM. |
| 2    | 2/2  | Linux/Unix                                                                                                                                        | Unix commands, hands-on lab, C++ programming                         |          |                                                                                                                                                                                                  |
| 3    | 2/9  | Networking. Socket programming.                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 4    | 2/16 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 5    | 2/23 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 6    | 3/2  |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 7    | 3/9  |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 8    | 3/16 | Midterm on Paper. You will have to prove you learned the programming you submit. Code with an AI or without, but you have to know it. No Excuses! |                                                                      |          |                                                                                                                                                                                                  |
| 9    | 3/23 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 10   | 3/30 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 11   | 4/6  |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 12   | 4/13 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 13   | 4/20 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 14   | 4/27 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 15   | 5/4  |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 16   | 5/11 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
| 17   | 5/18 |                                                                                                                                                   |                                                                      |          |                                                                                                                                                                                                  |
