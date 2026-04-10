#include "BankAccount.h"

long long BankAccount::getID() const { return id; }
std::string BankAccount::getOwner() const { return owner; }
void BankAccount::setOwner(const std::string& newOwner) { owner = newOwner; }
long long BankAccount::getBalance() const { return balance; }
bool BankAccount::canWithdraw(long long amount) const { return amount <= balance; }
