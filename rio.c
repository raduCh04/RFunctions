#include "rio.h"

#if defined(_WIN64) || defined(_WIN32)

#include <Windows.h>
#include <stdio.h>

enum
{
    RFAILURE = 0,
    RSUCCESS = 1,
};

static enum Error
{
    RINVALID_FILE_HANDLE = 0x00,
    RINVALID_FILE = 0x01,
    RFAILED_TO_GET_FILE_SIZE = 0x02,
    RFAILED_ALLOCATION = 0x03,
    RFAILED_READ_OPERATION = 0x04,
    RFAILED_WRITE_OPERATION = 0x05,

    RERROR_COUNT,
} error;

RFile rfile_open(const char *filename, RFileAccess access)
{
    RFile file = {0};

    phandle file_handle = CreateFileA(filename, access == RFILE_READ ? GENERIC_READ : GENERIC_WRITE, 
        0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        file.handle = file_handle;
        switch (access)
        {
            case RFILE_READ:
            {
                LARGE_INTEGER size = {0};
                if (GetFileSizeEx(file_handle, &size) != FALSE)
                {
                    file.len = size.QuadPart;
                    if (file.len > 0)
                    {
                        file.data = VirtualAlloc(NULL, file.len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                        if (file.data != NULL)
                        {
                            file.valid = true;
                        }
                        else
                        {
                            error = RFAILED_ALLOCATION;
                            CloseHandle(file.handle); //TODO: Handle Error
                        }
                    }
                }
                else 
                {
                    error = RFAILED_TO_GET_FILE_SIZE;
                    CloseHandle(file.handle); //TODO: Handle Error
                }
            } break;
            case RFILE_WRITE:
            {
                file.valid = true;
            } break;
        }
    }
    else
        error = RINVALID_FILE_HANDLE;

    return file;
}

b8 rfile_read(RFile *file)
{
    if (file->valid)
    {
        DWORD bytes_read = 0; // Warning if the var is not DWORD aka unsinged long...annoying
        usize bytes_to_be_read = file->len;
        u8 *ptr = file->data;

        while (bytes_to_be_read > 0)
        {
            if (ReadFile(file->handle, ptr, bytes_to_be_read, &bytes_read, NULL) == FALSE)
            {
                error = RFAILED_READ_OPERATION;
                return RFAILURE;
            }
            if (bytes_read == 0)
            {
                error = RFAILED_READ_OPERATION;
                return RFAILURE;
            }

            ptr += bytes_read;
            bytes_to_be_read -= bytes_read;
        }
        return RSUCCESS;
    }
    error = RINVALID_FILE;
    return RFAILURE;
}

b8 rfile_write(RFile *file, const char *data, long long data_len)
{
    if (file->valid)
    {
        DWORD bytes_written = 0;
        usize bytes_to_be_written = data_len;
        u8 *ptr = file->data;
        while (bytes_to_be_written > 0)
        {
            if (WriteFile(file->handle, data, bytes_to_be_written, &bytes_written, NULL) == false)
            {
                error = RFAILED_WRITE_OPERATION;
                return RFAILURE;
            }
            if (bytes_written == 0)
            {
                error = RFAILED_WRITE_OPERATION;
                return RFAILURE;
            }
            ptr += bytes_written;
            bytes_to_be_written -= bytes_written;
        }
        return RSUCCESS;
    }
    error = RINVALID_FILE;
    return RFAILURE;
}

void rfile_close(RFile *file)
{
    if (file->valid)
    {
        file->len = 0;
        if (file->data)
            VirtualFree(file->data, file->len, MEM_RELEASE); //TODO: Handle Error
        if (file->handle != INVALID_HANDLE_VALUE)
            CloseHandle(file->handle); //TODO: Handle Error
    }
    error = RINVALID_FILE;
}

const char *rio_get_error(void)
{
    const char *error_strings[RERROR_COUNT] = {"[Invalid file handle!]", "[Invalid file!]", 
        "[Failed to get file size!]", "[Failed allocation]", "[Failed read operation]", "[Failed write operation]"};
    return error_strings[error];
}

#else
    #error "Only Windows support for now!"
#endif


