all: compile exe

compile:
	gcc -fopenmp -O3 -o main main.c

exe:
	time ./all.sh

clean:
	rm -f main images/*.bin* && rm -f main *.bin*
