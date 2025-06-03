#ifndef FILESYSTEMMONITORMODULE_H
#define FILESYSTEMMONITORMODULE_H

// Объявления функций для модуля мониторинга файловой системы

// Функция для инициализации и запуска мониторинга указанных директорий.
// В реальной реализации будет использовать специфичные для ОС API (например, kqueue на macOS, inotify на Linux).
// Возможно, потребуется запуск в отдельном потоке для неблокирующей работы.
void startFileSystemMonitoring(const char* directories[], int count);

// Функция для остановки мониторинга (если необходимо).
void stopFileSystemMonitoring();

#endif // FILESYSTEMMONITORMODULE_H 