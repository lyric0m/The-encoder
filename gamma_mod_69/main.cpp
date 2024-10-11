#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>  // Для инициализации srand

// Размер блока и модуль гаммирования
const int BLOCK_SIZE = 16;
const int MODULUS = 69;

// Функция для определения, каких символов в тексте больше — латинских или кириллических
char detectMajorityAlphabet(const std::string& text) {
    int latinCount = 0;
    int cyrillicCount = 0;

    for (char c : text) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            latinCount++;
        } else if ((c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'я')) {
            cyrillicCount++;
        }
    }
    return (latinCount > cyrillicCount) ? 'L' : 'C';
}

// Функция для создания гаммы на основе выбранного алфавита
std::string generateGamma(char alphabetType, int gammaLength) {
    std::string gamma;

    // Инициализируем генератор случайных чисел
    srand(static_cast<unsigned int>(time(nullptr)));

    if (alphabetType == 'L') {
        // Генерация гаммы из латинских символов
        std::string latinChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (int i = 0; i < gammaLength; i++) {
            gamma += latinChars[rand() % latinChars.size()];
        }
    } else {
        // Генерация гаммы из кириллических символов
        std::string cyrillicChars = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЫЭЮЯ0123456789";
        for (int i = 0; i < gammaLength; i++) {
            gamma += cyrillicChars[rand() % cyrillicChars.size()];
        }
    }

    return gamma;
}

// Функция для выбора гаммы на основе временного маркера (секунды, число месяца, день недели)
std::string getTimeBasedGamma(int timeOption) {
    std::string evenGamma = "ABCDEFGHIJKLMNOP";  // Четное значение
    std::string oddGamma = "GHEFABCDOPMNIJKL";  // Нечетное значение

    // Получаем текущее время
    time_t now = time(0);
    tm *ltm = localtime(&now);

    switch (timeOption) {
        case 1:  // Ш4.2.4 (маркант от секунд)
            return (ltm->tm_sec % 20 < 10) ? evenGamma : oddGamma;
        case 2:  // Ш4.2.5 (маркант от числа месяца)
            return (ltm->tm_mday % 2 == 0) ? evenGamma : oddGamma;
        case 3:  // Ш4.2.6 (маркант от дня недели)
            switch (ltm->tm_wday) {
                case 0: // Воскресенье
                case 6: // Суббота
                    return "GHEFCDABOPMNKLIJ"; // Гамма для СБ, ВС
                case 1: // Понедельник
                case 3: // Среда
                    return evenGamma; // Гамма для ПН, СР
                case 2: // Вторник
                case 4: // Четверг
                    return "GHEFOPMNABCDIJKL"; // Гамма для ВТ, ЧТ
                case 5: // Пятница
                    return evenGamma; // Гамма для ПТ
            }
            break;
    }
    return evenGamma; // По умолчанию
}

// Функция шифрования методом XOR
std::string xorBlock(const std::string& text, const std::string& gamma) {
    std::string result;

    for (size_t i = 0; i < text.size(); i++) {
        char xorChar = text[i] ^ gamma[i];  // Применяем XOR
        result += xorChar;
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string inputFileName;
    char action;
    int methodChoice;

    // Предлагаем пользователю выбрать: шифровать или расшифровывать
    std::cout << "Выберите действие: (e) шифровать или (d) расшифровать: ";
    std::cin >> action;

    // Запрос имени файла у пользователя
    std::cout << "Введите имя файла: ";
    std::cin >> inputFileName;

    // Открываем файл для чтения
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
        return 1;
    }

    // Читаем содержимое файла
    std::string text((std::istreambuf_iterator<char>(inputFile)),
                     std::istreambuf_iterator<char>());
    inputFile.close();

    std::string resultText;

    if (action == 'e') {
        // Шифрование текста
        std::cout << "Шифрование файла..." << std::endl;

        // Выбор метода шифрования
        std::cout << "Выберите метод шифрования: \n1 - Ш4.1.2 \n2 - Ш4.2.4 (маркант от секунд) \n3 - Ш4.2.5 (маркант от числа месяца) \n4 - Ш4.2.6 (маркант от дня недели) \nВаш выбор: ";
        std::cin >> methodChoice;

        std::string gamma;
        if (methodChoice == 1) {
            // Определяем используемую гамму на основе символов в открытом тексте
            char majorityAlphabet = detectMajorityAlphabet(text);
            gamma = generateGamma(majorityAlphabet, BLOCK_SIZE);
        } else {
            gamma = getTimeBasedGamma(methodChoice - 1);
        }

        for (size_t i = 0; i < text.size(); i += BLOCK_SIZE) {
            std::string block = text.substr(i, BLOCK_SIZE);
            std::string encryptedBlock = xorBlock(block, gamma);
            resultText += encryptedBlock;
        }

        // Вывод зашифрованного текста и гаммы
        std::cout << "Зашифрованный текст: " << resultText << std::endl;
        std::cout << "Использованная гамма: " << gamma << std::endl;

    } else if (action == 'd') {
        // Дешифрование текста
        std::string gamma;
        std::cout << "Введите гамму, использованную при шифровании (должна быть длиной 16 символов): ";
        std::cin >> gamma;

        if (gamma.size() != BLOCK_SIZE) {
            std::cerr << "Ошибка: длина гаммы должна быть 16 символов!" << std::endl;
            return 1;
        }

        std::cout << "Дешифрование файла..." << std::endl;

        for (size_t i = 0; i < text.size(); i += BLOCK_SIZE) {
            std::string block = text.substr(i, BLOCK_SIZE);
            std::string decryptedBlock = xorBlock(block, gamma);
            resultText += decryptedBlock;
        }

        // Вывод расшифрованного текста
        std::cout << "Расшифрованный текст: " << resultText << std::endl;

    } else {
        std::cerr << "Некорректный выбор действия!" << std::endl;
        return 1;
    }

    // Сохранение результата в исходный файл
    std::ofstream outputFile(inputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка при создании файла для записи!" << std::endl;
        return 1;
    }
    outputFile << resultText;
    outputFile.close();

    std::cout << "Результат сохранен в файл: " << inputFileName << std::endl;

    return 0;
}
