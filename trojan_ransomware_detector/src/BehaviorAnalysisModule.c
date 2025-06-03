#include "../include/BehaviorAnalysisModule.h"
#include "../include/Database.h" // Для извлечения данных из БД
#include "../include/ResponseModule.h" // Для уведомления ResponseModule
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Реализация функций для модуля BehaviorAnalysisModule

// Извлекает данные об активности из базы данных.
ActivityData* fetchDataFromDB(int* count) {
    printf("Извлечение данных об активности из БД...\n");
    // Вызов реальной функции из модуля Database
    return fetchActivityData(count);
}

// Анализирует поведенческие данные и выявляет аномальные паттерны.
AnalysisResult analyzeBehavior(ActivityData* data, int dataCount) {
    printf("Анализ поведенческих данных...\n");
    AnalysisResult result;
    result.analyzedData = data; 
    result.analyzedDataCount = dataCount;
    
    result.suspiciousPatterns = NULL; 
    result.suspiciousPatternsCount = 0;
    
    int write_operations_count = 0;
    int delete_operations_count = 0;
    int potentially_renamed_count = 0; // Для эмуляции проверки расширений

    printf("Проверка операций с файлами-ловушками...\n");
    // В реальной системе здесь нужно сравнивать filePath с путем к известным файлам-ловушкам.
    // Поскольку у нас эмуляция, и мы добавляем фиктивные данные с путем к user_docs/financial_report.docx
    // (который может быть ловушкой или нет, в зависимости от конфигурации),
    // мы просто будем анализировать переданные данные об активности.
    // Ищем подозрительные операции (write, delete) на файлах, которые *могут* быть ловушками.
    
    // Эмуляция: Проходим по данным активности и считаем подозрительные операции
    for (int i = 0; i < dataCount; i++) {
        if (data[i].filePath != NULL && data[i].activityType != NULL) {
            // В реальной системе здесь проверяется, является ли data[i].filePath путем к файлу-ловушке
            // Для эмуляции, предположим, что все записи в data относятся к потенциально отслеживаемым файлам.
            
            if (strcmp(data[i].activityType, "write") == 0) {
                write_operations_count++;
            } else if (strcmp(data[i].activityType, "delete") == 0) {
                delete_operations_count++;
            } 
            // Эмуляция проверки изменения расширений: если есть операция write на файле,
            // и мы предполагаем, что это может быть попытка шифрования с заменой расширения.
            // В реальной системе это сложнее, нужно отслеживать переименования и изменения содержимого.
            // Упрощенная эмуляция: любая операция write может потенциально указывать на попытку изменения/шифрования.
            if (strcmp(data[i].activityType, "write") == 0) {
                 // В реальной системе тут была бы логика анализа имени файла до/после или содержимого
                 potentially_renamed_count++; 
            }
        }
    }
    
    printf("Подсчет подозрительных операций: запись=%d, удаление=%d, потенциальное_изменение_расширения=%d\n", 
           write_operations_count, delete_operations_count, potentially_renamed_count);

    // Эмуляция выявления аномальных паттернов
    // Проверка на массовое шифрование (эмуляция): большое количество операций записи/удаления
    if (write_operations_count > 5 || delete_operations_count > 3) { // Пороги для эмуляции
        result.suspiciousPatternsCount++;
        result.suspiciousPatterns = (char**)realloc(result.suspiciousPatterns, result.suspiciousPatternsCount * sizeof(char*));
        if (result.suspiciousPatterns) {
            result.suspiciousPatterns[result.suspiciousPatternsCount - 1] = strdup("Обнаружено большое количество операций записи/удаления (эмуляция массового шифрования).");
        }
        printf("Выявлен аномальный паттерн: массовое шифрование (эмуляция).\n");
    }
    
    // Эмуляция проверки изменения расширений
    if (potentially_renamed_count > 0 && write_operations_count > 0) { // Если были операции записи и потенциальные изменения
         // В реальной системе здесь проверяется, действительно ли расширение файла изменилось после операции
         // Для эмуляции, любое совпадение условия выше считается потенциальным изменением расширения.
         int found = 0;
         for(int i = 0; i < result.suspiciousPatternsCount; i++) {
             if(strcmp(result.suspiciousPatterns[i], "Обнаружено потенциальное изменение расширения файла (эмуляция).") == 0) {
                 found = 1;
                 break;
             }
         }
         if (!found) {
             result.suspiciousPatternsCount++;
             result.suspiciousPatterns = (char**)realloc(result.suspiciousPatterns, result.suspiciousPatternsCount * sizeof(char*));
             if (result.suspiciousPatterns) {
                 result.suspiciousPatterns[result.suspiciousPatternsCount - 1] = strdup("Обнаружено потенциальное изменение расширения файла (эмуляция).");
             }
             printf("Выявлен аномальный паттерн: потенциальное изменение расширения (эмуляция).\n");
         }
    }
    
    // В реальной системе здесь может быть больше типов анализа и паттернов.

    return result;
}

