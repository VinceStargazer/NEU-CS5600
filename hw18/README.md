# How this practicum works
This practicum uses a doubly linked list data structure to store the messages in JSON format on disk. It also utilizes a LRU design - Whenever a message of a certain ID is retrieved, the data structure will move this message to the head of the linked list, so that it will reduce time spent on retrieving those relatively popular messages. This practicum is made up by three programs: main.c for user interaction and manipulation of message store, test.c to test the correctness of the LRU cache, and evaluator.c to evaluate cache hits/misses/hit ratio based on the LRU design.

# How to run the main program
1.  Run `make` in the terminal.
2.  Run `./main`.
3.  Enter the command for different purposes:
    * `c`/`create`: Create a new message
    * `r`/`retrieve`: Retrieve a message by its unique id
    * `q`/`quit`: Abort the program
    * `dc`/`display cache`: Display the current program cache
4.  Follow the prompts and offer your answers in the terminal. Note that "sender" or "receiver" should be within 20 characters, "content" should be within 200 characters and not include a new line character (otherwise, the program will treat the answer as complete), "flag" should be either 1 (delivered) or 0 (not delivered), and "id" should be 36 characters (otherwise it doesn't match our UUID design).

# How to test the cache mechanism
1.  Run `make` in the terminal. (If not done yet)
2.  Run `./test` to test my official test cases. You can also test the cache mechanism in the following manual way:
3.  (Optional) In .env file, change 'MESSAGE_SIZE', 'CACHE_CAPACITY', and 'IS_LRU' (whether to use LRU replacement or random replacement strategy for the cache) to be desired values.
4.  Run the main program.
5.  Use `r`/`retrieve` command to retrieve messages as many as CACHE_CAPACITY from messages.dat. Meanwhile, use `dc`/`display cache` command constantly to check if the id of the latest added message ranks at the tail of the cache (tail -> most recently used page).
6.  Retrieve another new message from messages.dat. This time, the number of messages stored in the cache should remain CACHE_CAPACITY. One message got 'kicked out' and you might check if that message was originally placed at the head of this cache (head -> least recently used page).

# How to evaluate cache hits
1.  (Optional) Change the value of SAMPLE_SIZE in line 8 of evaluator.c to be the desired sample size. Change the value of TEST_COUNT in line 8 of evaluator.c to be the desired testing times. Change the 'CACHE_CAPACITY' value in .env file to be the desired cache capacity.
2.  Run `make` in the terminal. (If not done yet)
3.  Run `./evaluator`.
4.  Check the lowest part of evaluation.txt for the latest test result.