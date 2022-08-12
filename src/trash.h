#ifndef RMTRASH_TRASH_H
#define RMTRASH_TRASH_H

#include <sys/types.h>

dev_t init_trash();
void trash_file(const char* path, int is_regular_file, int copy);

#endif