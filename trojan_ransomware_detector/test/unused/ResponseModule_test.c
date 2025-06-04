#include "../include/ResponseModule.h"
#include "../include/Alert.h"
#include "../include/Anomaly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Вспомогательные функции для тестов
Anomaly* createTestAnomaly(const char* description, const char* severity, const char* filePath, const char* activityType) {
    Anomaly* anomaly = (Anomaly*)malloc(sizeof(Anomaly));
    anomaly->description = strdup(description);
    anomaly->severity = strdup(severity);
    anomaly->relatedFilePath = strdup(filePath);
    anomaly->relatedActivityType = strdup(activityType);
    return anomaly;
}

void freeTestAnomaly(Anomaly* anomaly) {
    free(anomaly->description);
    free(anomaly->severity);
    free(anomaly->relatedFilePath);
    free(anomaly->relatedActivityType);
    free(anomaly);
}

// Тесты для receiveNotification
void test_receiveNotification_empty() {
    printf("Тестирование receiveNotification с пустым списком аномалий...\n");
    receiveNotification(NULL, 0);
    printf("Тест receiveNotification_empty пройден успешно\n");
}

void test_receiveNotification_normal() {
    printf("Тестирование receiveNotification с нормальными данными...\n");
    Anomaly* anomalies = createTestAnomaly(
        "Нормальная аномалия",
        "Низкая",
        "test_file.txt",
        "read"
    );
    
    receiveNotification(anomalies, 1);
    printf("Тест receiveNotification_normal пройден успешно\n");
}

void test_receiveNotification_ransomware() {
    printf("Тестирование receiveNotification с данными о шифровальщике...\n");
    Anomaly* anomalies = createTestAnomaly(
        "Обнаружено массовое шифрование файлов",
        "Высокая",
        "important_file.txt",
        "write"
    );
    
    receiveNotification(anomalies, 1);
    printf("Тест receiveNotification_ransomware пройден успешно\n");
}

// Тесты для isolateInfectedFiles
void test_isolateInfectedFiles() {
    printf("Тестирование isolateInfectedFiles...\n");
    char* files[] = {"infected_file1.txt", "infected_file2.txt"};
    isolateInfectedFiles(files, 2);
    printf("Тест isolateInfectedFiles пройден успешно\n");
}

// Тесты для notifyAdministrator
void test_notifyAdministrator() {
    printf("Тестирование notifyAdministrator...\n");
    Alert alert;
    alert.message = strdup("Test alert message");
    alert.timestamp = strdup("2024-03-20 12:00:00");
    
    notifyAdministrator(alert);
    
    free(alert.message);
    free(alert.timestamp);
    printf("Тест notifyAdministrator пройден успешно\n");
}

int main() {
    printf("Запуск тестов для ResponseModule...\n");
    
    test_receiveNotification_empty();
    test_receiveNotification_normal();
    test_receiveNotification_ransomware();
    test_isolateInfectedFiles();
    test_notifyAdministrator();
    
    printf("Все тесты пройдены успешно!\n");
    return 0;
} 