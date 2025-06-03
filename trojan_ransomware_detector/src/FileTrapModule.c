#include "../include/FileTrapModule.h"
#include "../include/Database.h" // Для сохранения данных в БД
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Для функций работы с файлами и директориями
#include <sys/stat.h> // Для mkdir
#include <time.h>     // Для генерации временной метки и srand

// --- Конфигурация файлов-ловушек ---
const char* trap_file_bases[] = {"financial", "password", "account", "secret", "backup", "private", "documents", "data"};
const char* trap_file_extensions[] = {".docx", ".xlsx", ".pdf", ".txt", ".jpg", ".png", ".zip"};
const char* trap_directories[] = {"trojan_ransomware_detector/user_docs/", "trojan_ransomware_detector/user_data/", "trojan_ransomware_detector/config/", "trojan_ransomware_detector/temp/"};

#define NUM_BASE_NAMES (sizeof(trap_file_bases) / sizeof(trap_file_bases[0]))
#define NUM_EXTENSIONS (sizeof(trap_file_extensions) / sizeof(trap_file_extensions[0]))
#define NUM_DIRECTORIES (sizeof(trap_directories) / sizeof(trap_directories[0]))

#define MAX_TRAP_FILES 20 // Максимальное количество файлов-ловушек для создания

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

// Функция для создания директории, если она не существует
void ensure_directory_exists(const char* path) {
    // Это простая реализация. В реальном проекте нужно обрабатывать вложенные директории.
    // mkdir только создает последнюю директорию в пути.
    // Для полной реализации потребуется рекурсивное создание или использование системных утилит.
    // В POSIX системах можно использовать mkdir(path, 0777);
    // Для кроссплатформенности это сложнее.
    // Для нашей эмуляции просто попробуем создать, ошибки будем игнорировать или логировать.
#ifdef _WIN32
    // Для Windows может потребоваться CreateDirectory
#else
    mkdir(path, 0777); // Пример для POSIX систем. Игнорируем ошибку, если директория уже есть.
#endif
    printf("Убедитесь, что директория '%s' существует.\n", path); // Напоминание пользователю
}


// --- Реализация функций модуля FileTrapModule ---

// Создает один файл-ловушку с заданным путем и содержимым.
void createSingleTrapFile(const char* filePath, const char* content) {
    // Извлекаем путь к директории из полного пути файла
    char* dirPath = strdup(filePath);
    char* lastSlash = strrchr(dirPath, '/');
#ifdef _WIN32
    char* lastBackslash = strrchr(dirPath, '\\');
    if (lastBackslash > lastSlash) lastSlash = lastBackslash;
#endif

    if (lastSlash != NULL) {
        *lastSlash = '\0'; // Обрезаем строку после последнего слеша, чтобы получить путь к директории
        ensure_directory_exists(dirPath); // Убеждаемся, что директория существует
        free(dirPath);
    } else {
        // Если слешей нет, файл должен быть создан в текущей директории или data/.
        // ensure_directory_exists("trojan_ransomware_detector/data/"); // Пример, если создаем в data
        free(dirPath);
    }


    FILE* fp = fopen(filePath, "w");
    if (fp != NULL) {
        fputs(content, fp);
        fclose(fp);
        printf("Создан файл-ловушка: %s\n", filePath);

        // Логирование активности
        char logMsg[512];
        snprintf(logMsg, sizeof(logMsg), "Создан файл-ловушка: %s", filePath);
        logActivity(logMsg);

        // Сохранение данных в БД (эмуляция)
        TrapFileData trapData;
        trapData.filePath = strdup(filePath); // Дублируем строку
        trapData.creationTime = getCurrentTimestamp(); // Реальное время создания (эмуляция)
        trapData.lastAccessTime = getCurrentTimestamp(); // Реальное время доступа (эмуляция - пока совпадает с созданием)
        saveDataToDB_FileTrap(trapData);

        // Освобождение выделенной памяти
        free(trapData.filePath);
        free(trapData.creationTime);
        free(trapData.lastAccessTime);

    } else {
        perror("Ошибка при создании файла-ловушки");
        fprintf(stderr, "Не удалось создать файл-ловушку: %s\\n", filePath);
    }
}

