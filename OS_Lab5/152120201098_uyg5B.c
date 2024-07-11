#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Semaforlar ve mutex
sem_t sem_shirt_after_pants;
sem_t sem_tie_after_belt;
sem_t sem_hat_before_wallet;
sem_t sem_hat_after_jacket;
sem_t sem_shoes;
pthread_mutex_t mutex;

int current_step = 0;
const int total_steps = 11;

void print_step(const char *item) {
    pthread_mutex_lock(&mutex);
    current_step++;
    printf("%d. %s", current_step, item);
    if (current_step == 1) {
        printf(" --> Haydi baslayalim...\n");
    } else if (total_steps - current_step < 5 && current_step != total_steps) {
        printf(" --> Kalan oge sayisi: %d\n", total_steps - current_step);
    } else if (current_step == total_steps) {
        printf(" --> Hazirim...\n");
    } else {
        printf("\n");
    }
    pthread_mutex_unlock(&mutex);
}

void* head(void *arg) {
    print_step("(kafa ) Gozluk");
    sem_wait(&sem_hat_before_wallet);
    sem_wait(&sem_hat_after_jacket);
    print_step("(kafa ) Sapka");
    sem_post(&sem_shoes);
    return NULL;
}

void* body(void *arg) {
    sem_wait(&sem_shirt_after_pants);
    print_step("(govde) Gomlek");
    sem_post(&sem_hat_after_jacket);
    sem_wait(&sem_tie_after_belt);
    print_step("(govde) Kravat");
    sem_post(&sem_tie_after_belt);
    print_step("(govde) Ceket");
    print_step("(govde) Mont");
    return NULL;
}

void* legs(void *arg) {
    print_step("(bacak) Pantolon");
    sem_post(&sem_shirt_after_pants);
    print_step("(bacak) Kemer");
    sem_wait(&sem_hat_before_wallet);
    sem_post(&sem_tie_after_belt);
    print_step("(bacak) Cuzdan");
    sem_post(&sem_hat_before_wallet);
    return NULL;
}

void* feet(void *arg) {
    sem_wait(&sem_shoes);
    print_step("(ayak ) Corap");
    print_step("(ayak ) Ayakkabi");
    return NULL;
}

int main() {
    pthread_t threads[4];

    // Semaforları ve mutex'i başlatma
    sem_init(&sem_shirt_after_pants, 0, 0);
    sem_init(&sem_tie_after_belt, 0, 0);
    sem_init(&sem_hat_before_wallet, 0, 1);
    sem_init(&sem_hat_after_jacket, 0, 0);
    sem_init(&sem_shoes, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Threadleri oluşturma
    pthread_create(&threads[0], NULL, head, NULL);
    pthread_create(&threads[1], NULL, body, NULL);
    pthread_create(&threads[2], NULL, legs, NULL);
    pthread_create(&threads[3], NULL, feet, NULL);

    // Threadlerin tamamlanmasını bekleme
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Semaforları ve mutex'i yok etme
    sem_destroy(&sem_shirt_after_pants);
    sem_destroy(&sem_tie_after_belt);
    sem_destroy(&sem_hat_before_wallet);
    sem_destroy(&sem_hat_after_jacket);
    sem_destroy(&sem_shoes);
    pthread_mutex_destroy(&mutex);

    return 0;
}