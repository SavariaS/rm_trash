#ifndef RMTRASH_ARGS_H
#define RMTRASH_ARGS_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

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

int parse_arguments(int argc, const char** argv, struct options* opts);
int is_removeable(const char* file_name, mode_t file_type, dev_t file_device, dev_t trash_device, struct options* opts);

const char* get_help();
const char* get_version();

#endif