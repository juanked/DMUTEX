#include <stdio.h>

typedef struct clock{
    int *listaProcesos;
} reloj;

void printClock(int procesos, int *estados, char *proceso);
void addToClock(int procesos, int *estados, char *proceso);