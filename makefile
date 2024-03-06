CC	= gcc
CFLAGS	= -g -O2 -Wall -Wpedantic
RM	= rm -f


default: src/main.c
	$(CC) $(CFLAGS) -o unboxletters src/main.c 

clean:
	rm output/indices.bin
	rm output/solutions.txt
