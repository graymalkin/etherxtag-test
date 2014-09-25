all: clean etherxtag

build: etherxtag

etherxtag: main.o
	gcc bin/main.o -o etherxtag-test

main.o: main.c
	gcc -c main.c -o bin/main.o

clean:
	rm -rf bin/*.o etherxtag-test
