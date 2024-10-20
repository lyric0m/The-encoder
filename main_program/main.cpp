


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include <clocale>
#include "desigh.h"
using namespace std;

// размер блока и модуль гаммирования
const int BLOCK_SIZE = 16;
int MODULUS;


// функция для определения, каких символов в тексте больше — латинских или кириллических
char detectMajorityAlphabet(const string& text) {
    int latinCount = 0;
    int cyrillicCount = 0;

    for (unsigned char c : text) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            latinCount++;
        } else if (c >= 0xC0 && c <= 0xFF) { // кириллические символы в Windows-1251
            cyrillicCount++;
        }
    }
    return (latinCount > cyrillicCount) ? 'L' : 'C';
}

// Функция для создания гаммы на основе выбранного алфавита
string generateGamma(char alphabetType) {
    string gamma;

    if (alphabetType == 'L') {
        gamma = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    } else {
        gamma = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЫЭЮЯ";
    }
    int MODULUS;
    // Генерируем гамму длиной BLOCK_SIZE с использованием модуля
    string result;
    srand(static_cast<unsigned int>(time(nullptr))); // Инициализируем генератор случайных чисел
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        result += gamma[(rand() % gamma.size()) % MODULUS]; // Применение модуля гаммирования
    }
    return result;
}

// функция для создания гаммы на основе времени (Ш4.2.4, Ш4.2.5, Ш4.2.6)
string generateTimeBasedGamma(int methodChoice) {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    string gamma;

    switch (methodChoice) {
        case 1: // Ш4.2.4 (маркант от секунд)
            if (ltm->tm_sec / 10 % 2 == 0) {
                gamma = "ABCDEFGHIJKLMNOP"; // Чётные 10 секунд
            } else {
                gamma = "GHEFABCDOPMNIJKL"; // Нечётные 10 секунд
            }
            break;

        case 2: // Ш4.2.5 (маркант от дня месяца)
            if (ltm->tm_mday % 2 == 0) {
                gamma = "ABCDEFGHIJKLMNOP"; // Чётное число месяца
            } else {
                gamma = "GHEFABCDOPMNIJKL"; // Нечётное число месяца
            }
            break;

        case 3: // Ш4.2.6 (маркант от дня недели)
            switch (ltm->tm_wday) {
                case 0: // Воскресенье
                case 6: // Суббота
                    gamma = "GHEFCDABOPMNIJKL"; // Сб, Вс
                    break;
                case 1: // Понедельник
                case 3: // Среда
                case 5: // Пятница
                    gamma = "ABCDEFGHIJKLMNOP"; // Пн, Ср, Пт
                    break;
                case 2: // Вторник
                case 4: // Четверг
                    gamma = "GHEFOPMNABCDIJKL"; // Вт, Чт
                    break;
            }
            break;
    }

    // обеспечиваем длину гаммы равной BLOCK_SIZE
    while (gamma.size() < BLOCK_SIZE) {
        gamma += gamma; // повторяем гамму, чтобы достичь нужной длины
    }
    gamma.resize(BLOCK_SIZE); // обрезаем до нужной длины

    return gamma;
}

// функция шифрования/дешифрования методом XOR
string xorBlock(const string& text, const string& gamma, int MODULUS) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
            if (MODULUS == 69){
                result += text[i] ^ gamma[i % BLOCK_SIZE % MODULUS];  // XOR с гаммой, модуль используется для циклического доступа к гамме
            }
            else {
                 result += text[i] ^ gamma[i % BLOCK_SIZE];  // `BLOCK_SIZE` — это длина гаммы
            }
    }
    return result;
}

// функция для вычисления контрольной суммы
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c; // XOR для вычисления контрольной суммы
    }
    return checksum;
}

// функция для удаления байта длины или контрольной суммы
string removeLengthByte(const string& text) {
    if (text.size() > 1) {
        return text.substr(0, text.size() - 1); // Убираем последний байт
    }
    return text;
}

