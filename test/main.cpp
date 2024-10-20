#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

// Размер блока в байтах
const int BLOCK_SIZE = 16;
const string ENCRYPTION_MARK = "ENCRYPTED";

// Функция для гаммирования с модулем 2
vector<uint8_t> gamma_encrypt(const vector<uint8_t>& data, const vector<uint8_t>& gamma) {
    vector<uint8_t> processed_data(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        processed_data[i] = data[i] ^ gamma[i % gamma.size()];
    }
    return processed_data;
}

// Функция для вывода гаммы в удобочитаемом формате
void print_gamma(const vector<uint8_t>& gamma) {
    cout << "Gamma: ";
    for (const auto& byte : gamma) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
    }
    cout << dec << endl;
}

// Функция для генерации обычной случайной гаммы
vector<uint8_t> generate_random_gamma(size_t size) {
    vector<uint8_t> gamma(size);
    for (size_t i = 0; i < size; ++i) {
        gamma[i] = static_cast<uint8_t>(rand() % 256);
    }
    return gamma;
}

// Функция для генерации гаммы в зависимости от секунд (Ш4.2.1)
vector<uint8_t> generate_gamma_seconds_based() {
    time_t now = time(0);
    tm* local_time = localtime(&now);
    int seconds = local_time->tm_sec;

    vector<uint8_t> gamma(BLOCK_SIZE);

    // Предопределённые гаммы
    vector<uint8_t> gamma_even = {0xAB, 0xCD, 0xEF, 0x01,
                                  0x23, 0x45, 0x67, 0x89,
                                  0xAB, 0xCD, 0xEF, 0x01,
                                  0x23, 0x45, 0x67, 0x89};

    vector<uint8_t> gamma_odd = {0x67, 0x45, 0x23, 0x01,
                                 0xAB, 0xCD, 0xEF, 0x89,
                                 0x67, 0x45, 0x23, 0x01,
                                 0xAB, 0xCD, 0xEF, 0x89};

    if ((seconds / 10) % 2 == 0) {
        gamma = gamma_even;
    } else {
        gamma = gamma_odd;
    }

    return gamma;
}

// Функция для генерации гаммы в зависимости от числа месяца (Ш4.2.2)
vector<uint8_t> generate_gamma_date_based() {
    time_t now = time(0);
    tm* local_time = localtime(&now);
    int day_of_month = local_time->tm_mday;

    vector<uint8_t> gamma(BLOCK_SIZE);

    // Используем те же гаммы, что и в предыдущей функции
    vector<uint8_t> gamma_even = {0xAB, 0xCD, 0xEF, 0x01,
                                  0x23, 0x45, 0x67, 0x89,
                                  0xAB, 0xCD, 0xEF, 0x01,
                                  0x23, 0x45, 0x67, 0x89};

    vector<uint8_t> gamma_odd = {0x67, 0x45, 0x23, 0x01,
                                 0xAB, 0xCD, 0xEF, 0x89,
                                 0x67, 0x45, 0x23, 0x01,
                                 0xAB, 0xCD, 0xEF, 0x89};

    if (day_of_month % 2 == 0) {
        gamma = gamma_even;
    } else {
        gamma = gamma_odd;
    }

    return gamma;
}

// Функция для генерации гаммы в зависимости от дня недели (Ш4.2.3)
vector<uint8_t> generate_gamma_weekday_based() {
    time_t now = time(0);
    tm* local_time = localtime(&now);
    int day_of_week = local_time->tm_wday; // 0 - воскресенье, 1 - понедельник, ..., 6 - суббота

    vector<uint8_t> gamma(BLOCK_SIZE);

    vector<uint8_t> gamma_mon_wed_fri = {0xAB, 0xCD, 0xEF, 0x01,
                                         0x23, 0x45, 0x67, 0x89,
                                         0xAB, 0xCD, 0xEF, 0x01,
                                         0x23, 0x45, 0x67, 0x89};

    vector<uint8_t> gamma_tue_thu = {0x67, 0x45, 0x89, 0xEF,
                                     0xAB, 0xCD, 0x01, 0x23,
                                     0x67, 0x45, 0x89, 0xEF,
                                     0xAB, 0xCD, 0x01, 0x23};

    vector<uint8_t> gamma_sat_sun = {0x67, 0x45, 0xCD, 0xAB,
                                     0x89, 0xEF, 0x23, 0x01,
                                     0x67, 0x45, 0xCD, 0xAB,
                                     0x89, 0xEF, 0x23, 0x01};

    if (day_of_week == 1 || day_of_week == 3 || day_of_week == 5) {
        // Понедельник, среда, пятница
        gamma = gamma_mon_wed_fri;
    } else if (day_of_week == 2 || day_of_week == 4) {
        // Вторник, четверг
        gamma = gamma_tue_thu;
    } else {
        // Суббота, воскресенье
        gamma = gamma_sat_sun;
    }

    return gamma;
}

// Функция для чтения гаммы, введённой пользователем
vector<uint8_t> input_gamma(size_t size) {
    vector<uint8_t> gamma(size);
    cout << "Enter the gamma in hexadecimal format (" << size << " bytes): ";

    for (size_t i = 0; i < size; ++i) {
        int byte;
        cin >> hex >> byte;
        gamma[i] = static_cast<uint8_t>(byte);
    }

    return gamma;
}

