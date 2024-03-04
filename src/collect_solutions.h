#ifndef COLLECT_SOLUTIONS_H_
#define COLLECT_SOLUTIONS_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>

#include"box.h"
#include"check_word.h"
#include"word_stack.h"

#define EOF_REACHED ((size_t) -1)

size_t seek_word_start(index_t *data, size_t indices_read, struct WordStack *stack);

size_t seek_word_next(index_t *data, size_t indices_read, struct WordStack *stack);


void save_word_stack(struct Box *box, struct WordStack *stack);


void find_word_start(struct WordStack *stack);

void find_word_next(struct WordStack *stack);

void collect_solutions(struct Box *box);

size_t seek_word_start(index_t *data, size_t indices_read, struct WordStack *stack){
    size_t file_seek = stack->file_seek[stack->ptr+1];
    if(file_seek > indices_read -2)
    {
       return EOF_REACHED;
    }
    int phase = 0; // Phase 0: before collecting a word. Phase 1: Started collecting a word
    int word_length = 1;
    size_t word_start = 0;
    size_t new_seek = EOF_REACHED;

    for(size_t i = file_seek +2; i < indices_read -1; i++)
    {
        // Start collecting word
        if(data[i-2] == WORD_DELIMITER 
                && data[i-1] == WORD_DELIMITER 
                && phase == 0)
        {
            word_start = i;
            phase = 1;
        }
        // Collecting word
        if(phase == 1 && data[i] != WORD_DELIMITER)
        {
            word_length++;
            continue;
        }
        // Stop collecting if the score is what we want
        if(phase == 1 && data[i] == WORD_DELIMITER && data[i+1] == stack->score[stack->ptr+1]) 
        {
            new_seek = i;
            break;
        // Reset to Phase 0 if the score is not high enough
        } else if(phase == 1 && data[i] == WORD_DELIMITER)
        {
            word_start = 0;
            word_length = 1;
            phase = 0;
        }
    }
    throw_if(word_length < 1, "For-loop error in seek_word_start(). See collect_solutions.h\n"); // Get rid of this after debugging

    if(word_start)
    {
        wrd_push(stack, data + word_start, word_length, new_seek);
    }
    
    return new_seek;
}

size_t seek_word_next(index_t *data, size_t indices_read, struct WordStack *stack){
    size_t file_seek = stack->file_seek[stack->ptr+1];
    if(file_seek > indices_read -2)
    {
       return EOF_REACHED;
    }
    int phase = 0; // Phase 0: before collecting a word. Phase 1: Started collecting a word
    int word_length = 1;
    size_t word_start = 0;
    size_t new_seek = EOF_REACHED;

    occupancy old_occ = stack->letters_covered[stack->ptr];
    occupancy new_occ;
    index_t prev_last_letter = get_last_index( stack->wrd[stack->ptr] );

    for(size_t i = file_seek +2; i < indices_read -1; i++)
    {
        // Start collecting word
        if(data[i-2] == WORD_DELIMITER 
                && data[i-1] == WORD_DELIMITER 
                && data[i] == prev_last_letter
                && phase == 0)
        {
            word_start = i;
            phase = 1;
            new_occ = old_occ;
        }
        // Collecting word
        if(phase == 1 && data[i] != WORD_DELIMITER)
        {
            word_length++;
            new_occ |= conv_index2occ( data[i] );
            continue;
        }
        // Stop collecting if the score is what we want
        if(phase == 1 && data[i] == WORD_DELIMITER && stack->score[stack->ptr+1] == conv_occ2score(new_occ) )
        {
            new_seek = i;
            break;
        // Reset to Phase 0 if the score is not high enough
        } else if(phase == 1 && data[i] == WORD_DELIMITER)
        {
            word_start = 0;
            word_length = 1;
            phase = 0;
        }
    }
    throw_if(word_length < 1, "For-loop error in seek_word_start(). See collect_solutions.h\n"); // Get rid of this after debugging

    if(word_start)
    {
        wrd_push(stack, data + word_start, word_length, new_seek);
        throw_if(conv_occ2score(new_occ) <= conv_occ2score(old_occ), "Somehow the score diminished. seek_word_next()\n");
    }
    
    return new_seek;
}


