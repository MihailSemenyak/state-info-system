# state-info-system
# Консольное приложение: Работа с базой данных стран

## Описание

Это консольное приложение позволяет:

- Загружать данные о странах из текстового файла
- Добавлять новые страны
- Удалять страны
- Сохранять изменения обратно в файл
- Отображать список всех стран

Данные о странах включают:
- Название страны
- Тип государственного устройства
- Столицу
- Официальный язык
- Площадь территории
- Численность населения
- Валюту
- Курс валюты к доллару США

## Как использовать

1. Скомпилируйте проект при помощи любого C++ компилятора (например, g++).
2. Запустите программу.
3. Следуйте инструкциям в консоли для работы с базой данных.

## Пример кода

'''cpp
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct Country {
    string name;
    string government;
    string capital;
    string language;
    double area;
    int population;
    string currency;
    double exchangeRateToUSD;
    Country* next;
};

void displayCountry(const Country* current) {
    cout << setw(15) << left << current->name
         << setw(15) << current->government
         << setw(15) << current->capital
         << setw(15) << current->language
         << setw(15) << fixed << setprecision(0) << current->area
         << setw(15) << current->population
         << setw(20) << current->currency
         << fixed << setprecision(2) << current->exchangeRateToUSD
         << endl;
}
'''
## Требования

- Компилятор с поддержкой стандарта C++11 или выше
- Файлы данных в формате .txt должны быть корректно подготовлены (каждое поле в строке через пробел или разделитель)

## Автор

- Михаил

---
