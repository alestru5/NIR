#include "../include/Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Файлы для эмуляции хранения данных
#define TRAP_FILE_DB "trojan_ransomware_detector/data/trap_files.csv"
#define ACTIVITY_DB "trojan_ransomware_detector/data/activities.csv"

// Вспомогательная функция для создания директории data, если она не существует
void create_data_dir() {
    // POSIX-совместимое создание директории, если не существует
    struct stat st = {0};
    if (stat("trojan_ransomware_detector/data", &st) == -1) {
        mkdir("trojan_ransomware_detector/data", 0755);
    }
}

void storeTrapFileData(TrapFileData data) {
    create_data_dir();
    FILE* fp = fopen(TRAP_FILE_DB, "a"); // Открываем для добавления
    if (fp != NULL) {
        // Простой CSV формат: filePath,creationTime,lastAccessTime
        fprintf(fp, "%s,%s,%s\n", data.filePath, data.creationTime, data.lastAccessTime);
        fclose(fp);
    } else {
        perror("Ошибка открытия файла базы данных файлов-ловушек");
    }
}

void storeActivityData(ActivityData data) {
    create_data_dir();
    FILE* fp = fopen(ACTIVITY_DB, "a"); // Открываем для добавления
    if (fp != NULL) {
        // Простой CSV формат: filePath,activityType,timestamp
        fprintf(fp, "%s,%s,%s\n", data.filePath, data.activityType, data.timestamp);
        fclose(fp);
    } else {
        perror("Ошибка открытия файла базы данных активности");
    }
}

// Вспомогательная функция для подсчета строк в файле (для определения размера данных)
int count_lines(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0; // Файл не существует или ошибка
    }
    int count = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    fclose(fp);
    return count;
}

TrapFileData* fetchTrapFileData(int* count) {
    create_data_dir();
    *count = count_lines(TRAP_FILE_DB);
    if (*count == 0) return NULL;

    TrapFileData* data = (TrapFileData*)malloc(*count * sizeof(TrapFileData));
    if (data == NULL) {
        perror("Ошибка выделения памяти для данных файлов-ловушек");
        *count = 0;
        return NULL;
    }

    FILE* fp = fopen(TRAP_FILE_DB, "r");
    if (fp == NULL) {
        perror("Ошибка открытия файла базы данных файлов-ловушек для чтения");
        free(data);
        *count = 0;
        return NULL;
    }

    char line[512]; // Буфер для строки. Предполагаем, что строка не превысит 512 символов.
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL && i < *count) {
        // Парсим CSV строку
        char* token;
        token = strtok(line, ",");
        if (token) data[i].filePath = strdup(token);
        token = strtok(NULL, ",");
        if (token) data[i].creationTime = strdup(token);
        token = strtok(NULL, "\n"); // Используем \n как разделитель для последнего поля
        if (token) data[i].lastAccessTime = strdup(token);
        i++;
    }
    fclose(fp);
    return data;
}

ActivityData* fetchActivityData(int* count) {
    create_data_dir();
    *count = count_lines(ACTIVITY_DB);
    if (*count == 0) return NULL;

    ActivityData* data = (ActivityData*)malloc(*count * sizeof(ActivityData));
    if (data == NULL) {
        perror("Ошибка выделения памяти для данных активности");
        *count = 0;
        return NULL;
    }

    FILE* fp = fopen(ACTIVITY_DB, "r");
    if (fp == NULL) {
        perror("Ошибка открытия файла базы данных активности для чтения");
        free(data);
        *count = 0;
        return NULL;
    }

    char line[512]; // Буфер для строки
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL && i < *count) {
        // Парсим CSV строку
        char* token;
        token = strtok(line, ",");
        if (token) data[i].filePath = strdup(token);
        token = strtok(NULL, ",");
        if (token) data[i].activityType = strdup(token);
        token = strtok(NULL, "\n"); // Используем \n как разделитель для последнего поля
        if (token) data[i].timestamp = strdup(token);
        i++;
    }
    fclose(fp);
    return data;
} 