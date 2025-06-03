#ifndef ANOMALY_H
#define ANOMALY_H

#include <stdbool.h>
#include <stdio.h> // Для типа FILE в реальных сценариях, пока не используется

// Определение структуры Anomaly
typedef struct {
    char* description; // Описание аномального паттерна
    char* severity; // Уровень серьезности (например, "Низкая", "Средняя", "Высокая")
    
    // Добавляем поля для связывания аномалии с файлами/операциями
    char* relatedFilePath; // Путь к файлу, с которым связана аномалия (если применимо)
    char* relatedActivityType; // Тип активности, вызвавшей аномалию (если применимо)
    // В реальной системе здесь может быть список затронутых файлов, идентификатор процесса и т.д.

} Anomaly;

// Функция для освобождения памяти, выделенной для структуры Anomaly
void freeAnomaly(Anomaly* anomaly);

// Функция для освобождения памяти, выделенной для массива структур Anomaly
void freeAnomalyArray(Anomaly* anomalies, int count);

#endif // ANOMALY_H 