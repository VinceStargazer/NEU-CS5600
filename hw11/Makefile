all: getRand cipher readFile

getRand: getRand.c
	gcc -o getRand getRand.c

cipher: cipher.c polybius.c polybius.h
	gcc -o cipher cipher.c polybius.c

readFile: readFile.c queue.c queue.h
	gcc -o readFile readFile.c queue.c

clean:
	rm -f getRand cipher readFile output_batch_*