// Функция для чтения файла и преобразования его содержимого в байты
vector<uint8_t> read_file(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    vector<uint8_t> file_data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return file_data;
}

// Функция для записи данных в файл
void write_file(const string& filename, const vector<uint8_t>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
}

// Функция для проверки, зашифрован ли файл
bool is_file_encrypted(const vector<uint8_t>& data) {
    if (data.size() < ENCRYPTION_MARK.size()) {
        return false;
    }
    // Проверяем наличие метки "ENCRYPTED" в конце файла
    for (size_t i = 0; i < ENCRYPTION_MARK.size(); ++i) {
        if (data[data.size() - ENCRYPTION_MARK.size() + i] != ENCRYPTION_MARK[i]) {
            return false;
        }
    }
    return true;
}

// Функция для проверки, правильно ли расшифрован файл
bool check_decryption(const vector<uint8_t>& decrypted_data) {
    // Файл должен быть без метки "ENCRYPTED" в конце после расшифровки
    return !is_file_encrypted(decrypted_data);
}

int main() {
    srand(time(0));
    string input_filename;
    int choice, encryption_mode;

    cout << "Choose an option:\n";
    cout << "1. Encrypt file\n";
    cout << "2. Decrypt file\n";
    cin >> choice;

    cout << "Enter the name of the file: ";
    cin >> input_filename;

    // Чтение данных из файла
    vector<uint8_t> file_data = read_file(input_filename);

    if (choice == 1) {
        // Проверяем, зашифрован ли файл
        if (is_file_encrypted(file_data)) {
            cout << "Error: The file is already encrypted." << endl;
        } else {
            cout << "Choose encryption mode:\n";
            cout << "1. Standard random gamma\n";
            cout << "2. Time-based gamma (Ш4.2.1)\n";
            cout << "3. Date-based gamma (Ш4.2.2)\n";
            cout << "4. Weekday-based gamma (Ш4.2.3)\n";
            cin >> encryption_mode;

            vector<uint8_t> gamma;

            // Генерация гаммы в зависимости от выбранного режима
            if (encryption_mode == 1) {
                gamma = generate_random_gamma(BLOCK_SIZE);
            } else if (encryption_mode == 2) {
                gamma = generate_gamma_seconds_based();
            } else if (encryption_mode == 3) {
                gamma = generate_gamma_date_based();
            } else if (encryption_mode == 4) {
                gamma = generate_gamma_weekday_based();
            } else {
                cout << "Invalid encryption mode selected." << endl;
                return 1;
            }

            // Вывод гаммы пользователю
            print_gamma(gamma);

            // Шифрование данных
            vector<uint8_t> encrypted_data = gamma_encrypt(file_data, gamma);

            // Добавляем метку "ENCRYPTED" в конец данных
            for (char c : ENCRYPTION_MARK) {
                encrypted_data.push_back(static_cast<uint8_t>(c));
            }

            // Запись зашифрованных данных в тот же файл
            write_file(input_filename, encrypted_data);

            cout << "File encrypted and saved as " << input_filename << endl;
        }
    } else if (choice == 2) {
        // Проверяем, зашифрован ли файл
        if (!is_file_encrypted(file_data)) {
            cout << "Error: The file does not appear to be encrypted." << endl;
        } else {
            // Удаляем метку "ENCRYPTED" из файла перед расшифрованием
            vector<uint8_t> encrypted_data = file_data;
            encrypted_data.resize(file_data.size() - ENCRYPTION_MARK.size());

            cout << "Choose decryption mode:\n";
            cout << "1. Standard gamma (input gamma manually)\n";
            cout << "2. Time-based gamma (Ш4.2.1)\n";
            cout << "3. Date-based gamma (Ш4.2.2)\n";
            cout << "4. Weekday-based gamma (Ш4.2.3)\n";
            cin >> encryption_mode;

            vector<uint8_t> gamma;

            // Генерация гаммы в зависимости от выбранного режима
            if (encryption_mode == 1) {
                // Запрос гаммы у пользователя
                gamma = input_gamma(BLOCK_SIZE);
            } else if (encryption_mode == 2) {
                gamma = generate_gamma_seconds_based();
            } else if (encryption_mode == 3) {
                gamma = generate_gamma_date_based();
            } else if (encryption_mode == 4) {
                gamma = generate_gamma_weekday_based();
            } else {
                cout << "Invalid decryption mode selected." << endl;
                return 1;
            }

            // Расшифрование данных
            vector<uint8_t> decrypted_data = gamma_encrypt(encrypted_data, gamma);

            // Проверка, удалось ли расшифровать файл
            if (check_decryption(decrypted_data)) {
                // Запись расшифрованных данных в файл
                write_file(input_filename, decrypted_data);
                cout << "File decrypted and saved as " << input_filename << endl;
            } else {
                cout << "Error: Decryption failed. File remains encrypted." << endl;
            }
        }
    } else {
        cout << "Invalid choice. Exiting..." << endl;


    return 0;
}
