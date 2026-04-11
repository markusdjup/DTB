#include "BankAccount.h"

void BankAccount::setBalance(long long newBalance) { balance = newBalance; }
void BankAccount::setOwner(const std::string& newOwner) { owner = newOwner; }

long long BankAccount::getID() const { return id; }
std::string BankAccount::getOwner() const { return owner; }
long long BankAccount::getBalance() const { return balance; }
bool BankAccount::canWithdraw(long long amount) const { return amount <= balance; }
