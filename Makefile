all: build run_valgrind

run_valgrind:
	valgrind ./main.bin ./test.jpg

build:
	gcc -I/usr/include/libexif main.c -o main.bin -lexif

format:
	clang-format -i *.c

clean:
	rm -vf *.bin
