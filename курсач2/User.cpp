#include "EmployeeUser.h"
#include "Functions.h"
#include "PayrollSession.h"
#include "SalaryDocumentation.h"
#include <map>
EmployeeUser::EmployeeUser(const std::string& path, const std::string& login, const std::string& role)
    : path(path), login(login), role(role) {
    writeInfo();
}

EmployeeUser::EmployeeUser(const std::string& path)
    : path(path), role("viewer") {
    loadInfo();
}

EmployeeUser::~EmployeeUser() {
    slips.clear();
}

bool EmployeeUser::isAdmin() const {
    return role == "admin";
}

bool EmployeeUser::isManager() const {
    return role == "manager";
}

bool EmployeeUser::isViewer() const {
    return role == "viewer";
}

void EmployeeUser::loadInfo() {
    std::ifstream in(path, std::ios::binary);
    if (checkInpFile(in)) {
        bReadStr(role, in);
        bReadStr(login, in);
        bReadStr(surname, in);
        bReadStr(name, in);

        int n;
        in.read((char*)(&n), sizeof(n));
        for (int i = 0; i < n; ++i) {
            auto slip = std::make_shared<SalarySlip>();
            slip->read(in);
            slips.push_back(slip);
        }
    }
    in.close();
}

void EmployeeUser::writeInfo() {
    std::ofstream out(path, std::ios::binary);
    bWriteStr(role, out);
    bWriteStr(login, out);
    bWriteStr(surname, out);
    bWriteStr(name, out);

    int n = slips.size();
    out.write((char*)(&n), sizeof(n));
    for (const auto& slip : slips) {
        slip->write(out);
    }
    out.close();
}

void EmployeeUser::associateSlips(std::set<std::shared_ptr<PayrollPeriod>, payrollPeriodCmp>& periods) {
    for (int i = slips.size() - 1; i >= 0; --i) {
        auto slip = slips[i];
        auto session = slip->getSession();
        if (session == nullptr) {
            slips.erase(slips.begin() + i);
            continue;
        }

        for (const auto& period : periods) {
            auto found = period->sessions.find(session);
            if (found != period->sessions.end()) {
                association[slip->getCode()] = session;
                break;
            }


        }
    }
}

void EmployeeUser::slipBack() {
    if (slips.empty()) {
        std::cout << "Нет доступных ведомостей для возврата.\n";
        return;
    }

    showSlips();
    int ch;
    getInRange("Введите номер ведомости для удаления: ", ch, 0, slips.size());
    if (ch--) {
        slips.erase(slips.begin() + ch);
        std::cout << "Ведомость удалена.\n";
        writeInfo();
    }
    else {
        std::cout << "Отмена.\n";
    }
}

void EmployeeUser::getInfo() {
    get("Введите фамилию: ", surname);
    get("Введите имя: ", name);
    writeInfo();
}

void EmployeeUser::showInfo() {
    std::cout << "~~~ Информация о пользователе ~~~\n";
    std::cout << "     Роль: " << role << "\n"
        << "    Логин: " << login << "\n"
        << "  Фамилия: " << surname << "\n"
        << "      Имя: " << name << "\n"
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void EmployeeUser::pushSlip(std::shared_ptr<SalarySlip> slip) {
    slips.push_back(slip);
    writeInfo();
}

void EmployeeUser::showSlips() {
    int n = 0;
    for (const auto& slip : slips) {
        std::cout << "[" << ++n << "]\n"
            << "----------------------------------------\n";
        slip->show();
        std::cout << "----------------------------------------\n";
    }
}

std::string EmployeeUser::getLogin() const {
    return login;
}

std::string EmployeeUser::getSurname() const {
    return surname;
}

std::string EmployeeUser::getName() const {
    return name;
}

std::string EmployeeUser::getRole() const {
    return role;
}

std::shared_ptr<Employee> EmployeeUser::getEmployee() const {
    return employeeProfile;
}

void EmployeeUser::setEmployee(std::shared_ptr<Employee> emp) {
    employeeProfile = emp;
}
