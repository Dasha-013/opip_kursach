#pragma once

#include <string>
#include <iostream>
#include "Functions.h"  

class Employee {
public:
    virtual ~Employee() = default;

    virtual std::string getName() const = 0;
    virtual std::string getPosition() const = 0;
    virtual double calculateSalary() const = 0;

    virtual void edit() = 0;
    virtual void read(std::istream& in) = 0;
    virtual void write(std::ostream& out) const = 0;
};

class ConcreteEmployee : public Employee {
    std::string name;
    std::string position;
    double baseSalary;

public:
    ConcreteEmployee() = default;

    ConcreteEmployee(const std::string& name, const std::string& position, double salary)
        : name(name), position(position), baseSalary(salary) {}

    std::string getName() const override { return name; }
    std::string getPosition() const override { return position; }
    double calculateSalary() const override { return baseSalary; }

    void edit() override {
        get("Введите ФИО: ", name);
        get("Введите должность: ", position);
        get("Введите базовую зарплату: ", baseSalary);
    }

    void read(std::istream& in) override {
        bReadStr(name, in);
        bReadStr(position, in);
        in.read(reinterpret_cast<char*>(&baseSalary), sizeof(baseSalary));
    }

    void write(std::ostream& out) const override {
        bWriteStr(name, out);
        bWriteStr(position, out);
        out.write(reinterpret_cast<const char*>(&baseSalary), sizeof(baseSalary));
    }

    void setBaseSalary(double salary) { baseSalary = salary; }
    double getBaseSalary() const { return baseSalary; }

    friend std::ostream& operator<<(std::ostream& os, const ConcreteEmployee& emp) {//перегрузка оператора вывода 
        os << "Сотрудник: " << emp.name
            << " | Должность: " << emp.position
            << " | Зарплата: " << emp.baseSalary;
        return os;
    }
};