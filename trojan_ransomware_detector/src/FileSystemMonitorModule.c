#include "../include/FileSystemMonitorModule.h"
#include "../include/ActivityData.h" // Для структуры ActivityData и функции storeActivityData
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h> // Для генерации случайных чисел

// Включение специфичных для macOS заголовочных файлов
#ifdef __APPLE__
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#endif

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

// Функция, которая будет выполняться в отдельном потоке для мониторинга
void* monitor_loop(void* arg) {
    printf("\nЦикл мониторинга запущен в отдельном потоке...\n");
    struct kevent event_list[1]; // Читаем по одному событию
    struct timespec timeout; // Таймаут для kevent
    timeout.tv_sec = 0; // Неблокирующий вызов для проверки флага monitoring_running чаще
    timeout.tv_nsec = 100000000; // 100 миллисекунд

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    while (monitoring_running) {
        int nev = kevent(kq, NULL, 0, event_list, 1, &timeout);
        if (nev == -1) {
            perror("kevent wait");
            // В реальной системе здесь может потребоваться более сложная обработка ошибок
            break; // Выход из цикла при ошибке
        }
        
        if (nev > 0) {
            // Обработка события. event_list[0] содержит информацию о событии.
            // event_list[0].ident содержит файловый дескриптор директории, где произошло событие.
            int event_fd = (int)event_list[0].ident;
            char event_path[1024]; // Буфер для пути
            char actual_filePath[2048]; // Буфер для полного пути файла/директории

            // Попытка получить путь по файловому дескриптору события (fd директории)
            if (fcntl(event_fd, F_GETPATH, event_path) != -1) {
                 // event_path теперь содержит путь к директории, в которой произошло событие.
                 // Эмуляция: Пока будем использовать путь к директории как основу.
                 printf("Обнаружено событие в директории: %s\n", event_path);

                 // !!! Эмуляция получения имени файла/директории, вызвавшего событие !!!
                 // В реальной системе это сложно с kqueue EVFILT_VNODE. Требуются другие методы.
                 char simulated_filename[256];

                 // Простая эмуляция: генерируем фиктивное имя файла в зависимости от типа события
                 if (event_list[0].fflags & (NOTE_WRITE | NOTE_RENAME | NOTE_EXTEND | NOTE_CREATE)) {
                      // Эмулируем создание/изменение файла
                      snprintf(simulated_filename, sizeof(simulated_filename), "simulated_file_%d.dat", rand() % 1000);
                 } else if (event_list[0].fflags & NOTE_DELETE) {
                      // Эмулируем удаление файла
                       snprintf(simulated_filename, sizeof(simulated_filename), "deleted_file_%d.tmp", rand() % 1000);
                 } else { // Другие события
                       snprintf(simulated_filename, sizeof(simulated_filename), "other_event_%d", rand() % 1000);
                 }
                 
                 // Формируем полный эмулированный путь
                 snprintf(actual_filePath, sizeof(actual_filePath), "%s/%s", event_path, simulated_filename);

                 printf("!!! Эмулируемый путь к файлу: %s !!!\n", actual_filePath);
                 
            } else {
                 perror("fcntl F_GETPATH");
                 strcpy(actual_filePath, "неизвестный путь (ошибка fcntl)");
            }
            
            printf("Обнаружено событие файловой системы (эмуляция обработки). FD: %d, Flags: %x, Fflags: %x\n", 
                   event_list[0].ident, event_list[0].flags, event_list[0].fflags);
            
            // Эмуляция сбора и сохранения ActivityData
            ActivityData activity;
            activity.filePath = strdup(actual_filePath); // Используем полученный (или эмулированный) путь
            
            // Определить activityType на основе event_list[0].fflags
            if (event_list[0].fflags & NOTE_WRITE) {
                 activity.activityType = strdup("write");
            } else if (event_list[0].fflags & NOTE_DELETE) {
                 activity.activityType = strdup("delete");
            } else if (event_list[0].fflags & NOTE_RENAME) {
                 activity.activityType = strdup("rename"); 
            } else if (event_list[0].fflags & NOTE_ATTRIB) {
                 activity.activityType = strdup("attrib_change"); 
            } else if (event_list[0].fflags & NOTE_EXTEND) { // Изменение размера
                 activity.activityType = strdup("extend");
            } else if (event_list[0].fflags & NOTE_LINK) { // Изменение числа ссылок
                 activity.activityType = strdup("link_change");
            } else if (event_list[0].fflags & NOTE_READ) { // Чтение (менее распространено для мониторинга изменений, но возможно)
                 activity.activityType = strdup("read");
            } else if (event_list[0].fflags & NOTE_CREATE) { // Создание файла в директории (только для мониторинга директорий)
                 activity.activityType = strdup("create");
            } else {
                 activity.activityType = strdup("unknown");
            }

            // !!! Получить реальное время !!!
            activity.timestamp = strdup("реальный_timestamp (эмуляция)"); 
            
            // Сохранить данные в БД (эмуляция) - в реальной системе передать в BehaviorAnalysis
            storeActivityData(activity);
            
            // Освободить выделенную память
            free(activity.filePath);
            free(activity.activityType);
            free(activity.timestamp);

            // !!! В реальной системе: Здесь нужно передать данные в модуль анализа поведения !!!
            // notifyBehaviorAnalysisModule(&activity); // Требуется реализация этой функции и ее связи
        }
    }

    // Закрытие файловых дескрипторов мониторируемых директорий
    if (monitored_fds) {
        for (int i = 0; i < monitored_fds_count; i++) {
            if (monitored_fds[i] != -1) {
                close(monitored_fds[i]);
            }
        }
        free(monitored_fds);
        monitored_fds = NULL;
        monitored_fds_count = 0;
    }
    
    // Освобождение памяти для путей
    if (monitored_paths) {
        for (int i = 0; i < monitored_fds_count; i++) { // Используем monitored_fds_count, т.к. он соответствует количеству путей
            free(monitored_paths[i]);
        }
        free(monitored_paths);
        monitored_paths = NULL;
    }

    // Закрытие kqueue при остановке мониторинга
    if (kq != -1) {
        close(kq);
        kq = -1;
    }
    printf("\nЦикл мониторинга завершен.\n");
    pthread_exit(NULL); // Завершение потока
    return NULL;
}

