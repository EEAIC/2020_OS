all: cpu mem threads

clean:
	rm -f ../build/cpu ../build/mem ../build/threads

cpu: cpu.c common.h
	gcc -o ../build/cpu cpu.c -Wall

mem: mem.c common.h
	gcc -o ../build/mem mem.c -Wall

threads: threads.c common.h common_threads.h
	gcc -o ../build/threads threads.c -Wall -pthread

io: io.c common.h
	gcc -o io io.c -Wall
