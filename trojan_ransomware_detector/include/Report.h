#ifndef REPORT_H
#define REPORT_H

#include "Threat.h"
#include <stdbool.h>
// В C нет встроенных списков. 
// Здесь нужно будет использовать либо динамический массив, либо связанный список для List<Threat>.
// Для простоты определим поле как указатель, подразумевая, что оно будет указывать на массив или структуру списка.

// Определение структуры Report
typedef struct {
    char* reportId;
    char* generationTime;
    Threat* threats; // Представляет List<Threat>
    int threatsCount;
} Report;

#endif // REPORT_H 