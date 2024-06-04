#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_PROCESOS 10

// Estructura para representar un proceso
typedef struct {
    int id;
    int duracion;
} Proceso;

// Función para simular la ejecución de un proceso y generar su tabla de multiplicar
void ejecutar_proceso(Proceso proceso) {
    printf("Proceso %d iniciado. Duración: %d segundos.\n", proceso.id, proceso.duracion);
    
    // Generar tabla de multiplicar
    printf("Tabla de multiplicar del proceso %d:\n", proceso.id);
    for (int i = 1; i <= 10; ++i) {
        printf("%d x %d = %d\n", proceso.id, i, proceso.id * i);
    }
    
    sleep(proceso.duracion);
    printf("Proceso %d finalizado.\n", proceso.id);
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
    
    // Ejecutar los procesos en orden
    for (int i = 0; i < num_procesos; i++) {
        ejecutar_proceso(procesos[i]);
    }
    
    return 0;
}

