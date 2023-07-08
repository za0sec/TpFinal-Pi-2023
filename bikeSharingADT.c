#include "bikeSharing.h"

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
    size_t stationId;
    size_t trips;
}stationData;

typedef struct bikeSharingCDT{
    TList first;
    struct stationData * rankingStations;
}bikeSharingCDT;
 // LLEGAMOS HASTA ACA
bikeSharingADT newBikeSharing(){
    return calloc(1, sizeof(bikeSharingCDT));
}
