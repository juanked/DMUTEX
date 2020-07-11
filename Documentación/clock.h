#include <stdio.h>

typedef struct clock
{
    int *listaProcesos;
} reloj;

void addToClock(int procesos, char *proceso, int *estados);
void printClock(int procesos, char *proceso, int *estados);