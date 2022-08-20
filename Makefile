all: src/main.c src/args.c src/trash.c src/utils.c
	mkdir -p bin
	gcc -g src/main.c src/args.c src/trash.c src/utils.c -o bin/rm-trash -std=c99

install: all
	cp bin/rm-trash /usr/local/bin/rm-trash
	mkdir -p /usr/local/share/man/man1/
	cp doc/rm-trash.1 /usr/local/share/man/man1/
	mandb

uninstall:
	rm -f /usr/local/bin/rm-trash
	rm -f /usr/local/share/man/man1/rm-trash.1
	mandb

clean:
	rm -r bin/
