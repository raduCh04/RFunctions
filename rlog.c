#include "rlog.h"
#include "rio.h"

#if RPLATFORM_WINDOWS == 1

#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

enum
{
    RFAILURE = 0,
    RSUCCESS = 1,
};

static enum Error
{
    RINVALID_HANDLE = 0x00,
    RFAILED_TO_WRITE_CONSOLE = 0x01,

    RERROR_COUNT,
} error;

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

// NOTE: Static variables already initialized to 0/NULL
static phandle g_stdout_handle;
static RFile g_file;

b8 rlog_init(void)
{
    g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (g_stdout_handle != NULL && g_stdout_handle != INVALID_HANDLE_VALUE)
    {
        g_file = rfile_open("log.txt", RFILE_WRITE);
        if (g_file.valid)
            return RSUCCESS;
    }
    else
        error = RINVALID_HANDLE;
    return RFAILURE;
}

enum { MAX_LENGTH = 1024 };
b8 rlog_output(RLogLevel level, const char *str, ...)
{
    const char *level_str[RLOG_LEVEL_COUNT] = { "[FATAL]: ", "[ERROR]: ", "[WARNING]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };
    const i32 colors[RLOG_LEVEL_COUNT] = { RED, DARKRED, YELLOW, GREEN, BLUE, GRAY };

    if (level < RLOG_LEVEL_COUNT)
    {
        char buffer[MAX_LENGTH] = {0};
        char out_message[MAX_LENGTH] = {0};

        va_list list;
        va_start(list, str);
        vsnprintf_s(buffer, MAX_LENGTH, MAX_LENGTH, str, list); //TODO: Handle Error
        va_end(list);

        SYSTEMTIME st = {0};
        GetLocalTime(&st);

        _snprintf_s(out_message, MAX_LENGTH, MAX_LENGTH, "%02d:%02d:%02d: %s%s\n", st.wHour, st.wMinute, st.wSecond, level_str[level], buffer); //TODO: Handle Error
        rfile_write(&g_file, out_message, strnlen_s(out_message, MAX_LENGTH)); //TODO: Handle Error

        DWORD written = 0; // Warning if the var is not DWORD aka unsinged long...annoying
        SetConsoleTextAttribute(g_stdout_handle, colors[level]); //TODO: Handle Error
        if (!WriteConsoleA(g_stdout_handle, out_message, strnlen_s(out_message, MAX_LENGTH), &written, NULL))
        {
            error = RFAILED_TO_WRITE_CONSOLE;
            SetConsoleTextAttribute(g_stdout_handle, WHITE); //TODO: Handle Error
            return RFAILURE;
        }
        if ((u32)written != strlen(out_message))
        {
            error = RFAILED_TO_WRITE_CONSOLE;
            SetConsoleTextAttribute(g_stdout_handle, WHITE); //TODO: Handle Error
            return RFAILURE;
        }
    }
    else
        return RFAILURE;
    
    SetConsoleTextAttribute(g_stdout_handle, WHITE); //TODO: Handle Error
    return RSUCCESS;
}

void rlog_end(void)
{
    rfile_close(&g_file);
}

const char *rlog_get_error(void)
{
    const char *err_strings[RERROR_COUNT] = { "[Invalid handle!]", "[Failed to write to console!]" };
    return err_strings[error];
}

#else
    #error "Only Windows support for now!"
#endif

