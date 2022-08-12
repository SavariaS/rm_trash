#ifndef RMTRASH_UTILS_H
#define RMTRASH_UTILS_H

int prompt(const char* format, ...);
int dir_empty(const char* path);
void create_path(char* path);

void copy_directory(char* dest, char* src);
void copy_file(const char* dest, const char* src);

#endif