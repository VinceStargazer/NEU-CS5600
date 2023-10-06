all: qtest

qtest: queuetest.c queue.c queue.h
	gcc -o qtest queuetest.c queue.c

clean:
	rm -f qtest