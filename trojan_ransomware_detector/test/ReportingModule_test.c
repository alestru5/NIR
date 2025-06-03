#include "../include/ReportingModule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Вспомогательные функции для тестов
Threat* createTestThreat(const char* description, const char* severity) {
    Threat* threat = (Threat*)malloc(sizeof(Threat));
    threat->description = strdup(description);
    threat->severity = strdup(severity);
    return threat;
}

void freeTestThreat(Threat* threat) {
    free(threat->description);
    free(threat->severity);
    free(threat);
}

// Тесты для collectThreatData
void test_collectThreatData_empty() {
    printf("Тестирование collectThreatData с пустым списком угроз...\n");
    collectThreatData(NULL, 0);
    printf("Тест collectThreatData_empty пройден успешно\n");
}

void test_collectThreatData_single() {
    printf("Тестирование collectThreatData с одной угрозой...\n");
    Threat threat;
    threat.description = strdup("Test threat");
    threat.severity = strdup("High");
    
    collectThreatData(&threat, 1);
    
    free(threat.description);
    free(threat.severity);
    printf("Тест collectThreatData_single пройден успешно\n");
}

void test_collectThreatData_multiple() {
    printf("Тестирование collectThreatData с несколькими угрозами...\n");
    Threat threats[2];
    threats[0].description = strdup("Test threat 1");
    threats[0].severity = strdup("High");
    threats[1].description = strdup("Test threat 2");
    threats[1].severity = strdup("Medium");
    
    collectThreatData(threats, 2);
    
    free(threats[0].description);
    free(threats[0].severity);
    free(threats[1].description);
    free(threats[1].severity);
    printf("Тест collectThreatData_multiple пройден успешно\n");
}

// Тесты для generateReport
void test_generateReport() {
    printf("Тестирование generateReport...\n");
    
    // Сначала собираем данные об угрозах
    Threat threat;
    threat.description = strdup("Test threat");
    threat.severity = strdup("High");
    collectThreatData(&threat, 1);
    
    // Генерируем отчет
    Report report = generateReport();
    
    // Проверяем структуру отчета
    assert(report.reportId != NULL);
    assert(report.generationTime != NULL);
    assert(report.threatsCount == 1);
    assert(report.threats != NULL);
    assert(strcmp(report.threats[0].description, "Test threat") == 0);
    assert(strcmp(report.threats[0].severity, "High") == 0);
    
    // Освобождаем память
    free(threat.description);
    free(threat.severity);
    free(report.reportId);
    free(report.generationTime);
    free(report.threats[0].description);
    free(report.threats[0].severity);
    free(report.threats);
    
    printf("Тест generateReport пройден успешно\n");
}

// Тесты для exportReport
void test_exportReport() {
    printf("Тестирование exportReport...\n");
    
    // Создаем тестовый отчет
    Report report;
    report.reportId = strdup("TEST_REPORT");
    report.generationTime = strdup("2024-03-20 12:00:00");
    report.threatsCount = 1;
    report.threats = (Threat*)malloc(sizeof(Threat));
    report.threats[0].description = strdup("Test threat");
    report.threats[0].severity = strdup("High");
    
    // Экспортируем отчет
    exportReport(report, "text");
    
    printf("Тест exportReport пройден успешно\n");
}

// Тесты для freeCollectedThreats
void test_freeCollectedThreats() {
    printf("Тестирование freeCollectedThreats...\n");
    
    // Сначала собираем данные об угрозах
    Threat threat;
    threat.description = strdup("Test threat");
    threat.severity = strdup("High");
    collectThreatData(&threat, 1);
    
    // Освобождаем память
    freeCollectedThreats();
    
    free(threat.description);
    free(threat.severity);
    printf("Тест freeCollectedThreats пройден успешно\n");
}

int main() {
    printf("Запуск тестов для ReportingModule...\n");
    
    test_collectThreatData_empty();
    test_collectThreatData_single();
    test_collectThreatData_multiple();
    test_generateReport();
    test_exportReport();
    test_freeCollectedThreats();
    
    printf("Все тесты пройдены успешно!\n");
    return 0;
} 