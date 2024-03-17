objects = countdown
CFLAGS= -g -Wall -Wextra

.PHONY: all
all: $(objects)

countdown: countdown.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c countdown.c -o countdown

.PHONY: clean
clean: 
	rm $(objects)
	rm $(libraries)

