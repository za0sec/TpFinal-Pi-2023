# TPE FINAL 2023 PROGRAMACION IMPERATIVA

## Resumen

Este documento proporciona una descripción general de cómo compilar y ejecutar los programas `bikeSharingMON` y `bikeSharingNYC` utilizando `make` y el archivo `Makefile` proporcionado.

## Compilación

Para compilar ambos programas, simplemente ejecute `make` en la terminal desde el directorio en el que se encuentra el archivo `Makefile`:

```
make
```

Esto generará dos ejecutables llamados `bikeSharingMON.out` y `bikeSharingNYC.out`.

Si desea compilar un solo programa, puede hacerlo especificando el nombre del programa después del comando `make`:

```
make bikeSharingMON
```

o 

```
make bikeSharingNYC
```

## Ejecución

Una vez que los programas estén compilados, puede ejecutarlos desde la línea de comandos de la siguiente manera:

Para `bikeSharingMON`:

```
./bikeSharingMON.out bikesMON.csv stationsMON.csv
```

Para `bikeSharingNYC`:

```
./bikeSharingNYC.out bikesNYC.csv stationsNYC.csv
```

Por favor, asegúrese de que los archivos CSV `bikesMON.csv`, `stationsMON.csv`, `bikesNYC.csv` y `stationsNYC.csv` estén en el mismo directorio que los ejecutables o proporcionar la ruta completa a los archivos.

## Limpieza

Si desea limpiar los ejecutables y empezar de nuevo, puede usar el comando `clean` de la siguiente manera:

```
make clean
```

Esto eliminará los archivos `bikeSharingMON.out` y `bikeSharingNYC.out` generados anteriormente.

