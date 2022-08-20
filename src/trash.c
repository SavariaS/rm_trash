// Local includes
#include "trash.h"
#include "utils.h"
// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Function prototypes
static char* generate_file_name(const char* path);

// Globals
const char info_template[] = "[Trash Info]\nPath=%s\nDeletionDate=%04d-%02d-%02dT%02d:%02d:%02d\n";

char* files_directory = NULL;
char* info_directory = NULL;
size_t files_directory_length = 0;
size_t info_directory_length = 0;

/*
 * @brief Gets the paths to the files and info directories and creates those directories if they do not exist yet
 * @return The device (file system) on which the trash directory resides
 */
dev_t init_trash()
{
	// Get the user data directory
	int xdg_data_home_exists = 1;
	char* xdg_data_home = getenv("XDG_DATA_HOME");
	if(xdg_data_home == NULL)
	{
		xdg_data_home_exists = 0;
		const char* home = getenv("HOME");

		xdg_data_home = malloc(strlen(home) +  14);
		strcpy(xdg_data_home, home);
		strcat(xdg_data_home, "/.local/share");
	}

	// Get the paths to the file directory and info directory
	files_directory_length = strlen(xdg_data_home) + strlen("/Trash/files/");
	files_directory = malloc(files_directory_length + 256); // The maximum length for a file name is 255 bytes
	strcpy(files_directory, xdg_data_home);
	strcat(files_directory, "/Trash/files/");

	info_directory_length = strlen(xdg_data_home) + strlen("/Trash/info/");
	info_directory = malloc(info_directory_length + 256); // The maximum length for a file name is 255 bytes
	strcpy(info_directory, xdg_data_home);
	strcat(info_directory, "/Trash/info/");

	// Create the paths if they don't exist
	create_path(files_directory);
	create_path(info_directory);

	// Get information about the directories
	struct stat file_info;
	stat(files_directory, &file_info);

	// Cleanup
	if(xdg_data_home_exists == 0)
	{
		free(xdg_data_home);
	}

	// Return the device on which the trash is located
	return file_info.st_dev;
}

/*
 * @brief Moves/copies a file to the trash
 * @args path            The path to the original file
 *       is_regular_file Whether the original file is a regular file or a directory
 *       copy            Whether the file should be copied or moved
 */
void trash_file(const char* path, int is_regular_file, int copy)
{
	// Get the name of the file
	char* file_name = generate_file_name(path);

	// Get the timestamp
	time_t seconds = time(NULL);
	struct tm* timestamp = localtime(&seconds);

	// Generate the path to the trashinfo file
	strcat(info_directory, file_name);
	strcat(info_directory, ".trashinfo");

    // Generate the path to the trashed file
    strcat(files_directory, file_name);

    // Create the info file
	FILE* info_file = fopen(info_directory, "w");
	fprintf(info_file, info_template, path,                      // Absolute path
	                                  timestamp->tm_year + 1900, // Year, Start in 1900
							          timestamp->tm_mon + 1,     // Month, Start in January
							          timestamp->tm_mday,        // Days
							          timestamp->tm_hour,        // Hours
							          timestamp->tm_min,         // Minutes
							          timestamp->tm_sec);        // Seconds
	fclose(info_file);
	
    if(copy == 0)
    {
        // Move file to trash
        int success = rename(path, files_directory);
        if(success != 0) // If the file could not be moved to the trash, delete the info file
        {
            remove(info_directory);
        }
    }
    else
    {
        // Copy the file to the trash
        if(is_regular_file)
        {
            copy_file(files_directory, path);
        }
        else
        {
            // Copy the file to trash
            char* file_path = malloc(4096);
            strcpy(file_path, path);

            char* trash_path = malloc(4096);
            strcpy(trash_path, files_directory);

            copy_directory(trash_path, file_path);
        }
    }

	// Reset the paths
	free(file_name);
	files_directory[files_directory_length] = '\0';
	info_directory[info_directory_length] = '\0';
}


/*
 * @brief Generate a valid file name. Files in the 'files/' directory must not have duplicate names
 * @args path The path to the original file
 * @return The first valid file name, as a string
 */
char* generate_file_name(const char* path)
{
	// Get the name of the file
	const char* file_name = NULL;
	while(*path != '\0')
	{
		if(*path == '/' && (*path + 1) != '\0') // If '/' is found and it is not at the end of th path
		{
			file_name = path + 1; // Name starts there
		}
		
		++path;
	}

	// Check for duplicates
	int version = 0;
	char files_path[PATH_MAX];
	sprintf(files_path, "%s%s", files_directory, file_name);

	if(access(files_path, F_OK) == 0) // If the file already exists in the files directory, append a version until one is available
	{
		do
		{
			sprintf(files_path, "%s%s.%d", files_directory, file_name, version);
			++version;
		}
		while(access(files_path, F_OK) == 0);
	}

	
	// Return the generated file name
	char* available_name = malloc(256); // The maximum length for a file name is 255 bytes

	if(version == 0)
	{
		strcpy(available_name, file_name);
	}
	else
	{
		sprintf(available_name, "%s.%d", file_name, version);
	}

	return available_name;
}