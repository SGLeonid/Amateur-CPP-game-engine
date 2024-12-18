#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H

#include <cstdint>
#include <string>
#include <fstream>

#define DEBUG_USE_CONSOLE 0x1
#define DEBUG_USE_LOGFILE 0x2
#define DEBUG_USE_RESERVE 0x4
#define DEBUG_USE_EVERYTHING 0x7

typedef enum {
    DEBUG_TRACE,
    DEBUG_DEBUG,
    DEBUG_INFO,
    DEBUG_WARN,
    DEBUG_ERROR,
    DEBUG_FATAL
} LogLevel;

class Logger {
    public:
    Logger();
    Logger(const char *_tag);
    Logger(const char *_tag, const char *filename);
    
    ~Logger();

    void log(LogLevel level, const std::string &message);

    // template <typename T, typename ...Args>
    // void log(LogLevel level, T first, Args ...args) {
        // log (format(first, ...args));
    // }

    private:
    std::string tag;
    std::ofstream logFile;
    uint32_t flags;
};

class Debug {
    public:
    static void init(uint32_t flags);
    static void log(LogLevel level, std::string message);

    private:
    static Logger *logger;
};

#include <assert.hpp>

#endif