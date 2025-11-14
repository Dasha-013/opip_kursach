#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <direct.h>
#include <memory>
#include "Base.h"
#include "Login.h"       
#include "Functions.h"   

using namespace std;

streambuf* originalClogBuffer;
ofstream logFile;

void preSetup() {
    system("mode con cols=150 lines=40");
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    logFile.open("log.txt");
    if (!logFile.is_open()) {
        cerr << "Не удалось открыть файл логов.\n";
        exit(1);
    }

    originalClogBuffer = clog.rdbuf();
    clog.rdbuf(logFile.rdbuf());

    _mkdir("data");
    _mkdir("data\\users");

    srand(static_cast<unsigned>(time(0)));
}

void run() {
    cout.setf(ios::left);

    while (true) {
        LoginSystem loginSystem;
        Base* payrollSystem = loginSystem.run();  

        if (!payrollSystem) {
            cout << "Выход из программы. До свидания!" << endl;
            break;
        }

        system("CLS"); 
        payrollSystem->run();
        delete payrollSystem;

        cout << "\nВозврат в систему входа..." << endl;
        system("pause");
        system("CLS");
    }
}

void preExit() {
    clog.rdbuf(originalClogBuffer);
    logFile.close();
}

int main() {
    preSetup();
    run();
    preExit();
    return 0;
}
