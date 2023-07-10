#include <stdio.h>
#include "bikeSharingADT.h"
#include "lib/htmlTable.h"
#include <string.h>
#include <stdlib.h>

void query1(bikeSharingADT bikesh);

bikeSharingADT readAddCsv(const char * filename);

void readName(bikeSharingADT bikesh, const char * filename);

FILE * newFile(const char * filename);

int main( int argc, char * argv[] ){

    if (argc != 3){
        fprintf(stderr, "Arguments should be 2");
        exit(ARERR);
    }

    bikeSharingADT bikesh = readAddCsv(argv[1]);
    readName(bikesh, argv[2]);
    tripSort(bikesh);

    query1(bikesh);

    freeADT(bikesh);

    return 0;
}

bikeSharingADT readAddCsv(const char * filename){

    FILE * file = fopen(filename, "rt");
        if(file == NULL){
            fprintf(stderr, "Error opening file %s\n", filename);
            exit(OPENERR);
        }

    bikeSharingADT bikesh = newBikeSharing();
    if( bikesh == NULL ){
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
        char* token = strtok(readText, ";");
        if (token != NULL) {
            startDate = malloc(strlen(token) + 1);
            if (startDate != NULL) {
                strcpy(startDate, token);
            } else {
                fprintf(stderr, "Memory Error");
                exit(MEMERR);
            }
        } else {
            fprintf(stderr, "Null Token Error");
            exit(TOKERR);
        }
        station1Id = atoi(strtok(NULL, ";"));
        strtok(NULL, ";");
        station2Id = atoi(strtok(NULL, ";")); 
        isMember = atoi(strtok(NULL, "\n"));

        addStation(bikesh, station1Id, isMember); //SOLO QUERY 1

        //vamos a tener q poner otra funcion  para los otros queries
    }

    free(startDate);

    fclose(file);
   
    return bikesh;
    
}

void readName(bikeSharingADT bikesh, const char * filename){
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
    int flag = 0;

// EL ERROR ESTA EN ESTA FUNCION AL TRATAR DE LEER ARCHIVOS Y ACCEDE A MEMORIA NO DEFINIDA!!!!!!!!!

    while( fgets(readText, MAXCHAR, file) != NULL ){
        stationId = atoi(strtok(readText, ";"));
        token = strtok(NULL, ";");
        if(token != NULL){
            stationName = malloc((strlen(token)+1)); // * sizeof(char)
            if(stationName != NULL){
                strcpy(stationName, token);
            }else{
                fprintf(stderr, "Memory Error");
                exit(MEMERR);
            }
        }else{
            fprintf(stderr, "Null Token Error");
            exit(TOKERR);
        }
        strtok(NULL, ";");//Salteo Latitud 
        strtok(NULL, "\n"); //y longitud.
        
        stringcpy(bikesh, stationName, stationId, &flag);
        if (flag == MEMERR){    
            fprintf(stderr, "Memory Error");
            exit(MEMERR);
        } //copia en nuestro vector ordenado por stationIds, el nombre de la estación.

        free(stationName);

    }

    fclose(file);

}

//QUERY 1: INICIO DE VIAJES DE MIEMBROS POR ESTACION ORDENADOS DE MAYOR A MENOR
void query1(bikeSharingADT bikesh){

    tripSort(bikesh);

    FILE * query1File = newFile("query1.csv");
    if(query1File==NULL){
        fprintf(stderr,"Error al crear archivo query1\n");
        exit(CRERR);
    }


    fputs("Station;StartedTrips\n",query1File);

    htmlTable table = newTable("Query1.html", 2, "Station", "StartedTrips");

    char aux[10];

    for(int i = 0; i < getRealDim(bikesh); i++) {
        sprintf(aux, "%ld", getMemberTrips(bikesh, i));
        fprintf(query1File, "%s;%s\n", getStationName(bikesh, i), aux);
        addHTMLRow(table, getStationName(bikesh, i), aux);
    }

    fclose(query1File);

    closeHTMLTable(table); 


//voy a tener que ordenar una copia del vector del ADT por orden alfabetico de las stations

}

FILE * newFile(const char * filename){
    FILE * new = fopen(filename, "wt");
    return new;
}


//QUERY 2: CANTIDAD DE VIAJES DE A A B Y DE B A A, Y ASI SIGUE... (MATRIZ DE ADYACENCIA) LISTAR EN ORDEN ALFABETICO POR ORDEN DE LA ESTACION A

//QUERY 3: CANTIDAD DE VIAJES POR MES DE CADA ESTACION, LISTAR ALFABETICAMENTE

//QUERY 4: INICIO Y FIN MISMA ESTACION, ORDENAR ESTACIONES DE MAYOR A MENOR

