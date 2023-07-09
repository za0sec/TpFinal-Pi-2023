#include "bikeSharingADT.h"



typedef struct node * TList;

typedef struct node{
    char * startDate;
    size_t station1Id; //id de la estación de inicio
    char * endDate;
    size_t station2Id; //id de la estacion de fin
    size_t isMember; //0 si no es miembro, sino 1
    char * rideableType; //suponiendo que usamos el mismo ADT para las 2 ciudades
    struct node * tail;
}TNode;

typedef struct stationData{
    char * stationName; //TENEMOS QUE LEER EL OTRO ARCHIVO PARA CONSEGUIR EL NOMBRE DE LA ESTACION
    size_t memberTrips;
    size_t used;
    int vecMonths[TOTAL_MONTHS]; //todavia no lo usamos, cuando lleguemos al query 3
}stationData;

typedef struct bikeSharingCDT{
    TList first; //quizas no lo vayamos a usar
    stationData * rankingStations;
    size_t dim;
}bikeSharingCDT;

bikeSharingADT newBikeSharing(){
    return calloc(1, sizeof(bikeSharingCDT));
}

void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember){
    if(isMember){
        if (dim < station1Id){
            bikesh->rankingStation = realloc(bikesh->rankingStation, station1Id * sizeof(stationData)); // Agrego memoria si es que el station dado es menor a dim
        
            for(int i=dim; i<station1Id-1; i++){ // Voy hasta station1Id-1 porque ya voy a asignarle used a esta parte
                bikesh->rankingStation[i].used = 0;
            }
            bikesh->dim = station1Id;

            bikesh->rankingStation[station1Id-1].used = 1;
            bikesh->rankingStation[station1Id-1].memberTrips = 0;
        }
        if (!bikesh->rankingStation[station1Id-1].used){
            bikesh->rankingStation[station1Id-1].used = 1;
            bikesh->rankingStation[station1Id-1].memberTrips = 0;
        }

        bikesh->rankingStation[station1Id-1].memberTrips++;

    }

} //al final de esta funcion, deberiamos tener todos los stationData ordenador por stationId en un vector

void TripSort(stationData * vec1, stationData * vecOriginal, ){
    
}


bikeSharingADT addData(bikeSharingADT bikesh, char * startDate, size_t station1Id, size_t station2Id, size_t isMember){

}
