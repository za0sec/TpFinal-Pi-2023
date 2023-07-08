#include <stdio.h>
#include "bikeSharing.h"

#define CHUNK 20

int main( int argc, char * argv[] ){

    if (argc != 3){
        fprintf(stderr, "Arguments should be 2");
        exit(ARERR);
    }

}

bikeSharingADT readAddCsv(const char * filename){

    FILE * file = fopen(filename, "rt");
        if(file == NULL){
            fprintf(stderr, "Error opening file %s\n", filename);
            exit(OPENERR);
        }

    bikeSharingADT bikeSharing = newBikeSharing();
    if( bikeSharing == NULL ){
        fprintf(stderr, "Error creating file %s\n", filename);
        exit(CRERR);
    }

}



