#ifndef RMTRASH_UTILS_H
#define RMTRASH_UTILS_H

/*
 * @brief Prompts the user.
 * @args format The message, as a string to format
 *       ...    Arguments to the formatted string
 * @return 1 if the user responded positively, 0 if not
 */
int prompt(const char* format, ...);

/*
 * @brief Determines if a directory is empty
 * @args path The path to the directory
 * @return 1 if the directory is empty, 0 if not
 */
int dir_empty(const char* path);

/*
 * @brief Create a path if it does not exist yet
 * @args path The path to create
 */
void create_path(char* path);

/*
 * @brief Copies a directory recursively
 * @args dest The path to copy the directory to
 *       src  The directory to copy
 */
void copy_directory(char* dest, char* src);

/*
 * @brief Copies a file
 * @args dest The path to copy the file to
 *       src  The file to copy
 */
void copy_file(const char* dest, const char* src);

#endif