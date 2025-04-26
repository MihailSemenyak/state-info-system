#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stack>
using namespace std;

struct Country {
    string name;
    string government;
    string capital;
    string language;
    double area;
    long population;
    string currency;
    double exchangeRateToUSD;
    Country* next;
    Country* prev;
};

class CountryDatabase {
private:
    Country* head;
    Country* tail;
    stack<string> deletedCountries;

public:
    CountryDatabase() : head(nullptr), tail(nullptr) {}

    ~CountryDatabase() {
        while (head) {
            Country* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addCountry(const string& name, const string& government, const string& capital,
        const string& language, double area, long population, const string& currency,
        double exchangeRateToUSD) { // Добавлен новый параметр
        Country* newCountry = new Country{
            name, government, capital, language, area, population, currency, exchangeRateToUSD, nullptr, tail
        };
        if (!head) {
            head = tail = newCountry;
        }
        else {
            tail->next = newCountry;
            newCountry->prev = tail;
            tail = newCountry;
        }
    }


    void deleteCountry(const string& name) {
        if (!head) {
            cout << "Список стран пуст. Удаление невозможно.\n";
            return;
        }

        Country* current = head;
        while (current) {
            if (current->name == name) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                if (current == tail) tail = current->prev;
                deletedCountries.push(current->name); // Store the deleted country name
                delete current;
                cout << "Страна " << name << " успешно удалена.\n";
                return;
            }
            current = current->next;
        }
        cout << "Страна " << name << " не найдена.\n";
    }

    string getLastDeletedCountry() {
        if (deletedCountries.empty()) {
            return "Нет удалённых стран.";
        }
        string lastDeleted = deletedCountries.top();
        deletedCountries.pop();
        return "Последняя удалённая страна: " + lastDeleted;
    }

    void displayCountries() const {
        if (!head) {
            cout << "Список стран пуст.\n";
            return;
        }

        Country* current = head;
        cout << setw(15) << left << "Name"
            << setw(15) << "Government"
            << setw(15) << "Capital"
            << setw(15) << "Language"
            << setw(15) << "Area"
            << setw(15) << "Population"
            << setw(15) << "Currency"
            << "Exchange Rate" << endl; // Новый столбец
        cout << string(105, '-') << endl;
        while (current) {
            cout << setw(15) << left << current->name
                << setw(15) << current->government
                << setw(15) << current->capital
                << setw(15) << current->language
                << setw(15) << fixed << setprecision(0) << current->area
                << setw(15) << current->population
                << setw(20) << current->currency
                << fixed << setprecision(2) << current->exchangeRateToUSD << endl; // Новый столбец
            current = current->next;
        }
    }


    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, government, capital, language, currency;
            double area, exchangeRateToUSD; // Новый параметр
            long population;

            getline(ss, name, ';');
            getline(ss, government, ';');
            getline(ss, capital, ';');
            getline(ss, language, ';');
            ss >> area;
            ss.ignore();
            ss >> population;
            ss.ignore();
            getline(ss, currency, ';');
            ss >> exchangeRateToUSD; // Чтение курса валюты

            addCountry(name, government, capital, language, area, population, currency, exchangeRateToUSD);
        }
        file.close();
        cout << "Данные успешно загружены из файла " << filename << ".\n";
    }


    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return;
        }

        Country* current = head;
        while (current) {
            file << current->name << ';'
                << current->government << ';'
                << current->capital << ';'
                << current->language << ';'
                << current->area << ';'
                << current->population << ';'
                << current->currency << ';'
                << fixed << setprecision(2) << current->exchangeRateToUSD << endl; // Новый столбец
            current = current->next;
        }
        file.close();
        cout << "Данные успешно сохранены в файл " << filename << ".\n";
    }


    void sortByField(const string& field) {
        if (!head || !head->next) return;
        if (field != "name" && field != "population" && field != "area") {
            cout << "Неверное поле для сортировки. Попробуйте снова.\n";
            return;
        }

        bool swapped;
        do {
            swapped = false;
            Country* current = head;
            while (current->next) {
                bool needSwap = false;
                if (field == "name" && current->name > current->next->name) needSwap = true;
                else if (field == "population" && current->population > current->next->population) needSwap = true;
                else if (field == "area" && current->area > current->next->area) needSwap = true;

                if (needSwap) {
                    swap(current->name, current->next->name);
                    swap(current->government, current->next->government);
                    swap(current->capital, current->next->capital);
                    swap(current->language, current->next->language);
                    swap(current->area, current->next->area);
                    swap(current->population, current->next->population);
                    swap(current->currency, current->next->currency);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
        cout << "Список успешно отсортирован по полю " << field << ".\n";
    }

    void queryByField(const string& field, const string& value) const {
        if (field != "name" && field != "government" && field != "capital" && field != "language" && field != "currency") {
            cout << "Неверное поле для поиска. Попробуйте снова.\n";
            return;
        }

        Country* current = head;
        bool found = false;
        while (current) {
            if ((field == "name" && current->name == value) ||
                (field == "government" && current->government == value) ||
                (field == "capital" && current->capital == value) ||
                (field == "language" && current->language == value) ||
                (field == "currency" && current->currency == value)) {
                cout << setw(15) << left << current->name
                    << setw(15) << current->government
                    << setw(15) << current->capital
                    << setw(15) << current->language
                    << setw(15) << fixed << setprecision(0) << current->area
                    << setw(15) << fixed << setprecision(0) << current->population
                    << setw(20) << current->currency 
                    << fixed << setprecision(2) << current->exchangeRateToUSD << endl;
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "Результаты для поля '" << field << "' со значением '" << value << "' не найдены.\n";
        }
    }
};

void displayMenu() {
    cout << "1. Загрузить данные из файла\n";
    cout << "2. Сохранить данные в файл\n";
    cout << "3. Добавить страну\n";
    cout << "4. Удалить страну\n";
    cout << "5. Показать все страны\n";
    cout << "6. Поиск по полю\n";
    cout << "7. Сортировка по полю\n";
    cout << "8. Показать последнюю удалённую страну\n";
    cout << "9. Выход\n";
}

bool safeInput(string& value) {
    getline(cin, value);
    if (value.empty()) return false; // Проверка на пустой ввод
    for (char c : value) {
        if (isdigit(c)) {
            return false; // Строка не должна содержать цифр
        }
    }
    return true;
}

bool safeInput(double& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
    return true;
}

bool safeInput(long& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
    return true;
}

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    CountryDatabase db;
    int choice;

    do {
        displayMenu();
        cout << "Введите ваш выбор: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Попробуйте снова.\n";
            continue;
        }
        cin.ignore();

        switch (choice) {
        case 1: {
            string filename;
            cout << "Введите имя файла: ";
            getline(cin, filename);
            db.loadFromFile(filename);
            break;
        }
        case 2: {
            string filename;
            cout << "Введите имя файла: ";
            getline(cin, filename);
            db.saveToFile(filename);
            break;
        }
        case 3: {
            string name, government, capital, language, currency;
            double area, exchangeRateToUSD;
            long population;

            cout << "Введите название страны: ";
            while (!safeInput(name)) {
                cout << "Неверный ввод. Название страны не должно содержать цифр. Попробуйте снова: ";
            }
            cout << "Введите государственный строй: ";
            while (!safeInput(government)) {
                cout << "Неверный ввод. Государственный строй не должен содержать цифр. Попробуйте снова: ";
            }
            cout << "Введите столицу: ";
            while (!safeInput(capital)) {
                cout << "Неверный ввод. Название столицы не должно содержать цифр. Попробуйте снова: ";
            }
            cout << "Введите официальный язык: ";
            while (!safeInput(language)) {
                cout << "Неверный ввод. Название языка не должно содержать цифр. Попробуйте снова: ";
            }
            cout << "Введите площадь: ";
            while (!safeInput(area)) {
                cout << "Неверный ввод. Введите площадь снова: ";
            }
            cout << "Введите численность населения: ";
            while (!safeInput(population)) {
                cout << "Неверный ввод. Введите численность населения снова: ";
            }
            cout << "Введите денежную единицу: ";
            while (!safeInput(currency)) {
                cout << "Неверный ввод. Название валюты не должно содержать цифр. Попробуйте снова: ";
            }
            cout << "Введите курс валюты к доллару: ";
            while (!safeInput(exchangeRateToUSD)) {
                cout << "Неверный ввод. Введите курс валюты снова: ";
            }
            db.addCountry(name, government, capital, language, area, population, currency, exchangeRateToUSD);
            break;
        }

        case 4: {
            string name;
            cout << "Введите название страны для удаления: ";
            getline(cin, name);
            db.deleteCountry(name);
            break;
        }
        case 5:
            db.displayCountries();
            break;
        case 6: {
            string field, value;
            cout << "Введите поле для поиска (name/government/capital/language/currency): ";
            getline(cin, field);
            if (field != "name" && field != "government" && field != "capital" && field != "language" && field != "currency") {
                cout << "Неверное поле для поиска. Попробуйте снова.\n";
                break;
            }
            cout << "Введите значение для поиска: ";
            getline(cin, value);
            db.queryByField(field, value);
            break;
        }
        case 7: {
            string field;
            cout << "Введите поле для сортировки (name/population/area): ";
            getline(cin, field);
            if (field != "name" && field != "population" && field != "area") {
                cout << "Неверное поле для сортировки. Попробуйте снова.\n";
                break;
            }
            db.sortByField(field);
            break;
        }
        case 8:
            cout << db.getLastDeletedCountry() << endl;
            break;
        case 9:
            cout << "Выход из программы.\n";
            break;

        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 9);

    return 0;
}