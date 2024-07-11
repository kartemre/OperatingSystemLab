#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h> // usleep() kullanimi icin gerekli

// Veri yapısı tanımı
struct multiplePrintData {
    pthread_t id;
    int threadid;
    char *printText;
    double waitDuration;
};

// Ekrana metni yazdırma fonksiyonu
void printCharMultipleTimes(char *prTxt, double seconds) {
    int count = 0;
    // Belirli bir süre çalıştıktan sonra thread'i sonlandır
    while (1) {
        count++;
        fflush(stdout); // Flush buffer for instant changes
        printf("%s\n", prTxt);
        usleep((int)(seconds * 1000000)); // Wait for the specified seconds
    }
}

void *task(void *arg) {
    struct multiplePrintData *task_data = (struct multiplePrintData *)arg;
    printf("Thread %d started: %s\n", task_data->threadid, task_data->printText);
    printCharMultipleTimes(task_data->printText, task_data->waitDuration);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || (argc-1) % 2 != 0) {
        printf("Kullanım: %s toplam_calisma_suresi eşzamanlı_is_yükü_sayısı iş_yükü_1 bekleme_süresi_1 [iş_yükü_2 bekleme_süresi_2 ...]\n", argv[0]);
        return 1;
    }

    // Argümanlardan toplam çalışma süresini ve eş zamanlı iş yükü sayısını al
    int total_ms = atoi(argv[1]);
    int num_tasks = atoi(argv[2]);

    pthread_t threads[num_tasks];
    struct multiplePrintData tasks[num_tasks];
    // Eş zamanlı iş yüklerini oluştur
    for (int i = 0; i < num_tasks; i++) {
        tasks[i].threadid = i + 1;
        tasks[i].printText = argv[3 + 2 * i];
        tasks[i].waitDuration = atof(argv[4 + 2 * i]);
        pthread_create(&threads[i], NULL, task, &tasks[i]);
    }

    // Belirtilen toplam çalışma süresi boyunca programı çalıştır
    usleep((int)(total_ms * 1000000));

    // Threadleri sonlandır
    for (int i = 0; i < num_tasks; i++) {
        pthread_cancel(threads[i]);
    }

    // Join threads
    for (int i = 0; i < num_tasks; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
