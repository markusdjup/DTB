#pragma once
#include "BankAccount.h"

class SavingsAccount : public BankAccount
{
private:
    double interestRate;
public:
    SavingsAccount(long long id, const std::string& initialOwner, double initialInterestRate, long long initialBalance = 0)
    : BankAccount(id, initialOwner, initialBalance), interestRate(initialInterestRate) {}

    double getInterestRate() const { return interestRate; }
    void setInterestRate(double newInterestRate) { interestRate = newInterestRate; }
    
    long long calculateInterest() const { return static_cast<long long>(balance * interestRate); }
};
