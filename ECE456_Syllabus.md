# ECE456 - Network Centric Computing Syllabus

## Expectations
   - Everyone is expected to come to class every time
   - If you don't come to class, find out what you missed from a friend. Don't expect a private recap of a 3 hour class
   - DO NOT
       - Ask about grades unless there is a mistake.
           - Asking about getting your grade changed because you need it will reduce your grade.
           - Tell me about your medical conditions. If you have a legitimate excuse for a long absence, contact the dean of students who contacts all the professors. You do not have to share private health information with us.
       - Send private messages or emails about grades. If you must ask, come see me before class or during office hours.
       - Ask me for a recommendation if all you did was take the class. You must have interacted with me significantly.
       - Assume that I check your messages on Saturday night and send multiple messages when I don't answer the first within 12 hours.
       - Ask me if you can come to office hours. That's what they are for.
       - Let an LLM do the thinking for you. If you don't learn to code, you will be worthless in the job market. AI may eventually replace coding. But for the moment, the people who get jobs have to know how code works. The AI can do 90% of the work but the remaining 10% includes understanding how it works. The AI is not yet intelligent. It doesn't know what it's doing. It's just memorized a lot.
    - DO
       - Ask about the material in office hours
       - Make an appointment (on discord, on zoom, or in person) if you can't attend office hours.
       - Your work on time each week.
       - The readings
       - Write a lot of code so you learn to program well. You cannot learn by having chatgpt do it for you.
       - Use Chatgpt or an AI-enabled editor to check your work
          - Ask "what is wrong with this code?" Or "Why am I getting this error?"
          - Ask "Is there anything I could do to improve this code"

