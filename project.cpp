#include <iostream>
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define QUEUE_NAME "/cash_queue"
#define MAX_SIZE 1024

struct Transaction {
    int employeeId;
    double amount;
    bool isDeposit; // true - депозит, false - теглене
};

class CashRegister {
private:
    mqd_t queue;
    pthread_t withdrawThread, depositThread;
    bool running = false;
    
    static void* processTransactions(void* arg) {
        CashRegister* self = static_cast<CashRegister*>(arg);
        self->handleTransactions();
        return nullptr;
    }

    void handleTransactions() {
        std::cout << "CashRegister: Processing transactions...\n";
        
        double balance = 1000.0; // Начален баланс
        
        while (running) {
            Transaction trans;
            ssize_t bytes_read = mq_receive(queue, (char*)&trans, sizeof(Transaction), NULL);
            if (bytes_read > 0) {
                if (trans.isDeposit) {
                    balance += trans.amount;
                    std::cout << "[DEPOSIT] Employee " << trans.employeeId 
                              << " deposited " << trans.amount 
                              << ". New balance: " << balance << "\n";
                } else {
                    if (trans.amount <= balance) {
                        balance -= trans.amount;
                        std::cout << "[WITHDRAW] Employee " << trans.employeeId 
                                  << " withdrew " << trans.amount 
                                  << ". New balance: " << balance << "\n";
                    } else {
                        std::cout << "[FAIL] Employee " << trans.employeeId 
                                  << " attempted to withdraw " << trans.amount 
                                  << " but balance is insufficient!\n";
                    }
                }
            } else {
                std::cerr << "Error receiving message: " << strerror(errno) << "\n";
            }
        }
    }

public:
    CashRegister() {
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = sizeof(Transaction);
        attr.mq_curmsgs = 0;

        queue = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0700, &attr);
        if (queue == -1) {
            std::cerr << "Error creating queue: " << strerror(errno) << "\n";
            exit(1);
        }
    }

    ~CashRegister() {
        stop();
        mq_close(queue);
        mq_unlink(QUEUE_NAME);
    }

    void start() {
        running = true;
        pthread_create(&withdrawThread, nullptr, processTransactions, this);
    }

    void stop() {
        running = false;
        pthread_join(withdrawThread, nullptr);
    }

    void deposit(int employeeId, double amount) {
        Transaction trans = {employeeId, amount, true};
        mq_send(queue, (char*)&trans, sizeof(Transaction), 0);
        std::cout << "Deposit request: Employee " << employeeId << " -> " << amount << "\n";
    }

    void withdraw(int employeeId, double amount) {
        Transaction trans = {employeeId, amount, false};
        mq_send(queue, (char*)&trans, sizeof(Transaction), 0);
        std::cout << "Withdraw request: Employee " << employeeId << " -> " << amount << "\n";
    }   
};

int main() {
    CashRegister registerSystem;

    registerSystem.start();

    registerSystem.deposit(1, 500);
    sleep(1);
    registerSystem.withdraw(2, 200);
    sleep(1);
    registerSystem.withdraw(3, 1200);
    sleep(1);
    registerSystem.deposit(4, 300);
    sleep(1);
    registerSystem.withdraw(5, 700);

    sleep(2);
    registerSystem.stop();

    std::cout << "Main: Program completed. Exiting.\n";
    return 0;
}