#endif

void startFileSystemMonitoring(const char* directories[], int count) {
    printf("\nЗапуск мониторинга файловой системы (улучшенная эмуляция для macOS)...\n");
    
#ifdef __APPLE__
    if (monitoring_running) {
        printf("Мониторинг уже запущен.\n");
        return;
    }

    // Инициализация kqueue
    kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        return;
    }

    // Выделение памяти для сохранения файловых дескрипторов и путей
    monitored_fds = (int*)malloc(count * sizeof(int));
    monitored_paths = (char**)malloc(count * sizeof(char*));
    if (!monitored_fds || !monitored_paths) {
        perror("malloc");
        // Очистка уже выделенной памяти при ошибке
        free(monitored_fds);
        free(monitored_paths);
        monitored_fds = NULL;
        monitored_paths = NULL;
        close(kq);
        kq = -1;
        return;
    }
    monitored_fds_count = count;

    // Добавление директорий для мониторинга
    for (int i = 0; i < count; ++i) {
        // Убедимся, что директория существует (можно использовать ensure_directory_exists из FileTrapModule)
        // Пока опустим эту проверку для простоты.

        int fd = open(directories[i], O_RDONLY);
        if (fd == -1) {
            perror("open");
            monitored_fds[i] = -1; // Помечаем как ошибку
            monitored_paths[i] = NULL;
            continue;
        }

        monitored_fds[i] = fd; // Сохраняем fd
        monitored_paths[i] = strdup(directories[i]); // Сохраняем путь
        if (!monitored_paths[i]) {
             perror("strdup");
             close(fd);
             monitored_fds[i] = -1;
             // В реальной системе: корректно очистить все предыдущие fd и пути
             // Пока просто продолжаем, помечая этот как NULL
        }

        struct kevent event;
        // Мониторим создание, удаление, изменение атрибутов, запись, переименование, изменение размера, числа ссылок, чтение
        // Идентификатором события является файловый дескриптор (fd)
        EV_SET(&event, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, 
               NOTE_WRITE | NOTE_DELETE | NOTE_ATTRIB | NOTE_RENAME | NOTE_EXTEND | NOTE_LINK | NOTE_READ | NOTE_CREATE, 0, NULL);

        if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
            perror("kevent add");
            // В реальной системе: обработать ошибку, возможно, закрыть fd и пометить его как неактивный
        }
        printf("Добавлено для мониторинга: %s (fd: %d)\n", directories[i], fd);
    }

    monitoring_running = true;

    // Создание и запуск потока мониторинга
    int ret = pthread_create(&monitor_thread, NULL, monitor_loop, NULL);
    if (ret != 0) {
        perror("pthread_create");
        monitoring_running = false;
        // !!! В реальной системе: корректно очистить ресурсы (закрыть fd, kq, освободить память) !!!
        if (monitored_fds) {
            for (int i = 0; i < monitored_fds_count; i++) {
                 if (monitored_fds[i] != -1) close(monitored_fds[i]);
            }
            free(monitored_fds);
            monitored_fds = NULL;
        }
         if (monitored_paths) {
            for (int i = 0; i < monitored_fds_count; i++) {
                 free(monitored_paths[i]);
            }
            free(monitored_paths);
            monitored_paths = NULL;
        }
        if (kq != -1) close(kq);
        kq = -1;
        monitored_fds_count = 0; // Сброс счетчика при ошибке
        return;
    }

    // !!! В реальной системе основной поток может продолжать свою работу !!!
    printf("Основной поток продолжает работу.\n");

#else // Для других ОС
    printf("Мониторинг файловой системы не реализован для данной ОС.\n");
#endif
}

void stopFileSystemMonitoring() {
    printf("\nЗапрос на остановку мониторинга файловой системы...\n");
    monitoring_running = false;
    
#ifdef __APPLE__
    // Ожидание завершения потока мониторинга
    if (monitoring_running == false && kq != -1) { // Проверка, что мониторинг был запущен
        if (pthread_join(monitor_thread, NULL) != 0) {
            perror("pthread_join");
        }
        printf("Поток мониторинга остановлен.\n");
    }
#endif
    printf("\nМониторинг файловой системы остановлен.\n");
} 