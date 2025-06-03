#include "../include/ResponseModule.h"
#include "../include/Alert.h"
#include "../include/Anomaly.h"
#include "../include/ReportingModule.h" // Для передачи данных об угрозах
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Реализация функций для модуля ResponseModule

// Получает уведомление об аномалиях и инициирует действия реагирования.
void receiveNotification(Anomaly* anomalies, int anomaliesCount) {
    printf("ResponseModule: Получено уведомление об аномалиях (%d аномалий).\n", anomaliesCount);
    
    if (anomaliesCount > 0) {
        printf("ResponseModule: Анализ аномалий и принятие решений по реагированию...\n");

        // В реальной системе здесь будет более сложная логика определения типа угрозы
        // на основе анализа всех аномалий и их характеристик.
        // Сейчас, для эмуляции, будем принимать решения на основе описания и связанных данных аномалий.

        bool potential_ransomware_activity = false;
        char* file_to_isolate = NULL; // Пример: файл для изоляции

        for (int i = 0; i < anomaliesCount; i++) {
            printf("  Аномалия %d: %s (Серьезность: %s)\n", i + 1, anomalies[i].description, anomalies[i].severity);
            // Выводим связанные данные, если они есть
            if (anomalies[i].relatedFilePath != NULL) {
                printf("    Связанный файл: %s\n", anomalies[i].relatedFilePath);
            }
            if (anomalies[i].relatedActivityType != NULL) {
                printf("    Тип активности: %s\n", anomalies[i].relatedActivityType);
            }

            // Проверяем описание аномалии для принятия решения
            if (strstr(anomalies[i].description, "массовое шифрование") != NULL ||
                strstr(anomalies[i].description, "изменение расширения") != NULL) {
                
                printf("  -> Аномалия указывает на потенциальную активность трояна-шифровальщика.\n");
                potential_ransomware_activity = true;

                // Используем связанный путь файла из аномалии, если он доступен
                if (anomalies[i].relatedFilePath != NULL) {
                    // Копируем путь, чтобы он оставался доступным после освобождения памяти аномалии
                    file_to_isolate = strdup(anomalies[i].relatedFilePath);
                    printf("  -> Определен файл для изоляции: %s\n", file_to_isolate);
                } else {
                    // Если связанный файл не указан в аномалии (что не должно происходить при правильной работе),
                    // используем фиктивный путь как запасной вариант (или просто не изолируем ничего).
                    // В реальной системе здесь нужно будет обработать такую ситуацию.
                    printf("  -> В аномалии не указан связанный файл. Изоляция не будет выполнена (или используется заглушка).\n");
                    // file_to_isolate = strdup("trojan_ransomware_detector/user_docs/infected_file_emu_fallback.dat"); // Запасной фиктивный путь
                }
                 
                // Если найдена аномалия, указывающая на шифровальщик, можно сразу инициировать действия
                // и, возможно, прекратить анализ остальных аномалий в этом уведомлении.
                // !!! В реальной системе, возможно, потребуется собрать список всех затронутых файлов из всех аномалий!!!
                break; // Для эмуляции, обрабатываем первую найденную аномалию, связанную с шифровальщиком.
            }
            // Добавьте здесь проверки на другие типы аномалий и соответствующие действия
        }

        // Если обнаружена потенциальная активность шифровальщика
        if (potential_ransomware_activity) {
            printf("ResponseModule: Инициирование действий по реагированию на потенциальный троян-шифровальщик...\n");

            // 1. Создание угрозы и передача в ReportingModule
            Threat detectedThreat;
            detectedThreat.description = strdup("Обнаружена потенциальная активность трояна-шифровальщика.");
            detectedThreat.severity = strdup("Высокая"); // Устанавливаем высокую серьезность
            collectThreatData(&detectedThreat, 1); 
            free(detectedThreat.description);
            free(detectedThreat.severity);

            // 2. Уведомление администратора
            Alert systemAlert;
            systemAlert.message = strdup("!!! ВНИМАНИЕ: Обнаружена активность, похожая на троян-шифровальщик! Требуется немедленное вмешательство. Проверьте системный отчет.");
            // Получить текущее время для timestamp (можно использовать функцию из FileTrapModule или stdlib)
            // char timestamp_str[64];
            // time_t now = time(NULL);
            // strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
            systemAlert.timestamp = strdup("реальное_время (эмуляция)"); // Заглушка
            notifyAdministrator(systemAlert);
            free(systemAlert.message);
            free(systemAlert.timestamp);

            // 3. Попытка изоляции затронутого(ых) файла(ов)
            if (file_to_isolate != NULL) {
                printf("ResponseModule: Попытка изоляции потенциально зараженного файла.\n");
                char* files_list[] = {file_to_isolate}; // Массив из одного файла для примера
                isolateInfectedFiles(files_list, 1);
                free(file_to_isolate); // Освобождаем память для фиктивного пути
            }

        } else {
            printf("ResponseModule: Обнаружены аномалии, но не указывают на прямую угрозу (для данной эмуляции).\n");
            // В реальной системе здесь может быть логика логирования аномалий низкой серьезности,
            // или дальнейшего анализа.
        }

    }

    // Освобождение памяти, выделенной для аномалий в BehaviorAnalysisModule
    // Важно: Память для структур Anomaly и строк description/severity должна быть освобождена здесь.
     for (int i = 0; i < anomaliesCount; i++) {
        free(anomalies[i].description);
        free(anomalies[i].severity);
        // Освобождаем память для новых полей
        if (anomalies[i].relatedFilePath != NULL) {
            free(anomalies[i].relatedFilePath);
        }
        if (anomalies[i].relatedActivityType != NULL) {
            free(anomalies[i].relatedActivityType);
        }
    }
    free(anomalies);
    printf("ResponseModule: Память для аномалий освобождена.\n");
}

// Изолирует зараженные файлы.
void isolateInfectedFiles(char** fileList, int fileListCount) {
    printf("ResponseModule: Изоляция %d потенциально зараженных файлов...\n", fileListCount);
    // В реальной системе здесь будет логика изоляции файлов (перемещение, карантин, изменение прав доступа и т.д.)
    for (int i = 0; i < fileListCount; i++) {
        printf("  - Попытка изоляции файла: %s\n", fileList[i]);
        // Пример эмуляции изоляции: просто выводим сообщение
        // !!! РЕАЛИЗУЙТЕ ЗДЕСЬ ЛОГИКУ ИЗОЛЯЦИИ (перемещение, блокировка) !!!
    }
     // В зависимости от реализации, может потребоваться освобождение памяти для fileList и его содержимого
     // В данном случае fileList и его содержимое (строки путей) управляются вызывающей функцией (receiveNotification).
}

// Уведомляет администратора о тревоге.
void notifyAdministrator(Alert alert) {
    printf("ResponseModule: Уведомление администратора:\n");
    printf("  Сообщение: %s\n", alert.message);
    printf("  Время: %s\n", alert.timestamp);
    // В реальной системе здесь будет отправка уведомления (email, SMS, системное уведомление, запись в лог мониторинга и т.д.)
    printf("!!! РЕАЛИЗУЙТЕ ЗДЕСЬ ЛОГИКУ ОТПРАВКИ УВЕДОМЛЕНИЯ !!!\n");
} 