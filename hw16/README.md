# How this program works
This program uses a doubly linked list data structure to store the messages in JSON format on disk. It also utilizes a LRU design - Whenever a message of a certain ID is retrieved, the program will move this message to the head of the linked list, so that it will reduce time spent on retrieving those relatively popular messages.

# How to run this program
1.  Run `make` in the terminal.
2.  Run `/main`.
3.  If you want to create a new message, enter the command `c`/`create`. If you want to retrieve a message by its unique id, enter the command `r`/`retrieve`. If you want to abort this program, enter the command `q`/`quit`
4.  Follow the prompts and offer your answers in stdin. Note that "sender" or "receiver" should be within 20 characters, "content" should be within 200 characters and not include a new line character (otherwise, the program will treat the answer as complete), "flag" should be either 1 (delivered) or 0 (not delivered), and "id" should be 36 characters (otherwise it doesn't match our UUID design).

# How to check the correctness
If you create or retrieve a message and exit the program, check "messages.dat" to see if that message is located in the top of the file and if other messages remain in their previous positions. If so, it means LRU works well and your data are properly stored.