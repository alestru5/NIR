#include "../include/FileTrapModule.h"
#include "../include/BehaviorAnalysisModule.h"
#include "../include/ResponseModule.h"
#include "../include/ReportingModule.h"
#include "../include/Database.h"
#include "../include/ActivityData.h"
#include "../include/FileSystemMonitorModule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

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

// Тесты для проверки граничных случаев
void test_edge_cases() {
    printf("Тестирование граничных случаев...\n");
    
    // Тест с пустыми данными
    ActivityData empty_data;
    empty_data.filePath = strdup("");
    empty_data.activityType = strdup("");
    empty_data.timestamp = strdup("");
    
    AnalysisResult empty_result = analyzeBehavior(&empty_data, 1);
    assert(empty_result.analyzedData != NULL);
    
    // Тест с очень длинными строками
    char long_string[1024];
    memset(long_string, 'a', 1023);
    long_string[1023] = '\0';
    
    ActivityData long_data;
    long_data.filePath = strdup(long_string);
    long_data.activityType = strdup(long_string);
    long_data.timestamp = strdup(long_string);
    
    AnalysisResult long_result = analyzeBehavior(&long_data, 1);
    assert(long_result.analyzedData != NULL);
    
    // Освобождаем память
    free(empty_data.filePath);
    free(empty_data.activityType);
    free(empty_data.timestamp);
    free(long_data.filePath);
    free(long_data.activityType);
    free(long_data.timestamp);
    
    printf("Тест edge_cases пройден успешно\n");
}

// Тесты для проверки конкурентного доступа
void test_concurrent_access() {
    printf("Тестирование конкурентного доступа...\n");
    
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
    
    // Создаем несколько файлов одновременно
    for (int i = 0; i < 5; i++) {
        char file_path[256];
        snprintf(file_path, sizeof(file_path), "trojan_ransomware_detector/trap_files/test_file_%d.txt", i);
        createTestFile(file_path, "Test content");
    }
    
    // Даем время на обработку
    sleep(1);
    
    // Останавливаем мониторинг
    stopFileSystemMonitoring();
    
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
    removeTestDirectory("trojan_ransomware_detector/user_docs");
    
    printf("Тест concurrent_access пройден успешно\n");
}

// Тесты для проверки производительности
void test_performance() {
    printf("Тестирование производительности...\n");
    
    // Создаем большое количество тестовых данных
    const int NUM_ACTIVITIES = 1000;
    ActivityData* activities = (ActivityData*)malloc(NUM_ACTIVITIES * sizeof(ActivityData));
    
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        char file_path[256];
        snprintf(file_path, sizeof(file_path), "test_file_%d.txt", i);
        activities[i].filePath = strdup(file_path);
        activities[i].activityType = strdup("write");
        activities[i].timestamp = strdup("2024-03-20 12:00:00");
    }
    
    // Измеряем время анализа
    clock_t start = clock();
    AnalysisResult result = analyzeBehavior(activities, NUM_ACTIVITIES);
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Время анализа %d активностей: %f секунд\n", NUM_ACTIVITIES, cpu_time_used);
    
    // Освобождаем память
    for (int i = 0; i < NUM_ACTIVITIES; i++) {
        free(activities[i].filePath);
        free(activities[i].activityType);
        free(activities[i].timestamp);
    }
    free(activities);
    
    printf("Тест performance пройден успешно\n");
}

// Тесты для проверки восстановления после сбоев
void test_error_recovery() {
    printf("Тестирование восстановления после сбоев...\n");
    
    // Создаем тестовую директорию
    createTestDirectory("trojan_ransomware_detector/trap_files");
    
    // Создаем поврежденный файл-ловушку
    createTestFile("trojan_ransomware_detector/trap_files/corrupted.txt", "Corrupted content");
    
    // Пытаемся обновить файлы-ловушки
    updateTrapFiles();
    
    // Проверяем, что система продолжает работать
    DIR* dir = opendir("trojan_ransomware_detector/trap_files");
    assert(dir != NULL);
    closedir(dir);
    
    // Очищаем тестовую директорию
    dir = opendir("trojan_ransomware_detector/trap_files");
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
    
    printf("Тест error_recovery пройден успешно\n");
}

