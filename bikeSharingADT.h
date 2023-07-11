#ifndef __bikeSharingADT_h
#define __bikeSharingADT_h
#include <stdio.h>
#define MAXCHAR 150
#define TOTAL_MONTHS 12
#define MONTHS_LENGTH 21 // a saber porque
#define CHUNK 20
#define STATION_ID_LENGHT 10
#define TRIPS_LENGHT 10

#define MAX_SIZE(a,b) ( ((a) > (b)) ? (a) : (b) )

typedef struct bikeSharingCDT * bikeSharingADT;

// Crea un ADT
bikeSharingADT newBikeSharing(void);

// Agrega un nodo a la lista
void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember, char * startDate);

void tripSort(bikeSharingADT bikesh);

size_t getRealDim(bikeSharingADT bikesh);

size_t getDim(bikeSharingADT bikesh);

size_t getMemberTrips(bikeSharingADT bikesh, int pos);

char * getStationName(bikeSharingADT bikesh, int pos);

void addMatrix(bikeSharingADT bikesh, size_t station1Id, size_t station2Id, size_t * flagError);

//Copia un string 
void stringcpy(bikeSharingADT bikesh, char * from, size_t stationId, int * flag);
    
void freeADT(bikeSharingADT bikesh);

void sortAlpha(bikeSharingADT bikesh);

size_t getTripsAB(bikeSharingADT bikesh, int i, int j);

size_t getMonthTrip(bikeSharingADT bikesh, size_t stationId, int month);

enum errors{OK = 0, ARERR, OPENERR, CRERR, MEMERR, TOKERR};
/* Los errores se definen de la siguiente manera:
    ARERR = Error en la cantidad de argumentos.
    OPENERR = Error al abrir un archivo.
    CRERR = Error al crear el archivo.
    MEMERR = Error de creacion de memoria.
    TOKERR = Error de token nulo.
*/


#endif //__bikeSharingADT_h
