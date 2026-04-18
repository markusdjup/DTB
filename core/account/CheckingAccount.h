#pragma once
#include "BankAccount.h"

class CheckingAccount : public BankAccount
{
    friend class Ledger;
private:
    long long overdraftLimit;
    void setOverdraftLimit(long long newOverdraftLimit);
public:
    CheckingAccount(long long id, const std::string& initialOwner, long long initialOverdraftLimit = 0, long long initialBalance = 0)
    : BankAccount(id, initialOwner, initialBalance), overdraftLimit(initialOverdraftLimit) {}

    long long getOverdraftLimit() const;
    bool canWithdraw(long long amount) const override;
};
