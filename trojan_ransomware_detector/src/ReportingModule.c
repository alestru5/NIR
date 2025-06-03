#include "../include/ReportingModule.h"
#include "../include/Threat.h"
#include "../include/Report.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Реализация функций для модуля ReportingModule

// Временное хранилище для угроз (для эмуляции)
static Threat* collectedThreats = NULL;
static int collectedThreatsCount = 0;

// Собирает данные об угрозах.
void collectThreatData(Threat* threats, int threatsCount) {
    printf("ReportingModule: Получено %d угроз.\n", threatsCount);
    // В реальной системе эти данные могут сохраняться в базе данных для последующего анализа и генерации отчетов.
    // Для эмуляции, просто добавляем их во временный список.

    // Освобождаем старую память, если она была выделена (простая реализация для эмуляции)
    if (collectedThreats != NULL) {
        for (int i = 0; i < collectedThreatsCount; i++) {
            free(collectedThreats[i].description);
            free(collectedThreats[i].severity);
        }
        free(collectedThreats);
    }

    collectedThreatsCount = threatsCount;
    collectedThreats = (Threat*)malloc(collectedThreatsCount * sizeof(Threat));
    if (collectedThreats == NULL) {
        perror("Ошибка выделения памяти для collectedThreats");
        collectedThreatsCount = 0;
        return;
    }

    for (int i = 0; i < threatsCount; i++) {
        collectedThreats[i].description = strdup(threats[i].description);
        collectedThreats[i].severity = strdup(threats[i].severity);
    }

    // В реальной системе здесь может быть вызов функции сохранения в БД: storeThreatData(threats, threatsCount);
}

// Генерирует отчет на основе собранных данных об угрозах.
Report generateReport() {
    printf("ReportingModule: Генерация отчета...\n");
    Report report;
    report.reportId = strdup("REPORT_ID_123"); // Фиктивный ID
    report.generationTime = strdup("текущее время генерации"); // Заглушка

    // Передаем собранные угрозы в отчет (для примера, копируем)
    report.threatsCount = collectedThreatsCount;
    report.threats = (Threat*)malloc(report.threatsCount * sizeof(Threat));
     if (report.threats == NULL) {
        perror("Ошибка выделения памяти для report.threats");
        report.threatsCount = 0;
        // Возвращаем отчет с нулевыми угрозами, но с ID и временем
        return report;
    }

     for (int i = 0; i < report.threatsCount; i++) {
        report.threats[i].description = strdup(collectedThreats[i].description);
        report.threats[i].severity = strdup(collectedThreats[i].severity);
    }

    printf("Отчет сгенерирован: ID=%s, Время=%s, Угроз=%d\n", report.reportId, report.generationTime, report.threatsCount);
    return report;
}

// Экспортирует отчет в указанном формате.
void exportReport(Report report, char* format) {
    printf("ReportingModule: Экспорт отчета ID=%s в формате %s...\n", report.reportId, format);
    // В реальной системе здесь будет логика форматирования отчета (например, в PDF, CSV) 
    // и сохранение его в файл или отправка по сети.
    printf("Содержание отчета (эмуляция):\n");
    printf("  ID: %s\n", report.reportId);
    printf("  Время генерации: %s\n", report.generationTime);
    printf("  Угрозы (%d):\n", report.threatsCount);
    for (int i = 0; i < report.threatsCount; i++) {
        printf("    - Описание: %s, Серьезность: %s\n", report.threats[i].description, report.threats[i].severity);
    }

    // Освобождение памяти, выделенной для отчета
    free(report.reportId);
    free(report.generationTime);
    if (report.threats != NULL) {
        for (int i = 0; i < report.threatsCount; i++) {
            free(report.threats[i].description);
            free(report.threats[i].severity);
        }
        free(report.threats);
    }
    // Внимание: collectedThreats остается, его нужно освободить отдельно, возможно, при завершении работы программы
}

// Функция для освобождения памяти, выделенной для собранных угроз
void freeCollectedThreats() {
    if (collectedThreats != NULL) {
        for (int i = 0; i < collectedThreatsCount; i++) {
            free(collectedThreats[i].description);
            free(collectedThreats[i].severity);
        }
        free(collectedThreats);
        collectedThreats = NULL;
        collectedThreatsCount = 0;
    }
} 