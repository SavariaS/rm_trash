#define _GNU_SOURCE

#include "args.h"
#include "trash.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char** argv)
{
	// Parse command line arguments
	struct options opts;
	if(parse_arguments(argc, argv, &opts) != 0) // If the command line arguments could not be parsed, return failure
	{
		return 1;
	}

	// Print help message
	if(HELP(opts.flags))
	{
		puts(get_help());
		return 0;
	}

	// Print version
	if(VERSION(opts.flags))
	{
		puts(get_version());
		return 0;
	}

	// If prompting once, prompt once to remove all files
	if(opts.prompt_when == ONCE)
	{
		if(RECURSIVE(opts.flags))
		{
			if(prompt("rm-trash: remove %d arguments recursively [Y/n]? ", opts.args_count) == 0)
				return 0;
		}
		else
		{
			if(prompt("rm-trash: remove %d arguments [Y/n]? ", opts.args_count) == 0)
				return 0;
		}
	}

	// Initialise the Trash directory
	dev_t device = init_trash();

	// For file in the arguments
	char absolute_path[PATH_MAX + 1];
	for(int i = 1; i < argc; ++i) // Iterate over the arguments
	{
		if(argv[i][0] != '-') // If the argument is a file...
		{
			// Get the absolute path to the file
			if(realpath(argv[i], absolute_path) == NULL)
			{
				fprintf(stderr, "rm-trash: Cannot remove '%s': No such file or directory\n", argv[i]);
				continue;
			}

			// Get information about the file
			struct stat file_info;
			lstat(argv[i], &file_info);

			// If the file is to be removed...
			if(is_removeable(argv[i], file_info.st_mode, file_info.st_dev, device, &opts))
			{
				// If it's a symlink, delete it
				if(S_ISLNK(file_info.st_mode))
				{
					if(VERBOSE(opts.flags)) printf("rm-trash: Unlinking '%s'\n", argv[i]);
					unlink(argv[i]);
				}
				// If it's an empty directory, delete it
				else if(S_ISDIR(file_info.st_mode) && dir_empty(absolute_path))
				{
					if(VERBOSE(opts.flags)) printf("rm-trash: Removing empty directory '%s'\n", argv[i]);
					rmdir(absolute_path);
				}
				// Else, trash it
				else
				{
					if(VERBOSE(opts.flags))
					{
						if(device != file_info.st_dev)
							puts("rm-trash: Copying ");
						else
							puts("rm-trash: Moving ");
						
						if(S_ISDIR(file_info.st_mode))
							printf("directory '%s' to trash\n", argv[i]);
						else
							printf("'%s' to trash\n", argv[i]);
					}

					trash_file(absolute_path, S_ISREG(file_info.st_mode), file_info.st_dev != device);
				}
			}
		}
	}

    return 0;
}