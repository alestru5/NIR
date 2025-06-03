#ifndef DATABASE_H
#define DATABASE_H

#include "TrapFileData.h"
#include "ActivityData.h"
#include <stdbool.h>

// Прототипы функций для модуля Database (эмуляция)

/**
 * @brief Сохраняет данные о файле-ловушке в базу данных.
 * @param data Данные о файле-ловушке.
 */
void storeTrapFileData(TrapFileData data);

/**
 * @brief Сохраняет данные об активности в базу данных.
 * @param data Данные об активности.
 */
void storeActivityData(ActivityData data);

/**
 * @brief Извлекает данные о файлах-ловушках из базы данных.
 * @return Список данных о файлах-ловушках.
 */
TrapFileData* fetchTrapFileData(int* count); // Возвращает динамический массив TrapFileData и его размер через указатель

/**
 * @brief Извлекает данные об активности из базы данных.
 * @return Список данных об активности.
 */
ActivityData* fetchActivityData(int* count); // Возвращает динамический массив ActivityData и его размер через указатель

#endif // DATABASE_H 