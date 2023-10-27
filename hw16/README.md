# How to run this program
1.  Run `make` in the terminal.
2.  If you want to create a new message, call `./main -c <sender> <receiver> <content> <flag>`.
    Sender and receiver shall be strings less than 20 characters. Content shall be a string less than
    200 characters wrapped by double quote. Flag shall be an integer either 1 or 0 to indicate whether
    the message was delivered. If the action is successful, the terminal would display an output telling you the message id assigned.
3.  On the other hand, if you want to retrieve a message by looking up its id, call
    `./main -r <id>`

# How to check the correctness
1.  Run `./test.sh` to test our given cases.