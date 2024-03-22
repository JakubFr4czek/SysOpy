objects = zad1 zad2
CFLAGS= -g -Wall -Wextra

.PHONY: all
all: $(objects)

zad1: zad1.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad1.c -o zad1

zad2: zad2.c
	$(CC) $(CFLAGS) $(CPPFLAGS) zad2.c -o zad2

zad1_test: zad1
	./zad1 10

zad2_test: zad2
	./zad2 "sciezka_testowa"

.PHONY: clean
clean: 
	rm $(objects)