// Обнаруживает аномалии на основе результата анализа.
Anomaly* detectAnomalies(AnalysisResult result, int* count) {
    printf("Обнаружение аномалий...\n");
    
    *count = 0;
    Anomaly* anomalies = NULL;

    // Если analyzeBehavior выявил подозрительные паттерны, создаем аномалии.
    if (result.suspiciousPatternsCount > 0) {
        *count = result.suspiciousPatternsCount;
        anomalies = (Anomaly*)malloc(*count * sizeof(Anomaly));
        if (anomalies) {
            for (int i = 0; i < *count; i++) {
                anomalies[i].description = strdup(result.suspiciousPatterns[i]); // Описание берем из выявленного паттерна
                
                // Устанавливаем высокий приоритет для всех выявленных аномалий на данном этапе эмуляции
                anomalies[i].severity = strdup("Высокая"); 
                
                // !!! Эмуляция связывания аномалии с файлом/активностью !!!
                // В реальной системе нужно анализировать result.analyzedData (данные активности),
                // чтобы найти конкретные операции, соответствующие выявленному паттерну.
                // Для эмуляции, просто свяжем аномалию с последней записью в данных активности,
                // или с первой подозрительной операцией, найденной при анализе.
                
                anomalies[i].relatedFilePath = NULL; // Инициализация
                anomalies[i].relatedActivityType = NULL; // Инициализация

                // Попытка найти связанный файл/активность в данных анализа
                // (Простая эмуляция: ищем первую запись, которая могла бы соответствовать паттерну)
                for (int j = 0; j < result.analyzedDataCount; j++) {
                    bool match = false;
                    if (strstr(anomalies[i].description, "массовое шифрование") != NULL) {
                        // Если паттерн - массовое шифрование, ищем операции записи/удаления
                        if (result.analyzedData[j].activityType != NULL && 
                            (strcmp(result.analyzedData[j].activityType, "write") == 0 || 
                             strcmp(result.analyzedData[j].activityType, "delete") == 0)) {
                             match = true;
                        }
                    } else if (strstr(anomalies[i].description, "изменение расширения") != NULL) {
                         // Если паттерн - изменение расширения, ищем операции записи/переименования
                         // (переименование пока не обрабатывается явно в analyzeBehavior, но эмулируем проверку)
                         if (result.analyzedData[j].activityType != NULL && 
                            (strcmp(result.analyzedData[j].activityType, "write") == 0 || 
                             strcmp(result.analyzedData[j].activityType, "rename") == 0)) {
                              match = true;
                         }
                    } // Добавьте здесь проверки для других паттернов

                    if (match) {
                         // Найдена соответствующая запись активности. Сохраняем ее данные.
                         if (result.analyzedData[j].filePath != NULL) {
                             anomalies[i].relatedFilePath = strdup(result.analyzedData[j].filePath);
                         }
                         if (result.analyzedData[j].activityType != NULL) {
                             anomalies[i].relatedActivityType = strdup(result.analyzedData[j].activityType);
                         }
                         printf("  -> Связано с файлом: %s (Тип: %s)\n", 
                                anomalies[i].relatedFilePath ? anomalies[i].relatedFilePath : "N/A",
                                anomalies[i].relatedActivityType ? anomalies[i].relatedActivityType : "N/A");
                         // В реальной системе нужно собрать все соответствующие операции/файлы, а не только первую.
                         break; // Для эмуляции, берем первую подходящую и идем дальше к следующей аномалии.
                    }
                }

            }
        }
    }
    
    // Освобождение памяти, выделенной для suspiciousPatterns в analyzeBehavior
    for (int i = 0; i < result.suspiciousPatternsCount; i++) {
        free(result.suspiciousPatterns[i]);
    }
    free(result.suspiciousPatterns);
    result.suspiciousPatterns = NULL; // Обнуляем после освобождения
    result.suspiciousPatternsCount = 0;

    // !!! Важно: Память для аномалий (структур и их строковых полей) теперь должна освобождаться в receiveNotification !!!

    return anomalies;
}

// Уведомляет модуль ResponseModule об обнаруженных аномалиях.
void notifyResponseModule(Anomaly* anomalies, int anomaliesCount) {
    printf("Уведомление ResponseModule об аномалиях...\n");
    // Вызов функции из модуля ResponseModule
    receiveNotification(anomalies, anomaliesCount);
    // Память для anomalies и ее содержимого (description, severity, relatedFilePath, relatedActivityType) 
    // теперь должна освобождаться внутри receiveNotification.
}t 