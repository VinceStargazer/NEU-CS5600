all: getRand multithreading

getRand: getRand.c
	gcc -o getRand getRand.c

multithreading: multithreading.c queue.c queue.h cipher.c cipher.h
	gcc -o multithreading multithreading.c queue.c cipher.c

clean:
	rm -f getRand multithreading output_batch_*