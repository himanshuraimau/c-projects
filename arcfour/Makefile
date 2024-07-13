all: example arcfour

example: example.o arcfour.o
	gcc arcfour.o example.o -o example -O2 -Wall

example.o: example.c
	gcc -c example.c -O2 -Wall

arcfour: arcfour.o
	gcc arcfour.o -o arcfour.so -O2 -Wall -fPIC -shared -ldl -D_GNU_SOURCE

arcfour.o: arcfour.c
	gcc -c arcfour.c -O2 -Wall

clean:
	rm -f *.o *.so example
