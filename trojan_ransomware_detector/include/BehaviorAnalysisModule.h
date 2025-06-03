#ifndef BEHAVIORANALYSISMODULE_H
#define BEHAVIORANALYSISMODULE_H

#include "ActivityData.h"
#include "AnalysisResult.h"
#include "Anomaly.h"
#include <stdbool.h>

// Прототипы функций для модуля BehaviorAnalysisModule

/**
 * @brief Извлекает данные об активности из базы данных.
 * @return Список данных об активности.
 */
ActivityData* fetchDataFromDB(int* count); // Возвращает динамический массив ActivityData и его размер через указатель

/**
 * @brief Анализирует поведенческие данные.
 * @param data Список данных об активности.
 * @param dataCount Количество данных об активности.
 * @return Результат анализа.
 */
AnalysisResult analyzeBehavior(ActivityData* data, int dataCount);

/**
 * @brief Обнаруживает аномалии на основе результата анализа.
 * @param result Результат анализа.
 * @return Список обнаруженных аномалий.
 */
Anomaly* detectAnomalies(AnalysisResult result, int* count); // Возвращает динамический массив Anomaly и его размер через указатель

/**
 * @brief Уведомляет модуль ResponseModule об обнаруженных аномалиях.
 * @param anomalies Список аномалий.
 * @param anomaliesCount Количество аномалий.
 */
void notifyResponseModule(Anomaly* anomalies, int anomaliesCount);

#endif // BEHAVIORANALYSISMODULE_H 