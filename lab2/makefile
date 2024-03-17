objects = collatz_static collatz_shared client_static client_shared client_dynamic
libraries = collatz_static.a libcollatz_shared.so libcollatz_shared.so.1 libcollatz_shared.so.1.0.1
CFLAGS= -g -Wall -Wextra -O0

.PHONY: all
all: $(objects)

collatz_static: collatz.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c collatz.c -o collatz_static
	ar -rc collatz_static.a collatz_static

collatz_shared: collatz.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -c collatz.c -o collatz_shared
	$(CC) $(CFLAGS) $(CPPFLAGS) -shared -Wl,-soname,libcollatz_shared.so.1 -o libcollatz_shared.so.1.0.1 collatz_shared -lc
	ln -s libcollatz_shared.so.1.0.1 libcollatz_shared.so.1
	ln -s libcollatz_shared.so.1 libcollatz_shared.so

client_static: client_static.c collatz_static.a
	$(CC) $(CFLAGS) $(CPPFLAGS) client_static.c collatz_static.a -o client_static

client_shared: client_shared.c libcollatz_shared.so
	$(CC) $(CFLAGS) $(CPPFLAGS) client_shared.c -lcollatz_shared -L ./  -Wl,-rpath,. -o client_shared

client_dynamic: client_dynamic.c libcollatz_shared.so
	$(CC) $(CFLAGS) $(CPPFLAGS) -D DYNAMIC client_dynamic.c -o client_dynamic

.PHONY: clean
clean: 
	rm $(objects)
	rm $(libraries)

