objects = zad1 zad2_catcher zad2_sender
CFLAGS= -g

.PHONY: all
all: $(objects)

zad1: zad1.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad1.c -o zad1

zad2: zad2_catcher.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad2_catcher.c -o zad2_catcher

zad2: zad2_sender.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad2_sender.c -o zad2_sender

.PHONY: clean
clean: 
	rm $(objects)

