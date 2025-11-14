#include "SalaryDocumentation.h"
#include "Employee.h"
#include <iomanip>

SalarySlip::SalarySlip() : amount(0), type("Оклад"), issueDate(time(0)) {
    generateCode();
}

SalarySlip::SalarySlip(double amt, const std::string& tp, std::shared_ptr<Employee> emp)
    : amount(amt), type(tp), employee(emp), issueDate(time(0)) {
    generateCode();
}

std::string SalarySlip::getCode() const { return slipCode; }
double SalarySlip::getAmount() const { return amount; }
std::string SalarySlip::getType() const { return type; }
std::time_t SalarySlip::getDate() const { return issueDate; }
std::shared_ptr<Employee> SalarySlip::getEmployee() const { return employee; }

void SalarySlip::setAmount(double newAmount) { amount = newAmount; }
void SalarySlip::setType(const std::string& newType) { type = newType; }

void SalarySlip::generateCode() {
    slipCode.clear();
    for (int i = 0; i < codeLength; ++i)
        slipCode += (rand() % 2 ? 'A' + rand() % 26 : '0' + rand() % 10);
}

void SalarySlip::show() const {
    std::tm timeStruct;
    localtime_s(&timeStruct, &issueDate);
    std::cout << "Код: " << slipCode
        << "\nТип: " << type
        << "\nСумма: " << amount
        << "\nДата: " << std::put_time(&timeStruct, "%d.%m.%Y %H:%M")
        << "\nСотрудник: " << (employee ? employee->getName() : "Не указан") << "\n";
}

std::istream& SalarySlip::read(std::istream& in) {
    bReadStr(slipCode, in);
    bReadStr(type, in);
    in.read((char*)&amount, sizeof(amount));
    in.read((char*)&issueDate, sizeof(issueDate));
    return in;
}

std::ostream& SalarySlip::write(std::ostream& out) {
    bWriteStr(slipCode, out);
    bWriteStr(type, out);
    out.write((char*)&amount, sizeof(amount));
    out.write((char*)&issueDate, sizeof(issueDate));
    return out;
}

bool SalarySlip::operator<(const SalarySlip& other) const {
    return amount != other.amount ? amount < other.amount : slipCode < other.slipCode;
}

bool SalarySlip::operator==(const SalarySlip& other) const {
    return amount == other.amount && type == other.type;
}