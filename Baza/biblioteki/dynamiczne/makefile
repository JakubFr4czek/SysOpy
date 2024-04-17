bibl.o: bibl.c bibl.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -fPIC bibl.c -o bibl.o

libbibl.so: bibl.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -shared bibl.o -o libbibl.so

main: main.c libbibl.so
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -o main

clean:
	rm -f *.o *.so main