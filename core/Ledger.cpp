#include "Ledger.h"
#include "exceptions/LedgerExceptions.h"
#include <stdexcept>
#include <optional>

// checks if an account is actually owned by the ledger system
bool Ledger::ownsAccount(const BankAccount& account) const
{
    for (const auto& acc : accounts)
        if (acc.get() == &account)
            return true;
    return false;
};

// account creation
SavingsAccount& Ledger::createSavingsAccount(const std::string& initialOwner, double initialInterestRate, long long initialBalance)
{
    if (!isValidOwnerName(initialOwner))
        throw InvalidOwnerName();
    accounts.push_back(std::make_unique<SavingsAccount>(nextAccountID++, initialOwner, initialInterestRate, initialBalance));
    return static_cast<SavingsAccount&>(*accounts.back());
}
CheckingAccount& Ledger::createCheckingAccount(const std::string& initialOwner, long long initialOverdraftLimit, long long initialBalance)
{
    accounts.push_back(std::make_unique<CheckingAccount>(nextAccountID++, initialOwner, initialOverdraftLimit, initialBalance));
    return static_cast<CheckingAccount&>(*accounts.back());
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
}
void Ledger::applyInterest(SavingsAccount& account)
{
    if (!ownsAccount(account))
        throw AccountNotInLedger("Account not in Ledger");
    long long interest = account.calculateInterest();
    account.setBalance(account.getBalance() + interest);
    transactionLog.push_back({Transaction::Type::Interest, interest, std::nullopt, account.getID()});
}

// getters
const std::vector<std::unique_ptr<BankAccount>>& Ledger::getAccounts() const { return accounts; }
const std::vector<Transaction>& Ledger::getTransactionLog() const { return transactionLog; }
