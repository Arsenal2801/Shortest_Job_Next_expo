#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PROCESOS 100

// Estructura para representar un proceso
typedef struct {
    int id;             // Identificador del proceso
    int tiempo_llegada; // Tiempo de llegada del proceso
    int tiempo_ejecucion; // Tiempo de ejecución del proceso
    int tiempo_restante; // Tiempo de CPU restante del proceso
    int tiempo_finalizacion; // Tiempo en que el proceso termina
} Proceso;

sem_t mutex; // Semáforo para la exclusión mutua

int num_procesos; // Número de procesos
float promedio_tiempo_ejecucion = 0; // Promedio de los tiempos de ejecución anteriores

// Función para encontrar el proceso con el tiempo de ejecución más corto
int encontrar_proceso_corto(Proceso procesos[], int n, int tiempo_actual) {
    int min_tiempo_ejecucion = __INT_MAX__;
    int proceso_corto = -1;
    for (int i = 0; i < n; i++) {
        if (procesos[i].tiempo_llegada <= tiempo_actual && procesos[i].tiempo_restante < min_tiempo_ejecucion && procesos[i].tiempo_restante > 0) {
            min_tiempo_ejecucion = procesos[i].tiempo_restante;
            proceso_corto = i;
        }
    }
    return proceso_corto;
}

// Función para generar procesos aleatorios
void generar_procesos(Proceso procesos[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        procesos[i].id = i + 1;
        procesos[i].tiempo_llegada = rand() % 10; // Tiempo de llegada aleatorio entre 0 y 9
        procesos[i].tiempo_ejecucion = rand() % 10 + 1; // Tiempo de ejecución aleatorio entre 1 y 10
        procesos[i].tiempo_restante = procesos[i].tiempo_ejecucion;
    }
}

// Función para ejecutar el algoritmo SJN
void sjn(Proceso procesos[], int n) {
    int tiempo_espera[n];
    int tiempo_retorno[n];

    // Inicializar el tiempo de espera
    for (int i = 0; i < n; i++) {
        tiempo_espera[i] = 0;
    }

    // Ejecutar procesos
    int tiempo_actual = 0;
    for (int i = 0; i < n; i++) {
        sem_wait(&mutex); // Esperar a que el semáforo esté disponible
        promedio_tiempo_ejecucion = (promedio_tiempo_ejecucion * i + procesos[i].tiempo_ejecucion) / (i + 1); // Actualizar el promedio
        sem_post(&mutex); // Liberar el semáforo

        // Utilizar el promedio ponderado como estimación para el tiempo de ejecución restante
        int estimacion = promedio_tiempo_ejecucion;

        int proceso_actual = encontrar_proceso_corto(procesos, n, tiempo_actual);
        if (proceso_actual == -1)
            break;
        tiempo_espera[proceso_actual] = tiempo_actual - procesos[proceso_actual].tiempo_llegada;

        // Si la estimación es menor que el tiempo restante real, usar el tiempo restante real
        int tiempo_ejecucion_real = procesos[proceso_actual].tiempo_restante;
        if (tiempo_ejecucion_real < estimacion) {
            tiempo_ejecucion_real = estimacion;
        }

        tiempo_actual += tiempo_ejecucion_real;
        procesos[proceso_actual].tiempo_restante = 0;
        procesos[proceso_actual].tiempo_finalizacion = tiempo_actual;
    }

    // Calcular tiempos de retorno
    for (int i = 0; i < n; i++) {
        tiempo_retorno[i] = procesos[i].tiempo_finalizacion - procesos[i].tiempo_llegada;
    }

    // Calcular tiempos promedio
    float tiempo_espera_promedio = 0;
    float tiempo_retorno_promedio = 0;
    for (int i = 0; i < n; i++) {
        tiempo_espera_promedio += tiempo_espera[i];
        tiempo_retorno_promedio += tiempo_retorno[i];
    }
    tiempo_espera_promedio /= n;
    tiempo_retorno_promedio /= n;

    // Mostrar resultados
    printf("Proceso\t Tiempo de Espera\t Tiempo de Retorno\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t %d\t\t\t %d\n", procesos[i].id, tiempo_espera[i], tiempo_retorno[i]);
    }
    printf("Tiempo de espera promedio: %.2f\n", tiempo_espera_promedio);
    printf("Tiempo de retorno promedio: %.2f\n", tiempo_retorno_promedio);
}

int main() {
    Proceso procesos[MAX_PROCESOS];

    printf("Ingrese el número máximo de procesos: ");
    scanf("%d", &num_procesos);

    // Inicializar semáforo
    sem_init(&mutex, 0, 1);

    // Generar procesos aleatorios
    generar_procesos(procesos, num_procesos);

    // Crear procesos hijos
    for (int i = 0; i < num_procesos; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sjn(procesos + i, 1);
            exit(0);
        } else if (pid < 0) {
            fprintf(stderr, "Error al crear proceso hijo\n");
            return 1;
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < num_procesos; i++) {
        wait(NULL);
    }

    // Destruir semáforo
    sem_destroy(&mutex);

    return 0;
}
