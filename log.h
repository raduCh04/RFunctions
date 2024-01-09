#pragma once

typedef enum LogLevel
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_COUNT,
} LogLevel;

// Returns 0 on failure. If nothing shows up (or the program crashes) check the return value of the function!
int log_init(void);
// Returns 0 on failure. If nothing shows up (or the program crashes) check the return value of the function!
int log_output(LogLevel level, const char*, ...);

#define LOG_FATAL(str, ...) log_output(LOG_LEVEL_FATAL, str, __VA_ARGS__)
#define LOG_ERROR(str, ...) log_output(LOG_LEVEL_ERROR, str, __VA_ARGS__)
#define LOG_WARNING(str, ...) log_output(LOG_LEVEL_WARNING, str, __VA_ARGS__)
#define LOG_INFO(str, ...) log_output(LOG_LEVEL_INFO, str, __VA_ARGS__)
#define LOG_DEBUG(str, ...) log_output(LOG_LEVEL_DEBUG, str, __VA_ARGS__)
#define LOG_TRACe(str, ...) log_output(LOG_LEVEL_TRACE, str, __VA_ARGS__)