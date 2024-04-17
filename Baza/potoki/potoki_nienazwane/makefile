objects = potoki
CFLAGS= -g -Wall

.PHONY: all
all: $(objects)

potoki: potoki.c
	$(CC) $(CFLAGS) $(CPPFLAGS) potoki.c -o potoki

.PHONY: clean
clean: 
	rm $(objects)


