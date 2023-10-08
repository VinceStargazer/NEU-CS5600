all: getRand cipher multiprocessing

getRand: getRand.c
	gcc -o getRand getRand.c

cipher: cipher.c polybius.c polybius.h
	gcc -o cipher cipher.c polybius.c

multiprocessing: multiprocessing.c queue.c queue.h
	gcc -o multiprocessing multiprocessing.c queue.c

clean:
	rm -f getRand cipher multiprocessing output_batch_*