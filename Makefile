build_flags = -g -Wall -pedantic-errors -std=gnu99

all: clean etherxtag

build: etherxtag

etherxtag: main.o tcp.o udp.o xtag_test.o xscope_test.o
	gcc $(build_flags) bin/main.o bin/tcp.o bin/xtag_test.o bin/xscope_test.o bin/udp.o -o etherxtag-test

main.o: src/main.c
	gcc $(build_flags) -c src/main.c -o bin/main.o

tcp.o: src/tcp.c
	gcc $(build_flags) -c src/tcp.c -o bin/tcp.o

udp.o: src/udp.c
	gcc $(build_flags) -c src/udp.c -o bin/udp.o

xtag_test.o: src/xtag_test.c
	gcc $(build_flags) -c src/xtag_test.c -o bin/xtag_test.o

xscope_test.o: src/xscope_test.c
	gcc $(build_flags) -c src/xscope_test.c -o bin/xscope_test.o

clean:
	rm -rf bin/*.o etherxtag-test
