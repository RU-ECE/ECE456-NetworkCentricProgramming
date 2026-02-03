# Setup

1. If you do not have a laptop, please get in touch with me. We can see about a loaner, if possible.
2. If you have Windows, you should install WSL (Windows Subsystem for Linux), which will allow you to use Linux on your
   laptop, with about 1 GB of space, plug in whatever you use.
3. If you have a Mac, you may use it for much of the course. For reverse-engineering code, we will use Intel assembly,
   so you will need to use either the VLab or a GCP server.
4. Sign up for Google GCP: https://cloud.google.com/edu/students?hl=en
5. As a Rutgers student or professor, you are all entitled to Colab Pro, which will give you 100 units per month of
   computation on Colab, a nice benefit. How do we sign up for this? [TBD, help!]
6. On any Linux machine you set up, you will need software. On GCP or Colab, we will use a departmental drive, so you do
   not have to install all the software. However, it is still a great idea to learn to install it yourself. Moreover, if
   you want to install on WSL, you can. Here is a list of what we will be using (this list may grow):
	- gcc/g++
	- gdb (debugger)
	- git (version control)

# Basic Linux Setup

```shell
sudo apt update
sudo apt install -y build-essential g++ gdb git cmake pkg-config libssl-dev \
  openssh-client openssh-server libc6-dev libpthread-stubs0-dev zlib1g-dev \
  valgrind strace ltrace net-tools iproute2 tcpdump curl wget
```
