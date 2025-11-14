#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <fstream>
#include "PayrollPeriod.h"
#include "Employee.h"

class Base {
public:
    virtual ~Base() = default;
    virtual void run() = 0;
};

class EmployeeBase : public Base {
protected:
    std::set<std::shared_ptr<Employee>> employees;//контейнер с сотрудниками 

    virtual void showHeader() = 0;//метод для вывода зоголовка 
    virtual void readEmployees();
    virtual void writeEmployees();
public:
    EmployeeBase();
    virtual ~EmployeeBase();
    void showAll();
    std::shared_ptr<Employee> chooseEmployee();

    virtual void employeeMenu(std::shared_ptr<Employee> emp) = 0;//меню в зависимости от статуса 
};

class AdminEmployeeBase : public EmployeeBase {
protected:
    std::shared_ptr<EmployeeUser> adminUser;//текущий вошедший администратор 
    std::vector<std::shared_ptr<EmployeeUser>> employeeUsers;//все зарегестрированный пользователи 
    std::set<std::shared_ptr<PayrollPeriod>, payrollPeriodCmp> periods;//периоды начисления зарплат 

    void showHead(); 
    void showPeriods(); 
    void addPeriod();
    std::shared_ptr<PayrollPeriod> choosePeriod(); 
    void deletePeriod();
    void periodMenu(std::shared_ptr<PayrollPeriod> period);
    void sessionMenu(std::shared_ptr<PayrollSession> session);
    void slipMenu(std::shared_ptr<SalarySlip> slip);//расчетные листы 

public:
    AdminEmployeeBase(std::shared_ptr<EmployeeUser> u);
    void showHeader() override; 
    void run() override;
    void employeeMenu(std::shared_ptr<Employee> emp) override {}
};


class GuestEmployeeBase : public EmployeeBase {
public:
    GuestEmployeeBase() = default;
    void showHeader() override;
    void run() override;
    void employeeMenu(std::shared_ptr<Employee> emp) override;
};

class UserPayrollBase : public GuestEmployeeBase {
    std::shared_ptr<EmployeeUser> user;//текущий вошедший пользователь
    void showHead();
    void showPeriods();//расчетные периоды 
    std::shared_ptr<PayrollPeriod> choosePeriod();
    void periodMenu(std::shared_ptr<PayrollPeriod> period);
    void sessionMenu(std::shared_ptr<PayrollSession> session);
    void slipMenu(std::shared_ptr<SalarySlip> slip);
public:
    UserPayrollBase(std::shared_ptr<EmployeeUser> user);
    void showHeader() override;
    void run() override;
    void employeeMenu(std::shared_ptr<Employee> emp) override {}
};

