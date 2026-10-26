#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
using namespace std;


enum LogType {
    LOG_INFO,
    LOG_WARING,
    LOG_ERROR
};

struct LogEntry {
    LogType type;
    string message;
};

class Logger {
    private:
        static string timeStamp();
    public:
        static vector<LogEntry> messages;
        static void Log(const string& message);
        static void Err(const string& message);
};

#endif
