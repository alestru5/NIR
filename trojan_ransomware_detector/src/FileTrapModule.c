#include "../include/FileTrapModule.h"
#include "../include/ActivityData.h"
#include "../include/Database.h"
#include "../include/DataUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <libgen.h>
#include <fcntl.h>

// --- Конфигурация файлов-ловушек ---
const char* trap_file_bases[] = {"financial", "password", "account", "secret", "backup", "private", "documents", "data"};
const char* trap_file_extensions[] = {".docx", ".xlsx", ".pdf", ".txt", ".jpg", ".png", ".zip"};
const char* trap_directories[] = {"trojan_ransomware_detector/user_docs/", "trojan_ransomware_detector/user_data/", "trojan_ransomware_detector/config/", "trojan_ransomware_detector/temp/"};

#define NUM_BASE_NAMES (sizeof(trap_file_bases) / sizeof(trap_file_bases[0]))
#define NUM_EXTENSIONS (sizeof(trap_file_extensions) / sizeof(trap_file_extensions[0]))
#define NUM_DIRECTORIES (sizeof(trap_directories) / sizeof(trap_directories[0]))

#define MAX_TRAP_FILES 20 // Максимальное количество файлов-ловушек для создания

// Глобальные переменные
static const char* trap_directories[] = {
    "trojan_ransomware_detector/user_docs",
    "trojan_ransomware_detector/system_files",
    "trojan_ransomware_detector/important_data"
};
static const int num_trap_directories = sizeof(trap_directories) / sizeof(trap_directories[0]);

// --- Вспомогательные функции ---

// Функция для получения текущей временной метки (пример)
char* getCurrentTimestamp() {
    time_t rawtime;
    struct tm *timeinfo;
    char* buffer = malloc(80); // Достаточно большой буфер

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

// Вспомогательная функция для создания директории
static bool ensure_directory_exists(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0700) == -1) {
            perror("Ошибка создания директории");
            return false;
        }
    }
    return true;
}

// Вспомогательная функция для создания одного ловушка-файла
static bool createSingleTrapFile(const char* directory, int index) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/trap_file_%d.txt", directory, index);
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка создания ловушка-файла");
        return false;
    }
    
    // Записываем в файл случайные данные
    for (int i = 0; i < 100; i++) {
        fprintf(file, "Trap file content %d\n", rand());
    }
    
    fclose(file);
    
    // Логируем создание файла
    char logMsg[512];
    snprintf(logMsg, sizeof(logMsg), "Создан ловушка-файл: %s", filename);
    logActivity(logMsg);
    
    return true;
}

// --- Реализация функций модуля FileTrapModule ---

// Функция для создания и размещения ловушка-файлов
bool createAndPlaceTrapFiles() {
    printf("Создание и размещение ловушка-файлов...\n");
    
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    // Создаем директорию для логов
    ensure_directory_exists("trojan_ransomware_detector/logs");
    
    // Создаем директорию для данных
    ensure_directory_exists("trojan_ransomware_detector/data");
    
    // Создаем ловушка-файлы в каждой директории
    for (int i = 0; i < num_trap_directories; i++) {
        if (!ensure_directory_exists(trap_directories[i])) {
            printf("Ошибка создания директории: %s\n", trap_directories[i]);
            continue;
        }
        
        // Создаем несколько ловушка-файлов в каждой директории
        for (int j = 0; j < 5; j++) {
            if (!createSingleTrapFile(trap_directories[i], j)) {
                printf("Ошибка создания ловушка-файла в директории: %s\n", trap_directories[i]);
                continue;
            }
        }
    }
    
    printf("Ловушка-файлы успешно созданы и размещены.\n");
    return true;
}

// Функция для проверки состояния ловушка-файлов
bool checkTrapFiles() {
    printf("Проверка состояния ловушка-файлов...\n");
    
    bool all_files_ok = true;
    
    for (int i = 0; i < num_trap_directories; i++) {
        for (int j = 0; j < 5; j++) {
            char filename[256];
            snprintf(filename, sizeof(filename), "%s/trap_file_%d.txt", trap_directories[i], j);
            
            struct stat st;
            if (stat(filename, &st) == -1) {
                printf("Обнаружено изменение в ловушка-файле: %s\n", filename);
                all_files_ok = false;
                
                // Логируем обнаруженное изменение
                char logMsg[512];
                snprintf(logMsg, sizeof(logMsg), "Обнаружено изменение в ловушка-файле: %s", filename);
                logActivity(logMsg);
            }
        }
    }
    
    if (all_files_ok) {
        printf("Все ловушка-файлы в порядке.\n");
    }
    
    return all_files_ok;
}

// Эмуляция логирования активности
void logActivity(const char* activity) {
    // Вывод в консоль
    printf("%s\n", activity);
    
    // Запись в лог-файл
    FILE* log_file = fopen("trojan_ransomware_detector/logs/trap_activity.log", "a");
    if (log_file != NULL) {
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        char timestamp[80];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        fprintf(log_file, "[%s] %s\n", timestamp, activity);
        fclose(log_file);
    }
    
    // Запись в CSV файл для анализа поведения
    FILE* csv_file = fopen("trojan_ransomware_detector/data/behavior_analysis.csv", "a");
    if (csv_file != NULL) {
        // Если файл пустой, записываем заголовки
        if (ftell(csv_file) == 0) {
            fprintf(csv_file, "timestamp,file_path,activity_type,details\n");
        }
        
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        char timestamp[80];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
        
        // Извлекаем путь к файлу из сообщения
        const char* file_path = strstr(activity, ": ");
        if (file_path != NULL) {
            file_path += 2; // Пропускаем ": "
        } else {
            file_path = "unknown";
        }
        
        // Определяем тип активности
        const char* activity_type = "unknown";
        if (strstr(activity, "создан") != NULL) {
            activity_type = "create";
        } else if (strstr(activity, "изменение") != NULL) {
            activity_type = "modify";
        } else if (strstr(activity, "удален") != NULL) {
            activity_type = "delete";
        }
        
        fprintf(csv_file, "%s,%s,%s,%s\n", timestamp, file_path, activity_type, activity);
        fclose(csv_file);
    }
}

// Эмуляция сохранения данных о файле-ловушке в базу данных
void saveDataToDB_FileTrap(TrapFileData data) {
    printf("Сохранение данных о файле-ловушке в БД (эмуляция): %s\\n", data.filePath);
    // Вызов реальной функции сохранения в БД
    storeTrapFileData(data);
}

// Эмуляция сохранения данных об активности в базу данных
void saveDataToDB_Activity(ActivityData data) {
     printf("Сохранение данных об активности в БД (эмуляция): %s - %s\\n", data.filePath, data.activityType);
     // Вызов реальной функции сохранения в БД
     storeActivityData(data);
} 