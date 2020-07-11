#include "clock.h"

void addToClock(int procesos, char *proceso, int *estados)
{
    estados[procesos]++;
    printf("%s: TICK\n", proceso);
}

void printClock(int procesos, char *proceso, int *estados)
{
    printf("%s: LC[", proceso);
    int i = 0;
    while (i < procesos - 1)
    {
        printf("%d, ", estados[i]);
        i++;
    }
    printf("%d]\n", estados[procesos - 1]);
}
