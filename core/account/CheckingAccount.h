#pragma once
#include "BankAccount.h"

class CheckingAccount : public BankAccount
{
private:
    long long overdraftLimit;
public:
    CheckingAccount(long long id, const std::string& initialOwner, long long initialOverdraftLimit = 0, long long initialBalance = 0)
    : BankAccount(id, initialOwner, initialBalance), overdraftLimit(initialOverdraftLimit) {}

    long long getOverdraftLimit() const;
    void setOverdraftLimit(long long newOverdraftLimit);
    bool canWithdraw(long long amount) const override;
};
