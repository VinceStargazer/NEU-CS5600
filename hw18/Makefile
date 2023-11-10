all: main

main: main.c message.c message.h uuid.c uuid.h hashmap.c hashmap.h LRUcache.c LRUcache.h
	gcc -o main main.c message.c uuid.c hashmap.c LRUcache.c

clean:
	rm -f main