#include <stdio.h>
#include "bikeSharingADT.h"
#include "lib/htmlTable.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void query1(bikeSharingADT bikesh);

void query2(bikeSharingADT bikesh);

void query3(bikeSharingADT bikesh);

void query4(bikeSharingADT bikesh);

bikeSharingADT readAddCsv(const char * filename, size_t yearFrom, size_t yearTo);

void readName(bikeSharingADT bikesh, const char * filename);

FILE * newFile(const char * filename);

int main( int argc, char * argv[] ){

    size_t yearFrom=0, yearTo=0;

    if (argc > 5 || argc < 3){
        fprintf(stderr, "Invalid arguments\n");
        exit(ARERR);
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year+1900;

    if( argc == 3 ){
        yearFrom = 0;
        yearTo = year;
    }else if( argc == 4 ){
        yearFrom = atoi(argv[3]);
        yearTo = year;
    }else if( argc == 5 ){
        yearFrom = atoi(argv[3]);

        yearTo = atoi(argv[4]);
    }
    
    if( yearFrom > yearTo ){
        fprintf(stderr, "Invalid year arguments\n");
        exit(ARERR);
    }

    bikeSharingADT bikesh;
    
    if (strcmp(argv[1], argv[2]) > 0){
        bikesh = readAddCsv(argv[2], yearFrom, yearTo);
        readName(bikesh, argv[1]);
    }else if (strcmp(argv[1], argv[2]) < 0){
        bikesh = readAddCsv(argv[1], yearFrom, yearTo);
        readName(bikesh, argv[2]);
    }else{
        fprintf(stderr, "Invalid arguments order\n");
        exit(ARERR);
    }

    query1(bikesh);
    query2(bikesh);
    query3(bikesh);
    query4(bikesh);

    freeADT(bikesh);

    return 0;
}

bikeSharingADT readAddCsv(const char * filename, size_t yearFrom, size_t yearTo){

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

        addStation(bikesh, station1Id, isMember, startDate, station2Id, yearFrom, yearTo);
        addMatrix(bikesh, station1Id, station2Id, &flagError);
        if (flagError == MEMERR){    
            fprintf(stderr, "Memory Error");
            exit(MEMERR);
        }

        free(startDate);

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
        
        bikesh = stringcpy(bikesh, stationName, stationId); //Leakeo de memoria y no podemos ver donde...
        
        if (bikesh == NULL){ 
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

    FILE * query1File = newFile("out/Query1MON.csv");
    if(query1File==NULL){
        fprintf(stderr,"Error al crear archivo out/Query1MON\n");
        exit(CRERR);
    }

    fputs("Station;StartedTrips\n",query1File);

    htmlTable table = newTable("out/Query1MON.html", 2, "Station", "StartedTrips");

    char aux[STATION_ID_LENGHT];

    for(int i = 0; i < getRealDim(bikesh); i++) {
        sprintf(aux, "%ld", getMemberTrips(bikesh, i));
        char * stationName = getStationName(bikesh, i);
        fprintf(query1File, "%s;%s\n", stationName, aux);
        addHTMLRow(table, stationName, aux);
        free(stationName);
    }

    fclose(query1File);

    closeHTMLTable(table); 

}

void query2(bikeSharingADT bikesh){

    sortAlpha(bikesh);

    FILE * query2File = newFile("out/Query2MON.csv");
    if(query2File==NULL){
        fprintf(stderr,"Error al crear archivo Query2MON\n");
        exit(CRERR);
    }

    fputs("StationA;StationB;Trips A->B; Trips B->A\n",query2File);

    htmlTable table = newTable("out/Query2MON.html", 4, "StationA", "StationB", "Trips A->B", "Trips B->A");

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

    fclose(query2File);

    closeHTMLTable(table);

}

void query3(bikeSharingADT bikesh){

    FILE * query3File = newFile("out/Query3MON.csv");
    if(query3File==NULL){
        fprintf(stderr,"Error al crear archivo Query3MON\n");
        exit(CRERR);
    }

    fputs("J;F;M;A;M;J;J;A;S;O;N;D;Station\n",query3File);

    htmlTable table = newTable("out/Query3MON.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    
    char monthStrings[TOTAL_MONTHS][MONTHS_LENGTH];

    for(int i = 0 ; i < getRealDim(bikesh); i++){
        char * stationName = getStationName(bikesh, i);
        for(int j = 0; j < TOTAL_MONTHS; j++) {
            sprintf(monthStrings[j], "%ld", getMonthTrip(bikesh, i, j));
        }

        fprintf(query3File, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
            monthStrings[0], monthStrings[1], monthStrings[2], monthStrings[3], monthStrings[4], 
            monthStrings[5], monthStrings[6], monthStrings[7], monthStrings[8], monthStrings[9], 
            monthStrings[10], monthStrings[11], stationName);

        addHTMLRow(table, monthStrings[0], monthStrings[1], monthStrings[2], monthStrings[3], monthStrings[4], 
               monthStrings[5], monthStrings[6], monthStrings[7], monthStrings[8], monthStrings[9], 
               monthStrings[10], monthStrings[11], stationName);

        free(stationName);
    }

    fclose(query3File);

    closeHTMLTable(table);


}

void query4(bikeSharingADT bikesh){

    roundTripSort(bikesh);

    FILE * query4File = newFile("out/Query4MON.csv");
    if(query4File==NULL){
        fprintf(stderr,"Error al crear archivo Query4MON\n");
        exit(CRERR);
    }

    fputs("Station;RoundingTrips\n",query4File);

    htmlTable table = newTable("out/Query4MON.html", 2, "Station", "RoundingTrips");

    char roundTrip[TRIPS_LENGHT];

    for(int i = 0 ; i < getRealDim(bikesh) ; i++){
        char * stationName = getStationName(bikesh, i);
        sprintf(roundTrip, "%ld", getRoundTrip(bikesh, i));
        fprintf(query4File, "%s;%s\n", stationName, roundTrip);
        addHTMLRow(table, stationName, roundTrip);
        free(stationName);
    }

    fclose(query4File);

    closeHTMLTable(table);

}

FILE * newFile(const char * filename){
    FILE * new = fopen(filename, "wt");
    return new;
}