// Реализует алгоритм создания и размещения файлов-ловушек.
void createAndPlaceTrapFiles() {
    printf("Выполнение алгоритма создания файлов-ловушек:\n");
    srand(time(NULL)); // Инициализация генератора случайных чисел

    int files_created = 0;

    // Шаги 1-4: Автоматическое создание и размещение
    printf("Генерация и размещение файлов-ловушек...\n");
    for (int i = 0; i < MAX_TRAP_FILES && files_created < MAX_TRAP_FILES; i++) {
        // Случайный выбор имени, расширения и директории
        const char* base_name = trap_file_bases[rand() % NUM_BASE_NAMES];
        const char* extension = trap_file_extensions[rand() % NUM_EXTENSIONS];
        const char* directory = trap_directories[rand() % NUM_DIRECTORIES];

        // Формируем полный путь к файлу
        char filePath[256]; // Буфер для пути
        snprintf(filePath, sizeof(filePath), "%s%s_%d%s", directory, base_name, i, extension);

        // Пример простого содержимого. Можно сделать его более разнообразным.
        char content[128];
        snprintf(content, sizeof(content), "Это поддельный файл с данными %s.", base_name);

        createSingleTrapFile(filePath, content);
        files_created++;
    }

    printf("Создано и размещено %d файлов-ловушек.\n", files_created);

    // Шаги 5-7 (Регулярное обновление): Идея реализована в функции updateTrapFiles, которая должна вызываться периодически.
}

// Эмуляция мониторинга файлов-ловушек (требует системных API для полной реализации)
void monitorTrapFiles() {
    printf("Мониторинг файлов-ловушек (эмуляция)...\n");
    // В реальной системе здесь будет код, использующий inotify (Linux), ReadDirectoryChangesW (Windows)
    // или другие механизмы для отслеживания событий файловой системы (доступ, изменение, удаление).

    // При обнаружении активности с файлом-ловушкой:
    // 1. Определить тип активности (чтение, запись, удаление и т.д.)
    // 2. Получить временную метку.
    // 3. Создать структуру ActivityData.
    // 4. Сохранить ActivityData в базе данных: saveDataToDB_Activity(activityData);
    // 5. Возможно, инициировать анализ поведения, если активность подозрительная.

    // Для эмуляции, мы просто выведем сообщение.
    // Реальная логика мониторинга - сложная часть, зависящая от ОС.
}

// Эмуляция регулярного обновления файлов-ловушек.
// Эта функция должна вызываться периодически (например, по таймеру).
void updateTrapFiles() {
    printf("Обновление файлов-ловушек (эмуляция)...\n");
    // Шаги 6 и 7 алгоритма:
    // 1. Периодическое обновление содержимого: можно перечитывать файлы-ловушки и перезаписывать их с новым содержимым.
    // 2. Удаление старых и создание новых:
    //    - Получить список существующих файлов-ловушек (например, из БД).
    //    - Случайным образом выбрать часть для удаления.
    //    - Удалить выбранные файлы.
    //    - Создать новые файлы-ловушки, возможно, в новых местах или с новыми именами.
    // Это помогает предотвратить их обнаружение и поддерживать актуальность.

    // Простая эмуляция: удаляем один случайный старый файл (если он существует) и создаем один новый.
    TrapFileData* existingTraps;
    int existingTrapsCount;
    existingTraps = fetchTrapFileData(&existingTrapsCount);

    if (existingTraps != NULL && existingTrapsCount > 0) {
        int index_to_remove = rand() % existingTrapsCount;
        char* filePathToRemove = existingTraps[index_to_remove].filePath;

        printf("Попытка удалить старый файл-ловушку: %s\\n", filePathToRemove);
        if (remove(filePathToRemove) == 0) {
            printf("Удален файл-ловушка: %s\\n", filePathToRemove);
            // В реальной системе нужно также удалить запись из БД
        } else {
            //perror("Ошибка при удалении файла-ловушки");
             printf("Не удалось удалить файл-ловушку (возможно, не существует): %s\\n", filePathToRemove);
        }

        // Освобождаем память для fetched data
        freeTrapFileData(existingTraps, existingTrapsCount);
    } else {
        printf("Нет существующих файлов-ловушек для обновления/удаления.\n");
    }

    // Создаем один новый файл-ловушка
    printf("Создание нового файла-ловушки в рамках обновления...\n");
     const char* base_name = trap_file_bases[rand() % NUM_BASE_NAMES];
    const char* extension = trap_file_extensions[rand() % NUM_EXTENSIONS];
    const char* directory = trap_directories[rand() % NUM_DIRECTORIES];
    char newFilePath[256];
    snprintf(newFilePath, sizeof(newFilePath), "%s%s_updated_%d%s", directory, base_name, rand(), extension); // Добавляем rand() для уникальности
    char newContent[128];
    snprintf(newContent, sizeof(newContent), "Это обновленный поддельный файл с данными %s.", base_name);
    createSingleTrapFile(newFilePath, newContent);

    printf("Обновление файлов-ловушек завершено (эмуляция).\n");
}


// Эмуляция логирования активности
void logActivity(char* activity) {
    printf("[LOG] %s\n", activity);
    // В реальной системе здесь будет запись в лог-файл, syslog или отправка в систему логирования.
    // Также можно сохранять в БД активности.
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