#ifndef WORD_STACK_H_
#define WORD_STACK_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"box.h"
#include"check_word.h"


struct WordStack {
    int ptr;
    index_t wrd[N_WORD_GOAL + 1][MAX_WORD_LENGTH]; 
    occupancy letters_covered[N_WORD_GOAL + 1]; 
    int score[N_WORD_GOAL + 1];
    size_t file_seek[N_WORD_GOAL + 1];
};

struct WordStack init_wrd_stack();
void wrd_push(struct WordStack *stack, index_t *starts_at, int word_length, size_t seek);
void wrd_pop(struct WordStack *stack); 
int check_win(struct WordStack *stack); 


struct WordStack init_wrd_stack()
{
    struct WordStack ws =
    {
        .ptr = 0,
        .wrd = {{-1}},
        .letters_covered = {0},
        .score = {BOX_CIRCU},
        .file_seek = {0}
    };

    for(int i=1; i<N_WORD_GOAL+1; i++)
    {
        ws.wrd[i][0] = -1;
        ws.score[i] = BOX_CIRCU;
    }
    return ws;
}

void wrd_push(struct WordStack *stack, index_t *starts_at, int word_length, size_t seek){
    throw_if(stack->ptr > N_WORD_GOAL, "WordStack overflow!");

    occupancy prev_occ = stack->letters_covered[stack->ptr];

    stack->file_seek[stack->ptr+1] = seek;
    memcpy(stack->wrd[stack->ptr+1], starts_at, word_length*sizeof(index_t)); 
    stack->letters_covered[stack->ptr+1] = get_occ_word(stack->wrd[stack->ptr+1]) | prev_occ;
    stack->score[stack->ptr+1] = conv_occ2score( stack->letters_covered[stack->ptr+1] );

    stack->ptr++;
}


void wrd_pop(struct WordStack *stack){
    throw_if(stack->ptr == 0, "Error: Trying to wrd_pop() and empty WordStack\n");
    stack->letters_covered[stack->ptr] = 0;
    memset(stack->wrd[stack->ptr], 0, MAX_WORD_LENGTH*sizeof(index_t));
    stack->wrd[stack->ptr][0] = -1;
    if(stack->ptr < N_WORD_GOAL)
    {
        stack->file_seek[stack->ptr+1] = 0;
        stack->score[stack->ptr+1] = BOX_CIRCU;  
    }
    stack->ptr--;
}


int check_win(struct WordStack *stack){
    if( stack->letters_covered[stack->ptr] == (occupancy) ((1 << BOX_CIRCU) -1) )
        return 1;
    return 0;
}


#endif
