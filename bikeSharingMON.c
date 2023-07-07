#include <stdio.h>


#define CHUNK 20

int main( int argc, char * argv[] ){

    if (argc != 2){
        fprintf(stderr, "Arguments should be 2");
        exit(1);
    }

    char c;

    char * str = NULL;

    char * string[] = NULL;

    int i;

    while (argv[0] != EOF){

        for (i=0; argv[0] != '\n'; i++){
    
            c = argv[0][i];
            
            if (i%CHUNK)
                str = realloc(str, (i+CHUNK) * sizeof(char));

            str[i] = argv[0][i];

        }

        str = realloc(str, i * sizeof(char));

        

    }

}
