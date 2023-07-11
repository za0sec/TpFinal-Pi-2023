#ifndef __bikeSharingADT_h
#define __bikeSharingADT_h
#include <stdio.h>
#define MAXCHAR 150
#define TOTAL_MONTHS 12
#define CHUNK 20
//Estos define son para los strings estaticos que se usan en los querys y usamos numeros mas grandes de lo maximo que realmente puede pasar.
#define MONTHS_LENGTH 10
#define STATION_ID_LENGHT 10
#define TRIPS_LENGHT 10

#define MAX_SIZE(a,b) ( ((a) > (b)) ? (a) : (b) )

typedef struct bikeSharingCDT * bikeSharingADT;

// Crea un ADT
bikeSharingADT newBikeSharing(void);

// Agrega un nodo a la lista
void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember, char * startDate, size_t station2Id);

void tripSort(bikeSharingADT bikesh);

size_t getRealDim(bikeSharingADT bikesh);

size_t getDim(bikeSharingADT bikesh);

size_t getMemberTrips(bikeSharingADT bikesh, int pos);

char * getStationName(bikeSharingADT bikesh, int pos);

void addMatrix(bikeSharingADT bikesh, size_t station1Id, size_t station2Id, size_t * flagError);

bikeSharingADT stringcpy(bikeSharingADT bikesh, char * from, size_t stationId);
    
void freeADT(bikeSharingADT bikesh);

void sortAlpha(bikeSharingADT bikesh);

size_t getTripsAB(bikeSharingADT bikesh, int i, int j);

size_t getMonthTrip(bikeSharingADT bikesh, size_t stationId, size_t month);

void roundTripSort(bikeSharingADT bikesh);

size_t getRoundTrip(bikeSharingADT bikesh, int pos);

enum errors{OK = 0, ARERR, OPENERR, CRERR, MEMERR, TOKERR, NEXERR};
/* Los errores se definen de la siguiente manera:
    ARERR = Error en la cantidad de argumentos.
    OPENERR = Error al abrir un archivo.
    CRERR = Error al crear el archivo.
    MEMERR = Error de creacion de memoria.
    TOKERR = Error de token nulo.
    NEXERR = Error al chequear si hay un proximo en el iterador.
*/


#endif //__bikeSharingADT_h
