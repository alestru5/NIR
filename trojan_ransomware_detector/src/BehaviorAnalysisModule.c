#include "../include/BehaviorAnalysisModule.h"
#include "../include/Database.h" // Для извлечения данных из БД
#include "../include/ResponseModule.h" // Для уведомления ResponseModule
#include "../include/ActivityData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// Реализация функций для модуля BehaviorAnalysisModule

// Извлекает данные об активности из базы данных.
ActivityData* fetchDataFromDB(int* count) {
    printf("Извлечение данных об активности из БД...\n");
    // Вызов реальной функции из модуля Database
    return fetchActivityData(count);
}

// Анализирует поведенческие данные и выявляет аномальные паттерны.
AnalysisResult analyzeBehavior(ActivityData* data, int dataCount) {
    AnalysisResult result;
    result.analyzedData = data;
    result.analyzedDataCount = dataCount;

    printf("Анализ поведенческих данных...\n");
    
    result.suspiciousPatterns = NULL; 
    result.suspiciousPatternsCount = 0;
    
    int write_operations_count = 0;
    int delete_operations_count = 0;
    int potentially_renamed_count = 0;
    int rapid_operations_count = 0;
    int same_extension_count = 0; // Счетчик операций с одинаковым расширением
    int suspicious_extensions_count = 0; // Счетчик подозрительных расширений
    time_t last_operation_time = 0;
    char last_extension[10] = ""; // Для отслеживания последовательности расширений
    const char* suspicious_extensions[] = {".enc", ".locked", ".crypto", ".ransom", ".crypted"};

    // Анализ данных из CSV файла
    FILE* csvFile = fopen("trojan_ransomware_detector/data/behavior_analysis.csv", "r");
    if (csvFile != NULL) {
        char line[1024];
        // Пропускаем заголовок
        if (fgets(line, sizeof(line), csvFile) != NULL) {
            while (fgets(line, sizeof(line), csvFile) != NULL) {
                char timestamp[80], file_path[512], activity_type[50], details[512];
                if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", timestamp, file_path, activity_type, details) == 4) {
                    // Преобразуем timestamp в time_t
                    struct tm tm;
                    strptime(timestamp, "%Y-%m-%d %H:%M:%S", &tm);
                    time_t operation_time = mktime(&tm);

                    // Проверяем временной интервал между операциями
                    if (last_operation_time != 0) {
                        double time_diff = difftime(operation_time, last_operation_time);
                        if (time_diff < 1.0) {
                            rapid_operations_count++;
                        }
                    }
                    last_operation_time = operation_time;

                    // Анализируем тип активности
                    if (strcmp(activity_type, "create") == 0) {
                        write_operations_count++;
                    } else if (strcmp(activity_type, "delete") == 0) {
                        delete_operations_count++;
                    } else if (strcmp(activity_type, "modify") == 0) {
                        write_operations_count++;
                        potentially_renamed_count++;
                    }

                    // Анализ расширений файлов
                    char* extension = strrchr(file_path, '.');
                    if (extension != NULL) {
                        // Проверка на подозрительные расширения
                        for (int i = 0; i < sizeof(suspicious_extensions)/sizeof(suspicious_extensions[0]); i++) {
                            if (strcmp(extension, suspicious_extensions[i]) == 0) {
                                suspicious_extensions_count++;
                                break;
                            }
                        }

                        // Проверка на последовательность одинаковых расширений
                        if (strlen(last_extension) > 0 && strcmp(extension, last_extension) == 0) {
                            same_extension_count++;
                        }
                        strncpy(last_extension, extension, sizeof(last_extension) - 1);
                        last_extension[sizeof(last_extension) - 1] = '\0';
                    }
                }
            }
        }
        fclose(csvFile);
    }

    printf("Подсчет подозрительных операций: запись=%d, удаление=%d, потенциальное_изменение_расширения=%d, быстрые_операции=%d, одинаковые_расширения=%d, подозрительные_расширения=%d\n", 
           write_operations_count, delete_operations_count, potentially_renamed_count, rapid_operations_count, same_extension_count, suspicious_extensions_count);

    // Проверка на массовое шифрование
    if (write_operations_count > 5 || delete_operations_count > 3) {
        result.suspiciousPatternsCount++;
        result.suspiciousPatterns = (char**)realloc(result.suspiciousPatterns, result.suspiciousPatternsCount * sizeof(char*));
        if (result.suspiciousPatterns) {
            result.suspiciousPatterns[result.suspiciousPatternsCount - 1] = strdup("Обнаружено большое количество операций записи/удаления (эмуляция массового шифрования).");
        }
        printf("Выявлен аномальный паттерн: массовое шифрование (эмуляция).\n");
    }
    
    // Проверка на изменение расширений
    if (potentially_renamed_count > 0 && write_operations_count > 0) {
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

    // Проверка на быстрые последовательные операции
    if (rapid_operations_count > 3) {
        result.suspiciousPatternsCount++;
        result.suspiciousPatterns = (char**)realloc(result.suspiciousPatterns, result.suspiciousPatternsCount * sizeof(char*));
        if (result.suspiciousPatterns) {
            result.suspiciousPatterns[result.suspiciousPatternsCount - 1] = strdup("Обнаружена подозрительно быстрая последовательность операций (возможная автоматизация).");
        }
        printf("Выявлен аномальный паттерн: быстрые последовательные операции.\n");
    }

    // Проверка на подозрительные расширения
    if (suspicious_extensions_count > 0) {
        result.suspiciousPatternsCount++;
        result.suspiciousPatterns = (char**)realloc(result.suspiciousPatterns, result.suspiciousPatternsCount * sizeof(char*));
        if (result.suspiciousPatterns) {
            result.suspiciousPatterns[result.suspiciousPatternsCount - 1] = strdup("Обнаружены файлы с подозрительными расширениями, характерными для шифровальщиков.");
        }
        printf("Выявлен аномальный паттерн: подозрительные расширения файлов.\n");
    }

    // Проверка на массовое переименование с одинаковым расширением
    if (same_extension_count > 5) {
        result.suspiciousPatternsCount++;
        result.suspiciousPatterns = (char**)realloc(result.suspiciousPatterns, result.suspiciousPatternsCount * sizeof(char*));
        if (result.suspiciousPatterns) {
            result.suspiciousPatterns[result.suspiciousPatternsCount - 1] = strdup("Обнаружено массовое переименование файлов с одинаковым расширением (возможное шифрование).");
        }
        printf("Выявлен аномальный паттерн: массовое переименование с одинаковым расширением.\n");
    }

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
} 