objects = sender receiver
CFLAGS= -g

.PHONY: all
all: $(objects)

sender: sender.c
	$(CC) $(CFLAGS) $(CPPFLAGS) sender.c -o sender

receiver: receiver.c
	$(CC) $(CFLAGS) $(CPPFLAGS) receiver.c -o receiver

.PHONY: clean
clean: 
	rm $(objects)

