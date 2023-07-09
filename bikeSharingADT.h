#include <stdio.h>
#define MAXCHAR 150
#define TOTAL_MONTHS 12


typedef struct bikeSharingCDT * bikeSharingADT;

// Crea un ADT
bikeSharingADT newBikeSharing();

// Agrega un nodo a la lista
void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember);

bikeSharingADT addInfo(bikeSharingADT bikesh, char * startDate, size_t station1Id, char * endDate, size_t station2Id, size_t isMember, char * rideableType);

enum errors{OK = 0, ARERR, OPENERR, CRERR, MEMERR, TOKERR};
/* Los errores se definen de la siguiente manera:
    ARERR = Error en la cantidad de argumentos.
    OPENERR = Error al abrir un archivo.
    CRERR = Error al crear el archivo.
    MEMERR = Error de creacion de memoria.
    TOKERR = Error de token nulo.
*/