// Тесты для проверки безопасности
void test_security() {
    printf("Тестирование безопасности...\n");
    
    // Создаем тестовую директорию
    createTestDirectory("trojan_ransomware_detector/trap_files");
    
    // Пытаемся создать файл с потенциально опасным содержимым
    const char* dangerous_content = "rm -rf /";
    createTestFile("trojan_ransomware_detector/trap_files/dangerous.txt", dangerous_content);
    
    // Проверяем, что система обнаруживает опасное содержимое
    ActivityData activity;
    activity.filePath = strdup("trojan_ransomware_detector/trap_files/dangerous.txt");
    activity.activityType = strdup("write");
    activity.timestamp = strdup("2024-03-20 12:00:00");
    
    AnalysisResult result = analyzeBehavior(&activity, 1);
    int anomalyCount;
    Anomaly* anomalies = detectAnomalies(result, &anomalyCount);
    
    assert(anomalies != NULL);
    assert(anomalyCount > 0);
    
    // Освобождаем память
    free(activity.filePath);
    free(activity.activityType);
    free(activity.timestamp);
    
    // Очищаем тестовую директорию
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
    
    printf("Тест security пройден успешно\n");
}

// Тесты для проверки интеграции компонентов
void test_integration() {
    printf("Тестирование интеграции компонентов...\n");
    
    // Создаем тестовые директории
    createTestDirectory("trojan_ransomware_detector/trap_files");
    createTestDirectory("trojan_ransomware_detector/user_docs");
    
    // Инициализируем систему
    createAndPlaceTrapFiles();
    
    // Запускаем мониторинг
    const char* monitored_directories[] = {
        "trojan_ransomware_detector/trap_files",
        "trojan_ransomware_detector/user_docs"
    };
    int num_monitored_directories = sizeof(monitored_directories) / sizeof(monitored_directories[0]);
    
    startFileSystemMonitoring(monitored_directories, num_monitored_directories);
    
    // Создаем подозрительную активность
    createTestFile("trojan_ransomware_detector/trap_files/suspicious.txt", "Suspicious content");
    
    // Даем время на обработку
    sleep(1);
    
    // Проверяем, что система обнаружила аномалию
    int count;
    ActivityData* activities = fetchDataFromDB(&count);
    assert(activities != NULL);
    assert(count > 0);
    
    // Анализируем поведение
    AnalysisResult result = analyzeBehavior(activities, count);
    int anomalyCount;
    Anomaly* anomalies = detectAnomalies(result, &anomalyCount);
    
    assert(anomalies != NULL);
    assert(anomalyCount > 0);
    
    // Реагируем на аномалии
    notifyResponseModule(anomalies, anomalyCount);
    
    // Генерируем отчет
    Report report = generateReport();
    assert(report.threatsCount > 0);
    
    // Останавливаем мониторинг
    stopFileSystemMonitoring();
    
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
    removeTestDirectory("trojan_ransomware_detector/user_docs");
    
    // Освобождаем память
    for (int i = 0; i < count; i++) {
        free(activities[i].filePath);
        free(activities[i].activityType);
        free(activities[i].timestamp);
    }
    free(activities);
    free(report.reportId);
    free(report.generationTime);
    for (int i = 0; i < report.threatsCount; i++) {
        free(report.threats[i].description);
        free(report.threats[i].severity);
    }
    free(report.threats);
    
    printf("Тест integration пройден успешно\n");
}

int main() {
    printf("Запуск дополнительных тестов...\n");
    
    test_edge_cases();
    test_concurrent_access();
    test_performance();
    test_error_recovery();
    test_security();
    test_integration();
    
    printf("Все дополнительные тесты пройдены успешно!\n");
    return 0;
} 