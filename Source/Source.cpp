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
        double exchangeRateToUSD) { // �������� ����� ��������
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
            cout << "������ ����� ����. �������� ����������.\n";
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
                cout << "������ " << name << " ������� �������.\n";
                return;
            }
            current = current->next;
        }
        cout << "������ " << name << " �� �������.\n";
    }

    string getLastDeletedCountry() {
        if (deletedCountries.empty()) {
            return "��� �������� �����.";
        }
        string lastDeleted = deletedCountries.top();
        deletedCountries.pop();
        return "��������� �������� ������: " + lastDeleted;
    }

    void displayCountries() const {
        if (!head) {
            cout << "������ ����� ����.\n";
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
            << "Exchange Rate" << endl; // ����� �������
        cout << string(105, '-') << endl;
        while (current) {
            cout << setw(15) << left << current->name
                << setw(15) << current->government
                << setw(15) << current->capital
                << setw(15) << current->language
                << setw(15) << fixed << setprecision(0) << current->area
                << setw(15) << current->population
                << setw(20) << current->currency
                << fixed << setprecision(2) << current->exchangeRateToUSD << endl; // ����� �������
            current = current->next;
        }
    }


    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "������: �� ������� ������� ���� " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, government, capital, language, currency;
            double area, exchangeRateToUSD; // ����� ��������
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
            ss >> exchangeRateToUSD; // ������ ����� ������

            addCountry(name, government, capital, language, area, population, currency, exchangeRateToUSD);
        }
        file.close();
        cout << "������ ������� ��������� �� ����� " << filename << ".\n";
    }


    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "������: �� ������� ������� ���� " << filename << endl;
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
                << fixed << setprecision(2) << current->exchangeRateToUSD << endl; // ����� �������
            current = current->next;
        }
        file.close();
        cout << "������ ������� ��������� � ���� " << filename << ".\n";
    }


    void sortByField(const string& field) {
        if (!head || !head->next) return;
        if (field != "name" && field != "population" && field != "area") {
            cout << "�������� ���� ��� ����������. ���������� �����.\n";
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
        cout << "������ ������� ������������ �� ���� " << field << ".\n";
    }

    void queryByField(const string& field, const string& value) const {
        if (field != "name" && field != "government" && field != "capital" && field != "language" && field != "currency") {
            cout << "�������� ���� ��� ������. ���������� �����.\n";
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
            cout << "���������� ��� ���� '" << field << "' �� ��������� '" << value << "' �� �������.\n";
        }
    }
};

void displayMenu() {
    cout << "1. ��������� ������ �� �����\n";
    cout << "2. ��������� ������ � ����\n";
    cout << "3. �������� ������\n";
    cout << "4. ������� ������\n";
    cout << "5. �������� ��� ������\n";
    cout << "6. ����� �� ����\n";
    cout << "7. ���������� �� ����\n";
    cout << "8. �������� ��������� �������� ������\n";
    cout << "9. �����\n";
}

bool safeInput(string& value) {
    getline(cin, value);
    if (value.empty()) return false; // �������� �� ������ ����
    for (char c : value) {
        if (isdigit(c)) {
            return false; // ������ �� ������ ��������� ����
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ������
    return true;
}

bool safeInput(long& value) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ������
    return true;
}

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    CountryDatabase db;
    int choice;

    do {
        displayMenu();
        cout << "������� ��� �����: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����. ���������� �����.\n";
            continue;
        }
        cin.ignore();

        switch (choice) {
        case 1: {
            string filename;
            cout << "������� ��� �����: ";
            getline(cin, filename);
            db.loadFromFile(filename);
            break;
        }
        case 2: {
            string filename;
            cout << "������� ��� �����: ";
            getline(cin, filename);
            db.saveToFile(filename);
            break;
        }
        case 3: {
            string name, government, capital, language, currency;
            double area, exchangeRateToUSD;
            long population;

            cout << "������� �������� ������: ";
            while (!safeInput(name)) {
                cout << "�������� ����. �������� ������ �� ������ ��������� ����. ���������� �����: ";
            }
            cout << "������� ��������������� �����: ";
            while (!safeInput(government)) {
                cout << "�������� ����. ��������������� ����� �� ������ ��������� ����. ���������� �����: ";
            }
            cout << "������� �������: ";
            while (!safeInput(capital)) {
                cout << "�������� ����. �������� ������� �� ������ ��������� ����. ���������� �����: ";
            }
            cout << "������� ����������� ����: ";
            while (!safeInput(language)) {
                cout << "�������� ����. �������� ����� �� ������ ��������� ����. ���������� �����: ";
            }
            cout << "������� �������: ";
            while (!safeInput(area)) {
                cout << "�������� ����. ������� ������� �����: ";
            }
            cout << "������� ����������� ���������: ";
            while (!safeInput(population)) {
                cout << "�������� ����. ������� ����������� ��������� �����: ";
            }
            cout << "������� �������� �������: ";
            while (!safeInput(currency)) {
                cout << "�������� ����. �������� ������ �� ������ ��������� ����. ���������� �����: ";
            }
            cout << "������� ���� ������ � �������: ";
            while (!safeInput(exchangeRateToUSD)) {
                cout << "�������� ����. ������� ���� ������ �����: ";
            }
            db.addCountry(name, government, capital, language, area, population, currency, exchangeRateToUSD);
            break;
        }

        case 4: {
            string name;
            cout << "������� �������� ������ ��� ��������: ";
            getline(cin, name);
            db.deleteCountry(name);
            break;
        }
        case 5:
            db.displayCountries();
            break;
        case 6: {
            string field, value;
            cout << "������� ���� ��� ������ (name/government/capital/language/currency): ";
            getline(cin, field);
            if (field != "name" && field != "government" && field != "capital" && field != "language" && field != "currency") {
                cout << "�������� ���� ��� ������. ���������� �����.\n";
                break;
            }
            cout << "������� �������� ��� ������: ";
            getline(cin, value);
            db.queryByField(field, value);
            break;
        }
        case 7: {
            string field;
            cout << "������� ���� ��� ���������� (name/population/area): ";
            getline(cin, field);
            if (field != "name" && field != "population" && field != "area") {
                cout << "�������� ���� ��� ����������. ���������� �����.\n";
                break;
            }
            db.sortByField(field);
            break;
        }
        case 8:
            cout << db.getLastDeletedCountry() << endl;
            break;
        case 9:
            cout << "����� �� ���������.\n";
            break;

        default:
            cout << "�������� �����. ���������� �����.\n";
        }
    } while (choice != 9);

    return 0;
}