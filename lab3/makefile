objects = reverse file_size
CFLAGS= -g -Wall -Wextra

.PHONY: all
all: $(objects)

reverse: reverse.c
	$(CC) $(CFLAGS) $(CPPFLAGS) reverse.c -o reverse

file_size: file_size.c
	$(CC) $(CFLAGS) $(CPPFLAGS) file_size.c -o file_size

test_one_1: reverse
	./reverse "test_one" "test_one_reversed" 1

test_one_1024: reverse
	./reverse "test_one" "test_one_reversed" 1024

test_two_1: reverse
	./reverse "test_two.pdf" "test_two_reversed" 1

test_two_1024: reverse
	./reverse "test_two.pdf" "test_two_reversed" 1024

test_two_restore: reverse
	./reverse "test_two.pdf" "test_two_reversed" 1024
	./reverse "test_two_reversed" "restored.pdf" 1024

test_file_size: file_size
	./file_size "."

.PHONY: clean
clean: 
	rm $(objects)

