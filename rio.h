#pragma once

/**
 * @file rio.h
 * @author Radu-D. Chira
 * @brief My own I/O API for personal usage
 * @version 0.1
 * @date 2024-01-10 
 */

#include "rdefines.h"

/// @brief Struct to work with I/O specific functions
typedef struct RFile
{
    phandle handle;
    u64 len;
    u8 *data;
    b8 valid;
} RFile;

/// @brief RFile access options such as reading or writing
typedef enum RFileAccess
{
    RFILE_READ = 0,
    RFILE_WRITE
} RFileAccess;

/**
 * @brief Opens or creates a file depending on the second parameter
 * @param access FILE_READ or FILE_WRITE
 * @warning The function allocates memory for the data, if the access parameter is FILE_READ.
 * Call file_close(RFile *file) when the file is not needed anymore!
 * @return A RFile struct with member valid = 0 on error (check io_get_error())
*/
RFile rfile_open(const char *filename, RFileAccess access);

/**
 * @brief Reads the file content
 * @warning It ignores the fact of the file size being 0!
 * @return 1 on success or 0 on failure (check io_get_error())
*/
b8 rfile_read(RFile *file);

/**
 * @brief Write a data buffer to file.
 * @return 1 on success or 0 on failure (check io_get_error())
*/
b8 rfile_write(RFile *file, const char *data, long long data_len);

/**
 * @brief Closes the opened file and frees the file data.
*/
void rfile_close(RFile *file);

/**
 * @brief Helps at debugging stage to check for errors
 * @return A string with tiny description to check for the occured error
*/
const char *rio_get_error(void); 