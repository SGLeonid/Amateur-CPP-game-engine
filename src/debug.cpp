#include <debug.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

template <typename T>
std::string format(T val) {
    std::stringstream stream;
    stream << val;
    return stream.str();
}

template <typename T, typename ...Args>
std::string format(T first, Args ...args) {
    std::string str = format(first);
    str += format(args...);
    return str;
}

Logger::Logger() : tag("DEFAULT"), flags(DEBUG_USE_CONSOLE) {}

Logger::Logger(const char *_tag) : tag("DEFAULT"), flags(DEBUG_USE_CONSOLE) {}

Logger::Logger(const char *_tag, const char *filename) : tag("DEFAULT"), logFile(filename), flags(DEBUG_USE_CONSOLE | DEBUG_USE_LOGFILE) {
    if (!(logFile.is_open())) {
        std::cerr << "Debugger initialization failed!" << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(LogLevel level, const std::string &message) {
    const char *logLevels[] = {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
    };
    
    time_t currentTime = time(0);
    tm *localTm = localtime(&currentTime);
    
    std::stringstream timeStr;
    timeStr <<
        std::setfill('0') << std::setw(2) << localTm -> tm_hour << ":" <<
        std::setfill('0') << std::setw(2) << localTm -> tm_min << ":" <<
        std::setfill('0') << std::setw(2) << localTm -> tm_sec;

    std::string logStr = format("[", timeStr.str(), "] [", tag, "] [", logLevels[level], "] ", message, "\n");

    if (flags & DEBUG_USE_CONSOLE) {
        std::cout << logStr;
    }

    if (flags & DEBUG_USE_LOGFILE && logFile.is_open()) {
        logFile << logStr;
    }
}



Logger *Debug::logger = nullptr;

void Debug::init(uint32_t flags) {
#ifdef DEBUG
    if (logger) {
        delete logger;
    }

    if (flags & DEBUG_USE_LOGFILE) {
        std::stringstream dateStr;
        std::stringstream timeStr;
        
        time_t currentTime = time(0);
        tm *localTm = localtime(&currentTime);

        dateStr << 
            std::setfill('0') << std::setw(2) << localTm -> tm_mday << 
            std::setfill('0') << std::setw(2) << localTm -> tm_mon + 1 <<
            std::setfill('0') << std::setw(4) << localTm -> tm_year + 1900;
        
        timeStr << 
            std::setfill('0') << std::setw(2) << localTm -> tm_hour <<
            std::setfill('0') << std::setw(2) << localTm -> tm_min << 
            std::setfill('0') << std::setw(2) << localTm -> tm_sec;

        std::string filename = format("log_", dateStr.str(), "_", timeStr.str(), ".txt");
    
        logger = new Logger("DEBUG", filename.c_str());
    } else {
        logger = new Logger("DEBUG");
    }
#endif
}

void Debug::log(LogLevel level, std::string message) {
#ifdef DEBUG    
    if (!logger) {
        logger = new Logger("DEBUG");
    }
    logger -> log(level, message);
#endif
}