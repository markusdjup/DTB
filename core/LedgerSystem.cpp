#include "LedgerSystem.h"
#include <stdexcept>

// checks if an account is actually owned by the ledger system
bool LedgerSystem::ownsAccount(const BankAccount& account) const
{
    for (const auto& acc : accounts)
        if (acc.get() == &account)
            return true;
    return false;
};

// account creation
SavingsAccount& LedgerSystem::createSavingsAccount(const std::string& initialOwner, double initialInterestRate, long long initialBalance)
{
    accounts.push_back(std::make_unique<SavingsAccount>(nextAccountID++, initialOwner, initialInterestRate, initialBalance));
    return static_cast<SavingsAccount&>(*accounts.back());
}
CheckingAccount& LedgerSystem::createCheckingAccount(const std::string& initialOwner, long long initialOverdraftLimit, long long initialBalance)
{
    accounts.push_back(std::make_unique<CheckingAccount>(nextAccountID++, initialOwner, initialOverdraftLimit, initialBalance));
    return static_cast<CheckingAccount&>(*accounts.back());
}

// transactions
void LedgerSystem::deposit(BankAccount& account, long long amount)
{
    if (!ownsAccount(account))
        throw std::invalid_argument("Account not in LedgerSystem"); // change to specific exceptions later!
    if (amount <= 0)
        throw std::invalid_argument("Deposit amount must be positive");
    account.setBalance(account.getBalance() + amount);
    transactionLog.push_back({Transaction::Type::Deposit, amount, -1, account.getID()});    // use nullopt here?
}
void LedgerSystem::withdraw(BankAccount& account, long long amount)
{
    if (!ownsAccount(account))
        throw std::invalid_argument("Account not in LedgerSystem");
    if (amount <= 0)
        throw std::invalid_argument("Withdrawal amount must be positive");
    if (!account.canWithdraw(amount))
        throw std::runtime_error("Incufficient funds");
    account.setBalance(account.getBalance() - amount);
    transactionLog.push_back({Transaction::Type::Withdrawal, amount, account.getID(), -1});
}
void LedgerSystem::transfer(BankAccount& from, BankAccount& to, long long amount) 
{
    if (!ownsAccount(from) || !ownsAccount(to))
        throw std::invalid_argument("Account not in LedgerSystem");
    if (amount <= 0)
        throw std::invalid_argument("Transfer amount must be positive");
    if (!from.canWithdraw(amount))
        throw std::runtime_error("Insufficient funds");
    from.setBalance(from.getBalance() - amount);
    to.setBalance(to.getBalance() + amount);
    transactionLog.push_back({Transaction::Type::Transfer, amount, from.getID(), to.getID()});
}
void LedgerSystem::applyInterest(SavingsAccount& account)
{
    if (!ownsAccount(account))
        throw std::invalid_argument("Account not in LedgerSystem");
    long long interest = account.calculateInterest();
    account.setBalance(account.getBalance() + interest);
    transactionLog.push_back({Transaction::Type::Interest, interest, -1, account.getID()});
}
