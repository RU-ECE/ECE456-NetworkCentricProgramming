#pragma once
#include <atomic>
#include <cstddef>

using namespace std;

constexpr char shm_name[] = "/logclient_shm";
constexpr char sem_name[] = "/logclient_sem";

constexpr size_t LOG_MSG_SIZE = 256;
constexpr size_t LOG_BUFFER_SIZE = 64;

struct LogMessage {
	size_t length;
	char text[LOG_MSG_SIZE];
};

struct SharedLogBuffer {
	atomic<size_t> head{0}; // producer writes here
	atomic<size_t> tail{0}; // logger reads here
	LogMessage messages[LOG_BUFFER_SIZE];
};
