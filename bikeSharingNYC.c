#include <stdio.h>
#include "bikeSharingADT.h"
#include "lib/htmlTable.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

    bikeSharingADT bikesh = readAddCsv(argv[1], yearFrom, yearTo);
    readName(bikesh, argv[2]);

    //query2(bikesh);
    query1(bikesh);
    query4(bikesh);
    query3(bikesh);

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
        fprintf(stderr, "Memory Error\n");
        exit(MEMERR);
    }
 
    char readText[MAXCHAR];
 
    fscanf(file, "%s\n", readText); /* salta la primer linea */
 
    // example
    // started_at;start_station_id;ended_at;end_station_id;rideable_type;member_casual
    // 2022-11-17 19:05:10.000000;489509;2022-11-17 19:07:30.000000;490309;classic_bike;member
    // 2022-11-13 13:42:35.000000;702804;2022-11-13 14:24:49.000000;808505;classic_bike;casual
 
    char * startDate;
    size_t station1Id;
    size_t station2Id;
    //char member[2];
    size_t isMember;
    size_t flagError = 0;
 
    while( fgets(readText, MAXCHAR, file) != NULL ){
        char* token = strtok(readText, ";");
        if (token != NULL) {
            startDate = malloc(strlen(token) + 1);
            if (startDate != NULL) {
                strcpy(startDate, token);
                printf("%s  ||  ", startDate);
            } else {
                fprintf(stderr, "Memory Error\n");
                exit(MEMERR);
            }
        } else {
            fprintf(stderr, "Null Token Error\n");
            exit(TOKERR);
        }
 
        station1Id = atoi(strtok(NULL, ";"));
        strtok(NULL, ";"); // ended at
        station2Id = atoi(strtok(NULL, ";")); 
        strtok(NULL, ";"); // rideable type
        isMember = strtok(NULL, "\n")[0] == 'm' ? 1 : 0; //Quiero solo el primer caracter. Si es una m quiere decir que es miembro y sino no.
 
        addStation(bikesh, station1Id, isMember, startDate, station2Id, yearFrom, yearTo);
      //  addMatrix(bikesh, station1Id, station2Id, &flagError);
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
 
// EL ERROR ESTA EN ESTA FUNCION AL TRATAR DE LEER ARCHIVOS Y ACCEDE A MEMORIA NO DEFINIDA!!!!!!!!!
 
    while( fgets(readText, MAXCHAR, file) != NULL ){
        token = strtok(readText, ";");
        if(token != NULL){
            stationName = malloc((strlen(token)+1)); // * sizeof(char)
            if(stationName != NULL){
                strcpy(stationName, token);
            }else{
                fprintf(stderr, "Memory Error\n");
                exit(MEMERR);
            }
        }else{
            fprintf(stderr, "Null Token Error\n");
            exit(TOKERR);
        }
        strtok(NULL, ";");//Salteo Latitud 
        strtok(NULL, ";"); //y longitud.
        stationId = atoi(strtok(NULL, "\n"));
 
        bikesh = stringcpy(bikesh, stationName, stationId);
 
        if (bikesh == NULL){ 
            fprintf(stderr, "Memory Error\n");
            exit(MEMERR);
        } //copia en nuestro vector ordenado por stationIds, el nombre de la estación.
 
        free(stationName);
 
    }
 
    fclose(file);
 
}

//QUERY 1: INICIO DE VIAJES DE MIEMBROS POR ESTACION ORDENADOS DE MAYOR A MENOR
void query1(bikeSharingADT bikesh){

    tripSort(bikesh);

    FILE * query1File = newFile("out/Query1NYC.csv");
    if(query1File==NULL){
        fprintf(stderr,"Error al crear archivo out/Query1NYC\n");
        exit(CRERR);
    }

    fputs("Station;StartedTrips\n",query1File);

    htmlTable table = newTable("out/Query1NYC.html", 2, "Station", "StartedTrips");

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

    FILE * query2File = newFile("out/Query2NYC.csv");
    if(query2File==NULL){
        fprintf(stderr,"Error al crear archivo Query2NYC\n");
        exit(CRERR);
    }

    fputs("StationA;StationB;Trips A->B; Trips B->A\n",query2File);

    htmlTable table = newTable("out/Query2.htmlNYC", 4, "StationA", "StationB", "Trips A->B", "Trips B->A");

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

    sortAlpha(bikesh);

    FILE * query3File = newFile("out/Query3NYC.csv");
    if(query3File==NULL){
        fprintf(stderr,"Error al crear archivo Query3NYC\n");
        exit(CRERR);
    }

    fputs("J;F;M;A;M;J;J;A;S;O;N;D;Station\n",query3File);

    htmlTable table = newTable("out/Query3NYC.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station");
    
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

    FILE * query4File = newFile("out/Query4NYC.csv");
    if(query4File==NULL){
        fprintf(stderr,"Error al crear archivo Query4NYC\n");
        exit(CRERR);
    }

    fputs("Station;RoundingTrips\n",query4File);

    htmlTable table = newTable("out/Query4NYC.html", 2, "Station", "RoundingTrips");

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


//QUERY 2: CANTIDAD DE VIAJES DE A A B Y DE B A A, Y ASI SIGUE... (MATRIZ DE ADYACENCIA) LISTAR EN ORDEN ALFABETICO POR ORDEN DE LA ESTACION A

//QUERY 3: CANTIDAD DE VIAJES POR MES DE CADA ESTACION, LISTAR ALFABETICAMENTE

//QUERY 4: INICIO Y FIN MISMA ESTACION, ORDENAR ESTACIONES DE MAYOR A MENOR

