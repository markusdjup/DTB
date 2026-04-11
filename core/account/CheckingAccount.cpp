#include "CheckingAccount.h"

long long CheckingAccount::getOverdraftLimit() const { return overdraftLimit; }
void CheckingAccount::setOverdraftLimit(long long newOverdraftLimit) { overdraftLimit = newOverdraftLimit; }
bool CheckingAccount::canWithdraw(long long amount) const 
{
    return (balance - amount) >= -overdraftLimit;
}
