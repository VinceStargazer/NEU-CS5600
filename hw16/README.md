# How this program works
This program uses a doubly linked list data structure to store the messages on disk. It also utilizes a LRU design, whenever a message of
a certain ID is retrieved, the program will move this message to the head of the linked list, so that it will reduce time spent on retrieving those most popular messages.

# How to run this program
1.  Run `make` in the terminal.
2.  Run `/main`.
3.  If you want to create a new message, enter the command `c`/`create`. If you want to retrieve a message by its unique id, enter the
    command `r`/`retrieve`. If you want to abort this program, enter the command `q`/`quit`

# How to check the correctness
1.  Run `./test.sh` to test our given cases.