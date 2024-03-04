#ifndef CHECK_WORD_H_
#define CHECK_WORD_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"box.h"

#define WORD_DELIMITER ((index_t) -1)
// In the output file, each word is stored in the following format:
// |WORD_DELIMITER|WORD_DELIMITER|box_index|box_index|...|box_index|WORD_DELIMITER|score|
// were the score is a count of how many indices are covered by the given word.

struct LetterStack {
    int ptr;
    index_t box_index[MAX_WORD_LENGTH];
};

void lettr_push(struct LetterStack *stack, index_t index);
index_t lettr_pop(struct LetterStack *stack);
index_t lettr_viewtop(struct LetterStack *stack);

occupancy conv_index2occ(index_t index);
int conv_occ2score(occupancy);
index_t get_last_index(index_t *const word);
void print_indices(index_t *const start); 
void print_word(struct Box *box, index_t *word);
occupancy get_occ_word(index_t *word);
occupancy get_occ_letter_stack(struct LetterStack *stack);

char conv_index2char(struct Box *box, index_t);
int check_word(struct Box *box, char const *word, char const *dest);



void lettr_push(struct LetterStack *stack, index_t index){
    throw_if(stack->ptr >= MAX_WORD_LENGTH, "Error: LetterStack Overflow! Pushed word is more than 32 characters long!");
    stack->ptr++;
    stack->box_index[stack->ptr] = index;
}


index_t lettr_pop(struct LetterStack *stack){
    if(stack->ptr == -1){
        return BOX_CIRCU; // Stack is empty, so we pass impossible index
    }
    return stack->box_index[(stack->ptr)--];
}


index_t lettr_viewtop(struct LetterStack *stack){
    if(stack->ptr == -1){
        return BOX_CIRCU; // Stack is empty, so we pass impossible index
    }
    return stack->box_index[stack->ptr];
}


occupancy conv_index2occ(index_t index){
    throw_if(index > sizeof(occupancy)*8 || index < 0, "Value of argument conv_index2occ out of bounds\n");
    return 1 << index;
}


int conv_occ2score(occupancy argument){
    int sum = 0;
    while(argument){
        sum += argument % 2;
        argument = argument >> 1;
    }
    return sum;
}


index_t get_last_index(index_t *const word){
    index_t *p_word = word;
    while( *p_word != -1 ){
        p_word++;
    }
    return *(--p_word);
}


occupancy get_occ_word(index_t *const word){
    index_t *p_word = word;
    occupancy occ = 0;
    while( *p_word != -1 ){
        occ |= conv_index2occ(*p_word);
        p_word++;
    }
    return occ;
}


occupancy get_occ_letter_stack(struct LetterStack *stack) {
    occupancy occ = 0;
    for(int i = 0; i<= stack->ptr; i++) {
        occ |= conv_index2occ(stack->box_index[i]);
    }
    return occ;
}


char conv_index2char(struct Box *box, index_t index){
    return box->all_letters[index];
}


int check_word(struct Box *box, char const *word, char const *dest){

    int length = (int) strlen(word);
    struct LetterStack letter_stack;
    letter_stack.ptr = -1;
    int prev_side = 4; // Valid sides {0,1,2,3}. Set to FOUR means there is no previous side.
    index_t box_index = 0;
    const index_t word_delimiter = WORD_DELIMITER;
    int score;
    
    while(box_index < BOX_CIRCU || letter_stack.ptr > -1){
        // Exit loop when letter_stack is empty AND we're at the end of the box letters.
        if(box_index < BOX_CIRCU ){
            if(word[0] != box->all_letters[box_index] || prev_side == side(box_index)){
                box_index++;
                continue;
            }
            lettr_push(&letter_stack, box_index);
            prev_side = side(box_index);
            word++;
            if(word[0]!='\0'){ // If not yet at the last letter, starts checking next letter at the start of the box
                box_index = 0;
                continue;
            }
            FILE *output_indices = fopen(dest, "ab");
            throw_if(output_indices == NULL,"Error: Could not open output file\n"); 

            for(int i=0; i<length; i++){
               fwrite( letter_stack.box_index + i, sizeof(index_t),1, output_indices);
            }
            fwrite(&word_delimiter, sizeof(index_t), 1, output_indices);
            score = conv_occ2score( get_occ_letter_stack(&letter_stack) );
            fwrite(&score, sizeof(index_t), 1, output_indices);
            fwrite(&word_delimiter, sizeof(index_t), 1, output_indices);
            fwrite(&word_delimiter, sizeof(index_t), 1, output_indices);

            fclose(output_indices);
            // Keeps checking for more solutions
            box_index = lettr_pop(&letter_stack) + 1;
            prev_side = side( lettr_viewtop(&letter_stack) );
            word--;
        } else if(letter_stack.ptr > -1){    
        box_index = lettr_pop(&letter_stack) + 1;
        prev_side = side( lettr_viewtop(&letter_stack) );
        word--;
        } else return 0;
    }
    return 0;
}


void print_indices(index_t *const start){
    index_t *p_word = start;
    while( *p_word != -1 ){
        printf("%x ", *p_word);
        p_word++;
    }    
    printf("\n");
}


void print_word(struct Box *box, index_t *const start){
    index_t *p_word = start;
    while( *p_word != -1 ){
        printf("%c", conv_index2char(box, *p_word));
        p_word++;
    }
    printf("\n");
}

#endif
