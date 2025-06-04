#include "../include/FileSystemMonitorModule.h"
#include "../include/ActivityData.h" // Для структуры ActivityData и функции storeActivityData
#include "../include/Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h> // Для генерации случайных чисел
#include <fcntl.h>
#include <unistd.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

// Переменная для хранения файлового дескриптора kqueue
#ifdef __APPLE__
static int kq = -1;
// Переменная для хранения файловых дескрипторов мониторируемых директорий
// В реальной системе потребуется более сложная структура для сопоставления fd с путями
static int* monitored_fds = NULL;
static int monitored_fds_count = 0;

// Массив для хранения путей к мониторируемым директориям
static char** monitored_paths = NULL;

#endif

// Флаг для управления циклом мониторинга
static volatile bool monitoring_running = false;

// Переменная для хранения идентификатора потока мониторинга
#ifdef __APPLE__
static pthread_t monitor_thread;

// Структура для передачи контекста в поток мониторинга
typedef struct {
    const char** directories;
    int num_directories;
    bool should_stop;
} MonitorContext;

// Глобальные переменные для управления мониторингом
static MonitorContext* monitor_context = NULL;

// Функция, которая будет выполняться в отдельном потоке для мониторинга
void* monitor_loop(void* arg) {
    MonitorContext* context = (MonitorContext*)arg;
    int kq = kqueue();
    if (kq == -1) {
        perror("Ошибка создания kqueue");
        return NULL;
    }

    // Добавляем каждую директорию в kqueue
    for (int i = 0; i < context->num_directories; i++) {
        int fd = open(context->directories[i], O_RDONLY);
        if (fd == -1) {
            perror("Ошибка открытия директории для мониторинга");
            continue;
        }

        struct kevent event;
        EV_SET(&event, fd, EVFILT_VNODE,
               EV_ADD | EV_ENABLE | EV_CLEAR,
               NOTE_DELETE | NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_RENAME | NOTE_LINK,
               0, NULL);

        if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
            perror("Ошибка добавления события в kqueue");
            close(fd);
            continue;
        }
    }

    // Основной цикл мониторинга
    while (!context->should_stop) {
        struct kevent event_list[1];
        struct timespec timeout = {1, 0}; // Таймаут 1 секунда
        int nev = kevent(kq, NULL, 0, event_list, 1, &timeout);
        
        if (nev > 0) {
            int event_fd = (int)event_list[0].ident;
            char event_path[1024];
            
            // Получаем путь к файлу/директории, где произошло событие
            if (fcntl(event_fd, F_GETPATH, event_path) != -1) {
                // Получаем информацию о файле
                struct stat st;
                if (stat(event_path, &st) != -1) {
                    // Определяем тип события
                    const char* activity_type = NULL;
                    if (event_list[0].fflags & NOTE_DELETE) {
                        activity_type = "delete";
                    } else if (event_list[0].fflags & NOTE_WRITE) {
                        activity_type = "write";
                    } else if (event_list[0].fflags & NOTE_RENAME) {
                        activity_type = "rename";
                    } else if (event_list[0].fflags & NOTE_ATTRIB) {
                        activity_type = "attrib_change";
                    } else if (event_list[0].fflags & NOTE_EXTEND) {
                        activity_type = "extend";
                    } else if (event_list[0].fflags & NOTE_LINK) {
                        activity_type = "link_change";
                    }

                    if (activity_type != NULL) {
                        // Создаем запись о событии
                        ActivityData activity;
                        activity.filePath = strdup(event_path);
                        activity.activityType = strdup(activity_type);
                        
                        // Получаем текущее время
                        time_t now = time(NULL);
                        struct tm* tm_info = localtime(&now);
                        char timestamp[80];
                        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
                        activity.timestamp = strdup(timestamp);

                        // Сохраняем в базу данных
                        storeActivityData(activity);

                        // Логируем активность
                        char logMsg[1024];
                        snprintf(logMsg, sizeof(logMsg), "Обнаружена активность: %s - %s", event_path, activity_type);
                        logActivity(logMsg);

                        // Освобождаем память
                        free(activity.filePath);
                        free(activity.activityType);
                        free(activity.timestamp);
                    }
                }
            }
        }
    }

    // Закрываем все файловые дескрипторы
    for (int i = 0; i < context->num_directories; i++) {
        int fd = open(context->directories[i], O_RDONLY);
        if (fd != -1) {
            close(fd);
        }
    }
    close(kq);
    return NULL;
}

#endif

// Запускает мониторинг файловой системы в отдельном потоке
void startFileSystemMonitoring(const char** directories, int num_directories) {
    printf("Запуск мониторинга файловой системы...\n");
    
    // Создаем контекст для потока мониторинга
    monitor_context = (MonitorContext*)malloc(sizeof(MonitorContext));
    if (monitor_context == NULL) {
        perror("Ошибка выделения памяти для контекста мониторинга");
        return;
    }

    // Копируем пути директорий
    monitor_context->directories = (const char**)malloc(num_directories * sizeof(char*));
    if (monitor_context->directories == NULL) {
        perror("Ошибка выделения памяти для путей директорий");
        free(monitor_context);
        return;
    }

    for (int i = 0; i < num_directories; i++) {
        monitor_context->directories[i] = strdup(directories[i]);
        if (monitor_context->directories[i] == NULL) {
            perror("Ошибка копирования пути директории");
            // Освобождаем уже выделенную память
            for (int j = 0; j < i; j++) {
                free((void*)monitor_context->directories[j]);
            }
            free(monitor_context->directories);
            free(monitor_context);
            return;
        }
    }

    monitor_context->num_directories = num_directories;
    monitor_context->should_stop = false;

    // Создаем поток для мониторинга
    if (pthread_create(&monitor_thread, NULL, monitor_loop, monitor_context) != 0) {
        perror("Ошибка создания потока мониторинга");
        // Освобождаем память
        for (int i = 0; i < num_directories; i++) {
            free((void*)monitor_context->directories[i]);
        }
        free(monitor_context->directories);
        free(monitor_context);
        return;
    }

    printf("Мониторинг файловой системы запущен в отдельном потоке.\n");
}

// Останавливает мониторинг файловой системы
void stopFileSystemMonitoring() {
    if (monitor_context != NULL) {
        printf("Остановка мониторинга файловой системы...\n");
        monitor_context->should_stop = true;
        
        // Ждем завершения потока
        pthread_join(monitor_thread, NULL);
        
        // Освобождаем память
        for (int i = 0; i < monitor_context->num_directories; i++) {
            free((void*)monitor_context->directories[i]);
        }
        free(monitor_context->directories);
        free(monitor_context);
        monitor_context = NULL;
        
        printf("Мониторинг файловой системы остановлен.\n");
    }
} 