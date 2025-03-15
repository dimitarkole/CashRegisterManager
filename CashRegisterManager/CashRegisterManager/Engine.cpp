#include "Engine.h"

Engine::Engine() {
	this->cashRegister = new CashRegister();
}

Engine::~Engine() {
	delete this->cashRegister;
}

void Engine::employeeAction(int id) {
    int action;
    double amount;
    std::cout << "Employee " << id << " - Choose action:\n";
    std::cout << "1. Withdraw\n";
    std::cout << "2. Deposit\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> action;
    std::cout << "Enter amount: ";
    std::cin >> amount;

    switch (action)
    {
        case 0: break;
	    case 1:
            cashRegister->withdraw(id, amount);
            break;
	    case 2:
		    cashRegister->deposit(id, amount);  
		    break;
        default:
            std::cout << "Invalid action. Skipping...\n";
            break;
    }
}

void Engine::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Engine::printMenu() {
    std::cout << "\nChoose action: ";
    std::cout << "\n1. Check the balance";
    std::cout << "\n2. Perform transaction";
    std::cout << "\n3. View pending transactions";
    std::cout << "\n4. Clear console";
    std::cout << "\n0. Exit";
    std::cout << "\nEnter your choice: ";
}

void Engine::menu() {
    int choice, id;
    while (true) {
        printMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "Current balance: " << cashRegister->getBalance() << "\n";
                break;
            case 2:
                std::cout << "Enter employee ID: ";
                std::cin >> id;
                employeeAction(id);
                break;
            case 3:
                cashRegister->viewPendingTransactions();
                break;
            case 4:
                clearConsole();
                break;
            case 0:
                std::cout << "Final balance: " << cashRegister->getBalance() << "\n";
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
                break;
        }
    }
}

void Engine::run() {
    double initialBalance;
    std::cout << "Enter the initial balance of the cash register: ";
    std::cin >> initialBalance;
    delete cashRegister;
    cashRegister = new CashRegister(initialBalance);
    menu();
}