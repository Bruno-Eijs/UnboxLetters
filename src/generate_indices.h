#ifndef GENERATE_INDICES_H_
#define GENERATE_INDICES_H_

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"check_word.h"
#include"box.h"

#define INPUT_FILE "data/word-list.txt"
#define OUTPUT_FILE "output/indices.bin"

void generate_indices(struct Box *box);

void clean_up_indices();


void generate_indices(struct Box *box){
    
    FILE *input_words;
    input_words = fopen(INPUT_FILE,"r");
    throw_if( input_words == NULL, "Error: Could not open input file\n");
    
    const index_t word_delimiter = WORD_DELIMITER;
    FILE *output_indices = fopen(OUTPUT_FILE, "ab");
    throw_if( output_indices == NULL, "Error: Could not open output file\n");

    fwrite(&word_delimiter, sizeof(index_t), 1, output_indices);
    fwrite(&word_delimiter, sizeof(index_t), 1, output_indices);
    fclose(output_indices);

    const int buffer_length = MAX_WORD_LENGTH + 1;
    char buffer[buffer_length];
    // PUT CHECK HERE THAT MAKES SURE INPUT_LENGTH < MAX_WORD_LENGTH +1
    while(fgets(buffer, buffer_length, input_words)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        check_word(box, buffer, OUTPUT_FILE);
    }

    output_indices = fopen(OUTPUT_FILE, "ab");
    throw_if( output_indices == NULL, "Error: Could not open output file\n");

    fclose(output_indices);
    fclose(input_words);
}


void clean_up_indices(){

    FILE *output_indices = fopen(OUTPUT_FILE, "w");
    throw_if( output_indices == NULL, "Error: Could not find indices file to delete.\n");
    fclose(output_indices);
}







#endif
