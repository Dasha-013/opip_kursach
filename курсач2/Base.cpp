#include "Base.h"
#include "Functions.h"
#include "PayrollSession.h"
#include "PayrollPeriod.h"
#include "SalaryDocumentation.h"
#include "PayrollSession.h"

EmployeeBase::EmployeeBase() {
    readEmployees();
}

EmployeeBase::~EmployeeBase() {
    writeEmployees();
    employees.clear();
}

void EmployeeBase::readEmployees() {
    std::ifstream in("data\\employees.bin", std::ios::binary);
    if (checkInpFile(in)) {
        int size = 0;
        in.read((char*)(&size), sizeof(size));
        for (int i = 0; i < size; ++i) {
            auto emp = std::make_shared<ConcreteEmployee>();
            emp->read(in);
            employees.insert(emp);
        }
    }
    in.close();
}

void EmployeeBase::writeEmployees() {
    std::ofstream out("data\\employees.bin", std::ios::binary);
    int size = employees.size();
    out.write((char*)(&size), sizeof(size));
    for (auto& emp : employees) {
        emp->write(out);
    }
    out.close();
}

void EmployeeBase::showAll() {
    for (auto& emp : employees) {
        std::cout << "----------------------------------------\n";
        std::cout << "ФИО: " << emp->getName() << "\n"
            << "Должность: " << emp->getPosition() << "\n"
            << "Зарплата: " << emp->calculateSalary() << "\n";
    }
}

std::shared_ptr<Employee> EmployeeBase::chooseEmployee() {
    if (employees.empty()) {
        std::cout << "Список сотрудников пуст.\n";
        return nullptr;
    }

    std::vector<std::shared_ptr<Employee>> list(employees.begin(), employees.end());
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << list[i]->getName() << " — " << list[i]->getPosition() << "\n";
    }

    int ch;
    getInRange("Выберите сотрудника (0 - отмена): ", ch, 0, list.size());
    if (ch == 0) return nullptr;
    return list[ch - 1];
}

//GuestEmployeeBase

void GuestEmployeeBase::showHeader() {
    std::cout << "Добро пожаловать, гость!" << std::endl;
}

void GuestEmployeeBase::run() {
    int ch = 0;
    do {
        system("CLS");
        showHeader();
        std::cout << "                         ----------------" << std::endl
            << "                         | Гостевой режим |" << std::endl
            << "                         ----------------" << std::endl
            << "[1] Просмотреть сотрудников." << std::endl
            << "[2] Выбрать сотрудника." << std::endl
            << "[0] Выход." << std::endl;
        getInRange("Ваш выбор: ", ch, 0, 2);
        switch (ch) {
        case 1:
            showAll();
            system("pause");
            break;
        case 2:
            employeeMenu(chooseEmployee());
            break;
        }
    } while (ch != 0);
}

void GuestEmployeeBase::employeeMenu(std::shared_ptr<Employee> emp) {
    if (!emp) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "_____________________ Сотрудник _____________________" << std::endl
            << "ФИО: " << emp->getName() << std::endl
            << "Должность: " << emp->getPosition() << std::endl
            << "Зарплата: " << emp->calculateSalary() << std::endl
            << "_____________________________________________________" << std::endl
            << "[0] Назад." << std::endl;
        getInRange("Ваш выбор: ", ch, 0, 0);
    } while (ch != 0);
}

UserPayrollBase::UserPayrollBase(std::shared_ptr<EmployeeUser> u) {
    this->user = u;
}

void UserPayrollBase::showHead() {
    std::cout << "Добро пожаловать, " << user->getLogin() << "!" << std::endl << std::endl;
}

void UserPayrollBase::run() {
    int ch = 0;
    do {
        system("CLS");
        showHead();
        std::cout << "                         ----------------" << std::endl
            << "                         | Личный кабинет |" << std::endl
            << "                         ----------------" << std::endl
            << "[1] Просмотреть периоды расчёта." << std::endl
            << "[2] Выбрать период." << std::endl
            << "[3] Просмотреть свои ведомости." << std::endl
            << "[4] Просмотреть личные данные." << std::endl
            << "[5] Редактировать личные данные." << std::endl
            << "[0] Выход." << std::endl;
        getInRange("Ваш выбор: ", ch, 0, 5);
        switch (ch) {
        case 1:
            showPeriods();
            system("pause");
            break;
        case 2:
            periodMenu(choosePeriod());
            break;
        case 3:
            user->showSlips();
            system("pause");
            break;
        case 4:
            user->showInfo();
            system("pause");
            break;
        case 5:
            std::cout << "Введите новые данные:\n";
            user->getInfo();
            std::cout << "Данные обновлены!\n";
            system("pause");
            break;
        }
    } while (ch != 0);
}

