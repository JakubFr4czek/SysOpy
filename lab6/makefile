objects = zad1 zad1_2 zad2_sender zad2_catcher
CFLAGS= -g

.PHONY: all
all: $(objects)

zad1: zad1.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad1.c -o zad1

zad1_2: zad1_2.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad1_2.c -o zad1_2

zad2_sender: zad2_sender.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad2_sender.c -o zad2_sender

zad2_catcher: zad2_catcher.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad2_catcher.c -o zad2_catcher

.PHONY: clean
clean: 
	rm $(objects)

