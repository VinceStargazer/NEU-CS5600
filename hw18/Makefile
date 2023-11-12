all: main evaluator

main: main.c message.c message.h uuid.c uuid.h hashmap.c hashmap.h LRUcache.c LRUcache.h get_env.c get_env.h
	gcc -o main main.c message.c uuid.c hashmap.c LRUcache.c get_env.c

evaluator: evaluator.c message.c message.h uuid.c uuid.h hashmap.c hashmap.h LRUcache.c LRUcache.h get_env.c get_env.h
	gcc -o evaluator evaluator.c message.c uuid.c hashmap.c LRUcache.c get_env.c

clean:
	rm -f main evaluator