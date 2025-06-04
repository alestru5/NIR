#include "../include/Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include "test_common.h"

#define DATA_DIR "trojan_ransomware_detector/data"

// Вспомогательные функции для тестов
void createTestDirectory(const char* path) {
    mkdir(path, 0755);
}

void removeTestDirectory(const char* path) {
    rmdir(path);
}

void createTestFile(const char* path, const char* content) {
    FILE* fp = fopen(path, "w");
    if (fp != NULL) {
        fputs(content, fp);
        fclose(fp);
    }
}

void removeTestFile(const char* path) {
    remove(path);
}

// Тесты для storeTrapFileData
void test_storeTrapFileData() {
    printf("Тестирование storeTrapFileData...\n");
    
    // Создаем тестовую директорию
    createTestDirectory(DATA_DIR);
    
    // Создаем тестовые данные
    TrapFileData data;
    data.filePath = strdup("test_file.txt");
    data.creationTime = strdup("2024-03-20 12:00:00");
    data.lastAccessTime = strdup("2024-03-20 12:00:00");
    
    // Сохраняем данные
    storeTrapFileData(data);
    
    // Проверяем, что данные сохранены
    int count;
    TrapFileData* fetched_data = fetchTrapFileData(&count);
    assert(fetched_data != NULL);
    assert(count == 1);
    assert(strcmp(fetched_data[0].filePath, "test_file.txt") == 0);
    assert(strcmp(fetched_data[0].creationTime, "2024-03-20 12:00:00") == 0);
    assert(strcmp(fetched_data[0].lastAccessTime, "2024-03-20 12:00:00") == 0);
    
    // Освобождаем память
    free(data.filePath);
    free(data.creationTime);
    free(data.lastAccessTime);
    freeTrapFileData(fetched_data, count);
    
    // Удаляем тестовые файлы
    removeTestFile("trojan_ransomware_detector/data/trap_files.csv");
    removeTestDirectory(DATA_DIR);
    
    printf("Тест storeTrapFileData пройден успешно\n");
}

// Тесты для storeActivityData
void test_storeActivityData() {
    printf("Тестирование storeActivityData...\n");
    
    // Создаем тестовую директорию
    createTestDirectory(DATA_DIR);
    
    // Создаем тестовые данные
    ActivityData data;
    data.filePath = strdup("test_file.txt");
    data.activityType = strdup("write");
    data.timestamp = strdup("2024-03-20 12:00:00");
    
    // Сохраняем данные
    storeActivityData(data);
    
    // Проверяем, что данные сохранены
    int count;
    ActivityData* fetched_data = fetchActivityData(&count);
    assert(fetched_data != NULL);
    assert(count == 1);
    assert(strcmp(fetched_data[0].filePath, "test_file.txt") == 0);
    assert(strcmp(fetched_data[0].activityType, "write") == 0);
    assert(strcmp(fetched_data[0].timestamp, "2024-03-20 12:00:00") == 0);
    
    // Освобождаем память
    free(data.filePath);
    free(data.activityType);
    free(data.timestamp);
    freeActivityData(fetched_data, count);
    
    // Удаляем тестовые файлы
    removeTestFile("trojan_ransomware_detector/data/activities.csv");
    removeTestDirectory(DATA_DIR);
    
    printf("Тест storeActivityData пройден успешно\n");
}

// Тесты для fetchTrapFileData
void test_fetchTrapFileData_empty() {
    printf("Тестирование fetchTrapFileData с пустой базой данных...\n");
    
    // Создаем тестовую директорию
    createTestDirectory(DATA_DIR);
    
    int count;
    TrapFileData* data = fetchTrapFileData(&count);
    assert(data == NULL);
    assert(count == 0);
    
    // Удаляем тестовую директорию
    removeTestDirectory(DATA_DIR);
    
    printf("Тест fetchTrapFileData_empty пройден успешно\n");
}

void test_fetchTrapFileData_with_data() {
    printf("Тестирование fetchTrapFileData с данными...\n");
    
    // Создаем тестовую директорию
    createTestDirectory(DATA_DIR);
    
    // Создаем тестовые данные
    const char* test_data = "test_file.txt,2024-03-20 12:00:00,2024-03-20 12:00:00\n";
    createTestFile("trojan_ransomware_detector/data/trap_files.csv", test_data);
    
    int count;
    TrapFileData* data = fetchTrapFileData(&count);
    assert(data != NULL);
    assert(count == 1);
    assert(strcmp(data[0].filePath, "test_file.txt") == 0);
    assert(strcmp(data[0].creationTime, "2024-03-20 12:00:00") == 0);
    assert(strcmp(data[0].lastAccessTime, "2024-03-20 12:00:00") == 0);
    
    // Освобождаем память
    freeTrapFileData(data, count);
    
    // Удаляем тестовые файлы
    removeTestFile("trojan_ransomware_detector/data/trap_files.csv");
    removeTestDirectory(DATA_DIR);
    
    printf("Тест fetchTrapFileData_with_data пройден успешно\n");
}

// Тесты для fetchActivityData
void test_fetchActivityData_empty() {
    printf("Тестирование fetchActivityData с пустой базой данных...\n");
    
    // Создаем тестовую директорию
    createTestDirectory(DATA_DIR);
    
    int count;
    ActivityData* data = fetchActivityData(&count);
    assert(data == NULL);
    assert(count == 0);
    
    // Удаляем тестовую директорию
    removeTestDirectory(DATA_DIR);
    
    printf("Тест fetchActivityData_empty пройден успешно\n");
}

void test_fetchActivityData_with_data() {
    printf("Тестирование fetchActivityData с данными...\n");
    
    // Создаем тестовую директорию
    createTestDirectory(DATA_DIR);
    
    // Создаем тестовые данные
    const char* test_data = "test_file.txt,write,2024-03-20 12:00:00\n";
    createTestFile("trojan_ransomware_detector/data/activities.csv", test_data);
    
    int count;
    ActivityData* data = fetchActivityData(&count);
    assert(data != NULL);
    assert(count == 1);
    assert(strcmp(data[0].filePath, "test_file.txt") == 0);
    assert(strcmp(data[0].activityType, "write") == 0);
    assert(strcmp(data[0].timestamp, "2024-03-20 12:00:00") == 0);
    
    // Освобождаем память
    freeActivityData(data, count);
    
    // Удаляем тестовые файлы
    removeTestFile("trojan_ransomware_detector/data/activities.csv");
    removeTestDirectory(DATA_DIR);
    
    printf("Тест fetchActivityData_with_data пройден успешно\n");
}

int main() {
    printf("Запуск тестов для Database...\n");
    
    test_storeTrapFileData();
    test_storeActivityData();
    test_fetchTrapFileData_empty();
    test_fetchTrapFileData_with_data();
    test_fetchActivityData_empty();
    test_fetchActivityData_with_data();
    
    printf("Все тесты для Database пройдены успешно!\n");
    return 0;
} 