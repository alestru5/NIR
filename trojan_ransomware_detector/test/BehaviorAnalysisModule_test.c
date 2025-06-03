#include "../include/BehaviorAnalysisModule.h"
#include "../include/Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Вспомогательные функции для тестов
ActivityData* createTestActivityData(int count) {
    ActivityData* data = (ActivityData*)malloc(count * sizeof(ActivityData));
    for (int i = 0; i < count; i++) {
        data[i].filePath = strdup("test_file.txt");
        data[i].activityType = strdup("write");
    }
    return data;
}

void freeTestActivityData(ActivityData* data, int count) {
    for (int i = 0; i < count; i++) {
        free(data[i].filePath);
        free(data[i].activityType);
    }
    free(data);
}

// Тесты для fetchDataFromDB
void test_fetchDataFromDB() {
    printf("Тестирование fetchDataFromDB...\n");
    int count;
    ActivityData* data = fetchDataFromDB(&count);
    assert(data != NULL);
    assert(count >= 0);
    printf("Тест fetchDataFromDB пройден успешно\n");
}

// Тесты для analyzeBehavior
void test_analyzeBehavior_normal() {
    printf("Тестирование analyzeBehavior с нормальными данными...\n");
    int count = 3;
    ActivityData* data = createTestActivityData(count);
    
    AnalysisResult result = analyzeBehavior(data, count);
    
    assert(result.analyzedData == data);
    assert(result.analyzedDataCount == count);
    assert(result.suspiciousPatternsCount == 0);
    
    freeTestActivityData(data, count);
    printf("Тест analyzeBehavior_normal пройден успешно\n");
}

void test_analyzeBehavior_suspicious() {
    printf("Тестирование analyzeBehavior с подозрительными данными...\n");
    int count = 10;
    ActivityData* data = createTestActivityData(count);
    
    AnalysisResult result = analyzeBehavior(data, count);
    
    assert(result.analyzedData == data);
    assert(result.analyzedDataCount == count);
    assert(result.suspiciousPatternsCount > 0);
    
    freeTestActivityData(data, count);
    printf("Тест analyzeBehavior_suspicious пройден успешно\n");
}

// Тесты для detectAnomalies
void test_detectAnomalies() {
    printf("Тестирование detectAnomalies...\n");
    int count = 10;
    ActivityData* data = createTestActivityData(count);
    
    AnalysisResult result = analyzeBehavior(data, count);
    int anomaliesCount;
    Anomaly* anomalies = detectAnomalies(result, &anomaliesCount);
    
    assert(anomalies != NULL);
    assert(anomaliesCount > 0);
    
    // Проверяем структуру аномалий
    for (int i = 0; i < anomaliesCount; i++) {
        assert(anomalies[i].description != NULL);
        assert(anomalies[i].severity != NULL);
        assert(strcmp(anomalies[i].severity, "Высокая") == 0);
    }
    
    freeTestActivityData(data, count);
    printf("Тест detectAnomalies пройден успешно\n");
}

// Тесты для notifyResponseModule
void test_notifyResponseModule() {
    printf("Тестирование notifyResponseModule...\n");
    int count = 1;
    Anomaly* anomalies = (Anomaly*)malloc(count * sizeof(Anomaly));
    anomalies[0].description = strdup("Test anomaly");
    anomalies[0].severity = strdup("Высокая");
    anomalies[0].relatedFilePath = strdup("test_file.txt");
    anomalies[0].relatedActivityType = strdup("write");
    
    notifyResponseModule(anomalies, count);
    
    // Освобождаем память
    free(anomalies[0].description);
    free(anomalies[0].severity);
    free(anomalies[0].relatedFilePath);
    free(anomalies[0].relatedActivityType);
    free(anomalies);
    
    printf("Тест notifyResponseModule пройден успешно\n");
}

int main() {
    printf("Запуск тестов для BehaviorAnalysisModule...\n");
    
    test_fetchDataFromDB();
    test_analyzeBehavior_normal();
    test_analyzeBehavior_suspicious();
    test_detectAnomalies();
    test_notifyResponseModule();
    
    printf("Все тесты пройдены успешно!\n");
    return 0;
} 