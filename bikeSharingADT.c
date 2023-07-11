#include "bikeSharingADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct stationData{
    char * stationName;
    size_t memberTrips;
    size_t stationId; // Id de estacion de salida
    size_t used;
    size_t vecMonths[TOTAL_MONTHS];
    size_t roundTrips;
}stationData;

typedef struct bikeSharingCDT{
    stationData * rankingStations;
    size_t dim; //Dimension de todas las stations sin usar y usadas. ---- Para NYC es la dimension de la lista.
    size_t realDim; //Dimension solo de las stations usadas. ---- PARA NYC no existe.
    size_t ** mat; // Matriz de adyacencia
    size_t dimMat; // Filas y columnas de la matriz ( es cuadrada ) 
}bikeSharingCDT;


/* -----------------------------------------------------------------------BikesharingMON-------------------------------------------------------------------------------- */

static int my_strcasecmp(const char *s1, const char *s2)
{
    while(*s1 && (toupper((unsigned char)*s1) == toupper((unsigned char)*s2)))
        s1++, s2++;
    return toupper((unsigned char)*s1) - toupper((unsigned char)*s2);
}

static int compare_stationData(const void *a, const void *b) {
    stationData *dataA = (stationData *)a;
    stationData *dataB = (stationData *)b;

    return my_strcasecmp(dataA->stationName, dataB->stationName); // Si 'name' es el campo que deseas comparar.
}

//Funcion compare para tener un criterio de comparacion.
static int compare(const void *a, const void *b){
    stationData *station1 = (stationData *)a;
    stationData *station2 = (stationData *)b;

    int cmp = 0;

    //Me fijo si el primero tiene menor cantidad de viajes o si tienen igual cantidad y son distintos lexicograficamente.
    if (station1->memberTrips < station2->memberTrips){
        cmp = 1;
    } else if (station1->memberTrips > station2->memberTrips){
        cmp = -1;
    }

    if (!cmp)
        cmp = my_strcasecmp(station1->stationName, station2->stationName);
    
    return cmp;
}

static int compareRoundTrips(const void *a, const void *b){
    stationData *station1 = (stationData *)a;
    stationData *station2 = (stationData *)b;

    int cmp = 0;

    if(station1->roundTrips < station2->roundTrips){
        cmp = 1;
    } else if (station1->roundTrips > station2->roundTrips){
        cmp = -1;
    }

    if(!cmp)
        cmp = my_strcasecmp(station1->stationName, station2->stationName);
    
    return cmp;

}


static int getMonth(const char * startDate){
   
    char monthStr[3];

    strncpy(monthStr, &startDate[5], 2);
    monthStr[2] = '\0';

    int month = atoi(monthStr);

    return month;
}

void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember, char * startDate, size_t station2Id){
    if (bikesh->dim < station1Id){
        bikesh->rankingStations = realloc(bikesh->rankingStations, station1Id * sizeof(stationData)); // Agrego memoria si es que el station dado es menor a dim
    
        for(int i=bikesh->dim; i<station1Id; i++){
            bikesh->rankingStations[i].memberTrips = 0;
            bikesh->rankingStations[i].stationName = NULL;
            bikesh->rankingStations[i].stationId = 0;
            bikesh->rankingStations[i].used = 0;
            bikesh->rankingStations[i].roundTrips = 0;
            for(int j = 0; j < TOTAL_MONTHS; j++){
                bikesh->rankingStations[i].vecMonths[j] = 0;
            }

        }
        bikesh->dim = station1Id;
    }

    if (!bikesh->rankingStations[station1Id-1].used){
        bikesh->realDim++;
        bikesh->rankingStations[station1Id-1].used = 1;
    }

    if(isMember)
        bikesh->rankingStations[station1Id-1].memberTrips++;
    
    bikesh->rankingStations[station1Id-1].vecMonths[getMonth(startDate)-1]++;

    if(station1Id == station2Id)
        bikesh->rankingStations[station1Id-1].roundTrips++;
}

static char * copyStr(const char * s){
    char * copy = malloc(strlen(s) + 1);
    if(copy == NULL) {
        return NULL; // or handle the error appropriately
    }
    return strcpy(copy, s);
}

size_t getDim(bikeSharingADT bikesh){
    return bikesh->dim;
}

size_t getRealDim(bikeSharingADT bikesh){
    return bikesh->realDim;
}

size_t getMemberTrips(bikeSharingADT bikesh, int pos){
    return bikesh->rankingStations[pos].memberTrips;
}

char * getStationName(bikeSharingADT bikesh, int pos){
    return copyStr(bikesh->rankingStations[pos].stationName);
}

