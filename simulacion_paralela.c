/*
 * simulacion_paralela.c
 * Simulación de tráfico usando OpenMP con paralelismo dinámico
 * Proyecto: Mini Proyecto Tráfico - Computación Paralela y Distribuida
 * Autor: [Tu Nombre]
 * Fecha: Agosto 2025
 *
 * Este programa simula vehículos y semáforos.
 * Los vehículos avanzan si su semáforo está en verde,
 * usando paralelismo dinámico para balancear el trabajo entre hilos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

// Enumeración para los estados de un semáforo
typedef enum {
    ROJO = 0,
    VERDE = 1,
    AMARILLO = 2
} EstadoSemaforo;

// Estructura para representar un semáforo
typedef struct {
    int id;
    EstadoSemaforo estado;
    int contador_cambio;
} Semaforo;

// Estructura para representar un vehículo
typedef struct {
    int id;
    int posicion;
} Vehiculo;

// Función para inicializar los vehículos
void inicializar_vehiculos(Vehiculo* vehiculos, int total) {
    int i;
    for (i = 0; i < total; i = i + 1) {
        vehiculos[i].id = i;
        vehiculos[i].posicion = 0;
    }
}

// Función para inicializar los semáforos
void inicializar_semaforos(Semaforo* semaforos, int total) {
    int i;
    for (i = 0; i < total; i = i + 1) {
        semaforos[i].id = i;
        semaforos[i].estado = i % 3;  // ROJO, VERDE, AMARILLO en secuencia
        semaforos[i].contador_cambio = 0;
    }
}

// Función para actualizar el estado de los semáforos
void actualizar_semaforos(Semaforo* semaforos, int total) {
    int i;
    for (i = 0; i < total; i = i + 1) {
        semaforos[i].contador_cambio = semaforos[i].contador_cambio + 1;

        if (semaforos[i].contador_cambio >= 3) {
            semaforos[i].estado = (semaforos[i].estado + 1) % 3;
            semaforos[i].contador_cambio = 0;
        }
    }
}

// Función para mover vehículos con paralelismo dinámico
void mover_vehiculos_dinamico(Vehiculo* vehiculos, int total_vehiculos, Semaforo* semaforos, int total_semaforos) {
    int i;

    // Activar paralelismo dinámico
    omp_set_dynamic(1);

    #pragma omp parallel for schedule(dynamic)
    for (i = 0; i < total_vehiculos; i = i + 1) {
        int id_hilo = omp_get_thread_num();
        int id_semaforo = vehiculos[i].id % total_semaforos;

        if (semaforos[id_semaforo].estado == VERDE) {
            vehiculos[i].posicion = vehiculos[i].posicion + 1;
            printf("Hilo %d movió el Vehículo %d a posición %d (Semáforo %d en VERDE)\n",
                   id_hilo, vehiculos[i].id, vehiculos[i].posicion, id_semaforo);
        } else {
            printf("Hilo %d NO movió el Vehículo %d (Semáforo %d en estado %d)\n",
                   id_hilo, vehiculos[i].id, id_semaforo, semaforos[id_semaforo].estado);
        }
    }
}

// Función para mostrar el estado actual de la simulación
void mostrar_estado(Vehiculo* vehiculos, int total_vehiculos, Semaforo* semaforos, int total_semaforos, int iteracion) {
    int i;
    printf("\n====== Iteración %d ======\n", iteracion + 1);

    for (i = 0; i < total_vehiculos; i = i + 1) {
        printf("Vehículo %d --> Posición: %d\n", vehiculos[i].id, vehiculos[i].posicion);
    }

    for (i = 0; i < total_semaforos; i = i + 1) {
        printf("Semáforo %d --> Estado: %d\n", semaforos[i].id, semaforos[i].estado);
    }

    printf("===========================\n");
}

// Función principal de simulación
void simular(int ciclos, Vehiculo* vehiculos, int total_vehiculos, Semaforo* semaforos, int total_semaforos) {
    int i;
    for (i = 0; i < ciclos; i = i + 1) {
        actualizar_semaforos(semaforos, total_semaforos);
        mover_vehiculos_dinamico(vehiculos, total_vehiculos, semaforos, total_semaforos);
        mostrar_estado(vehiculos, total_vehiculos, semaforos, total_semaforos, i);
        sleep(1);  // Esperar 1 segundo entre ciclos
    }
}

// Función principal del programa
int main() {
    int total_vehiculos = 20;
    int total_semaforos = 4;
    int ciclos_simulacion = 10;

    Vehiculo vehiculos[total_vehiculos];
    Semaforo semaforos[total_semaforos];

    inicializar_vehiculos(vehiculos, total_vehiculos);
    inicializar_semaforos(semaforos, total_semaforos);

    simular(ciclos_simulacion, vehiculos, total_vehiculos, semaforos, total_semaforos);

    return 0;
}
