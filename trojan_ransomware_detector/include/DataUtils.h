#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include "TrapFileData.h"
#include "ActivityData.h"

// Функции для работы с данными ловушек
void freeTrapFileData(TrapFileData* data, int count);
void updateTrapFiles(void);

// Функции для работы с данными активности
void freeActivityData(ActivityData* data, int count);

// Функции для работы с угрозами
void freeCollectedThreats(void);

#endif // DATA_UTILS_H 