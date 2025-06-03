#ifndef ANALYSISRESULT_H
#define ANALYSISRESULT_H

#include "ActivityData.h"
#include <stdbool.h>
// В C нет встроенных списков как в UML. 
// Здесь нужно будет использовать либо динамический массив, либо связанный список для List<ActivityData> и List<String>.
// Для простоты определим поля как указатели, подразумевая, что они будут указывать на массив или структуру списка.

// Определение структуры AnalysisResult
typedef struct {
    ActivityData* analyzedData; // Представляет List<ActivityData>
    int analyzedDataCount;
    char** suspiciousPatterns; // Представляет List<String>
    int suspiciousPatternsCount;
} AnalysisResult;

#endif // ANALYSISRESULT_H 