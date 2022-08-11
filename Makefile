all: src/main.c src/args.c src/trash.c src/utils.c
	\mkdir -p bin
	\gcc -g src/main.c src/args.c src/trash.c src/utils.c -o bin/rm-trash -std=c99

install: all
	\cp bin/rm-trash /bin/rm-trash

uninstall:
	\rm /bin/rm-trash
