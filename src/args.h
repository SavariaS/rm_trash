#ifndef RMTRASH_ARGS_H
#define RMTRASH_ARGS_H

// Standard library includes
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

// Macros
#define FORCE(f)       (((f) & (1<<0)) != 0)
#define RECURSIVE(f)   (((f) & (1<<1)) != 0)
#define DIRECTORY(f)   (((f) & (1<<2)) != 0)
#define VERBOSE(f)     (((f) & (1<<3)) != 0)
#define ONE_FSYSTEM(f) (((f) & (1<<4)) != 0)
#define NO_ROOT(f)     (((f) & (1<<5)) != 0)
#define HELP(f)        (((f) & (1<<6)) != 0)
#define VERSION(f)     (((f) & (1<<7)) != 0)


enum prompt
{
	NEVER, 
	ONCE, 
	ALWAYS
};

// Flags: version | help | root | one_fsystem | verbose | dir | recursive | force
struct options
{
	size_t args_count;
	uint8_t flags;
	enum prompt prompt_when;
};

/*
 * @brief Parses the command line arguments
 * @args argc    The number of arguments
 *       argv    The arguments
 *       opts    The structure that will hold the parsed command line arguments
 * @return 1 if the command line arguments were parsed successfully, 0 if not
 */
int parse_arguments(int argc, const char** argv, struct options* opts);

/*
 * @brief  Determines if a file should be trashed from the command line arguments
 * @args file_name    The name of the file
 *       file_type    The type of file
 *       file_device  The file system on which the file resides
 *       trash_device The file system on which the trash directory resides
 *       opts         The parsed command line arguments
 * @return 1 if the file should be trashed, 0 if not
 */
int is_removeable(const char* file_name, mode_t file_type, dev_t file_device, dev_t trash_device, struct options* opts);

/*
 * @brief  Returns an help message with the syntax for this command and all possible options.
 * @return The help message.
 */
const char* get_help();

/*
 * @brief  Returns the current version.
 * @return The current version as a string.
 */
const char* get_version();

#endif