DEBUG_FLAGS=-pedantic-errors -ggdb -O0 -Wall -Wextra -Wconversion -Wsign-conversion -o

RELEASE_FLAGS=-O2 -DNDEBUG -Wall -Wextra -Wconversion -Wsign-conversion  -o

CC=gcc

default: debug

debug: src/child_operations.c src/tracing_operations.c src/main.c
	$(CC) $(DEBUG_FLAGS) debug.elf $^

release: src/child_operations.c src/tracing_operations.c src/main.c
	$(CC) $(RELEASE_FLAGS) analySYS.elf $^

clean:
	rm -v *.elf

install: release
	mkdir -p ~/.local/bin/ && \
	mv -T ./analySYS.elf ~/.local/bin/analySYS && \
	echo "Success!"

uninstall:
	rm -v ~/.local/bin/analySYS