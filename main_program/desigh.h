#ifndef DESIGH_H_INCLUDED
#define DESIGH_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h> // ��� ������ � ��������� Windows API
using namespace std;
void setTextColorRed() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

// ������� ��� ������ ����� ������ �� ����������� (�����)
void resetTextColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // ����� �����
}
void setTextColorBlue() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void printASCIIArt() {
    setTextColorRed(); // �������� ������� ����
    cout << "                                                                                                        " << endl;
    cout << "               ###### ######    #####  ##  ## ##### #####   ####  ##  ##    ###### ##     ##  ##  ####  " << endl;
    cout << "                 ##     ##      ##     ### ## ##    ##  ## ##      ####     ##  ## ##     ##  ## ##     " << endl;
    cout << "                 ##     ##      ####   ## ### ####  #####  ## ###   ##      #####  ##     ##  ##  ####  " << endl;
    cout << "                 ##     ##      ##     ##  ## ##    ##  ## ##  ##   ##      ##     ##     ##  ##     ## " << endl;
    cout << "               ######   ##      #####  ##  ## ##### ##  ##  ####    ##      ##     ######  ####   ####  " << endl;
    cout << "                                                                                                        " << endl;
    resetTextColor(); // ���������� ���� �� �����������
    setTextColorBlue();
    cout << "                                                                                                        " << endl;
    cout << "                                               Scrum: Georgiy Galichev                                  " << endl;
    cout << "                            Developers: Furman Arseniy, Alexandr Gilev, Sergey Vernigor                 " << endl;
    cout << "                                         Testers: Yarushin Artemii, Chukin Kirill                       " << endl;
    cout << "                                       Analitics: Bushuev Andrey, Maks Kuzevanov                        " << endl;
    resetTextColor();
    cout << "                                                                                                        " << endl;
    cout << "                                                                                                        " << endl;
}

#endif // DESIGH_H_INCLUDED
