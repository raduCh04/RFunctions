#pragma once

/// @brief Struct to work with I/O specific functions
typedef struct File
{
    void *handle;
    long long len;
    char *data;
    int valid;
} File;
typedef File *pFile;

/// @brief File access options such as reading or writing
typedef enum FileAccess
{
    FILE_READ = 0,
    FILE_WRITE
} FileAccess;


/**
 * @brief Opens or creates a file depending on the second parameter
 * @param access FILE_READ or FILE_WRITE
 * @warning The function allocates memory for the data, if the access parameter is FILE_READ.
 * Call file_close(File *file) when the file is not needed anymore!
 * @return A File struct with member valid = 0 on error (check io_get_error())
*/
File file_open(const char *filename, FileAccess access);

/**
 * @brief Reads the file content
 * @warning It ignores the fact of the file size being 0!
 * @return 1 on success or 0 on failure (check io_get_error())
*/
int file_read(File *file);

/**
 * @brief Write a data buffer to file.
 * @return 1 on success or 0 on failure (check io_get_error())
*/
int file_write(File *file, const char *data, long long data_len);

/**
 * @brief Closes the opened file and frees the file data.
*/
void file_close(File *file);

/**
 * @brief Helps at debugging stage to check for errors
 * @return A string with tiny description to check for the occured error
*/
const char *io_get_error(void); 