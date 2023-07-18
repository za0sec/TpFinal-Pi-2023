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
void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember, char * startDate, size_t station2Id, size_t yearFrom, size_t yearTo);

//Ordena el vector con un criterio.
void tripSort(bikeSharingADT bikesh);

//Obtengo la dimension real.
size_t getRealDim(bikeSharingADT bikesh);

//Obtengo los viajes de miembros.
size_t getMemberTrips(bikeSharingADT bikesh, int pos);

//Obtengo el nombre de la estacion en una posicion.
char * getStationName(bikeSharingADT bikesh, int pos);

//CArga los viajes a la matriz de adyacencia.
void addMatrix(bikeSharingADT bikesh, size_t station1Id, size_t station2Id, size_t * flagError);

//Copia el nombre de la estacion al adt.
bikeSharingADT stringcpy(bikeSharingADT bikesh, char * from, size_t stationId);
    
//libera todos los recursos utilizados por el TAD.
void freeADT(bikeSharingADT bikesh);

// ORdena el vector alfabeticamente.
void sortAlpha(bikeSharingADT bikesh);

//Obtiene los viajes de A a B
size_t getTripsAB(bikeSharingADT bikesh, int i, int j);

//Obtengo los viajes en un mes en especifico.
size_t getMonthTrip(bikeSharingADT bikesh, size_t stationId, size_t month);

//Ordena el vector en base a los round trips.
void roundTripSort(bikeSharingADT bikesh);

//Obtengo los round trips en una estacion.
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
