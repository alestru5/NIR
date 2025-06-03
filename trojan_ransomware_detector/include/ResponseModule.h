#ifndef RESPONSEMODULE_H
#define RESPONSEMODULE_H

#include "Anomaly.h"
#include "Alert.h"
#include <stdbool.h>

// Прототипы функций для модуля ResponseModule

/**
 * @brief Получает уведомление об аномалиях.
 * @param anomalies Список аномалий.
 * @param anomaliesCount Количество аномалий.
 */
void receiveNotification(Anomaly* anomalies, int anomaliesCount);

/**
 * @brief Изолирует зараженные файлы.
 * @param fileList Список путей к файлам для изоляции.
 * @param fileListCount Количество файлов.
 */
void isolateInfectedFiles(char** fileList, int fileListCount);

/**
 * @brief Уведомляет администратора о тревоге.
 * @param alert Данные о тревоге.
 */
void notifyAdministrator(Alert alert);

#endif // RESPONSEMODULE_H 