void save_word_stack(struct Box *box, struct WordStack *stack){

    char *dest = "output/solutions.txt";
    FILE *solutions_file = fopen(dest, "a");
    throw_if( solutions_file == NULL,"Error: Could not open solutions file\n");
    throw_if(!stack->ptr,"Error: Empty Word Stack cannot be a solution.\n");
    throw_if(stack->ptr < 0,"Error: Invalid WordStack pointer: save_word_stack().\n");

    fprintf(solutions_file, "======================================================\n");

    for(int s_ptr=1; s_ptr <= stack->ptr; s_ptr++){
        index_t *p_letter = stack->wrd[s_ptr];
        // Prints characters
        while( *p_letter != -1 ){
            fputc( conv_index2char(box, *p_letter), solutions_file);
            p_letter++;
        }
        fputc('\n', solutions_file);
        // Prints the indices in case of ambiguity
        p_letter = stack->wrd[s_ptr];
        while( *p_letter != -1 ){
            fprintf(solutions_file, "%d ", *p_letter );
            p_letter++;
        }
        fputc('\n', solutions_file);
    }

    fprintf(solutions_file, "Solution uses %d word(s).\n", stack->ptr);
    fclose(solutions_file);
}


void find_word_start(struct WordStack *stack){

    char *index_file_path = "output/indices.bin";
    FILE *index_file = fopen(index_file_path, "rb");
    throw_if( !index_file, "Error: fopen()\n");

    struct stat sb;
    throw_if( stat(index_file_path, &sb) != 0, "Error: stat()\n");

    index_t *words_data;
    words_data = (index_t *) malloc(sb.st_size + 1);
    throw_if( !words_data, "Error: malloc()\n");

    size_t indices_read;
    indices_read = fread(words_data, sizeof(index_t), sb.st_size/sizeof(index_t), index_file);

    
    do
    {
        if(seek_word_start(words_data, indices_read, stack) < indices_read ) 
        {
            free(words_data);
            fclose(index_file);
            return;
        }

        stack->score[stack->ptr+1]--;
        stack->file_seek[stack->ptr+1] = 0;        
    } while( stack->score[stack->ptr+1] > 0 );

    free(words_data);
    fclose(index_file);
     
    printf("WE'RE DONE!\n");
    clean_up_indices();
    exit(0);
}


void find_word_next(struct WordStack *stack){

    char *index_file_path = "output/indices.bin";
    FILE *index_file = fopen(index_file_path, "rb");
    throw_if( !index_file, "Error: fopen()\n");

    struct stat sb;
    throw_if( stat(index_file_path, &sb) != 0, "Error: stat()\n");

    index_t *words_data;
    words_data = (index_t *) malloc(sb.st_size + 1);
    throw_if( !words_data, "Error: malloc()\n");

    size_t indices_read;
    indices_read = fread(words_data, sizeof(index_t), sb.st_size/sizeof(index_t), index_file);

    occupancy old_occ = stack->letters_covered[stack->ptr];

    do
    {
        if(seek_word_next(words_data, indices_read, stack) < indices_read ) 
        {
            free(words_data);
            fclose(index_file);
            return;
        }

        stack->score[stack->ptr+1]--;
        stack->file_seek[stack->ptr+1] = 0;        
    } while( stack->score[stack->ptr+1] > conv_occ2score(old_occ) );

    free(words_data);
    fclose(index_file);
     
    wrd_pop(stack);
    return;
}


void collect_solutions(struct Box *box)
{
    struct WordStack ws; 
    ws = init_wrd_stack();
    
    int solutions_left = 10000000;
    while(solutions_left > 0)
    {
        if(ws.ptr == 0)
        {
            find_word_start(&ws);
        } else if(ws.ptr < N_WORD_GOAL)
        {
            find_word_next(&ws);
        } else
        {
           wrd_pop(&ws);
        }

        if(check_win(&ws))
        {
            save_word_stack(box, &ws);
            solutions_left--;
            wrd_pop(&ws);
        }
    }

}


#endif
