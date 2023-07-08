#include <stdio.h>
#include "bikeSharingADT.h"

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

    fscanf(file, "%s\n", readText); /* salta la primer linea */

    // example
    // start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member
    // 2021-09-20 06:31:28;348;2021-09-20 07:02:22;332;1
    // 2022-09-02 11:19:47;753;2022-09-02 11:22:23;702;0

    char * startDate;
    
    size_t station1Id;
    
    char * endDate;
    
    size_t station2Id;
    
    size_t isMember;

    while( fgets(readText, MAXCHAR, file) != NULL ){
        startDate = strtok(readText, ";");
        station1Id = atoi(strtok(NULL, ";"));
        endDate = strtok(NULL, ";");
        station2Id = atoi(strtok(NULL, ";"));
        isMember = atoi(strtok(NULL, "\n"));
    } // LLEGAMOS HASTA ACA

    

}

//QUERY 1: INICIO DE VIAJES DE MIEMBROS POR ESTACION ORDENADOS DE MAYOR A MENOR

//QUERY 2: CANTIDAD DE VIAJES DE A A B Y DE B A A, Y ASI SIGUE... (MATRIZ DE ADYACENCIA)

//QUERY 3: CANTIDAD DE VIAJES POR MES DE CADA ESTACION, LISTAR ALFABETICAMENTE

//QUERY 4: INICIO Y FIN MISMA ESTACION, ORDENAR ESTACIONES DE MAYOR A MENOR

