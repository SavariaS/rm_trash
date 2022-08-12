#include "args.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int parse_arguments(int argc, const char** argv, struct options* opts)
{
	opts->flags = 0;
	opts->prompt_when = NEVER;
	opts->args_count = 0;

	if(argc < 2)
	{
		fputs("rm-trash: missing operand\nTry 'rm-trash --help' for more information.\n", stderr);
		return 1;
	}

	for(int i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--force") == 0)
				opts->flags |= (1<<0);
			else if(strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "--recursive") == 0)
				opts->flags |= (1<<1);
			else if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dir") == 0)
				opts->flags |= (1<<2);
			else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
				opts->flags |= (1<<3);
            else if(strcmp(argv[i], "--no-preserve-root") == 0)
				opts->flags |= (1<<5);
			else if(strcmp(argv[i], "--help") == 0)
				opts->flags |= (1<<6);
			else if(strcmp(argv[i], "--version") == 0)
				opts->flags |= (1<<7);

            else if(strcmp(argv[i], "--one-file-system") == 0)
				opts->flags |= (1<<4);
            else if(strcmp(argv[i], "--preserve-root=all") == 0)
                opts->flags |= (1<<4);
			
			else if(strcmp(argv[i], "-i") == 0)
				opts->prompt_when = ALWAYS;
			else if(strcmp(argv[i], "-I") == 0)
				opts->prompt_when = ONCE;
            else if(strcmp(argv[i], "--interactive"))
                opts->prompt_when = ALWAYS;
            else if(strcmp(argv[i], "--interactive=always"))
                opts->prompt_when = ALWAYS;
            else if(strcmp(argv[i], "--interactive=once"))
                opts->prompt_when = ONCE;
            else if(strcmp(argv[i], "--interactive=never"))
                opts->prompt_when = NEVER;

			else
			{
				fprintf(stderr, "rm-trash: invalid option '%s'\nTry 'rm-trash --help' for more information.\n", argv[i]);
				return 1;
			}
		}
		else
		{
			opts->args_count += 1;
		}
	}

	return 0;
}

int is_removeable(const char* file_name, mode_t file_type, dev_t file_device, dev_t trash_device, struct options* opts)
{
	// If the --force option is provided, do not prompt and always trash the file
	if(FORCE(opts->flags))
	{
		return 1;
	}

	// If the file isn't a regular file or a symbolic link
	if(!S_ISREG(file_type) && !S_ISLNK(file_type))
	{
		// If it isn't a directory either, don't trash it
		if(!S_ISDIR(file_type))
		{
			return 0;
		}
		// If it is a directory
		else
		{
			// If the --recursive option isn't provided or the --dir option is provided but the file is not empty, don't trash it
			if(!RECURSIVE(opts->flags) && !(DIRECTORY(opts->flags) && dir_empty(file_name)))
			{
				fprintf(stderr, "rm-trash: cannot remove '%s': Is a directory\n", file_name);
				return 0;
			}
		}
	}

	// 
	if(ONE_FSYSTEM(opts->flags) && file_device != trash_device)
	{
		fprintf(stderr, "rm-trash: cannot remove '%s': Different file systems\n", file_name);
		return 0;
	}

	// If prompting before every removal
	if(opts->prompt_when == ALWAYS)
	{
		if(S_ISREG(file_type))
		{
			return prompt("rm-trash: remove regular file '%s' [Y/n]? ", file_name);
		}
		if(S_ISLNK(file_type))
		{
			return prompt("rm-trash: remove symbolic link '%s' [Y/n]? ", file_name);
		}
		if(S_ISDIR(file_type))
		{
			if(RECURSIVE(opts->flags))
			{
				return prompt("rm-trash: remove directory '%s' recursively [Y/n]? ", file_name);
			}
			else
			{
				return prompt("rm-trash: remove directory '%s' [Y/n]? ", file_name);
			}
		}
	}

	// If every condition is met, trash the file
	return 1;
}

const char* get_help()
{
	return "Usage: rm-trash [OPTION]... [FILE]...\
Move FILE(s) to trash.\n";
}

const char* get_version()
{
	return "rm-trash 0.10\n";
}