bikeSharingADT stringcpy(bikeSharingADT bikesh, char * from, size_t stationId){

    bikesh->rankingStations[stationId-1].stationName = realloc(bikesh->rankingStations[stationId-1].stationName, (strlen(from)+1) * sizeof(char));
    if (bikesh->rankingStations[stationId-1].stationName == NULL){
        return NULL;
    }
    
    strcpy(bikesh->rankingStations[stationId-1].stationName, from); 

    return bikesh;

}

void addMatrix(bikeSharingADT bikesh, size_t station1Id, size_t station2Id, size_t * flagError){ // Crea la matriz de adyacencia

    size_t size = MAX_SIZE(station1Id, station2Id);

    if (bikesh->dimMat < size){
        // Agrego memoria para las filas
        bikesh->mat = realloc(bikesh->mat, size * sizeof(size_t*));
        if (bikesh->mat == NULL) {
            (*flagError) = MEMERR;
            return;
        }

        // Para cada fila agrego memoria a las columnas
        for (size_t i = 0; i < size; i++) {
            if (i >= bikesh->dimMat) {
                bikesh->mat[i] = NULL;  // Asegúrate de que bikesh->mat[i] esté inicializado
            }
            bikesh->mat[i] = realloc(bikesh->mat[i], size * sizeof(size_t));
            if (bikesh->mat[i] == NULL) {
                (*flagError) = MEMERR;
                return;
            }
        }

        // Inicializamos una parte de la matriz con 0
        for (size_t i = 0; i < bikesh->dimMat; i++) {
            for (size_t j = bikesh->dimMat; j < size; j++) {
                bikesh->mat[i][j] = 0;
            }
        }

        for (size_t i = bikesh->dimMat; i < size; i++){
            for (size_t j = 0; j < size; j++){
                bikesh->mat[i][j] = 0;
            }
        }
        bikesh->dimMat = size; // Actualizamos el tamano de la matriz
    }

    // Llenamos la matriz con los datos
    bikesh->mat[station1Id-1][station2Id-1]++;

}

// Ordena la matriz por orden alfabetico
void sortAlpha(bikeSharingADT bikesh){

    qsort(bikesh->rankingStations, bikesh->realDim, sizeof(stationData), compare_stationData);

}

// Devuelve los viajes de A a B
size_t getTripsAB(bikeSharingADT bikesh, int i, int j){
    return bikesh->mat[i][j];
}

size_t getMonthTrip(bikeSharingADT bikesh, size_t stationId, size_t month){
    return bikesh->rankingStations[stationId].vecMonths[month];
}

void roundTripSort(bikeSharingADT bikesh){

    qsort(bikesh->rankingStations, bikesh->realDim, sizeof(stationData), compareRoundTrips);

}

size_t getRoundTrip(bikeSharingADT bikesh, int pos){
    return bikesh->rankingStations[pos].roundTrips;
}

void freeADT(bikeSharingADT bikesh){

    if (bikesh != NULL) {
        if (bikesh->rankingStations != NULL) {
            for (int i = 0; i < bikesh->dimMat; i++){
                free(bikesh->mat[i]);
            }
            free(bikesh->mat);
            for (int i = 0; i < bikesh->realDim; i++) {
                if (bikesh->rankingStations[i].stationName != NULL) {
                    free(bikesh->rankingStations[i].stationName);
                }
            }
            free(bikesh->rankingStations);
        }
        free(bikesh);
    }


}


/*----------------------------------------------------------------------- Funciones que comparten -----------------------------------------------------------------------*/

bikeSharingADT newBikeSharing(void){
    return calloc(1, sizeof(bikeSharingCDT));
}

//Funcion que ordena el vector previamente asignado con valores. Ordena en base a los viajes realizados de mayor a menor (de miembros)
void tripSort(bikeSharingADT bikesh){

    int k=0;
    for (size_t i=0; i<bikesh->dim; i++){

        if (bikesh->rankingStations[i].used){
            free(bikesh->rankingStations[k].stationName);  // liberar la cadena original
            bikesh->rankingStations[k].stationName = copyStr(bikesh->rankingStations[i].stationName);
            bikesh->rankingStations[k].memberTrips = bikesh->rankingStations[i].memberTrips;
            bikesh->rankingStations[k++].stationId = i+1;
        }    
    }
    
    bikesh->rankingStations = realloc(bikesh->rankingStations, k * sizeof(stationData)); //Con este realloc eliminamos del vector todas las estaciones que tengan el used en 0.
    
    qsort(bikesh->rankingStations, bikesh->realDim, sizeof(stationData), compare);

}

