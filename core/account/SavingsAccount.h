#pragma once
#include "BankAccount.h"

class SavingsAccount : public BankAccount
{
    friend class Ledger;
private:
    double interestRate;
    void setInterestRate(double newInterestRate);
public:
    SavingsAccount(long long id, const std::string& initialOwner, double initialInterestRate, long long initialBalance = 0)
    : BankAccount(id, initialOwner, initialBalance), interestRate(initialInterestRate) {}

    double getInterestRate() const;
    long long calculateInterest() const;
};
