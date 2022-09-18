#ifndef RMTRASH_TRASH_H
#define RMTRASH_TRASH_H

// Standard library includes
#include <sys/types.h>

/*
 * @brief Gets the paths to the files and info directories and creates those directories if they do not exist yet
 * @return The device (file system) on which the trash directory resides
 */
dev_t init_trash();

/*
 * @brief Moves/copies a file to the trash
 * @args path            The path to the original file
 *       is_regular_file Whether the original file is a regular file or a directory
 *       copy            Whether the file should be copied or moved
 * @return 0 if successful, -1 if not
 */
int trash_file(const char* path, int is_regular_file, int copy);

#endif