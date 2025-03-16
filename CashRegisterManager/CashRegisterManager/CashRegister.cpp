#include "CashRegister.h"

CashRegister::CashRegister(double initial_balance) : balance(initial_balance) {}

void CashRegister::withdraw(int employee_id, double amount) {
    std::cout << "Employee " << employee_id << " wants to withdraw " << amount << "\n";

    if (amount > balance) {
        std::cout << "Not enough money! Employee " << employee_id << " is waiting.\n";
        waitQueue.push({ employee_id, amount });
        return;
    }

    balance -= amount;
    std::cout << "Employee " << employee_id << " withdrew " << amount << " - New balance: " << balance << "\n";
}

void CashRegister::deposit(int employee_id, double amount) {
    balance += amount;
    std::cout << "Employee " << employee_id << " deposited " << amount << " - New balance: " << balance << "\n";
    while (!waitQueue.empty()) {
        std::pair<int, int> waiting = waitQueue.front();
        int waiting_id = waiting.first;
        int waiting_amount = waiting.second;
        if (waiting_amount <= balance) {
            waitQueue.pop();
            balance -= waiting_amount;
            std::cout << "Employee " << waiting_id << "'s pending withdrawal of " 
                << waiting_amount << " has been processed."
                << " - New balance: " << balance << "\n";
        }
    }
}

double CashRegister::getBalance() {
	return balance;
}

void CashRegister::viewPendingTransactions() {
    if (waitQueue.empty()) {
        std::cout << "No pending transactions.\n";
        return;
    }

    std::queue<std::pair<int, int>> tempQueue = waitQueue;
    std::cout << "Pending Transactions:\n";
    while (!tempQueue.empty()) {
        auto transaction = tempQueue.front();
        tempQueue.pop();
        std::cout << "Employee " << transaction.first << " - Withdraw " << transaction.second << "\n";
    }
}