#include "../include/FileTrapModule.h"
#include "../include/BehaviorAnalysisModule.h"
#include "../include/ResponseModule.h"
#include "../include/ReportingModule.h"
#include "../include/Database.h"
#include "../include/ActivityData.h"
#include "../include/FileSystemMonitorModule.h"
#include "../include/DataUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Для функции sleep

#define SCHEDULE_INTERVAL 3 // Вызывать updateTrapFiles каждые 3 итерации главного цикла (эмуляция)

int main() {
    printf("Система выявления ВПО типа троян-шифровальщик запущена.\n");

    // Шаг 1: Создание и размещение файлов-ловушек
    createAndPlaceTrapFiles();

    // Шаг 2: Запуск мониторинга файловой системы
    // Указываем директории для мониторинга (пример)
    const char* monitored_directories[] = {
        "trojan_ransomware_detector/trap_files", // Директория с файлами-ловушками
        "trojan_ransomware_detector/user_docs"   // Пример пользовательской директории
    };
    int num_monitored_directories = sizeof(monitored_directories) / sizeof(monitored_directories[0]);
    
    startFileSystemMonitoring(monitored_directories, num_monitored_directories);

    // Основной цикл работы: анализ данных, реагирование, обновление
    printf("\nЗапуск основного цикла анализа и реагирования (опирается на мониторинг)...\n");
    
    // Этот цикл будет периодически анализировать данные, собранные модулем мониторинга.
    // Для демонстрации, выполним несколько итераций.
    int iteration = 0;
    int schedule_counter = 0;

    while (iteration < 10) { // Выполним 10 итераций цикла для примера
        printf("\n--- Итерация %d ---\n", iteration + 1);

        // Шаг 3: Извлечение данных об активности из БД для анализа
        // (Данные в БД теперь сохраняет модуль мониторинга)
        printf("\nИзвлечение данных об активности для анализа (собранных мониторингом)...\n");
        int activityCount = 0;
        // В реальной системе здесь извлекаются *новые* данные с момента последнего анализа.
        // В нашей эмуляции извлекаются все данные из файла.
        ActivityData* activities = fetchDataFromDB(&activityCount);

        if (activities != NULL && activityCount > 0) {
            printf("Извлечено %d записей активности.\n", activityCount);

            // Шаг 4: Анализ поведенческих данных
            printf("\nЗапуск анализа поведения...\n");
            AnalysisResult analysisResult = analyzeBehavior(activities, activityCount);

            // Шаг 5: Обнаружение аномалий
            printf("\nОбнаружение аномалий...\n");
            int anomalyCount = 0;
            Anomaly* anomalies = detectAnomalies(analysisResult, &anomalyCount);

            // Шаг 6: Реагирование на аномалии (уведомление ResponseModule)
            if (anomalies != NULL && anomalyCount > 0) {
                printf("\nОбнаружено %d аномалий. Уведомление ResponseModule...\n", anomalyCount);
                notifyResponseModule(anomalies, anomalyCount); // ResponseModule освободит память anomalies
            } else {
                printf("\nАномалий не обнаружено в этой итерации.\n");
            }

            // Освобождение памяти, выделенной для fetched activities
            freeActivityData(activities, activityCount);

        } else {
            printf("Нет новых данных об активности для анализа в этой итерации.\n");
        }
        
        // Шаг 7: Эмуляция регулярного обновления файлов-ловушек по расписанию
        schedule_counter++;
        if (schedule_counter >= SCHEDULE_INTERVAL) {
             printf("\nЗапуск обновления файлов-ловушек по расписанию (эмуляция)...\n");
            updateTrapFiles();
            schedule_counter = 0; // Сброс счетчика
        }

        // Пауза перед следующей итерацией основного цикла
        printf("\nОжидание перед следующей итерацией основного цикла...\n");
        sleep(1); // Пауза на 1 секунду для примера

        iteration++;
    }
    
    // Шаг 8: Остановка мониторинга файловой системы перед завершением
    stopFileSystemMonitoring();

    // Шаг 9: Генерация и экспорт финального отчета (может быть вызвано по запросу или при завершении)
    printf("\nГенерация и экспорт финального отчета...\n");
    Report systemReport = generateReport();
    exportReport(systemReport, "Console"); // Экспорт в консоль для примера

    // Освобождение памяти, выделенной в ReportingModule для collectedThreats
    freeCollectedThreats();

    printf("\nСистема выявления ВПО типа троян-шифровальщик завершила работу.\n");

    return 0;
} 