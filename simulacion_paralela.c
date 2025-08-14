#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

//-------------- Estructuras y enumeraciones -------------- 
//Se define el estado del semáforo y las estrucuturas par amodelar vehículos y semáforos
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

// --------------------------------------------------------

// -------------- Inicialización --------------------------
//  Estas funciones llenan los arreglos con valores iniciales para los objetivos
//  -  Cada vehiculo inicia en posición 0 
//  -  Cada semáforo inciia con un estaado cídiclico (Rojo, Verde, Amarillo, ....)

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
                                      // Calcula el residuo de dividir i entre 3.
                                      // Los posibles resultados son: 0, 1, 2.
        semaforos[i].contador_cambio = 0;
    }
}
// --------------------------------------------------------

// ------------- Lógica de actualización ------------------
// Incrementa un contador en cada semafóro
// Si el contado llega a 3, cambia de estado: Rojo -> Verde -> Amarillo -> Rojo ...

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
//--------------------------------------------------------
// ------------- Paralelismo con OpenMP ------------------
// - Divide automanticamente los vehiculos entre los hilos disposnibles
// - Si el semáforo asociado está en VERDE, el hilo avanza el vehículo
// - Se imprime que hilo movió qué vehículo
//La clásula "Schedule(dynamic)"es el que permite OpenMP asigne tareas a 
//medida que los hilos se van desocupando -> mejor en el balance de carga.

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

//-------------------------------------------------------
// ------------- Mostrar el estado actual ---------------
//Imprime en consola la posición de cada vehícul y el estado 
//de cada semáforo por cada iteración.

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
//--------------------------------------------------------

// ------------- Función de simulación principal ---------
// Aqui se realizan N interaciones de la simualción.
// - 1. Actualiza los semáforos
// - 2. Mueve los vehículos (con paralelismo dinámico)
// - 3. Muestra el estado actual
// - 4. Espera 1 segundo "sleep(1)" entre iteraciones 

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
//--------------------------------------------------------

// ------------- Main -------------------------------------
// Función principal del programa
int main() {
    int total_vehiculos = 400;
    int total_semaforos = 4;
    int ciclos_simulacion = 10;

    Vehiculo vehiculos[total_vehiculos];
    Semaforo semaforos[total_semaforos];

    inicializar_vehiculos(vehiculos, total_vehiculos);
    inicializar_semaforos(semaforos, total_semaforos);

    simular(ciclos_simulacion, vehiculos, total_vehiculos, semaforos, total_semaforos);

    return 0;
}
