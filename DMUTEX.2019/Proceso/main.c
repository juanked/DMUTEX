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

#define MSG 0
#define LOCK 1
#define OK 2

int puertoUDP, socketUDP;
int indiceProc = 0;
int indiceSecc = 0;
char *proceso;

typedef struct procesos
{
    int port;
    char *dir, *proccess
} Proceso;

typedef struct regiones
{
    char *nombre;
    int identificador, *peticiones, nPeticionarios, okays;
    int pedido, mutex;
} RegionCritica;

reloj relojPract;
RegionCritica *memReg;
Proceso *memProc;
char *buffer;
int procesoYo;

// Los necesito en los enviar
struct sockaddr_in dirAux;

int main(int argc, char *argv[])
{
    int port;
    char line[80], proc[80], *dirUDP;
    char act[10]; //MESSAGETO ocupa 10
    char par[70];
    memProc = (Proceso *)malloc(sizeof(struct procesos));
    memReg = (RegionCritica *)malloc(sizeof(struct regiones));
    struct sockaddr_in dir;
    proceso = argv[1];

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
    dir.sin_addr.s_addr = INADDR_ANY;
    dir.sin_port = htons(0);

    // TODO: Revisar qué es lo más correcto
    if (bind(socketUDP, (struct sockaddr *)&dir, sizeof(dir)))
    // if (bind(socketUDP, (struct sockaddr *)&dir, sizeof(struct sockaddr_in)) != 0)
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
        if (strcmp(line, "START\n") == 0)
            break;

        sscanf(line, "%[^:]: %d", proc, &port);
        /* Habra que guardarlo en algun sitio */

        if (strcmp(proc, argv[1]) == 0)
        { /* Este proceso soy yo */
            procesoYo = indiceProc;
        }

        Proceso procAux;
        procAux.proccess = proc;
        procAux.dir = (char *)malloc(strlen(dirUDP));
        strcpy(procAux.dir, dirUDP);
        procAux.port = (char *)malloc(strlen(port));
        strcpy(procAux.port, port);
        memProc[indiceProc++] = procAux;

        memProc = (Proceso *)realloc(memProc, (indiceProc + 1) * sizeof(struct procesos));
        if (memProc == NULL)
        {
            perror("Fallo en el realloc de memProc");
            return -1;
        }
    }

    /* Inicializar Reloj */
    relojPract.listaProcesos = (int *)malloc(sizeof(int) * indiceProc);
    for (int i = 0; i < indiceProc; i++)
    {
        relojPract.listaProcesos[i] = 0;
    }

    /* Procesar Acciones */
    for (; fgets(line, 80, stdin);)
    {
        sscanf(line, "%s %s", act, par);
        buffer = (char *)malloc((sizeof(int) * indiceProc + 2)) + strlen(par);

        if (strcmp(line, "EVENT\n") == 0)
        {
            addToClock(procesoYo, proceso, relojPract.listaProcesos);
        }
        else if (strcmp(line, "GETCLOCK\n") == 0)
        {
            printClock(procesoYo, proceso, relojPract.listaProcesos);
        }
        else if (strcmp(act, "MESSAGETO") == 0)
        {
            addToClock(procesoYo, proceso, relojPract.listaProcesos);
            sendMessageTo(par);
        }
        else if (strcmp(line, "RECEIVE\n") == 0)
        {
            if (read(socketUDP, buffer, sizeof(buffer)))
            {
                perror("Error read() en RECEIVE\n");
                close(socketUDP);
                return -1;
            }

            int relojAux[indiceProc];
            for (int i = 0; i < indiceProc; i++)
            {
                relojAux[i] = buffer[i + 2];
            }
            int auxOrden = 0;
            int auxIguales = 1;
            for (int i = 0; i < indiceProc; i++)
            {
                if (relojAux[i] < relojPract.listaProcesos[i])
                {
                    auxOrden = 1;
                    auxIguales = 0;
                }
                else if (relojAux[i] == relojPract.listaProcesos[i])
                    auxIguales = auxIguales && 1; //si siempre es 1, es que todos son
                else
                    auxIguales = 0;
                // TODO: Revisar que pasa si todos los elementos de los array son iguales
            }

            if (auxIguales == 1)
                auxOrden = 2;

            for (int i = 0; i < indiceProc; i++)
            {
                if (relojPract.listaProcesos[i] < buffer[i + 2])
                    relojPract.listaProcesos = buffer[i + 2];
            }
            int aux = (int)buffer[1];
            if (buffer[0] == MSG)
                printf("%s: RECEIVE(MSG,%)\n", proceso, memProc[aux].proccess);
            else if (buffer[0] == LOCK)
            {
                printf("%s: RECEIVE(LOCK,%)\n", proceso, memProc[aux].proccess);

                char nombre[strelen(&buffer[indiceProc + 2])];
                strcpy(nombre, &buffer[indiceProc + 2]);

                int aux = sectionToInt(nombre);
                if (aux == -1)
                {
                    addToClock(procesoYo, proceso, relojPract.listaProcesos);
                    addSection(nombre);
                    sendReceive(nombre);
                }
                else
                {
                    if (memReg[aux].mutex == 1)
                        waitProc(aux, buffer[1]);
                    else if (memReg[aux].pedido == 1)
                    {
                        switch (auxOrden)
                        {
                        case 0:
                            addToClock(procesoYo, proceso, relojPract.listaProcesos);
                            sendReceive(nombre);
                            break;
                        case 1:
                            waitProc(aux, buffer);
                            break;
                        case 2:
                            if (aux > (int)buffer[1])
                            {
                                addToClock(procesoYo, proceso, relojPract.listaProcesos);
                                sendReceive(nombre);
                            }
                            else
                                waitProc(aux, buffer[1]);
                            break;
                        default:
                            perror("Valor no reconocido en la variable auxOrden");
                            break;
                        }
                    }
                    else
                    {
                        addToClock(procesoYo, proceso, relojPract.listaProcesos);
                        sendReceive(nombre);
                    }
                }
            }
            else
            {
                printf("%s: RECEIVE(OK,%)\n", proceso, memProc[aux].proccess);
                int aux = sectionToInt(&buffer[indiceProc + 2]);
                memReg[aux].okays--;
                if (memReg[aux].okays == 0)
                {
                    memReg[aux].pedido = 0;
                    memReg[aux].mutex = 1;
                    printf("%s: MUTEX(%s)", proceso, &buffer[indiceProc + 2]);
                }
            }

            addToClock(procesoYo, proceso, relojPract.listaProcesos);
        }
        else if (strcmp(act, "LOCK") == 0)
        {
            char stAux[strlen(par)];
            strcpy(stAux, par);
            int aux = sectionToInt(stAux);

            //TODO: pasar pedir sección a aquí, que no haya una función solo para eso
            if (aux == -1)
                pedirSeccion(addSection(stAux));
            else
                pedirSeccion(aux);

            addToClock(procesoYo, proceso, relojPract.listaProcesos);
            sendLock(stAux);
        }
        else if (strcmp(act, "UNLOCK") == 0)
        {
            char stAux[strlen(par)];
            strcpy(stAux, par);

            sendUnlock(stAux);

            int aux = sectionToInt(stAux);
            //TODO: ver que es idSolicitantes y mutex y añadirlos a memReg

            memReg[aux].peticiones = (int *)malloc(sizeof(int));
            memReg[aux].mutex = 0;
        }

        else if (strcmp(line, "FINISH\n") == 0)
            break;

        free(buffer);
    }

    return 0;
}

