#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>  // ��� ������������� srand

// ������ ����� � ������ ������������
const int BLOCK_SIZE = 16;
const int MODULUS = 69;

// ������� ��� �����������, ����� �������� � ������ ������ � ��������� ��� �������������
char detectMajorityAlphabet(const std::string& text) {
    int latinCount = 0;
    int cyrillicCount = 0;

    for (char c : text) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            latinCount++;
        } else if ((c >= '�' && c <= '�') || (c >= '�' && c <= '�')) {
            cyrillicCount++;
        }
    }
    return (latinCount > cyrillicCount) ? 'L' : 'C';
}

// ������� ��� �������� ����� �� ������ ���������� ��������
std::string generateGamma(char alphabetType, int gammaLength) {
    std::string gamma;

    // �������������� ��������� ��������� �����
    srand(static_cast<unsigned int>(time(nullptr)));

    if (alphabetType == 'L') {
        // ��������� ����� �� ��������� ��������
        std::string latinChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (int i = 0; i < gammaLength; i++) {
            gamma += latinChars[rand() % latinChars.size()];
        }
    } else {
        // ��������� ����� �� ������������� ��������
        std::string cyrillicChars = "������������������������������0123456789";
        for (int i = 0; i < gammaLength; i++) {
            gamma += cyrillicChars[rand() % cyrillicChars.size()];
        }
    }

    return gamma;
}

// ������� ��� ������ ����� �� ������ ���������� ������� (�������, ����� ������, ���� ������)
std::string getTimeBasedGamma(int timeOption) {
    std::string evenGamma = "ABCDEFGHIJKLMNOP";  // ������ ��������
    std::string oddGamma = "GHEFABCDOPMNIJKL";  // �������� ��������

    // �������� ������� �����
    time_t now = time(0);
    tm *ltm = localtime(&now);

    switch (timeOption) {
        case 1:  // �4.2.4 (������� �� ������)
            return (ltm->tm_sec % 20 < 10) ? evenGamma : oddGamma;
        case 2:  // �4.2.5 (������� �� ����� ������)
            return (ltm->tm_mday % 2 == 0) ? evenGamma : oddGamma;
        case 3:  // �4.2.6 (������� �� ��� ������)
            switch (ltm->tm_wday) {
                case 0: // �����������
                case 6: // �������
                    return "GHEFCDABOPMNKLIJ"; // ����� ��� ��, ��
                case 1: // �����������
                case 3: // �����
                    return evenGamma; // ����� ��� ��, ��
                case 2: // �������
                case 4: // �������
                    return "GHEFOPMNABCDIJKL"; // ����� ��� ��, ��
                case 5: // �������
                    return evenGamma; // ����� ��� ��
            }
            break;
    }
    return evenGamma; // �� ���������
}

// ������� ���������� ������� XOR
std::string xorBlock(const std::string& text, const std::string& gamma) {
    std::string result;

    for (size_t i = 0; i < text.size(); i++) {
        char xorChar = text[i] ^ gamma[i];  // ��������� XOR
        result += xorChar;
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string inputFileName;
    char action;
    int methodChoice;

    // ���������� ������������ �������: ��������� ��� ��������������
    std::cout << "�������� ��������: (e) ��������� ��� (d) ������������: ";
    std::cin >> action;

    // ������ ����� ����� � ������������
    std::cout << "������� ��� �����: ";
    std::cin >> inputFileName;

    // ��������� ���� ��� ������
    std::ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "������ ��� �������� �����!" << std::endl;
        return 1;
    }

    // ������ ���������� �����
    std::string text((std::istreambuf_iterator<char>(inputFile)),
                     std::istreambuf_iterator<char>());
    inputFile.close();

    std::string resultText;

    if (action == 'e') {
        // ���������� ������
        std::cout << "���������� �����..." << std::endl;

        // ����� ������ ����������
        std::cout << "�������� ����� ����������: \n1 - �4.1.2 \n2 - �4.2.4 (������� �� ������) \n3 - �4.2.5 (������� �� ����� ������) \n4 - �4.2.6 (������� �� ��� ������) \n��� �����: ";
        std::cin >> methodChoice;

        std::string gamma;
        if (methodChoice == 1) {
            // ���������� ������������ ����� �� ������ �������� � �������� ������
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

        // ����� �������������� ������ � �����
        std::cout << "������������� �����: " << resultText << std::endl;
        std::cout << "�������������� �����: " << gamma << std::endl;

    } else if (action == 'd') {
        // ������������ ������
        std::string gamma;
        std::cout << "������� �����, �������������� ��� ���������� (������ ���� ������ 16 ��������): ";
        std::cin >> gamma;

        if (gamma.size() != BLOCK_SIZE) {
            std::cerr << "������: ����� ����� ������ ���� 16 ��������!" << std::endl;
            return 1;
        }

        std::cout << "������������ �����..." << std::endl;

        for (size_t i = 0; i < text.size(); i += BLOCK_SIZE) {
            std::string block = text.substr(i, BLOCK_SIZE);
            std::string decryptedBlock = xorBlock(block, gamma);
            resultText += decryptedBlock;
        }

        // ����� ��������������� ������
        std::cout << "�������������� �����: " << resultText << std::endl;

    } else {
        std::cerr << "������������ ����� ��������!" << std::endl;
        return 1;
    }

    // ���������� ���������� � �������� ����
    std::ofstream outputFile(inputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "������ ��� �������� ����� ��� ������!" << std::endl;
        return 1;
    }
    outputFile << resultText;
    outputFile.close();

    std::cout << "��������� �������� � ����: " << inputFileName << std::endl;

    return 0;
}
