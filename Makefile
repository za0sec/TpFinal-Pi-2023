CC = gcc
ALL_CFLAGS = -pedantic -Wall -std=c99 -fsanitize=address -g
all: bikeSharingNYC bikeSharingMON

bikeSharingNYC: bikeSharingNYC.c bikeSharingADT.c lib/htmlTable.c
	$(CC) $(ALL_CFLAGS) -o bikeSharingNYC.out $^

bikeSharingMON: bikeSharingMON.c bikeSharingADT.c lib/htmlTable.c
	$(CC) $(ALL_CFLAGS) -o bikeSharingMON.out $^

clean: 
	rm -f bikeSharingNYC.out bikeSharingMON.out 
