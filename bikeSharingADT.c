#include "bikeSharingADT.h"

/*typedef struct node * TList;

typedef struct node{
    char * startDate;
    size_t station1Id; //id de la estación de inicio
    char * endDate;
    size_t station2Id; //id de la estacion de fin
    size_t isMember; //0 si no es miembro, sino 1
    char * rideableType; //suponiendo que usamos el mismo ADT para las 2 ciudades
    struct node * tail;
}TNode;
*/
typedef struct stationData{
    char * stationName; //TENEMOS QUE LEER EL OTRO ARCHIVO PARA CONSEGUIR EL NOMBRE DE LA ESTACION
    size_t stationId;
    size_t memberTrips;
}stationData;

typedef struct bikeSharingCDT{
    //TList first;
    stationData * rankingStations;
    size_t dim;
}bikeSharingCDT;

bikeSharingADT newBikeSharing(){
    return calloc(1, sizeof(bikeSharingCDT));
}

static int addRec(stationData * rankingStations, size_t station1Id){//SI DEVUELVO 0, LA ESTACION NO EXISTIA, SI DEVUELVO 1 ES QUE EXISTIA Y LE AGREGUE UN VIAJE
    for(int i=0 ; i<dim ; i++){
        if(rankingStations[i].stationId == station1Id){
            rankingStations[i].memberTrips += 1;
            return 1;
        }
    }
    return 0;
}

void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember){
    if(isMember){
        int aux = addRec(bikesh->rankingStations, station1Id);

    }
}