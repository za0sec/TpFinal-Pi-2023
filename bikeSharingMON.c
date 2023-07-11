#include <stdio.h>
#include "bikeSharingADT.h"
#include "lib/htmlTable.h"
#include <string.h>
#include <stdlib.h>

void query1(bikeSharingADT bikesh);

void query2(bikeSharingADT bikesh);

void query3(bikeSharingADT bikesh);

void query4(bikeSharingADT bikesh);

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

    query1(bikesh);
    query2(bikesh);
    query3(bikesh);

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
    size_t flagError = 0;

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

        addStation(bikesh, station1Id, isMember, startDate, station2Id);
        addMatrix(bikesh, station1Id, station2Id, &flagError);
        if (flagError == MEMERR){    
            fprintf(stderr, "Memory Error");
            exit(MEMERR);
        }

        free(startDate);

        //vamos a tener q poner otra funcion  para los otros queries
    }


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

    FILE * query1File = newFile("Query1.csv");
    if(query1File==NULL){
        fprintf(stderr,"Error al crear archivo Query1\n");
        exit(CRERR);
    }

    fputs("Station;StartedTrips\n",query1File);

    htmlTable table = newTable("Query1.html", 2, "Station", "StartedTrips");

    char aux[STATION_ID_LENGHT];

    for(int i = 0; i < getRealDim(bikesh); i++) {
        sprintf(aux, "%ld", getMemberTrips(bikesh, i));
        char * stationName = getStationName(bikesh, i);
        fprintf(query1File, "%s;%s\n", stationName, aux);
        addHTMLRow(table, stationName, aux);
        free(stationName); // libera la memoria
    }

    fclose(query1File);

    closeHTMLTable(table); 


//voy a tener que ordenar una copia del vector del ADT por orden alfabetico de las stations

}

void query2(bikeSharingADT bikesh){

    sortAlpha(bikesh);

    FILE * query2File = newFile("Query2.csv");
    if(query2File==NULL){
        fprintf(stderr,"Error al crear archivo Query2\n");
        exit(CRERR);
    }

    fputs("StationA;StationB;Trips A->B; Trips B->A\n",query2File);

    htmlTable table = newTable("Query2.html", 4, "StationA", "StationB", "Trips A->B", "Trips B->A");

    char ab[TRIPS_LENGHT];
    char ba[TRIPS_LENGHT];

    for(int i = 0; i < getRealDim(bikesh); i++) {
        char * station1Name = getStationName(bikesh, i);
        for(int j = i+1; j < getRealDim(bikesh); j++){
            char * station2Name = getStationName(bikesh, j);
            sprintf(ab, "%ld", getTripsAB(bikesh, i, j));
            sprintf(ba, "%ld", getTripsAB(bikesh, j, i));
            if (ab[0] != '0' && ba[0] != '0'){
                fprintf(query2File, "%s;%s;%s;%s\n", station1Name, station2Name, ab, ba);
                addHTMLRow(table, station1Name, station2Name, ab, ba);
            }
            free(station2Name);
        }
        free(station1Name);
    }

}
                                    // EN LOS MESES 1 2 3 y 12 NO HAY VIAJES
void query3(bikeSharingADT bikesh){  // ESTA IMPRIMIENDO NUMEROS NEGATIVOS

    FILE * query3File = newFile("Query3.csv");
    if(query3File==NULL){
        fprintf(stderr,"Error al crear archivo Query3\n");
        exit(CRERR);
    }

    fputs("J;F;M;A;M;J;J;A;S;O;N;D;Station\n",query3File);

    htmlTable table = newTable("Query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    
    char m1[MONTHS_LENGTH];
    char m2[MONTHS_LENGTH];
    char m3[MONTHS_LENGTH];
    char m4[MONTHS_LENGTH];
    char m5[MONTHS_LENGTH];
    char m6[MONTHS_LENGTH];
    char m7[MONTHS_LENGTH];
    char m8[MONTHS_LENGTH];
    char m9[MONTHS_LENGTH];
    char m10[MONTHS_LENGTH];
    char m11[MONTHS_LENGTH];
    char m12[MONTHS_LENGTH];

    for(int i = 0 ; i < getRealDim(bikesh); i++){
        char * stationName = getStationName(bikesh, i);
        sprintf(m1, "%ld", getMonthTrip(bikesh, i, 0));
        sprintf(m2, "%ld", getMonthTrip(bikesh, i, 1));
        sprintf(m3, "%ld", getMonthTrip(bikesh, i, 2));
        sprintf(m4, "%ld", getMonthTrip(bikesh, i, 3));
        sprintf(m5, "%ld", getMonthTrip(bikesh, i, 4));
        sprintf(m6, "%ld", getMonthTrip(bikesh, i, 5));
        sprintf(m7, "%ld", getMonthTrip(bikesh, i, 6));
        sprintf(m8, "%ld", getMonthTrip(bikesh, i, 7));
        sprintf(m9, "%ld", getMonthTrip(bikesh, i, 8));
        sprintf(m10, "%ld", getMonthTrip(bikesh, i, 9));
        sprintf(m11, "%ld", getMonthTrip(bikesh, i, 10));
        sprintf(m12, "%ld", getMonthTrip(bikesh, i, 11)); //a mejorar
        fprintf(query3File, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, stationName);
        addHTMLRow(table, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, stationName);
        free(stationName);
    }
}

void query4(bikeSharingADT bikesh){

    FILE * query4File = newFile("Query4.csv");
    if(query4File==NULL){
        fprintf(stderr,"Error al crear archivo Query4\n");
        exit(CRERR);
    }

    fputs("Station;RoundingTrips\n",query4File);

    htmlTable table = newTable("Query4.html", 2, "Station", "RoundingTrips");

    char roundTrip[TRIPS_LENGHT];

    for(int i = 0 ; i < getRealDim(bikesh) ; i++){
        char * stationName = getStationName(bikesh, i);
        sprint(roundTrip, "%ld", getRoundTrip(bikesh, i));
        fprintf(query4File, "%s;%s\n", stationName, roundTrip);
        addHTMLRow(table, stationName, roundTrip);
        free(stationName);
    }

    

}

FILE * newFile(const char * filename){
    FILE * new = fopen(filename, "wt");
    return new;
}


//QUERY 2: CANTIDAD DE VIAJES DE A A B Y DE B A A, Y ASI SIGUE... (MATRIZ DE ADYACENCIA) LISTAR EN ORDEN ALFABETICO POR ORDEN DE LA ESTACION A

//QUERY 3: CANTIDAD DE VIAJES POR MES DE CADA ESTACION, LISTAR ALFABETICAMENTE

//QUERY 4: INICIO Y FIN MISMA ESTACION, ORDENAR ESTACIONES DE MAYOR A MENOR

