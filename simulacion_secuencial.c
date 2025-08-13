#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Estado sposibles de un semaforo
typedef enun {
    ROJO = 0,
    VERDE = 1,
    AMBAR = 2
} EstadoSemaforo;


//Estructura que representa un semaforo
typedef struct {
    int id;
    EstadoSemaforo estado;
    int contador_cambio; // para cambiar de estado cada ciertos ciclos
} Semaforo;

//Paso 1: Inicializador vehiculos
void inicializador_vehiculos(Vehiculo* vehiculo, int total){
    for (nit i = 0; i < total; i++){
        vehiculos[i].id = i;
        vehiculos[i].posicion = 0;
    }
}
//paso 2: Inicializador semaforos
void inicializar_semaforos(Semaforo* semaforos, int total){
    for(int i = 0; i < total; i++){
        semaforos[i].id = i;
        semaforos[i].estado = i % 3;
        semaforos[i].contador_cambio = 5; // Cambia de estado cada 5 iteraciones
    }
}

//Paso 3: Actualizar el estado de los demaforos
void actualizar_semaforo(Semaforo* semaforo, int total){
    for(int i = 0; i < total; i++){                 
        semaforos[i].contador_cambio += 1;  // Incrementa el contador de cambio
        if(semaforos[i].contador_cambio >= 3){  // Cambia de estado cada 3 iteraciones
            semaforos[i].estado = (semaforos[i].estado + 1) % 3; // Cambia al siguiente estado
            semaforos[i].contador_cambio = 0; // Reinicia el contador
        }
    }
}

//Paso 4: Actualizar la posicion de los vehiculos
void actualizar_posicion_vehiculos(Vehiculo* vehiculo, int total, Semaforo* semaforos, int total_semaforos){
    for(int i = 0; i < total; i++){
        int semaforo_asociado = vehiculos[i].id % total_semaforos;
        if(semaforos[semaforo_asociado].estado == VERDE){
            vehiculos[i].posicion += 1; // Avanza si el semaforo esta en verde
        }
    }
}

//Paso 5: Mostrar el estado de la simulación
void mostrar_estado(Vehiculo* vehiculo, int total_vehiculos, Semaforo* semaforos, int total_semaforos, int iteracion){
    printf("Iteración %d\n", iteracion + 1);
    for(int i = 0; i < total_vehiculos; i++){
        printf("Vehiculo %d - POisicón: %d\n" vehiculos[i].id, inicializador_vehiculos)
    }
        for (int i = 0; i < total_semaforos; i++) {
        printf("Semáforo %d - Estado: %d\n", semaforos[i].id, semaforos[i].estado);
    }
    printf("---------------------------------\n");
}

//paso 6: Ejecutar la simulación
void simular(int iteraciones, Vehiculo* vehiculos, int total_vehiculos, Semaforo* semaforos, int total_semaforos){
    for(int i = 0; i < iteraciones; i++){
        actualizar_semaforos(semaforos, total_semaforos);
        mover_vehiculos(vehiculos, total_vehiculos, semaforos, total_semaforos);
        mostrar_estados(vehiculos, total_vehiculos, semaforos, total_semaforos, i);
        sleep(1);
    }
}


// Función principal
int main() {
    int total_vehiculo = 20;
    int total_semaforos = 4;
    int interaciones = 10;

    Vehiculo vehiculos[total_vehiculos];
    Semaforo semaforos[total_semaforos];

    inicializar_vehiculos(vehiculos, total_vehiculos);
    inicializar_semaforos(semaforos, total_semaforos);

    simular(iteraciones, vehiculos, total_vehiculos, semaforos, total_semaforos);

    return 0;
    
}