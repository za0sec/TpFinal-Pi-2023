#include <stdio.h>
#include "bikeSharing.h"

#define CHUNK 20

int main( int argc, char * argv[] ){

    if (argc != 3){
        fprintf(stderr, "Arguments should be 2");
        exit(ARERR);
    }

    bikeSharingADT bikeSh = readAddCsv(argv[1]);

}

bikeSharingADT readAddCsv(const char * filename){

    FILE * file = fopen(filename, "rt");
        if(file == NULL){
            fprintf(stderr, "Error opening file %s\n", filename);
            exit(OPENERR);
        }

    bikeSharingADT bikeSharing = newBikeSharing();
    if( bikeSharing == NULL ){
        fprintf(stderr, "Memory Error");
        exit(MEMERR);
    }

    char readText[MAXCHAR];

    fscanf(file, "%s\n", readText);

    char * startDate;
    
    size_t station1Id;
    
    char * endDate;
    
    size_t station2Id;
    
    size_t isMember;

    

}



