/* DMUTEX (2009) Sistemas Operativos Distribuidos
 * Código de Apoyo
 *
 * ESTE CÓDIGO DEBE COMPLETARLO EL ALUMNO:
 *    - Para desarrollar las funciones de mensajes, reloj y
 *      gestión del bucle de tareas se recomienda la implementación
 *      de las mismas en diferentes ficheros.
 *      Se han implementado en clock.c y clock.h
 */

/* Nombre: David Cristóbal Pascual
 * Matrícula: 160336 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>

#include "clock.h"

int puertoUDP, socketUDP;
int indiceProc = 0;

typedef struct procesos
{
    int port;
    char *dir, *proccess
} Proceso;

reloj relojPract;

int main(int argc, char *argv[])
{
    int port;
    char line[80], proc[80], *dirUDP;
    struct sockaddr_in dir;
    Proceso *memProc;

    if (argc < 2)
    {
        fprintf(stderr, "Uso: proceso <ID>\n");
        return 1;
    }

    /* Establece el modo buffer de entrada/salida a l�nea */
    setvbuf(stdout, (char *)malloc(sizeof(char) * 80), _IOLBF, 80);
    setvbuf(stdin, (char *)malloc(sizeof(char) * 80), _IOLBF, 80);

    // puertoUDP = 1111; /* Se determina el puerto UDP que corresponda.
    //                     Dicho puerto debe estar libre y quedar�
    //                     reservado por dicho proceso. */
    
    
    if ((socketUDP = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("No se ha podido crear el socket UDP\n");
        close(socketUDP);
        return -1;
    }

    dir.sin_family = AF_INET;
    dir.sin_port = INADDR_ANY;
    dir.sin_family = htons(0);

    // TODO: Revisar qué es lo más correcto
    //  if (bind(socketUDP, (struct sockaddr *)&dir, sizeof(dir)))
    if (bind(socketUDP, (struct sockaddr *)&dir, sizeof(struct sockaddr_in)) != 0)
    {
        perror("Error en el bind\n");
        close(socketUDP);
        return -1;
    }

    struct sockaddr_in socketStor;
    socklen_t socketLen = sizeof(socketStor);

    if (getsockname(socketUDP, (struct sockaddr *)&socketStor, (socklen_t *)&socketLen) == -1)
    {
        perror("Error en getsockname()\n");
        close(socketUDP);
        return -1;
    }

    puertoUDP = socketStor.sin_port;
    dirUDP = inet_ntoa(socketStor.sin_addr);

    fprintf(stdout, "%s: %d\n", argv[1], puertoUDP);

    for (; fgets(line, 80, stdin);)
    {
        if (!strcmp(line, "START\n"))
            break;

        sscanf(line, "%[^:]: %d", proc, &port);
        /* Habra que guardarlo en algun sitio */

        if (!strcmp(proc, argv[1]))
        { /* Este proceso soy yo */
        }

        Proceso procAux;
        procAux.proccess = proc;
        procAux.dir = (char *)malloc(strlen(dirUDP));
        strcpy(procAux.dir, dirUDP);
        procAux.port = (char *)malloc(strlen(port));
        strcpy(procAux.port, port);
        memProc[indiceProc++] = procAux;

        //TODO: Revisar si está bien hecha la reserva (indice+1)

        memProc = (Proceso *)realloc(memProc, (indiceProc + 1) * sizeof(struct procesos));
        if (memProc == NULL)
            return -1;
    }

    /* Inicializar Reloj */
    relojPract.listaProcesos=

    /* Procesar Acciones */

    return 0;
}
