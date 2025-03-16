#pragma once
#include <iostream>
#include <queue>

class CashRegister {
private:
    double balance;
    std::queue<std::pair<int, int>> waitQueue;

public:
    CashRegister(double initial_balance = 0);
    void withdraw(int employee_id, double amount);
    void deposit(int employee_id, double amount);
    double getBalance();
    void viewPendingTransactions();
};
