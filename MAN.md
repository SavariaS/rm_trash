% RM-TRASH(1)
% Samuel Savaria
% August 2022

# NAME
rm-trash - moves files or directories to the trash bin

# SYNOPSIS
**rm-trash** [OPTION]... [FILE]...

# DESCRIPTION
**rm-trash** moves the specified files to the trash bin.

By default, it does not trash directories. Directories can be trashed using the *-r*, *-R* or *--recursive* options.

By default, it does not prompt. If the *-I* or *--interactive=once* options are given, the program will prompt once before trashing the files. If the *-i* or *--interactive==always* options are given, the program will prompt before trashing every file specified.

By default, it will copy files from other file systems into the trash. If the *--one-file-system* option is given, the program will ignore files from other file systems.

# OPTIONS
**-f, --force**
: Never prompt, ignore nonexistent files.

**-i**
: Prompt before trashing every FILE.

**-I**
: Prompt once before trashing the FILE(s).

**-r, -R, --recursive**
: Trash directories and their content.

**-d, --dir**
: Remove empty directories.

**-v, --verbose**
Provide verbose output of what is being done.


**--one-file-system**
: Do not trash files on different file systems.

**--interactive[=WHEN]**
: Prompt according to WHEN: never (default), once (-I) or always (-i). Without WHEN, prompt always.


**--no-preserve-root**
: Not implemented. For compatiblity with GNU coreutils 'rm'.

**--preserve-root[=all]**
: Not implemented. For compatiblity with GNU coreutils 'rm'.


**--help**
: Display this help message and exit.

**--version**
: Display version and exit.

# SEE ALSO
rm(1)