int sectionToInt(char *nombre)
{
    for (int i = 0; i < indiceSecc; i++)
    {
        if (strcmp(memReg[i].nombre, nombre) == 0)
            return i;
    }
    return -1;
}

void pedirSeccion(int identificador)
{
    memReg[identificador].pedido = 1;
}

int addSection(char *nombre)
{
    RegionCritica aux;
    aux.pedido = 0;
    aux.mutex = 0;
    aux.nombre = (char *)malloc(strlen(nombre));
    strcpy(aux.nombre, nombre);
    aux.nPeticionarios = 0;
    aux.peticiones = (int *)malloc(strlen(nombre));
    aux.okays = indiceProc - 1;
    // TODO: añadir más cosas si hacen falta a RegiónCrítica
    memReg[indiceSecc++] = aux;
    memReg = (RegionCritica *)realloc(memReg, (indiceSecc + 1) * sizeof(struct regiones));
    // TODO: comprobar si está bien
    return indiceSecc - 1;
}

void waitProc(int identificadorSeccion, int proceso)
{
    memReg[identificadorSeccion].peticiones[memReg[identificadorSeccion].nPeticionarios++] = (int)proceso;
    memReg[identificadorSeccion].peticiones = (int *)realloc(memReg[identificadorSeccion].peticiones, (memReg[identificadorSeccion].nPeticionarios + 1) * sizeof(int));
}