## Topics
1. Setup
   - Do you have a laptop? If not please contact me. I will try to arrange a loaner. You can live without one, but it is nice to have.
   - Do you want to buy a laptop? Prices rising sharply on RAM. If you want to lock in, Costco currenty $379 for the one I am using and a number of others for slightly more. Since the price is RAM is now more than that, I suspect this will change very fast!
      [Costco laptops](https://www.costco.com/s?keyword=laptop)

1. Learning Unix Shell commands for development
   - A curated set of the most important Unix commands.
   - You will not be tested on them, but to be a competent developer you have to be able to get around.
   - We do not have time to lecture on all these. Self-tests will be provided. You will see me using a lot of these in the course.
      - developer tools
        - gcc, g++, ld, as
        - gdb, gprof
        - clang, clang++, lldb
        - objdump, readelf, nm
        - ldd
        - xxd, hexdump, od
        - make, cmake
        - ar, ranlib (static libraries)
        - strings
        - strace, ltrace (system/library call tracing)
        - valgrind (memory debugging)
        - perf (performance profiling)
        - man (manual pages)
      - file
        - ls, pwd, cp, mv, ln, ln -s
        - find, grep, locate
        - tree
        - chmod, chown, chgrp (permissions)
        - stat (file information)
        - file (file type)
        - touch, mkdir, rmdir, rm
        - tar, gzip, gunzip, zip, unzip
        - diff, patch
        - head, tail, less, more, cat
        - wc (word count)
      - pipeline
        - date
        - cut, awk, sed
        - sort, uniq
        - tr (translate characters)
        - tee (split output)
        - xargs
      - process management
        - which, kill, pkill, killall
        - top, htop, ps, pgrep, pstree
        - jobs, fg, bg, nohup
        - ulimit (resource limits)
        - nice, renice (priority)
        - wait, sleep
      - benchmarking
        - time, /usr/bin/time (execution time measurement)
        - times (shell builtin)
        - hyperfine (modern benchmarking tool)
        - strace -c (system call timing)
        - perf stat (hardware performance counters)
      - commands
        - history
      - networking
        - netstat, ss (socket statistics)
        - ip, ifconfig, route
        - nslookup, host, dig
        - ping, traceroute, tracepath
        - nc (netcat - network testing)
        - telnet (nearly obsolete, testing connections)
        - curl, wget (HTTP testing)
        - tcpdump, wireshark (packet capture/analysis)
        - iptables, ufw (firewalls)
        - iostat, netstat -i (network interface stats)
        - ssh, scp, ssh-copy-id ssh-keygen, sftp, sshd
      - system discovery
        - uname, lsb_release
        - lsblk, lspci, lscpu
        - df, du (disk usage)
        - mount, umount
        - free, vmstat (memory stats)
        - dmesg (kernel messages)
        - requiring sudo (admin rights)
          - sudo, su
          - systemctl, service (service management)
      - miscellaneous topics
        - apt install, dpkg -i, apt update, apt upgrade
        - editing in shell, emacs commands
        - piping commands together
        - redirecting stdin, stdout and stderr
        - programming editors
            - vi, vim
            - emacs
            - vscode
            - cursor
        - Shell variables
            - PATH
            - LD_LIBRARY_PATH
            - C_INCLUDE_PATH, CPLUS_INCLUDE_PATH
            - PKG_CONFIG_PATH
        - environment
            - env, export, set, unset
            - alias, unalias
            - source, .
        - text processing
            - sed, awk (already in pipeline)
            - tr, rev
            - comm, join
        - version control
            - git (basic commands)
1. Using Virtual machines in the cloud
   - Google colab: we will be doing some labs/homeworks using google colab to give interactive access to a Unix machine in the google cloud
   - Google GCP: colab remembers nothing. Each time you have to run scripts to load what you need. So you can spin up a google cloud server, and that gives you a virtual machine in the cloud. Students can run one GCP micro host (0.6cpus, 1GB RAM) for free. For this course we have \$50 credit per student so you can do more without paying. You will have to sign up your rutgers email for google cloud.
   - Amazon Web Services (AWS) I haven't learned this, but you are welcome to try on their own. AWS is the leading provider of cloud computing worldwide.
   - HW: You will learn a subset of the Linux commands listed above following instructions in a colab step by step.
1. I/O
   - In class we will cover C I/O (fopen, fprintf, printf, scanf, etc).
   - C++ I/O
   - In-class exercises:
     - Given a prime number program, modify to only print prime numbers that have 7s in them.
     - Read in lines, find lines containing a word, count the numbers of occurrences of that word.
     - Read in lines, break into tokens, store all tokens in a dictionary. print the most common k tokens.
   - HW: Follow instructions and set up your google GCP server and ssh on your computer so you can remotely log in.
1. High performance coding
   - understanding how C++ generates machine language code
   - Compiler optimizations
   - Tuning a program
   - Inclass
     - Writing a file using C (we provide the code)
     - Writing a file in C++ (you write the code)
     - Writing high performance binary files
1. Client/Server
   - Networks
   - IP addresses
   - DNS
   - ssh
   - configuring ssh for automatic login
   - Inclass
     - Write a simple client/server in colab
     - Debug a simple client/server using gdb
   - HW
     - configure your GCP server for remote access 
   - 
   
## Books

## Schedule

| Week | Date   | Topic        | Description                                                         | Readings | HW |
|------|--------|--------------|---------------------------------------------------------------------|----------|----|
| 1    | 1/26   | Introduction | Syllabus, developing in C/C++, hands on with colab programming C/C++|          | Sign up for collab pro, GCP, install WSL on windows or create your VM. You may just use vlab if you don't have resources at home. If you have any kind of computer, better to use colab and a google VM. |
| 2    | 2/2    | Linux/Unix   | Unix commands, hands-on lab, C++ programming                        |          |    |
| 3    | 2/9    | Networking. Sockets programming. | | |
| 4    | 2/16   | | |
| 5    | 2/23   | | |
| 6    | 3/2    | | |
| 7    | 3/9    | | | 
| 8    | 3/16   | Midterm on Paper. You will have to prove you learned the programming you submit. Code with an AI or without, but you have to know it. No Excuses! | |
| 9    | 3/23   | | |
| 10   | 3/30   | | |
| 11   | 4/6    | | |
| 12   | 4/13   | | |
| 13   | 4/20   | | |
| 14   | 4/27   | | |
| 15   | 5/4    | | |
| 16   | 5/11   | | |
| 17   | 5/18   | | |
