# How to run this program
1. Run `make` in the terminal
2. Call getRand to create a list of random words of the given number: `./getRand <number> random_words.txt`
3. Call multiprocessing: `./multiprocessing`
4. (Optional) Run `make clean` to remove all created files

# How to check the correctness
1. Browse through created output text files to make sure the number of encoded strings (each for a line)
is the same as what you indicated in getRand argument
2. Run `./cipher -d` and then try typing down some encoded strings in the terminal to see if they can be
decoded to the corresponding word in random_words.txt. You can also try `./cipher -e` to test it reversely