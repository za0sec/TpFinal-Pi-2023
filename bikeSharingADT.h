#ifndef __bikeSharingADT_h
#define __bikeSharingADT_h
#include <stdio.h>
#define MAXCHAR 150
#define TOTAL_MONTHS 12
#define CHUNK 20

#define SWAP(a,b) ()

typedef struct bikeSharingCDT * bikeSharingADT;

// Crea un ADT
bikeSharingADT newBikeSharing(void);

// Agrega un nodo a la lista
void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember);

bikeSharingADT addInfo(bikeSharingADT bikesh, char * startDate, size_t station1Id, char * endDate, size_t station2Id, size_t isMember, char * rideableType);

void tripSort(bikeSharingADT bikesh);

size_t getRealDim(bikeSharingADT bikesh);

size_t getMemberTrips(bikeSharingADT bikesh, int pos);

char * getStationName(bikeSharingADT bikesh, int pos);

void cpyStationName(bikeSharingADT bikesh, size_t stationId, char * from);

//Copia un string 
void stringcpy(bikeSharingADT bikesh, char * from, size_t stationId, int * flag);
    
void freeADT(bikeSharingADT bikesh);




enum errors{OK = 0, ARERR, OPENERR, CRERR, MEMERR, TOKERR};
/* Los errores se definen de la siguiente manera:
    ARERR = Error en la cantidad de argumentos.
    OPENERR = Error al abrir un archivo.
    CRERR = Error al crear el archivo.
    MEMERR = Error de creacion de memoria.
    TOKERR = Error de token nulo.
*/


#endif //__bikeSharingADT_h
