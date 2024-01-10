#include "log.h"

#if RPLATFORM_WINDOWS == 1

#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

enum
{
    FAILURE = 0,
    SUCCESS = 1,
};

//TODO: Add error handling
// static enum Error
// {
//     INVALID_HANDLE = 0x00,
//     FAILED_TO_WRITE_CONSOLE = 0x01,

// } error;

enum
{
    BLACK             = 0,
    DARKBLUE          = FOREGROUND_BLUE,
    DARKGREEN         = FOREGROUND_GREEN,
    DARKCYAN          = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED           = FOREGROUND_RED,
    DARKMAGENTA       = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW        = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY          = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY              = FOREGROUND_INTENSITY,
    BLUE              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED               = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

static phandle g_stdout_handle = NULL;

b8 log_init(void)
{
    g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    return (g_stdout_handle != NULL && g_stdout_handle != INVALID_HANDLE_VALUE) ? SUCCESS : FAILURE;
}

enum { MAX_LENGTH = 1024 };
b8 log_output(LogLevel level, const char *str, ...)
{
    const char *level_str[LOG_LEVEL_COUNT] = { "[FATAL]: ", "[ERROR]: ", "[WARNING]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };
    const i32 colors[LOG_LEVEL_COUNT] = { RED, DARKRED, YELLOW, GREEN, BLUE, GRAY };

    if (level < LOG_LEVEL_COUNT)
    {
        char buffer[MAX_LENGTH] = {0};
        char out_message[MAX_LENGTH] = {0};

        va_list list;
        va_start(list, str);
        vsnprintf_s(buffer, MAX_LENGTH, MAX_LENGTH, str, list);
        va_end(list);

        SYSTEMTIME st = {0};
        GetLocalTime(&st);

        _snprintf_s(out_message, MAX_LENGTH, MAX_LENGTH, "%02d:%02d:%02d: %s%s\n", st.wHour, st.wMinute, st.wSecond, level_str[level], buffer);

        DWORD written = 0; // Warning if the var is not DWORD aka unsinged long...annoying
        SetConsoleTextAttribute(g_stdout_handle, colors[level]);
        if (!WriteConsoleA(g_stdout_handle, out_message, strnlen_s(out_message, MAX_LENGTH), &written, NULL))
        {
            SetConsoleTextAttribute(g_stdout_handle, WHITE);
            return FAILURE;
        }
        if ((u32)written != strlen(out_message))
        {
            SetConsoleTextAttribute(g_stdout_handle, WHITE);
            return FAILURE;
        }
    }
    else
        return FAILURE;
    
    SetConsoleTextAttribute(g_stdout_handle, WHITE);
    return SUCCESS;
}
#undef MAX_LENGTH

#else
    #error "Only Windows support for now!"
#endif

