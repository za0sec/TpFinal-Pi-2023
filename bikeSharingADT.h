#ifndef __bikeSharingADT_h
#define __bikeSharingADT_h

#include <stdio.h>
#define MAXCHAR 150
#define TOTAL_MONTHS 12
#define CHUNK 20

//Funcion compare para tener un criterio de comparacion.
int compare(stationData station1, stationData station2){
    //Me fijo si el primero tiene menor cantidad de viajes o si tienen igual cantidad y son distintos lexicograficamente.
    if (station1->memberTrips < station2.memberTrips || (station1->memberTrips == station2.memberTrips && strcmp(station1->stationName, station2.stationName) > 0)){
        return 1;
    }

    return 0;

}
typedef struct bikeSharingCDT * bikeSharingADT;

// Crea un ADT
bikeSharingADT newBikeSharing();

// Agrega un nodo a la lista
void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember);

bikeSharingADT addInfo(bikeSharingADT bikesh, char * startDate, size_t station1Id, char * endDate, size_t station2Id, size_t isMember, char * rideableType);

void tripSort(bikeSharingADT bikesh);

size_t getRealDim(bikeSharingADT bikesh);

size_t getMemberTrips(bikeSharingADT bikesh, int pos);

char * getStationName(bikeSharingADT bikesh, int pos);


//Copia un string 
void stringcpy(bikeSharingADT bikesh, char * from, size_t stationId, int * flag);
    





enum errors{OK = 0, ARERR, OPENERR, CRERR, MEMERR, TOKERR};
/* Los errores se definen de la siguiente manera:
    ARERR = Error en la cantidad de argumentos.
    OPENERR = Error al abrir un archivo.
    CRERR = Error al crear el archivo.
    MEMERR = Error de creacion de memoria.
    TOKERR = Error de token nulo.
*/


#endif //__bikeSharingADT_h
