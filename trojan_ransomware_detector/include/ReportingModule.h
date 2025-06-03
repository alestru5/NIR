#ifndef REPORTINGMODULE_H
#define REPORTINGMODULE_H

#include "Threat.h"
#include "Report.h"
#include <stdbool.h>

// Прототипы функций для модуля ReportingModule

/**
 * @brief Собирает данные об угрозах.
 * @param threats Список угроз.
 * @param threatsCount Количество угроз.
 */
void collectThreatData(Threat* threats, int threatsCount);

/**
 * @brief Генерирует отчет на основе собранных данных об угрозах.
 * @return Сгенерированный отчет.
 */
Report generateReport();

/**
 * @brief Экспортирует отчет в указанном формате.
 * @param report Отчет для экспорта.
 * @param format Формат экспорта (например, "PDF", "CSV").
 */
void exportReport(Report report, char* format);

#endif // REPORTINGMODULE_H 