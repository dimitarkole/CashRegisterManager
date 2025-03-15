#pragma once
#include <iostream>
#include "CashRegister.h"

class Engine {
private:
    CashRegister* cashRegister;
    void employeeAction(int id);
    void clearConsole();
    void menu();
	void printMenu();
public:
    Engine();
    ~Engine();
    void run();
};