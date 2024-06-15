objects = bibl.o libbibl.so main

.PHONY: all
all: $(objects)

bibl.o: 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -fPIC bibl.c -o bibl.o

libbibl.so: bibl.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -shared bibl.o -o libbibl.so

main: main.c libbibl.so
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -lbibl -L ./  -Wl,-rpath,. -o main

clean:
	rm -f $(objects)