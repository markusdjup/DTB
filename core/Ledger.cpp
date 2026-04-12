#include "Ledger.h"
#include "exceptions/LedgerExceptions.h"
#include <stdexcept>
#include <optional>

// checks if an account is actually owned by the ledger
bool Ledger::ownsAccount(const BankAccount& account) const
{
    for (const auto& acc : accounts)
        if (acc.get() == &account)
            return true;
    return false;
};

// user functions
bool Ledger::userExists(const std::string& name) const 
{
    for (const User& u : users) {
        if (u.name == name) 
            return true;
    }
    return false;
}
bool Ledger::loginUser(const std::string& name, const std::string& password) const
{
    for (const User& u : users) {
        if (u.name == name && u.password == password) 
            return true;
    }
    return false;
}
void Ledger::registerUser(const std::string& name, const std::string& password) 
{
    if (!isValidUserName(name)) 
        throw InvalidUserName("Invalid username. Please only use letters, spaces or dashes, and keep length between 5 and 100 characters");
    if (!isValidPassword(password)) 
        throw InvalidPassword("Invalid password. Please keep length between 6 and 30 characters");
    if (userExists(name)) 
        throw UserAlreadyExists(name + " is already a registered user");
    users.push_back({name, password});
    LedgerSerializer::save(*this);
};
std::vector<BankAccount*> Ledger::getAccountsForUser(const std::string& name) const
{
    std::vector<BankAccount*> userAccounts;
    for (const auto& acc : accounts) {
        if (acc->getOwner() == name)
            userAccounts.push_back(acc.get());
    }
    return userAccounts;
}

// account creation
SavingsAccount& Ledger::createSavingsAccount(const std::string& initialOwner, double initialInterestRate, long long initialBalance)
{
    if (!userExists(initialOwner))
        throw UserNotInLedger(initialOwner + " is not a registered user");
    accounts.push_back(std::make_unique<SavingsAccount>(nextAccountID++, initialOwner, initialInterestRate, initialBalance));
    return static_cast<SavingsAccount&>(*accounts.back());
    LedgerSerializer::save(*this);
}
CheckingAccount& Ledger::createCheckingAccount(const std::string& initialOwner, long long initialOverdraftLimit, long long initialBalance)
{
    if (!userExists(initialOwner))
        throw UserNotInLedger(initialOwner + " is not a registered user");
    accounts.push_back(std::make_unique<CheckingAccount>(nextAccountID++, initialOwner, initialOverdraftLimit, initialBalance));
    return static_cast<CheckingAccount&>(*accounts.back());
    LedgerSerializer::save(*this);
}

// transactions
void Ledger::deposit(BankAccount& account, long long amount)
{
    if (!ownsAccount(account))
        throw AccountNotInLedger("Account not in Ledger");
    if (amount <= 0)
        throw InvalidAmount("Deposited amount must be positive");
    account.setBalance(account.getBalance() + amount);
    transactionLog.push_back({Transaction::Type::Deposit, amount, std::nullopt, account.getID()});
    LedgerSerializer::save(*this);
}
void Ledger::withdraw(BankAccount& account, long long amount)
{
    if (!ownsAccount(account))
        throw AccountNotInLedger("Account not in Ledger");
    if (amount <= 0)
        throw InvalidAmount("Withdrawn amount must be positive");
    if (!account.canWithdraw(amount))
        throw InsufficientFunds();
    account.setBalance(account.getBalance() - amount);
    transactionLog.push_back({Transaction::Type::Withdrawal, amount, account.getID(), std::nullopt});
    LedgerSerializer::save(*this);
}
void Ledger::transfer(BankAccount& from, BankAccount& to, long long amount) 
{
    if (!ownsAccount(from))
        throw AccountNotInLedger("Sender account not in Ledger");
    if (!ownsAccount(to))
        throw AccountNotInLedger("Receiver account not in Ledger");
    if (amount <= 0)
        throw InvalidAmount("Transferred amount must be positive");
    if (!from.canWithdraw(amount))
        throw InsufficientFunds();
    from.setBalance(from.getBalance() - amount);
    to.setBalance(to.getBalance() + amount);
    transactionLog.push_back({Transaction::Type::Transfer, amount, from.getID(), to.getID()});
    LedgerSerializer::save(*this);
}
void Ledger::applyInterest(SavingsAccount& account)
{
    if (!ownsAccount(account))
        throw AccountNotInLedger("Account not in Ledger");
    long long interest = account.calculateInterest();
    account.setBalance(account.getBalance() + interest);
    transactionLog.push_back({Transaction::Type::Interest, interest, std::nullopt, account.getID()});
    LedgerSerializer::save(*this);
}

// getters
const std::vector<User>& Ledger::getUsers() const { return users; };
const std::vector<std::unique_ptr<BankAccount>>& Ledger::getAccounts() const { return accounts; }
const std::vector<Transaction>& Ledger::getTransactionLog() const { return transactionLog; }
BankAccount* Ledger::getAccountByID(long long id) const {
    for (const auto& acc : accounts) {
        if (acc->getID() == id)
            return acc.get();
    }
    return nullptr; // returns nullptr if the account isn't found in accounts
}