void UserPayrollBase::periodMenu(std::shared_ptr<PayrollPeriod> period) {
    if (!period) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "_____________________ Период _____________________\n"
            << *period << "\n"
            << "__________________________________________________\n"
            << "[1] Показать сессии расчёта.\n"
            << "[2] Выбрать сессию.\n"
            << "[0] Назад.\n";
        getInRange("Ваш выбор: ", ch, 0, 2);
        switch (ch) {
        case 1:
            period->showSessions();
            system("pause");
            break;
        case 2:
            sessionMenu(period->chooseSession());
            break;
        }
    } while (ch != 0);
}

void UserPayrollBase::sessionMenu(std::shared_ptr<PayrollSession> session) {
    if (!session) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "_____________________ Сессия _____________________\n"
            << *session << "\n"
            << "__________________________________________________\n"
            << "[1] Показать ведомости.\n"
            << "[2] Выбрать ведомость.\n"
            << "[0] Назад.\n";
        getInRange("Ваш выбор: ", ch, 0, 2);
        switch (ch) {
        case 1:
            session->showSlips();
            system("pause");
            break;
        case 2:
            slipMenu(session->chooseSlip());
            break;
        }
    } while (ch != 0);
}

void UserPayrollBase::slipMenu(std::shared_ptr<SalarySlip> slip) {
    if (!slip) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "__________________________________________________\n"
            << "                 Ведомость расчёта                \n"
            << "__________________________________________________\n\n";
        slip->show();
        std::cout << "__________________________________________________\n"
            << "[0] Назад.\n";
        getInRange("Ваш выбор: ", ch, 0, 0);
    } while (ch != 0);
}
AdminEmployeeBase::AdminEmployeeBase(std::shared_ptr<EmployeeUser> u) {
    adminUser = u;
}

void AdminEmployeeBase::showHead() {
    std::cout << "Добро пожаловать, администратор " << adminUser->getLogin() << "!\n\n";
}

void AdminEmployeeBase::run() {
    int ch = 0;
    do {
        system("CLS");
        showHead();
        std::cout << "                         ----------------\n"
            << "                         | Панель Админа |\n"
            << "                         ----------------\n"
            << "[1] Просмотреть периоды расчёта.\n"
            << "[2] Добавить период.\n"
            << "[3] Выбрать период.\n"
            << "[4] Удалить период.\n"
            << "[5] Просмотреть личные данные.\n"
            << "[6] Редактировать личные данные.\n"
            << "[0] Выход.\n";
        getInRange("Ваш выбор: ", ch, 0, 6);
        switch (ch) {
        case 1:
            showPeriods();
            system("pause");
            break;
        case 2:
            addPeriod();
            std::cout << "Период успешно добавлен!\n";
            system("pause");
            break;
        case 3:
            periodMenu(choosePeriod());
            break;
        case 4:
            deletePeriod();
            system("pause");
            break;
        case 5:
            adminUser->showInfo();
            system("pause");
            break;
        case 6:
            std::cout << "Введите новые данные:\n";
            adminUser->getInfo();
            std::cout << "Данные обновлены!\n";
            system("pause");
            break;
        }
    } while (ch != 0);
}

void AdminEmployeeBase::showPeriods() {
    if (periods.empty()) {
        std::cout << "Нет доступных периодов.\n";
        return;
    }
    for (const auto& p : periods) {
        std::cout << *p << "\n";
    }
}

void AdminEmployeeBase::addPeriod() {
    double baseSalary;
    std::cout << "Введите базовую зарплату для расчёта: ";
    std::cin >> baseSalary;

    auto session = std::make_shared<PayrollSession>();
    session->setDate(std::time(nullptr));
    session->setID(periods.size() + 1);

    session->generateSlips(employeeUsers, baseSalary);

    auto period = std::make_shared<PayrollPeriod>();
    period->addSession(session);
    periods.insert(period);

    std::cout << "Сессия успешно добавлена!\n";
    system("pause");
}

