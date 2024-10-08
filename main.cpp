#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// ������ ����� � ������
const int BLOCK_SIZE = 16;
const string ENCRYPTION_MARK = "ENCRYPTED";

// ������� ��� ������������ � ������� 2
vector<uint8_t> gamma_encrypt(const vector<uint8_t>& data, const vector<uint8_t>& gamma) {
    vector<uint8_t> processed_data(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        processed_data[i] = data[i] ^ gamma[i % gamma.size()];
    }
    return processed_data;
}

// ������� ��� ������ ����� � �������������� ��� ����������� � �����
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

// ������� ��� ������ ������ � ����
void write_file(const string& filename, const vector<uint8_t>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
}

// ������� ��� ��������, ���������� �� ����
bool is_file_encrypted(const vector<uint8_t>& data) {
    if (data.size() < ENCRYPTION_MARK.size()) {
        return false;
    }
    // ��������� ������� ����� "ENCRYPTED" � ����� �����
    for (size_t i = 0; i < ENCRYPTION_MARK.size(); ++i) {
        if (data[data.size() - ENCRYPTION_MARK.size() + i] != ENCRYPTION_MARK[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    string input_filename;
    int choice;

    // ������ ����� (����� ���� ������������ �����)
    vector<uint8_t> gamma = {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};

    // ����� ����� ����������� � ��������������
    cout << "Choose an option:\n";
    cout << "1. Encrypt file\n";
    cout << "2. Decrypt file\n";
    cin >> choice;

    cout << "Enter the name of the file: ";
    cin >> input_filename;

    // ������ ������ �� �����
    vector<uint8_t> file_data = read_file(input_filename);

    if (choice == 1) {
        // ���������, ���������� �� ����
        if (is_file_encrypted(file_data)) {
            cout << "Error: The file is already encrypted." << endl;
        } else {
            // ����������
            // ���������� ������
            vector<uint8_t> encrypted_data = gamma_encrypt(file_data, gamma);

            // ��������� ����� "ENCRYPTED" � ����� ������
            for (char c : ENCRYPTION_MARK) {
                encrypted_data.push_back(static_cast<uint8_t>(c));
            }

            // ������ ������������� ������ � ��� �� ����
            write_file(input_filename, encrypted_data);

            cout << "File encrypted and saved as " << input_filename << endl;
        }
    } else if (choice == 2) {
        // ���������, ���������� �� ����
        if (!is_file_encrypted(file_data)) {
            cout << "Error: The file does not appear to be encrypted." << endl;
        } else {
            // ������� ����� "ENCRYPTED" �� ����� ����� ��������������
            file_data.resize(file_data.size() - ENCRYPTION_MARK.size());

            // ������������� ������ (���������� ��� �� ������ ���������� ��������)
            vector<uint8_t> decrypted_data = gamma_encrypt(file_data, gamma);

            // ������ �������������� ������ � ��� �� ����
            write_file(input_filename, decrypted_data);

            cout << "File decrypted and saved as " << input_filename << endl;
        }
    } else {
        cout << "Invalid choice. Exiting..." << endl;
    }

    return 0;
}
