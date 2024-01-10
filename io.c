#include "io.h"

#if defined(_WIN64) || defined(_WIN32)

#include <Windows.h>
#include <stdio.h>

enum
{
    FAILURE = 0,
    SUCCESS = 1,
};

static enum Error
{
    INVALID_FILE_HANDLE = 0x00,
    INVALID_FILE = 0x01,
    FAILED_TO_GET_FILE_SIZE = 0x02,
    FAILED_ALLOCATION = 0x03,
    FAILED_READ_OPERATION = 0x04,
    FAILED_WRITE_OPERATION = 0x05,

    ERROR_COUNT,
} error;

File file_open(const char *filename, FileAccess access)
{
    File file = {0};

    phandle file_handle = CreateFileA(filename, access == FILE_READ ? GENERIC_READ : GENERIC_WRITE, 
        0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        file.handle = file_handle;
        switch (access)
        {
            case FILE_READ:
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
                            error = FAILED_ALLOCATION;
                            CloseHandle(file.handle);
                        }
                    }
                }
                else 
                {
                    error = FAILED_TO_GET_FILE_SIZE;
                    CloseHandle(file.handle);
                }
            } break;
            case FILE_WRITE:
            {
                file.valid = true;
            } break;
        }
    }
    else
        error = INVALID_FILE_HANDLE;

    return file;
}

b8 file_read(File *file)
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
                error = FAILED_READ_OPERATION;
                return FAILURE;
            }
            if (bytes_read == 0)
            {
                error = FAILED_READ_OPERATION;
                return FAILURE;
            }

            ptr += bytes_read;
            bytes_to_be_read -= bytes_read;
        }
        return SUCCESS;
    }
    error = INVALID_FILE;
    return FAILURE;
}

b8 file_write(File *file, const char *data, long long data_len)
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
                error = FAILED_WRITE_OPERATION;
                return FAILURE;
            }
            if (bytes_written == 0)
            {
                error = FAILED_WRITE_OPERATION;
                return FAILURE;
            }
            ptr += bytes_written;
            bytes_to_be_written -= bytes_written;
        }
        return SUCCESS;
    }
    error = INVALID_FILE;
    return FAILURE;
}

void file_close(File *file)
{
    if (file->valid)
    {
        file->len = 0;
        if (file->data)
            VirtualFree(file->data, file->len, MEM_RELEASE);
        if (file->handle != INVALID_HANDLE_VALUE)
            CloseHandle(file->handle);
    }
    error = INVALID_FILE;
}

const char *io_get_error(void)
{
    const char *error_strings[ERROR_COUNT] = {"[Invalid file handle!]", "[Invalid file!]", 
        "[Failed to get file size!]", "[Failed allocation]", "[Failed read operation]", "[Failed write operation]"};
    return error_strings[error];
}

#else
    #error "Only Windows support for now!"
#endif


