# UnboxLetters
Letter Boxed is a puzzle game on the New York Times website:
https://www.nytimes.com/puzzles/letter-boxed

**UnboxLetters** is CLI program that generates solutions to the Letter Boxed puzzles. Only available for GNU/Linux type operating systems. This project is not affiliated with the New York Times. It is mostly an excuse to write something in C.

##### Note: This is a work in progress!
The program works on my own system, but there is no real UI to configure parameters. For example this means that if you would like to generate a more narrow or a wider class of solutions, this would need to be changed in the source and then the binary would need to be recompiled. See the "Configuration" section of this document.

### Setup (GNU/Linux type OS)
First clone the repo.
```
git clone https://github.com/Bruno-Eijs/UnboxLetters.git
```
Then if you have GCC and MAKE, run `make` inside the `UnboxLetters/` directory. If you want to use a different compiler you can modify the `makefile`. If you don't use MAKE you can compile the C code yourself with your compiler of choice; for example
```
gcc -g -O2 -o unboxletters src/main.c
```
### Use 
Run the executable inside its directory.
```
./unboxletters
```
You will be prompted to input the letters of the Letter Boxed puzzle.

Once the text `WE'RE DONE!` appears, the list of the generated solutions can then be found in `output/solutions.txt`. If it is does not appear and no errors were thrown then no solutions were found.

By default solutions that use **more than two words are not considered**. This default value of `2` is set by the macro constant `N_WORD_GOAL` in `src/main.c` and can be modified. Do note that raising this value a lot will significantly lengthn the search for solutions.

###### Output formating
In `output/solutions.txt` the words are shown with their positions on the Letter Box underneath. This is to avoid ambiguity in case letters are repeated on the Letter Box. The position indices start counting from `0` and are in the order in which the letters are entered by the user (clockwise starting from the top-left).

#### Configuration
There are 3 macro constants in the code that can be modified within limits, though not much testing has been performed with values outside the default values:
- `N_WORD_GOAL` in `src/main.c` is the maximum number of words that are allowed in a single solution. It is set to `2` by default. Setting this to values such as `5`, `6` or higher will make the computation take very long and will potentially make the `output/solutions.txt` file very large.
- `BOX_WIDTH` in `src/box.h` is the width of the Letter Box. Its default value is `3`. Some minor things will surely break if this value is set to `7` or higher.
- `MAX_WORD_LENGTH` in `src/box.h`, is the maximum length of a word in the word list.  Its default value is `40`.

#### Word List 
The word list that is used resides in `data/word-list.txt`. 
It was taken from 
https://github.com/dwyl/english-words/tree/master
which is in turn taken from **Infochimps**.
https://www.infochimps.com/datasets/word-list-350000-simple-english-words-excel-readable

**Note:** This is not an ideal word list for this purpose. It contains many words that are not accepted by the Letter Boxed game.

You can use your own word list by replacing `data/word-list.txt` with a text file with the same name. The text should by in ASCII encoding and the formatting should be as follows: Words should be in all-caps and separated by a LF character.
Lastly it should be noted that `MAX_WORD_LENGTH` is set in `src/box.h` to `40` but can be modified to the necessary value in case the word list contains longer words.
