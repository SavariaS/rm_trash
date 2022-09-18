// Local includes
#include "args.h"
#include "utils.h"

// Standard library includes
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/*
 * @brief Parses the command line arguments
 * @args argc    The number of arguments
 *       argv    The arguments
 *       opts    The structure that will hold the parsed command line arguments
 * @return 1 if the command line arguments were parsed successfully, 0 if not
 */
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

/*
 * @brief  Determines if a file should be trashed from the command line arguments
 * @args file_name    The name of the file
 *       file_type    The type of file
 *       file_device  The file system on which the file resides
 *       trash_device The file system on which the trash directory resides
 *       opts         The parsed command line arguments
 * @return 1 if the file should be trashed, 0 if not
 */
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

	// If the file is on a different file system from the trash directory and one file system is being enforced, don't trash it
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

/*
 * @brief  Returns an help message with the syntax for this command and all possible options.
 * @return The help message.
 */
const char* get_help()
{
	return "Usage: rm-trash [OPTION]... [FILE]...\n\
Move FILE(s) to trash.\n\
\n\
  -f, --force             Never prompt, ignore nonexistent files.\n\
  -i                      Prompt before trashing every FILE.\n\
  -I                      Prompt once before trashing the FILE(s).\n\
  -r, -R, --recursive     Trash directories and their content.\n\
  -d, --dir               Remove empty directories.\n\
  -v, --verbose           Provide verbose output of what is being done.\n\
\n\
    --one-file-system     Do not trash files on different file systems.\n\
    --interactive[=WHEN]  Prompt according to WHEN: never (default), once (-I)\n\
	                      or always (-i). Without WHEN, prompt always.\n\
\n\
    --no-preserve-root    Not implemented. For compatiblity with GNU coreutils\n\
                          'rm'.\n\
    --preserve-root[=all] Not implemented. For compatiblity with GNU coreutils\n\
                          'rm'.\n\
\n\
    --help                Display this help message and exit.\n\
    --version             Display version and exit.\n\
\n\
By default, rm-trash does not trash directories and their content.\n\
\n\
Files from a file system different from the trash directory are moved by copy.\n\
\n\
To remove a file whose name starts with '-', reference it from the current\n\
directory:\n\
    rm-trash ./-foo";
}

/*
 * @brief  Returns the current version.
 * @return The current version as a string.
 */
const char* get_version()
{
	return "rm-trash 1.0";
}