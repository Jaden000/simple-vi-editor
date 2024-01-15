CC = gcc
ifeq ($(OS), Windows_NT)
    LIBS = -L. -lpdcurses
else
    LIBS = -lncurses
endif

run: main.o linkedlist.o 
	gcc -o vite main.o linkedlist.o $(LIBS)
main.o: main.c
	gcc -c main.c -o main.o 
linkedlist.o: linkedlist.c
	gcc -c linkedlist.c -o linkedlist.o
clean:
	rm -f *.o
	rm *.o vite


