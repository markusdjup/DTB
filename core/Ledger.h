#pragma once
#include "account/BankAccount.h"
#include "account/SavingsAccount.h"
#include "account/CheckingAccount.h"
#include "LedgerSerializer.h"
#include "Transaction.h"
#include "User.h"
#include <vector>
#include <memory>

class Ledger
{
friend class LedgerSerializer;
private:
    std::vector<User> users;
    std::vector<std::unique_ptr<BankAccount>> accounts;
    std::vector<Transaction> transactionLog;
    long long nextAccountID = 1;
    bool ownsAccount(const BankAccount& account) const;
public:
    Ledger() { LedgerSerializer::load(*this); }
    ~Ledger() { LedgerSerializer::save(*this); };

    // user functions
    bool userExists(const std::string& name) const;
    bool loginUser(const std::string& name, const std::string& password) const;
    void registerUser(const std::string& name, const std::string& password);
    std::vector<BankAccount*> getAccountsForUser(const std::string& name) const; // important for GUI

    // account creation
    SavingsAccount& createSavingsAccount(const std::string& initialOwner, double initialInterestRate, long long initialBalance = 0);
    CheckingAccount& createCheckingAccount(const std::string& initialOwner, long long initialOverdraftLimit, long long initialBalance = 0);
    
    // transactions
    void deposit(BankAccount& account, long long amount);
    void withdraw(BankAccount& account, long long amount);
    void transfer(BankAccount& from, BankAccount& to, long long amount);
    void applyInterest(SavingsAccount& account);

    // getters
    const std::vector<User>& getUsers() const;
    const std::vector<std::unique_ptr<BankAccount>>& getAccounts() const;
    const std::vector<Transaction>& getTransactionLog() const;
    BankAccount* getAccountByID(long long id) const; // important for GUI
};