/* -----------------------------------------------------------------------BikesharingNYC-------------------------------------------------------------------------------- */
// static TList addDataRec(TList list, size_t station1Id, size_t isMember, char * startDate, size_t station2Id){
//     if(list == NULL || list->elem.stationId > station1Id){
//         TList aux = malloc(sizeof(TNode));
//         aux->elem.stationId = station1Id;
//         aux->elem.stationName = NULL;
//         // aux->elem.roundTrips = (station1Id == station2Id) ? 1 : 0;
//         aux->elem.memberTrips = isMember ? 1 : 0;
//         for(int i = 0; i < TOTAL_MONTHS; i++){
//             aux->elem.vecMonths[i] = 0;
//         }
//         return aux;
//     }
//     if(list->elem.stationId == station1Id){
//         // list->elem.roundTrips += (station1Id == station2Id) ? 1 : 0; 
//         list->elem.memberTrips += isMember ? 1 : 0;
//         return list;
//     }
//     list->tail = addDataRec(list->tail, station1Id, isMember, startDate, station2Id);
//     return list;
// } //cargamos toda la data de todos los viajes en una lista ordenada por stationsIds.

// bikeSharingADT addData(bikeSharingADT bikesh, size_t station1Id, size_t isMember, char * startDate, size_t station2Id){
//     bikesh->first = addDataRec(bikesh->first, station1Id, isMember, startDate, station2Id);
//     return bikesh;
// }

// static TList tripSortRec(bikeSharingADT bikesh){

// }

// bikeSharingADT tripSortNYC(bikeSharingADT bikesh){
//     bikesh->first = tripSortRec(bikesh->first);
//     return bikesh;
// }

// void toBegin(bikeSharingADT bikesh){
//     bikesh->iterator = bikesh->first;
// }

// size_t hasNext(bikeSharingADT bikesh){
//     return bikesh->iterator != NULL;
// }

// element next(bikeSharingADT bikesh){
//    if (hasNext(bikesh)){
//         element aux = bikesh->iterator->elem;
//         bikesh->iterator = bikesh->iterator->tail;
//         return aux;
//     }
//     exit(NEXERR);
// }

// void freeListRec(TList list){

// }

char * stationName;
    size_t memberTrips;
    size_t stationId; // Id de estacion de salida
    // size_t used;
    size_t vecMonths[TOTAL_MONTHS];
    size_t roundTrips;

void addStationNYC(bikeSharingADT bikesh, size_t station1Id, size_t isMember, char * startDate, size_t station2Id){
  
    if (!(bikesh->realDim % CHUNK)){

        bikesh->rankingStations = realloc(bikesh->rankingStations, ((bikesh->realDim)+CHUNK) * sizeof(stationData)); // Agrego memoria si es que el station dado es menor a dim
      
        for(int i=bikesh->realDim; i<(bikesh->realDim + CHUNK); i++){
            bikesh->rankingStations[i].memberTrips = 0;
            bikesh->rankingStations[i].stationName = NULL;
            bikesh->rankingStations[i].stationId = 0;
            bikesh->rankingStations[i].used = 0;
            bikesh->rankingStations[i].roundTrips = 0;
            for(int j = 0; j < TOTAL_MONTHS; j++){
                bikesh->rankingStations[i].vecMonths[j] = 0;
            }
        }
    }

    bikesh->realDim++;
    bikesh->dim = bikesh->realDim;
    bikesh->rankingStations[bikesh->realDim - 1].stationId = station1Id;
    bikesh->rankingStations[bikesh->realDim - 1].used = 1;
    for(int j = 0; j < bikesh->realDim; j++){
        if(bikesh->rankingStations[j].stationId == station1Id){
            bikesh->rankingStations[j].memberTrips += isMember;
            // bikesh->rankingStations[j].roundTrips += (station1Id == station2Id)? 1 : 0; QUERY 4?
            return;
        }
    }

    // bikesh->rankingStations[station1Id-1].vecMonths[getMonth(startDate)-1]++;
    
    // if(station1Id == station2Id)
    //     bikesh->rankingStations[station1Id-1].roundTrips++;
}


bikeSharingADT stringcpyNYC(bikeSharingADT bikesh, char * from, size_t stationId){
    for(int i = 0; i < bikesh->realDim; i++){
        if(stationId == bikesh->rankingStations[i].stationId){
            bikesh->rankingStations[i].stationName = realloc(bikesh->rankingStations[i].stationName, (strlen(from)+1) * sizeof(char));
            if (bikesh->rankingStations[i].stationName == NULL){
                return NULL;
            }
            strcpy(bikesh->rankingStations[i].stationName, from); 
            return bikesh;
        }
    }
    return bikesh;
}