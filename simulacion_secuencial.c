#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// --------------- Enumeración EstadoSemaforo --------------- 
// Esto define los estados posibles de un semaforo como constantes legíbles

// Estado posibles de un semáforo
typedef enum {
    ROJO = 0,
    VERDE = 1,
    AMBAR = 2
} EstadoSemaforo;
// --------------------------------------------------------

// --------------- Estructura Semaforo ---------------
// Cada semaforo tiene ID(identificador único), estado(ROJO, VERDE, AMBAR)
//     contador_cambio(cuanta cuántos ciclos han pasado para decidir si debe cambiar de estado)

// Estructura que representa un semáforo
typedef struct {
    int id;
    EstadoSemaforo estado;
    int contador_cambio;
} Semaforo;
// --------------------------------------------------------

// --------------- Estrucutra vehiculo ---------------
// Cada vehículo tiene: id(identifciador único), posicion(cuántas veces ha avanzado(simula el movimiento))
// Estructura que representa un vehículo
typedef struct {
    int id;
    int posicion;
} Vehiculo;
// --------------------------------------------------------

// ------------- Inicializar vehículos ---------------
//    Crea vehiculos de 0 a total-1
//    posicion = 0 al inicio

// Paso 1: Inicializar vehículos
void inicializar_vehiculos(Vehiculo* vehiculos, int total) {
    for (int i = 0; i < total; i++) {
        vehiculos[i].id = i;
        vehiculos[i].posicion = 0;
    }
}
// --------------------------------------------------------


// ------------- Inicializar semáforos ---------------
// - Asigna a cada semáforo: ID
// - Un estado cíclico (ROJO, VERDE, AMBAR) usando el residuo de la operación i % 3 o modulo z3
//  - contador_cambio = 0 al inicio

// Paso 2: Inicializar semáforos
void inicializar_semaforos(Semaforo* semaforos, int total) {
    for (int i = 0; i < total; i++) {
        semaforos[i].id = i;
        semaforos[i].estado = i % 3;  // ROJO, VERDE, AMBAR
        semaforos[i].contador_cambio = 0;
    }
}
// --------------------------------------------------------


// ------------- Actualizar semáforos ---------------
// Incrementa el contenedor de cada semáforo
// Si llega a 3 ciclos, cambia el estado en orden
//   - ROJO -> VERDE -> AMBAR -> ROJO

// Paso 3: Actualizar el estado de los semáforos
void actualizar_semaforos(Semaforo* semaforos, int total) {
    for (int i = 0; i < total; i++) {
        semaforos[i].contador_cambio += 1;
        if (semaforos[i].contador_cambio >= 3) {
            semaforos[i].estado = (semaforos[i].estado + 1) % 3;
            semaforos[i].contador_cambio = 0;
        }
    }
}
//-----------------------------------------------------

// ----------------- Mover vehiculos -----------------
//  Cada vehículo está asociado a un semáforo según su id % total_semaforos
//  Si el semáforo aociado está en VERDE, el vehículo avanza 1 posición

// Paso 4: Actualizar la posición de los vehículos
void mover_vehiculos(Vehiculo* vehiculos, int total, Semaforo* semaforos, int total_semaforos) {
    for (int i = 0; i < total; i++) {
        int semaforo_asociado = vehiculos[i].id % total_semaforos;
        if (semaforos[semaforo_asociado].estado == VERDE) {
            vehiculos[i].posicion += 1;
        }
    }
}
// -------------------------------------------------------

// ------------------ Mostrar estado de la simulación -------
// Imprime por consola:
//  - La iteración actual.
//  - La posición de cada vehículo.
//  - El estado de cada semáforo.

// Paso 5: Mostrar el estado actual
void mostrar_estados(Vehiculo* vehiculos, int total_vehiculos, Semaforo* semaforos, int total_semaforos, int iteracion) {
    printf("Iteración %d\n", iteracion + 1);
    for (int i = 0; i < total_vehiculos; i++) {
        printf("Vehículo %d - Posición: %d\n", vehiculos[i].id, vehiculos[i].posicion);
    }
    for (int i = 0; i < total_semaforos; i++) {
        printf("Semáforo %d - Estado: %d\n", semaforos[i].id, semaforos[i].estado);
    }
    printf("---------------------------------\n");
}
// ----------------------------------------------------

// ------------- Ciclo principal de simulación -------------
// Se repite durante iteraciones veces:
//  - Actualiza semáforos.
//   - Mueve vehículos si su semáforo lo permite.
//   - Muestra el estado del sistema.
// Pausa de 1 segundo (sleep(1)), para que el cambio sea visible al usuario


// Paso 6: Ejecutar la simulación
void simular(int iteraciones, Vehiculo* vehiculos, int total_vehiculos, Semaforo* semaforos, int total_semaforos) {
    for (int i = 0; i < iteraciones; i++) {
        actualizar_semaforos(semaforos, total_semaforos);
        mover_vehiculos(vehiculos, total_vehiculos, semaforos, total_semaforos);
        mostrar_estados(vehiculos, total_vehiculos, semaforos, total_semaforos, i);
        sleep(1);
    }
}
// -----------------------------------------------------

// ------------------ Main -------------------------
// Define cuantos vehiculos y semaforos,
// Crea los arreglos de estructuras
// Inicializa ambos
// Llama a simular() para ejecutar todo el sistema duante los cilos 

// Función principal
int main() {
    int total_vehiculos = 20;
    int total_semaforos = 4;
    int iteraciones = 10;

    Vehiculo vehiculos[total_vehiculos];
    Semaforo semaforos[total_semaforos];

    inicializar_vehiculos(vehiculos, total_vehiculos);
    inicializar_semaforos(semaforos, total_semaforos);
    simular(iteraciones, vehiculos, total_vehiculos, semaforos, total_semaforos);

    return 0;
}
