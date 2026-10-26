#include "logger.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      // Color for the message
#define RED     "\033[31m"      // for errors

std::vector<LogEntry> Logger::messages;

/*
 * Returns the current timestamp in the format "dd-mm-yyyy hh:mm:ss"
*/
string Logger::timeStamp() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&currentTime);
    ostringstream putTimeResult;
    putTimeResult << put_time(localTime, "%d-%m-%Y %H:%M:%S");
    return putTimeResult.str();
}

void Logger::Log(const string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [ " + timeStamp() + " ] - " + message;
    cout << GREEN << logEntry.message << RESET << endl;
    logEntry.message = logEntry.message + "\n";
    messages.push_back(logEntry);
}

void Logger::Err(const string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERROR: [ " + timeStamp() + " ] -  " + message;
    cerr << RED << logEntry.message << RESET << endl;
    logEntry.message = logEntry.message + "\n";
    messages.push_back(logEntry);
}