void sendMessageTo(char *parametro)
{
    dirAux.sin_family = AF_INET;
    dirAux.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < indiceProc; i++)
        buffer[i + 2] = relojPract.listaProcesos[i];

    for (int i = 0; i < indiceProc; i++)
    {
        if (strcmp(memProc[i].proccess, parametro))
        {
            dirAux.sin_port = memProc[i].port;
            break;
        }
    }
    buffer[0] = MSG;
    buffer[1] = procesoYo;

    if (sendto(socketUDP, buffer, sizeof(buffer), 0, (struct sockaddr *)&dirAux, sizeof(dirAux)) < 0)
    {
        perror("Error sendto() de MessageTo");
        close(socketUDP);
        return -1;
    }

    printf("%s: SEND(MSG,%s)", proceso, parametro);
}

void sendReceive(char *nombre)
{
    dirAux.sin_family = AF_INET;
    dirAux.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < indiceProc; i++)
        buffer[i + 2] = relojPract.listaProcesos[i];
    int destino = (int)buffer[1];
    dirAux.sin_port = memProc[destino].port;

    buffer[0] = OK;
    buffer[1] = procesoYo;
    strcpy(&buffer[indiceProc + 2], nombre);

    if (sendto(socketUDP, buffer, sizeof(buffer), 0, (struct sockaddr *)&dirAux, sizeof(dirAux)) < 0)
    {
        perror("Error sendto() de Receive");
        close(socketUDP);
        return -1;
    }
    printf("%s: SEND(OK,%s)", proceso, memProc[destino].proccess);
}

void sendLock(char *seccion)
{
    dirAux.sin_family = AF_INET;
    dirAux.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < indiceProc; i++)
        buffer[i + 2] = relojPract.listaProcesos[i];

    for (int i = 0; i < indiceProc; i++)
    {
        if (i != procesoYo)
        {
            dirAux.sin_port = memProc[i].port;
            buffer[0] = LOCK;
            buffer[1] = procesoYo;
            strcpy(&buffer[indiceProc + 2], seccion);

            if (sendto(socketUDP, buffer, sizeof(buffer), 0, (struct sockaddr *)&dirAux, sizeof(dirAux)) < 0)
            {
                perror("Error sendto() de Lock");
                close(socketUDP);
                return -1;
            }

            printf("%s: SEND(LOCK,%s)", proceso, memProc[i].proccess);
        }
    }
}

void sendUnlock(char *seccion)
{
    int auxSec = sectionToInt(seccion);
    dirAux.sin_family = AF_INET;
    dirAux.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < indiceProc; i++)
        buffer[i + 2] = relojPract.listaProcesos[i];

    for (int i = 0; i < memReg[auxSec].nPeticionarios; i++)
    {
        addToClock(procesoYo, proceso, relojPract.listaProcesos);
        dirAux.sin_port = memProc[memReg[auxSec].peticiones[i]].port;
        for (int j = 0; j < indiceProc; j++)
            memcpy((int *)&buffer[j + 2], (int *)&relojPract.listaProcesos[j], sizeof(int));

        buffer[0] = OK;
        buffer[1] = procesoYo;
        strcpy(&buffer[indiceProc + 2], seccion);
        if (sendto(socketUDP, buffer, sizeof(buffer), 0, (struct sockaddr *)&dirAux, sizeof(dirAux)) < 0)
        {
            perror("Error sendto() de Unlock");
            close(socketUDP);
            return -1;
        }

        printf("%s: SEND(OK,%s)", proceso, memProc[memReg[auxSec].peticiones[i]].proccess);
    }
}