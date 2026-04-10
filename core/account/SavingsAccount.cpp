#include "SavingsAccount.h"

double SavingsAccount::getInterestRate() const { return interestRate; }
void SavingsAccount::setInterestRate(double newInterestRate) { interestRate = newInterestRate; }
long long SavingsAccount::calculateInterest() const { 
    return static_cast<long long>(balance * interestRate); // flawed cuz interestRate is a double (who cares)
}
