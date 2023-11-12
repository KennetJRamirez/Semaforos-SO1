#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int n =15;
int barco1_x = 0, barco1_y = 0, barco2_x = 0, barco2_y = 0;
sem_t semaforo;

void* mover_barco(void* args) {
    int id = *((int*)args);

    for (int i = 0; i < n; ++i) {
        sem_wait(&semaforo);

        if (id == 1)
            printf("Barco %d: Posicion (%d, %d)\n", id, ++barco1_x, ++barco1_y);
        else
            printf("Barco %d: Posicion (%d, %d)\n", id, --barco2_x, ++barco2_y);

        sem_post(&semaforo);
    }

    pthread_exit(NULL);
}

void inicializar_semaforo() {
    if (sem_init(&semaforo, 0, 1) != 0) {
        perror("Error al inicializar el semaforo");
        exit(EXIT_FAILURE);
    }
}

void destruir_semaforo() {
    if (sem_destroy(&semaforo) != 0) {
        perror("Error al destruir el semaforo");
        exit(EXIT_FAILURE);
    }
}

void crear_y_unir_hilo(pthread_t* id, int* args) {
    if (pthread_create(id, NULL, mover_barco, args) != 0) {
        perror("Error al crear el hilo");
        exit(EXIT_FAILURE);
    }
}

void unir_hilo(pthread_t id) {
    if (pthread_join(id, NULL) != 0) {
        perror("Error al unir el hilo");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int ret1, ret2;
    pthread_t id1, id2;

    inicializar_semaforo();

    int id_barco1 = 1, id_barco2 = 2;

    // Se crean los  hilos
    crear_y_unir_hilo(&id1, &id_barco1);
    crear_y_unir_hilo(&id2, &id_barco2);

    // Hilos en espera
    unir_hilo(id1);
    unir_hilo(id2);

    printf("Los barcos han completado sus iteraciones.\n");

    // Destruir el semaforo
    destruir_semaforo();

    return 0;
}
