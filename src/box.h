#ifndef BOX_H_
#define BOX_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define BOX_WIDTH 3
#define BOX_CIRCU 4*BOX_WIDTH
#define MAX_WORD_LENGTH 40


struct Box {
    char all_letters[BOX_CIRCU];
};

typedef int index_t;
typedef int occupancy;

int side(index_t);
void init_box(struct Box *box, char const *letters);
void show_box(struct Box *box);

void throw_if(bool condition, char *message);

void request_box_string(struct Box *box);


void throw_if(bool condition, char *message)
{
    if(condition)
    {
        perror(message); 
        exit(EXIT_FAILURE); 
    }

}


void show_box(struct Box *box){
    //Printing Top Side
    printf(" ");
    for(int iter=0; iter < BOX_WIDTH; iter++){
        printf(" %c ", box->all_letters[iter]);
    }
    printf("\n");
    //Printing Left and Right sides
    for(int iter=0; iter< BOX_WIDTH; iter++){
        printf("%c", box->all_letters[BOX_CIRCU-1-iter]);
        for(int space=0; space< BOX_WIDTH; space++){
            printf("   ");
        }
        printf("%c\n", box->all_letters[iter+BOX_WIDTH]); 

    }
    //Printing Bottom Side
    printf(" ");
    for(int iter=0; iter < BOX_WIDTH; iter++){
        printf(" %c ", box->all_letters[3*BOX_WIDTH-iter-1]);
    }
    printf("\n");
    return;
}

void init_box(struct Box *box, char const *letters){
    char const *s = letters;
    throw_if(strlen(s) < BOX_CIRCU,"Error: Not enough letters to fill the box.\n");

    for(int i=0; i<BOX_CIRCU; i++){
        box->all_letters[i] = s[i];
    }
}

int side(index_t index){
    // Note that lettr_viewtop() and lettr_pop() will return BOX_CIRCU when the letter_stack is empty. Correspondingly side(BOX_CIRCU) will return 4 instead of {0,1,2,3}.
    return (index / BOX_WIDTH);
}



void request_box_string(struct Box *box)
{
    char letters[BOX_CIRCU + 1] = {0};

    throw_if(!fgets(letters, sizeof(letters), stdin), "Error: fgets(). Please try again." );

    for(int i=0; i<BOX_CIRCU; i++)
    {
        if(letters[i] > 64 && letters[i] < 91)
        {
            box->all_letters[i] = letters[i];
        } else if(letters[i] > 96 && letters[i] < 123)
        { // convert lower to uppercase
            box->all_letters[i] = letters[i] - 32;
        } else
        {
            puts("Please enter only letters (without accents)\n");
            exit(EXIT_FAILURE);
        }
    }
}
#endif
