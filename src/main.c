#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define N_WORD_GOAL 2 

#include"box.h"
#include"check_word.h"
#include"generate_indices.h"
#include"word_stack.h"
#include"collect_solutions.h"


int main(){

    puts("\nPlease enter the letters of the Letter Box in clockwise fashion, starting with the left-most letter on the top side. Then hit the <return>.\n");

    if(BOX_CIRCU < 27){
        struct Box display_box;
        char box_string[BOX_CIRCU + 1]; 
        for(int i=0; i<BOX_CIRCU; i++){
            box_string[i] = ((char) i + 'A' );
        }
        box_string[BOX_CIRCU] = '\0';
        init_box(&display_box, box_string);
    
        printf("%s <-- Like this!\n\n", box_string);
        show_box(&display_box);
        puts("\n");
    }

    struct Box box;
    request_box_string(&box);
    
    printf("\n");
    show_box(&box);
    printf("\n");

    printf("Generating solutions..\n");
    printf("\n");
    generate_indices(&box);

    collect_solutions(&box);

    return 0;
}


