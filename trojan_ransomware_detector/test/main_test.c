#include "../include/FileTrapModule.h"
#include "../include/BehaviorAnalysisModule.h"
#include "../include/ResponseModule.h"
#include "../include/ReportingModule.h"
#include "../include/Database.h"
#include "../include/ActivityData.h"
#include "../include/FileSystemMonitorModule.h"
#include "test_common.h"
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

// Тесты для инициализации системы
void test_system_initialization() {
    printf("Тестирование инициализации системы...\n");
    
    // Создаем тестовые директории
    createTestDirectory("trojan_ransomware_detector/user_docs");
    
    // Создаем и размещаем файлы-ловушки
    createAndPlaceTrapFiles();
    
    // Проверяем, что файлы-ловушки созданы в user_docs
    DIR* dir = opendir("trojan_ransomware_detector/user_docs");
    assert(dir != NULL);
    int files_found = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            files_found++;
        }
    }
    closedir(dir);
    assert(files_found > 0);
    
    // Очищаем тестовые директории
    dir = opendir("trojan_ransomware_detector/user_docs");
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                char file_path[512];
                snprintf(file_path, sizeof(file_path), "trojan_ransomware_detector/user_docs/%s", entry->d_name);
                remove(file_path);
            }
        }
        closedir(dir);
    }
    removeTestDirectory("trojan_ransomware_detector/user_docs");
    
    printf("Тест system_initialization пройден успешно\n");
}

// Тесты для мониторинга файловой системы
void test_file_system_monitoring() {
    printf("Тестирование мониторинга файловой системы...\n");
    
    // Создаем тестовые директории
    createTestDirectory("trojan_ransomware_detector/trap_files");
    createTestDirectory("trojan_ransomware_detector/user_docs");
    
    // Запускаем мониторинг
    const char* monitored_directories[] = {
        "trojan_ransomware_detector/trap_files",
        "trojan_ransomware_detector/user_docs"
    };
    int num_monitored_directories = sizeof(monitored_directories) / sizeof(monitored_directories[0]);
    
    startFileSystemMonitoring(monitored_directories, num_monitored_directories);
    
    // Даем время на инициализацию мониторинга
    sleep(1);
    
    // Останавливаем мониторинг
    stopFileSystemMonitoring();
    
    // Очищаем тестовые директории
    removeTestDirectory("trojan_ransomware_detector/trap_files");
    removeTestDirectory("trojan_ransomware_detector/user_docs");
    
    printf("Тест file_system_monitoring пройден успешно\n");
}

// Тесты для анализа поведения
void test_behavior_analysis() {
    printf("Тестирование анализа поведения...\n");
    
    // Создаем тестовые данные активности
    ActivityData activities[2];
    activities[0].filePath = strdup("test_file1.txt");
    activities[0].activityType = strdup("write");
    activities[0].timestamp = strdup("2024-03-20 12:00:00");
    activities[1].filePath = strdup("test_file2.txt");
    activities[1].activityType = strdup("delete");
    activities[1].timestamp = strdup("2024-03-20 12:00:01");
    
    // Анализируем поведение
    AnalysisResult result = analyzeBehavior(activities, 2);
    
    // Проверяем результаты анализа
    assert(result.analyzedData == activities);
    assert(result.analyzedDataCount == 2);
    
    // Освобождаем память
    free(activities[0].filePath);
    free(activities[0].activityType);
    free(activities[0].timestamp);
    free(activities[1].filePath);
    free(activities[1].activityType);
    free(activities[1].timestamp);
    
    printf("Тест behavior_analysis пройден успешно\n");
}

