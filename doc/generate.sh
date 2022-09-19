#!/bin/bash
if hash pandoc 2>/dev/null ; then
    pandoc ../MAN.md -s -t man -o rm-trash.1
else
    echo "Pandoc is required to generate the man page. Make sure it is installed.
https://pandoc.org/installing.html#linux"
fi