int main() {
    setlocale(LC_ALL, "Russian"); // устанавливаем локаль для Windows-1251
    printASCIIArt();

    while (true) {
        string inputFileName;
        char action;

        // запрашиваем действие от пользователя: шифровать или расшифровывать
        cout << "Выберите действие: (e) шифровать или (d) расшифровать (q для выхода): ";
        cin >> action;

        int MODULUS = 69;


        // запрашиваем имя файла
        cout << "Введите имя файла: ";
        cin >> inputFileName;

        // открываем файл для чтения в бинарном режиме
        ifstream inputFile(inputFileName, ios::binary);
        if (!inputFile.is_open()) {
            cerr << "Ошибка при открытии файла!" << endl;
            return 1;
        }

        // читаем содержимое файла
        string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        string resultText;

        if (action == 'e') {
            cout << "Выберите модуль гаммирования:(69 или 2) ";
            cin >> MODULUS;
            // проверка выхода
            if (action == 'q') {
            cout << "Выход успешен!" << endl;
            break;
        }
            // запрашиваем метод шифрования
            int methodChoice;
            cout << "Выберите метод шифрования:\n";
            cout << "1. Ш4.1 блочный шифр гаммирования\n";
            cout << "2. Ш4.2.1 (маркант от секунд)\n";
            cout << "3. Ш4.2.2 (маркант от числа месяца)\n";
            cout << "4. Ш4.2.3 (маркант от дня недели)\n";
            cout << "5. Ш4.3 Ш4.1 с байтом длины в конце\n";
            cout << "6. Ш4.4 Ш4.1 с байтом контрольной суммы в конце\n";
            cout << "Выберите метод (1-6): ";
            cin >> methodChoice;


            cout << "Шифрование файла..." << endl;

            string gamma;
            // Генерация гаммы на основе метода
            if (methodChoice == 1 || methodChoice == 5 || methodChoice == 6) {
    char majorityAlphabet = detectMajorityAlphabet(text);
    gamma = generateGamma(majorityAlphabet); // Генерация гаммы
} else if (methodChoice == 2 || methodChoice == 3 || methodChoice == 4) {
    gamma = generateTimeBasedGamma(methodChoice - 1); // Генерация гаммы на основе времени
    }


            resultText = xorBlock(text, gamma, MODULUS);

            // Добавление байта длины или контрольной суммы, если это требуется
            if (methodChoice == 5) {
                unsigned char lengthByte = static_cast<unsigned char>(text.size());
                resultText += lengthByte; // Добавляем байт длины
                cout << "Использованная длина: " << lengthByte << endl;

            } else if (methodChoice == 6) {
                unsigned char checksumByte = calculateChecksum(text);
                resultText += checksumByte; // Добавляем байт контрольной суммы
                cout << "Использованная контрольная сумма: " << checksumByte << endl;
            }

            // Выводим информацию для пользователя
            cout << "Модуль гаммирования: " << MODULUS << endl;
            cout << "Зашифрованный текст: " << resultText << endl;
            cout << "Использованная гамма: " << gamma << endl;

            // Сохраняем гамму в файл для последующего дешифрования
            ofstream gammaFile("gamma.txt", ios::binary);
            gammaFile << gamma;
            gammaFile.close();

        } else if (action == 'd') {
            // Дешифрование текста
            string gamma;
            cout << "Загружаем гамму для дешифрования из файла..." << endl;

            // Открываем файл с гаммой
            ifstream gammaFile("gamma.txt", ios::binary);
            if (!gammaFile.is_open()) {
                cerr << "Ошибка при открытии файла с гаммой!" << endl;
                return 1;
            }
            getline(gammaFile, gamma); // Читаем гамму
            gammaFile.close();

            // Проверяем, что гамма правильной длины
            if (gamma.size() != BLOCK_SIZE) {
                cerr << "Ошибка: гамма должна быть длиной 16 символов!" << endl;
                return 1;
            }


            // Дешифруем блоки текста
            resultText = xorBlock(text, gamma, MODULUS);


             char hasLengthByte;
            cout << "Присутствует ли байт длины или контрольной суммы в зашифрованном тексте? (y/n): ";
            cin >> hasLengthByte;

            if (hasLengthByte == 'y') {
                // Удаляем байт длины
                resultText = removeLengthByte(resultText);
                cout << "Байт длины удалён." << endl;
            }

            // Вывод расшифрованного текста
            cout << "Текст расшифрован: " << resultText << endl;
        }

        // Сохраняем результат в новый файл
        ofstream outputFile("test.txt", ios::binary);
        outputFile << resultText;
        outputFile.close();
        cout << "Результат сохранён." << endl;
    }

    return 0;
}