// Тесты для обнаружения аномалий
void test_anomaly_detection() {
    printf("Тестирование обнаружения аномалий...\n");
    
    // Создаем тестовые данные активности
    ActivityData activities[2];
    activities[0].filePath = strdup("test_file1.txt");
    activities[0].activityType = strdup("write");
    activities[0].timestamp = strdup("2024-03-20 12:00:00");
    activities[1].filePath = strdup("test_file2.txt");
    activities[1].activityType = strdup("delete");
    activities[1].timestamp = strdup("2024-03-20 12:00:01");
    
    // Анализируем поведение
    AnalysisResult result = analyzeBehavior(activities, 2);
    
    // Обнаруживаем аномалии
    int anomalyCount;
    Anomaly* anomalies = detectAnomalies(result, &anomalyCount);
    
    // Проверяем результаты обнаружения аномалий
    assert(anomalies != NULL);
    assert(anomalyCount > 0);
    
    // Освобождаем память
    free(activities[0].filePath);
    free(activities[0].activityType);
    free(activities[0].timestamp);
    free(activities[1].filePath);
    free(activities[1].activityType);
    free(activities[1].timestamp);
    
    printf("Тест anomaly_detection пройден успешно\n");
}

// Тесты для реагирования на аномалии
void test_anomaly_response() {
    printf("Тестирование реагирования на аномалии...\n");
    
    // Создаем тестовые данные активности
    ActivityData activities[2];
    activities[0].filePath = strdup("test_file1.txt");
    activities[0].activityType = strdup("write");
    activities[0].timestamp = strdup("2024-03-20 12:00:00");
    activities[1].filePath = strdup("test_file2.txt");
    activities[1].activityType = strdup("delete");
    activities[1].timestamp = strdup("2024-03-20 12:00:01");
    
    // Анализируем поведение
    AnalysisResult result = analyzeBehavior(activities, 2);
    
    // Обнаруживаем аномалии
    int anomalyCount;
    Anomaly* anomalies = detectAnomalies(result, &anomalyCount);
    
    // Реагируем на аномалии
    if (anomalies != NULL && anomalyCount > 0) {
        notifyResponseModule(anomalies, anomalyCount);
    }
    
    // Освобождаем память
    free(activities[0].filePath);
    free(activities[0].activityType);
    free(activities[0].timestamp);
    free(activities[1].filePath);
    free(activities[1].activityType);
    free(activities[1].timestamp);
    
    printf("Тест anomaly_response пройден успешно\n");
}

// Тесты для обновления файлов-ловушек
void test_trap_files_update() {
    printf("Тестирование обновления файлов-ловушек...\n");
    
    // Создаем тестовые директории
    createTestDirectory("trojan_ransomware_detector/trap_files");
    
    // Создаем и размещаем файлы-ловушки
    createAndPlaceTrapFiles();
    
    // Обновляем файлы-ловушки
    updateTrapFiles();
    
    // Очищаем тестовые директории
    DIR* dir = opendir("trojan_ransomware_detector/trap_files");
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                char file_path[512];
                snprintf(file_path, sizeof(file_path), "trojan_ransomware_detector/trap_files/%s", entry->d_name);
                remove(file_path);
            }
        }
        closedir(dir);
    }
    removeTestDirectory("trojan_ransomware_detector/trap_files");
    
    printf("Тест trap_files_update пройден успешно\n");
}

// Тесты для генерации и экспорта отчета
void test_report_generation_and_export() {
    printf("Тестирование генерации и экспорта отчета...\n");
    
    // Создаем тестовые данные угроз
    Threat threat;
    threat.description = strdup("Test threat");
    threat.severity = strdup("High");
    
    // Собираем данные об угрозах
    collectThreatData(&threat, 1);
    
    // Генерируем отчет
    Report report = generateReport();
    
    // Экспортируем отчет (освобождение памяти происходит внутри exportReport)
    exportReport(report, "text");
    
    // Освобождаем только threat
    free(threat.description);
    free(threat.severity);
    
    printf("Тест report_generation_and_export пройден успешно\n");
}

int main() {
    printf("Запуск тестов для main...\n");
    
    test_system_initialization();
    test_file_system_monitoring();
    test_behavior_analysis();
    test_anomaly_detection();
    test_anomaly_response();
    test_trap_files_update();
    test_report_generation_and_export();
    
    printf("Все тесты пройдены успешно!\n");
    return 0;
} 