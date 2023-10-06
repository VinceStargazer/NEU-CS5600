all: allocmem loadmem

allocmem: alloc-mem-yu.c
	gcc -o allocmem alloc-mem-yu.c

loadmem: load-mem-yu.c
	gcc -o loadmem load-mem-yu.c

clean:
	rm -f allocmem loadmem