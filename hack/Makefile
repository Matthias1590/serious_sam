CC = gcc
CFLAGS = -Wall -Wextra -fPIC -O2 -ggdb

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

preload.so: $(OBJS)
	$(CC) $(CFLAGS) -shared -fPIC -o preload.so $(OBJS) -ldl

run: preload.so
	cd .. && ./start_game.sh

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o preload.so
