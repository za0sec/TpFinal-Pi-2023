#include <stdio.h>
#include "bikeSharingADT.h"

#define CHUNK 20

int main( int argc, char * argv[] ){

    if (argc != 3){
        fprintf(stderr, "Arguments should be 2");
        exit(ARERR);
    }

    bikeSharingADT bikesh = readAddCsv(argv[1]);
    bikesh = readName(bikesh, argv[2]);

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
    
    size_t station2Id;
    
    size_t isMember;

    while( fgets(readText, MAXCHAR, file) != NULL ){
        startDate = strtok(readText, ";");
        station1Id = atoi(strtok(NULL, ";"));
        strtok(NULL, ";");
        station2Id = atoi(strtok(NULL, ";"));
        isMember = atoi(strtok(NULL, "\n"));

        addStation(bikeSharing, station1Id, isMember); //SOLO QUERY 1
        //a la par, creo un vector que este ordenado por trips (tripsort)
        //vamos a tener q poner otra funcion  para los otros queries
    }
    
    
}

bikeSharingADT readName(bikeSharingADT bikesh, const char * filename){
    FILE * file = fopen(filename, "rt");
        if(file == NULL){
            fprintf(stderr, "Error opening file %s\n", filename);
            exit(OPENERR);
        }
    char readText[MAXCHAR];

    fscanf(file, "%s\n", readText); /* salta la primer linea */

    size_t stationId;
    char * token;
    char * stationName;

    while( fgets(readText, MAXCHAR, file) != NULL ){
        stationId = atoi(strtok(readText, ";"));
        token = strtok(NULL, ";");
        if(token != NULL){
            stationName = malloc((strlen(token)+1)); // * sizeof(char)
            if(stationName != NULL){
                strcpy(stationName, token);
            } else{
                fprintf(stderr, "Memory Error");
                exit(MEMERR);
            }
        } else{
            fprintf(stderr, "Null Token Error");
            exit(TOKERR);
        }
        strtok(NULL, ";"); 
        strtok(NULL, "\n"); //salteo las latitudes y longitudes

        bikesh->rankingStations[stationId-1].stationName = malloc(strlen(stationName)+1);
        if(bikesh->rankingStations[stationId-1].stationName != NULL){
            strcpy(bikesh->rankingStations[stationId-1].stationName, stationName);
        } else{
            fprintf(stderr, "Memory Error");
            exit(MEMERR);
        } //copia en nuestro vector ordenado por stationIds, el nombre de la estación.

    }
}

void query1(station){

//voy a tener que ordenar una copia del vector del ADT por orden alfabetico de las stations
creamo el vector
fx(vector, vector original , ....)
te tiene que dejar todo en "vector"
}

//QUERY 1: INICIO DE VIAJES DE MIEMBROS POR ESTACION ORDENADOS DE MAYOR A MENOR

//QUERY 2: CANTIDAD DE VIAJES DE A A B Y DE B A A, Y ASI SIGUE... (MATRIZ DE ADYACENCIA) LISTAR EN ORDEN ALFABETICO POR ORDEN DE LA ESTACION A

//QUERY 3: CANTIDAD DE VIAJES POR MES DE CADA ESTACION, LISTAR ALFABETICAMENTE

//QUERY 4: INICIO Y FIN MISMA ESTACION, ORDENAR ESTACIONES DE MAYOR A MENOR

