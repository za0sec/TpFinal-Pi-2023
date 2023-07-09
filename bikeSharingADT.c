#include "bikeSharingADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node * TList;

typedef struct node{
    size_t stationId;
    size_t memberTrips;
    char * stationName;
    struct node * tail;
}TNode;

typedef struct stationData{
    char * stationName; //TENEMOS QUE LEER EL OTRO ARCHIVO PARA CONSEGUIR EL NOMBRE DE LA ESTACION
    size_t memberTrips;
    size_t stationId;
    size_t used;
    int vecMonths[TOTAL_MONTHS]; //todavia no lo usamos, cuando lleguemos al query 3
}stationData;

typedef struct bikeSharingCDT{
    TList trips; //Usada              CHECK
    TList month;
    stationData * rankingStations;
    size_t dim; //Dimension de todas las stations sin usar y usadas.
    size_t realDim; //Dimension solo de las stations usadas.
}bikeSharingCDT;

bikeSharingADT newBikeSharing(){
    return calloc(1, sizeof(bikeSharingCDT));
}

void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember){
    if(isMember){
        if (bikesh->dim < station1Id){
            bikesh->rankingStations = realloc(bikesh->rankingStations, station1Id * sizeof(stationData)); // Agrego memoria si es que el station dado es menor a dim
        
            for(int i=bikesh->dim; i<station1Id; i++){
                bikesh->rankingStations[i].used = 0;
            }
            bikesh->dim = station1Id;
        }
        if (!bikesh->rankingStations[station1Id-1].used){
            bikesh->realDim++;
            bikesh->rankingStations[station1Id-1].used = 1;
            bikesh->rankingStations[station1Id-1].memberTrips = 0;
        }


        bikesh->rankingStations[station1Id-1].memberTrips++;

    }


} //al final de esta funcion, deberiamos tener todos los stationData ordenador por stationId en un vector



//Funcion que ordena el vector previamente asignado con valores. Ordena en base a los viajes realizados de mayor a menor (de miembros)
void tripSort(bikeSharingADT bikesh){
    
    qsort(bikesh->rankingStations, bikesh->real);

    rankingStations vec[bikesh->realDim];

    int k=0;
    for (int i=0; i<bikesh->dim; i++){
        if (bikesh->rankingStations[i].used){

            vec[k].stationName = malloc(strlen(bikesh->)+1);
            

        }

    }

}

void stringcpy(bikeSharingADT bikesh, char * from, size_t stationId, int * flag){
    
    bikesh->rankingStations[stationId-1].stationName = malloc(strlen(from)+1);
    if (bikesh->rankingStations[stationId-1].stationName == NULL){
        (*flag) = MEMERR;
        return;
    }
    
   strcpy(bikesh->rankingStations[stationId-1].stationName, from); 

}
