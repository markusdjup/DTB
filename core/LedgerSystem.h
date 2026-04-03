#pragma once
#include "account/BankAccount.h"
#include "account/SavingsAccount.h"
#include "account/CheckingAccount.h"
#include "Transaction.h"
#include <vector>
#include <memory>

class LedgerSystem
{
private:
    std::vector<std::unique_ptr<BankAccount>> accounts;
    std::vector<Transaction> transactionLog;
    long long nextAccountID = 1;
    bool ownsAccount(const BankAccount& account) const;
public:
    // account creation
    SavingsAccount& createSavingsAccount(const std::string& initialOwner, double initialInterestRate, long long initialBalance = 0);
    CheckingAccount& createCheckingAccount(const std::string& initialOwner, long long initialOverdraftLimit, long long initialBalance = 0);
    
    // transactions
    void deposit(BankAccount& account, long long amount);
    void withdraw(BankAccount& account, long long amount);
    void transfer(BankAccount& from, BankAccount& to, long long amount);
    void applyInterest(SavingsAccount& account);

    // getters
    const std::vector<std::unique_ptr<BankAccount>>& getAccounts() const { return accounts; }
    const std::vector<Transaction>& getTransactionLog() const { return transactionLog; }
};
