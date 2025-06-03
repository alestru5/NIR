#include "../include/FileTrapModule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>

// Вспомогательные функции для тестов
void createTestDirectory(const char* path) {
    mkdir(path, 0755);
}

void removeTestDirectory(const char* path) {
    rmdir(path);
}

// Тесты для createSingleTrapFile
void test_createSingleTrapFile() {
    printf("Тестирование createSingleTrapFile...\n");
    const char* test_dir = "test_trap_dir";
    createTestDirectory(test_dir);
    
    const char* file_path = "test_trap_dir/test_trap.txt";
    const char* content = "Test trap file content";
    
    createSingleTrapFile(file_path, content);
    
    // Проверяем, что файл создан
    FILE* fp = fopen(file_path, "r");
    assert(fp != NULL);
    
    char buffer[256];
    fgets(buffer, sizeof(buffer), fp);
    assert(strcmp(buffer, content) == 0);
    
    fclose(fp);
    remove(file_path);
    removeTestDirectory(test_dir);
    printf("Тест createSingleTrapFile пройден успешно\n");
}

// Тесты для createAndPlaceTrapFiles
void test_createAndPlaceTrapFiles() {
    printf("Тестирование createAndPlaceTrapFiles...\n");
    
    // Создаем тестовые директории
    for (int i = 0; i < NUM_DIRECTORIES; i++) {
        createTestDirectory(trap_directories[i]);
    }
    
    createAndPlaceTrapFiles();
    
    // Проверяем, что файлы созданы
    int files_found = 0;
    for (int i = 0; i < NUM_DIRECTORIES; i++) {
        DIR* dir = opendir(trap_directories[i]);
        if (dir != NULL) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_type == DT_REG) {
                    files_found++;
                }
            }
            closedir(dir);
        }
    }
    
    assert(files_found > 0);
    
    // Очищаем тестовые директории
    for (int i = 0; i < NUM_DIRECTORIES; i++) {
        DIR* dir = opendir(trap_directories[i]);
        if (dir != NULL) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_type == DT_REG) {
                    char file_path[512];
                    snprintf(file_path, sizeof(file_path), "%s/%s", trap_directories[i], entry->d_name);
                    remove(file_path);
                }
            }
            closedir(dir);
        }
        removeTestDirectory(trap_directories[i]);
    }
    
    printf("Тест createAndPlaceTrapFiles пройден успешно\n");
}

// Тесты для monitorTrapFiles
void test_monitorTrapFiles() {
    printf("Тестирование monitorTrapFiles...\n");
    monitorTrapFiles();
    printf("Тест monitorTrapFiles пройден успешно\n");
}

// Тесты для updateTrapFiles
void test_updateTrapFiles() {
    printf("Тестирование updateTrapFiles...\n");
    
    // Создаем тестовые директории
    for (int i = 0; i < NUM_DIRECTORIES; i++) {
        createTestDirectory(trap_directories[i]);
    }
    
    // Создаем несколько файлов-ловушек
    createAndPlaceTrapFiles();
    
    // Обновляем файлы-ловушки
    updateTrapFiles();
    
    // Очищаем тестовые директории
    for (int i = 0; i < NUM_DIRECTORIES; i++) {
        DIR* dir = opendir(trap_directories[i]);
        if (dir != NULL) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_type == DT_REG) {
                    char file_path[512];
                    snprintf(file_path, sizeof(file_path), "%s/%s", trap_directories[i], entry->d_name);
                    remove(file_path);
                }
            }
            closedir(dir);
        }
        removeTestDirectory(trap_directories[i]);
    }
    
    printf("Тест updateTrapFiles пройден успешно\n");
}

// Тесты для logActivity
void test_logActivity() {
    printf("Тестирование logActivity...\n");
    char* activity = "Test activity log";
    logActivity(activity);
    printf("Тест logActivity пройден успешно\n");
}

// Тесты для saveDataToDB_FileTrap
void test_saveDataToDB_FileTrap() {
    printf("Тестирование saveDataToDB_FileTrap...\n");
    TrapFileData data;
    data.filePath = strdup("test_file.txt");
    data.creationTime = strdup("2024-03-20 12:00:00");
    data.lastAccessTime = strdup("2024-03-20 12:00:00");
    
    saveDataToDB_FileTrap(data);
    
    free(data.filePath);
    free(data.creationTime);
    free(data.lastAccessTime);
    printf("Тест saveDataToDB_FileTrap пройден успешно\n");
}

// Тесты для saveDataToDB_Activity
void test_saveDataToDB_Activity() {
    printf("Тестирование saveDataToDB_Activity...\n");
    ActivityData data;
    data.filePath = strdup("test_file.txt");
    data.activityType = strdup("write");
    data.timestamp = strdup("2024-03-20 12:00:00");
    
    saveDataToDB_Activity(data);
    
    free(data.filePath);
    free(data.activityType);
    free(data.timestamp);
    printf("Тест saveDataToDB_Activity пройден успешно\n");
}

int main() {
    printf("Запуск тестов для FileTrapModule...\n");
    
    test_createSingleTrapFile();
    test_createAndPlaceTrapFiles();
    test_monitorTrapFiles();
    test_updateTrapFiles();
    test_logActivity();
    test_saveDataToDB_FileTrap();
    test_saveDataToDB_Activity();
    
    printf("Все тесты пройдены успешно!\n");
    return 0;
} 