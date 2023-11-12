all: main evaluator

main: main.c message.c message.h uuid.c uuid.h hashmap.c hashmap.h LRUcache.c LRUcache.h
	gcc -o main main.c message.c uuid.c hashmap.c LRUcache.c

evaluator: evaluator.c message.c message.h uuid.c uuid.h hashmap.c hashmap.h LRUcache.c LRUcache.h
	gcc -o evaluator evaluator.c message.c uuid.c hashmap.c LRUcache.c

clean:
	rm -f main evaluator