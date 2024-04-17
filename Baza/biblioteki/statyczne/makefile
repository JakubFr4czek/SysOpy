objects = lib main main_lib
CFLAGS= -g

.PHONY: all
all: $(objects)

lib: lib.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c lib.c -o lib

main: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c main.c -o main

main_lib: main lib
	$(CC) $(CFLAGS) $(CPPFLAGS) main lib -o main_lib

.PHONY: clean
clean: 
	rm $(objects)

