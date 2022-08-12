#define _GNU_SOURCE

#include "utils.h"

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int prompt(const char* format, ...)
{
	// Prompt the user
	va_list args;
    va_start(args, format);
	vprintf(format, args);
	va_end(args);

	// Read the response
	int c = getchar();
	if(c != '\n') while(getchar() != '\n');

	// If the response is 'Y' or nothing, return true
	return (c == 'y' || c == 'Y' || c == '\n') ? 1 : 0;
}

int dir_empty(const char* path)
{
	int count = 0;
	struct dirent* directory;

	DIR* dir = opendir(path);
	while(readdir(dir) != NULL) 
	{
		++count;
		if(count > 2)
		{
			break;
		}
	}
	closedir(dir);

	if (count <= 2)
		return 1;
	else
		return 0;
}

void create_path(char* path)
{
	char* cursor = path;
	while(*cursor != '\0')
	{
		if(*cursor == '/')
		{
			*cursor = '\0';
			mkdir(path, 0777);
			*cursor = '/';
		}

		++cursor;
	}	
}

void copy_directory(char* dest, char* src)
{
    DIR* d;
    struct dirent* dir;
    int src_len = strlen(src) + 1;
    int dest_len = strlen(dest) + 1;
    d = opendir(src);

    strcat(dest, "/");
    strcat(src, "/");
    mkdir(dest, 0777);

    printf("Creating directory %s from %s\n", dest, src);

    if(d != NULL)
    {
        while((dir = readdir(d)) != NULL)
        {
            // Ignore . and ..
            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            {
                continue;
            }

            strcat(dest, dir->d_name);
            strcat(src, dir->d_name);

            if(dir->d_type == DT_DIR)
            {
                copy_directory(dest, src);
            }
            else if(dir->d_type == DT_REG)
            {
                copy_file(dest, src);
            }

            dest[dest_len] = '\0';
            src[src_len] = '\0';
        }

        closedir(d);
    }

    rmdir(src);
}

void copy_file(const char* dest, const char* src)
{
    printf("Copying %s to %s\n", src, dest);

    FILE* original = fopen(src, "r");
    FILE* copy = fopen(dest, "w");
    char buffer[4096];
    size_t bytes_read = 0;

    while(bytes_read = fread(buffer, 1, 4096, original), bytes_read > 0)
    {
        fwrite(buffer, 1, bytes_read, copy);
    }

    fclose(original);
    fclose(copy);

    remove(src);
}