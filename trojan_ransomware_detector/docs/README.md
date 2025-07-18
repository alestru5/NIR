# Система обнаружения троянских программ и шифровальщиков

## Общее описание
Система представляет собой комплексное решение для обнаружения и предотвращения атак троянских программ и шифровальщиков. Система использует комбинацию методов мониторинга файловой системы, анализа поведения и файлов-ловушек для выявления подозрительной активности.

## Основные компоненты

### 1. Модуль файлов-ловушек (FileTrapModule)
- Создание и размещение файлов-ловушек в критических директориях
- Мониторинг изменений файлов-ловушек
- Регулярное обновление файлов-ловушек
- Логирование активности
- Сохранение данных в базу данных

### 2. Модуль анализа поведения (BehaviorAnalysisModule)
- Анализ активности файловой системы
- Выявление подозрительных паттернов
- Обнаружение аномалий
- Уведомление модуля реагирования

### 3. Модуль реагирования (ResponseModule)
- Обработка уведомлений об аномалиях
- Изоляция зараженных файлов
- Уведомление администратора
- Принятие мер по предотвращению распространения угрозы

### 4. Модуль мониторинга файловой системы (FileSystemMonitorModule)
- Мониторинг указанных директорий
- Отслеживание изменений файлов
- Запуск и остановка мониторинга
- Обработка событий файловой системы

### 5. Модуль отчетности (ReportingModule)
- Сбор данных об угрозах
- Генерация отчетов
- Экспорт отчетов в различных форматах
- Управление собранными данными

### 6. Модуль базы данных (Database)
- Хранение данных о файлах-ловушках
- Хранение данных об активности
- Извлечение данных для анализа
- Управление данными

## Тестирование

Система включает комплексный набор тестов, охватывающих различные аспекты:

### 1. Базовые тесты
- Тестирование инициализации системы
- Тестирование мониторинга файловой системы
- Тестирование анализа поведения
- Тестирование обнаружения аномалий
- Тестирование реагирования на аномалии
- Тестирование обновления файлов-ловушек
- Тестирование генерации и экспорта отчетов

### 2. Дополнительные тесты
- Тестирование граничных случаев
- Тестирование конкурентного доступа
- Тестирование производительности
- Тестирование восстановления после сбоев
- Тестирование безопасности
- Тестирование интеграции компонентов

## Требования к системе
- Операционная система: Linux/Unix
- Компилятор C
- Доступ к файловой системе
- Права на создание и модификацию файлов

## Использование

### Компиляция
```bash
gcc -o trojan_detector src/*.c -I include
```

### Запуск
```bash
./trojan_detector
```

### Конфигурация
Система может быть настроена путем изменения параметров в конфигурационных файлах:
- Директории для мониторинга
- Параметры файлов-ловушек
- Настройки анализа поведения
- Параметры реагирования
- Настройки отчетности

## Безопасность
- Система использует файлы-ловушки для обнаружения несанкционированного доступа
- Реализована защита от потенциально опасного содержимого
- Обеспечена изоляция зараженных файлов
- Реализовано уведомление администратора о подозрительной активности

## Производительность
- Система оптимизирована для обработки большого количества файлов
- Реализовано эффективное управление памятью
- Обеспечена быстрая реакция на подозрительную активность
- Оптимизирована работа с базой данных

## Расширяемость
Система может быть расширена путем:
- Добавления новых типов файлов-ловушек
- Реализации дополнительных алгоритмов анализа
- Интеграции с другими системами безопасности
- Добавления новых форматов отчетов 