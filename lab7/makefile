objects = server client
CFLAGS= -Wall -Wextra

.PHONY: all
all: $(objects)

server: server.c
	$(CC) $(CFLAGS) $(CPPFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) $(CPPFLAGS) client.c -o client

.PHONY: clean
clean: 
	rm $(objects)

