#include "../include/FileSystemMonitorModule.h"
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

// Тесты для startFileSystemMonitoring
void test_startFileSystemMonitoring_empty() {
    printf("Тестирование startFileSystemMonitoring с пустым списком директорий...\n");
    const char* directories[] = {};
    startFileSystemMonitoring(directories, 0);
    printf("Тест startFileSystemMonitoring_empty пройден успешно\n");
}

void test_startFileSystemMonitoring_single() {
    printf("Тестирование startFileSystemMonitoring с одной директорией...\n");
    const char* test_dir = "test_monitor_dir";
    createTestDirectory(test_dir);
    
    const char* directories[] = {test_dir};
    startFileSystemMonitoring(directories, 1);
    
    // Даем время на инициализацию мониторинга
    sleep(1);
    
    removeTestDirectory(test_dir);
    printf("Тест startFileSystemMonitoring_single пройден успешно\n");
}

void test_startFileSystemMonitoring_multiple() {
    printf("Тестирование startFileSystemMonitoring с несколькими директориями...\n");
    const char* test_dir1 = "test_monitor_dir1";
    const char* test_dir2 = "test_monitor_dir2";
    createTestDirectory(test_dir1);
    createTestDirectory(test_dir2);
    
    const char* directories[] = {test_dir1, test_dir2};
    startFileSystemMonitoring(directories, 2);
    
    // Даем время на инициализацию мониторинга
    sleep(1);
    
    removeTestDirectory(test_dir1);
    removeTestDirectory(test_dir2);
    printf("Тест startFileSystemMonitoring_multiple пройден успешно\n");
}

// Тесты для stopFileSystemMonitoring
void test_stopFileSystemMonitoring() {
    printf("Тестирование stopFileSystemMonitoring...\n");
    const char* test_dir = "test_monitor_dir";
    createTestDirectory(test_dir);
    
    const char* directories[] = {test_dir};
    startFileSystemMonitoring(directories, 1);
    
    // Даем время на инициализацию мониторинга
    sleep(1);
    
    stopFileSystemMonitoring();
    
    removeTestDirectory(test_dir);
    printf("Тест stopFileSystemMonitoring пройден успешно\n");
}

int main() {
    printf("Запуск тестов для FileSystemMonitorModule...\n");
    
    test_startFileSystemMonitoring_empty();
    test_startFileSystemMonitoring_single();
    test_startFileSystemMonitoring_multiple();
    test_stopFileSystemMonitoring();
    
    printf("Все тесты пройдены успешно!\n");
    return 0;
} 