void AdminEmployeeBase::deletePeriod() {
    if (periods.empty()) {
        std::cout << "Нет доступных периодов.\n";
        return;
    }
    showPeriods();
    std::string label;
    get("Введите название периода для удаления: ", label);
    label = toLower(label);
    PayrollPeriod temp;
    temp.setLabel(label);
    auto it = periods.lower_bound(std::make_shared<PayrollPeriod>(temp));
    std::vector<std::set<std::shared_ptr<PayrollPeriod>>::iterator> matches;
    while (it != periods.end() && toLower((*it)->getLabel()).find(label) != std::string::npos) {
        matches.push_back(it++);
    }

    if (matches.empty()) {
        std::cout << "Период не найден.\n";
        return;
    }

    if (matches.size() == 1) {
        std::cout << "Удалён:\n" << **matches[0] << "\n";
        periods.erase(matches[0]);
    }
    else {
        for (size_t i = 0; i < matches.size(); ++i) {
            std::cout << "[" << i + 1 << "]\n" << **matches[i] << "\n";
        }
        int ch;
        getInRange("Выберите номер периода для удаления (0 - отмена): ", ch, 0, matches.size());
        if (ch == 0) return;
        std::cout << "Удалён:\n" << **matches[ch - 1] << "\n";
        periods.erase(matches[ch - 1]);
    }
}

void AdminEmployeeBase::periodMenu(std::shared_ptr<PayrollPeriod> period) {
    if (!period) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "_____________________ Период _____________________\n"
            << *period << "\n"
            << "__________________________________________________\n"
            << "[1] Показать сессии.\n"
            << "[2] Добавить сессию.\n"
            << "[3] Выбрать сессию.\n"
            << "[4] Редактировать информацию.\n"
            << "[0] Назад.\n";
        getInRange("Ваш выбор: ", ch, 0, 4);
        switch (ch) {
        case 1:
            period->showSessions();
            system("pause");
            break;
        case 2:
            period->addSession();
            std::cout << "Сессия успешно добавлена!\n";
            system("pause");
            break;
        case 3:
            sessionMenu(period->chooseSession());
            break;
        case 4:
            period->getInfo();
            break;
        }
    } while (ch != 0);
}

void AdminEmployeeBase::sessionMenu(std::shared_ptr<PayrollSession> session) {
    if (!session) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "_____________________ Сессия _____________________\n"
            << *session << "\n"
            << "__________________________________________________\n"
            << "[1] Показать ведомости.\n"
            << "[2] Сгенерировать ведомости.\n"
            << "[3] Выбрать ведомость.\n"
            << "[4] Редактировать сессию.\n"
            << "[0] Назад.\n";
        getInRange("Ваш выбор: ", ch, 0, 4);
        switch (ch) {
        case 1:
            session->showSlips();
            system("pause");
            break;
        case 2: {
            std::cout << "Создание новых ведомостей:\n";

            if (employees.empty()) {
                std::cout << "Нет сотрудников для создания ведомостей!\n";
            }
            else {
                for (const auto& emp : employees) {
                    double salary = emp->calculateSalary();
                    auto slip = std::make_shared<SalarySlip>(salary, "Расчет", emp);
                    session->addSlip(slip);
                }
                std::cout << "Создано ведомостей: " << employees.size() << "\n";
            }

            system("pause");
            break;
        }
        case 3:
            slipMenu(session->chooseSlip());
            break;
        case 4: {
            SalaryDateTime date;
            std::string label;
            get("Введите новую дату (dd.mm.yyyy): ", date);
            get("Введите новый заголовок периода: ", label);

            session->setDate(date);
            session->setPeriodLabel(label);
            break;
        }
        }
    } while (ch != 0);
}

void AdminEmployeeBase::slipMenu(std::shared_ptr<SalarySlip> slip) {
    if (!slip) return;
    int ch = 0;
    do {
        system("CLS");
        std::cout << "__________________________________________________\n"
            << "                 Ведомость расчёта                \n"
            << "_____ _____________________________________________\n\n";
        slip->show();
        std::cout << "__________________________________________________\n"
            << "[1] Изменить сумму выплаты.\n"
            << "[0] Назад.\n";
        getInRange("Ваш выбор: ", ch, 0, 1);
        switch (ch) {
        case 1: {
            double amount;
            get("Введите новую сумму: ", amount);
            while (amount < 0) {
                get("Сумма не может быть отрицательной. Повторите: ", amount);
            }
            slip->setAmount(amount);
            std::cout << "Сумма обновлена!\n";
            system("pause");
            break;
        }
        }
    } while (ch != 0);
}