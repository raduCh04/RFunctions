#pragma once

/**
 * @file rlog.h
 * @author Radu-D. Chira
 * @brief A tiny logging system (Only Windows support yet)
 * @version 0.1
 * @date 2024-01-10 
 */

#include "rdefines.h"

typedef enum RLogLevel
{
    RLOG_LEVEL_FATAL = 0,
    RLOG_LEVEL_ERROR,
    RLOG_LEVEL_WARNING,
    RLOG_LEVEL_INFO,
    RLOG_LEVEL_DEBUG,
    RLOG_LEVEL_TRACE,
    RLOG_LEVEL_COUNT,
} RLogLevel;

/// @brief Initializes all the handles to work with stdout
/// @return 1 on success or 0 on failure (call rio_check_error())
b8 rlog_init(void);

/// @brief Function to write to the console (used in macro)
/// @param level What level is the output (Fatal, Error etc.)
/// @return 1 on success or 0 on failure (call rio_check_error())
b8 rlog_output(RLogLevel level, const char*, ...);
/**
 * @brief Helps at debugging stage to check for errors
 * @return A string with tiny description to check for the occured error
*/
const char *rlog_get_error(void);

#define RLOG_FATAL(str, ...) log_output(RLOG_LEVEL_FATAL, str, __VA_ARGS__)
#define RLOG_ERROR(str, ...) log_output(RLOG_LEVEL_ERROR, str, __VA_ARGS__)
#define RLOG_WARNING(str, ...) log_output(RLOG_LEVEL_WARNING, str, __VA_ARGS__)
#define RLOG_INFO(str, ...) log_output(RLOG_LEVEL_INFO, str, __VA_ARGS__)
#define RLOG_DEBUG(str, ...) log_output(RLOG_LEVEL_DEBUG, str, __VA_ARGS__)
#define RLOG_TRACe(str, ...) log_output(RLOG_LEVEL_TRACE, str, __VA_ARGS__)