#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>

#define MAX_PROCESOS 10

// Estructura para representar un proceso
typedef struct {
    int id;
    int duracion;
} Proceso;

// Función para simular la ejecución de un proceso y generar su tabla de multiplicar
void ejecutar_proceso(Proceso proceso) {
    // Obtener el semáforo correspondiente al proceso
    int sem_id = proceso.id - 1;

    // Esperar a que el semáforo esté disponible
    struct sembuf operacion;
    operacion.sem_num = sem_id;
    operacion.sem_op = -1; // Decrementar el valor del semáforo
    operacion.sem_flg = SEM_UNDO; // Revertir la operación al finalizar

    if (semop(semid, &operacion, 1) == -1) {
        perror("Error al esperar semáforo");
        exit(1);
    }

    // Sección crítica: Ejecutar el proceso

    printf("Proceso %d iniciado. Duración: %d segundos.\n", proceso.id, proceso.duracion);

    // Generar tabla de multiplicar
    printf("Tabla de multiplicar del proceso %d:\n", proceso.id);
    for (int i = 1; i <= 10; ++i) {
        printf("%d x %d = %d\n", proceso.id, i, proceso.id * i);
    }

    sleep(proceso.duracion);
    printf("Proceso %d finalizado.\n", proceso.id);

    // Liberar el semáforo
    operacion.sem_op = 1; // Incrementar el valor del semáforo
    if (semop(semid, &operacion, 1) == -1) {
        perror("Error al liberar semáforo");
        exit(1);
    }
}

// Función para ordenar los procesos por duración (ascendente)
void ordenar_procesos(Proceso procesos[], int num_procesos) {
    for (int i = 0; i < num_procesos - 1; i++) {
        for (int j = 0; j < num_procesos - i - 1; j++) {
            if (procesos[j].duracion > procesos[j + 1].duracion) {
                // Intercambiar procesos
                Proceso temp = procesos[j];
                procesos[j] = procesos[j + 1];
                procesos[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero_de_procesos>\n", argv[0]);
        return 1;
    }

    int num_procesos = atoi(argv[1]);
    if (num_procesos < 1 || num_procesos > MAX_PROCESOS) {
        printf("Número de procesos inválido. Debe ser un valor entre 1 y %d.\n", MAX_PROCESOS);
        return 1;
    }

    Proceso procesos[MAX_PROCESOS];

    // Crear los procesos con duraciones aleatorias entre 1 y 5 segundos
    for (int i = 0; i < num_procesos; i++) {
        procesos[i].id = i + 1;
        procesos[i].duracion = rand() % 5 + 1;
    }

    // Ordenar los procesos por duración
    ordenar_procesos(procesos, num_procesos);

    // Crear el semáforo
    key_t clave = ftok(__FILE__, 'S');
    int semid = semget(clave, num_procesos, IPC_CREAT | 0666);

    if (semid == -1) {
        perror("Error al crear semáforo");
        exit(1);
    }
// Inicializar el semáforo
union semun arg;
arg.val = 1;

for (int i = 0; i < num_procesos; i++) {
    if (semctl(semid, i, SETVAL, arg) == -1) {
        perror("Error al inicializar semáforo");
        exit(1);
    }
}

// Ejecutar los procesos en orden
pid_t pid;
for (int i = 0; i < num_procesos; i++) {
    pid = fork();

    if (pid == 0) { // Proceso hijo
        ejecutar_proceso(procesos[i]);
        exit(0);
    } else if (pid == -1) {
        perror("Error al crear proceso hijo");
        exit(1);
    }
}

// Esperar a que finalicen todos los procesos hijos
for (int i = 0; i < num_procesos; i++) {
    wait(NULL, NULL, WNOHANG);
}

// Eliminar el semáforo
if (semctl(semid, 0, IPC_RMID) == -1) {
    perror("Error al eliminar semáforo");
    exit(1);
}

return 0;
}
