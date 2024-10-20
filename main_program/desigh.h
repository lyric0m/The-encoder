#ifndef DESIGH_H_INCLUDED
#define DESIGH_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h> // Для работы с функциями Windows API
using namespace std;
void setTextColorRed() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

// Функция для сброса цвета текста на стандартный (белый)
void resetTextColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Белый текст
}
void setTextColorBlue() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void printASCIIArt() {
    setTextColorRed(); // Включаем красный цвет
    cout << "                                                                                                        " << endl;
    cout << "               ###### ######    #####  ##  ## ##### #####   ####  ##  ##    ###### ##     ##  ##  ####  " << endl;
    cout << "                 ##     ##      ##     ### ## ##    ##  ## ##      ####     ##  ## ##     ##  ## ##     " << endl;
    cout << "                 ##     ##      ####   ## ### ####  #####  ## ###   ##      #####  ##     ##  ##  ####  " << endl;
    cout << "                 ##     ##      ##     ##  ## ##    ##  ## ##  ##   ##      ##     ##     ##  ##     ## " << endl;
    cout << "               ######   ##      #####  ##  ## ##### ##  ##  ####    ##      ##     ######  ####   ####  " << endl;
    cout << "                                                                                                        " << endl;
    resetTextColor(); // Сбрасываем цвет на стандартный
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
