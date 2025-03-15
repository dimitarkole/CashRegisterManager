#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>

class CashRegister {
private:
    int balance;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::pair<int, int>> waitQueue; // <employee_id, amount>

public:
    CashRegister(int initial_balance = 0);
    void withdraw(int employee_id, int amount);
    void deposit(int employee_id, int amount);
    int getBalance();
    void viewPendingTransactions();
};
