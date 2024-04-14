objects = dup2_write dup2_read
CFLAGS= -g -Wall

.PHONY: all
all: $(objects)

dup2_write: dup2_write.c
	$(CC) $(CFLAGS) $(CPPFLAGS) dup2_write.c -o dup2_write

dup2_read: dup2_read.c
	$(CC) $(CFLAGS) $(CPPFLAGS) dup2_read.c -o dup2_read

.PHONY: clean
clean: 
	rm $(objects)

