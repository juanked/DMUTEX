#include "clock.h"

void printClock(int procesos, int *estados, char *proceso)
{
    printf("%s: clock(", proceso);
    int i = 0;
    while (i < procesos - 1)
    {
        printf("%d, ", estados[i]);
        i++;
    }
    printf("%d)\n", estados[procesos - 1]);
}

void addToClock(int procesos, int *estados, char *proceso)
{
    estados[procesos]++;
    printf("%s: TICK\n", proceso);
}