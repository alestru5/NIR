#ifndef FILETRAPMODULE_H
#define FILETRAPMODULE_H

#include "TrapFileData.h"
#include <stdbool.h>

// Прототипы функций для модуля FileTrapModule

/**
 * @brief Создает и размещает файлы-ловушки в системе.
 */
void createAndPlaceTrapFiles();

/**
 * @brief Мониторит файлы-ловушки на предмет доступа или изменений.
 */
void monitorTrapFiles();

/**
 * @brief Логирует активность, связанную с файлами-ловушками.
 * @param activity Описание активности.
 */
void logActivity(char* activity);

/**
 * @brief Сохраняет данные о файле-ловушке в базу данных.
 * @param data Данные о файле-ловушке.
 */
void saveDataToDB_FileTrap(TrapFileData data); // Переименовано во избежание конфликта имен

#endif // FILETRAPMODULE_H 