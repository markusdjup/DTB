#include "core/Ledger.h"
#include <iostream>

int main() {

    // TESTING!!!!!!!!
    std::cout << "Starting up" << std::endl;
    Ledger ls;
    SavingsAccount& sa = ls.createSavingsAccount("Markus Djup", 0.04);
    std::cout << "SA Owner: " << sa.getOwner() << std::endl;
    std::cout << "SA ID: " << sa.getID() << std::endl;
    std::cout << "SA Interest: " << sa.getInterestRate() << std::endl;
    ls.deposit(sa, 10000);
    std::cout << "SA Balance: " << sa.getBalance() << std::endl;
    SavingsAccount& sa2 = ls.createSavingsAccount("Elon Musk", 0.15, 1000000000);
    std::cout << "SA2 Owner: " << sa2.getOwner() << std::endl;
    std::cout << "SA2 ID: " << sa2.getID() << std::endl;
    std::cout << "SA2 Interest: " << sa2.getInterestRate() << std::endl;
    std::cout << "SA2 Balance: " << sa2.getBalance() << std::endl;
    ls.withdraw(sa2, 1000);
    std::cout << "SA2 Balance (after withdrawal): " << sa2.getBalance() << std::endl;
    ls.transfer(sa2, sa, 1000000);
    std::cout << "SA Balance (after transfer): " << sa.getBalance() << std::endl;
    std::cout << "SA2 Balance (after transfer): " << sa2.getBalance() << std::endl;
    CheckingAccount& ca = ls.createCheckingAccount("Markus Djup", 5000, 10000);
    std::cout << "CA Overdraft limit: " << ca.getOverdraftLimit() << std::endl;
    std::cout << "CA Initial balance: " << ca.getBalance() << std::endl;
    ls.withdraw(ca, 10000);
    std::cout << "CA Balance (after withdrawal): " << ca.getBalance() << std::endl;
    ls.applyInterest(sa2);
    std::cout << "SA2 Balance (after interest)" << sa2.getBalance() << std::endl;
    ls.transfer(ca, sa, 1000);
    std::cout << "SA Balance (after transfer): " << sa.getBalance() << std::endl;
    std::cout << "CA Balance (after transfer): " << ca.getBalance() << std::endl;
    ls.transfer(sa, sa, 1000); // tranfer to oneself???

    // These should give exceptions
    // ls.withdraw(ca, 100000);
    // ls.transfer(ca, sa, 100000);
    // ls.deposit(sa, -10);
    // ls.withdraw(sa, -10);
    // ls.withdraw(sa, 99999999999999);
    // BankAccount rogue{2, "Hacker Hackerson", 99999999999999};
    // ls.withdraw(hacker, 99999999999999);

    // test getAccounts()
    const auto& accounts = ls.getAccounts();
    std::cout << "\n--- Accounts (" << accounts.size() << ") ---" << std::endl;
    for (const auto& acc : accounts)
        std::cout << "  ID: " << acc->getID() << " | Owner: " << acc->getOwner() << " | Balance: " << acc->getBalance() << std::endl;

    // test getTransactionLog()
    const auto& log = ls.getTransactionLog();
    std::cout << "\n--- Transaction log (" << log.size() << " entries) ---" << std::endl;
    for (const auto& t : log) {
        switch (t.type) {
            case Transaction::Type::Deposit:    std::cout << "  Deposit    "; break;
            case Transaction::Type::Withdrawal: std::cout << "  Withdrawal "; break;
            case Transaction::Type::Transfer:   std::cout << "  Transfer   "; break;
            case Transaction::Type::Interest:   std::cout << "  Interest   "; break;
        }
        std::cout << "| Amount: " << t.amount
                  << " | From: " << (t.fromID.has_value() ? std::to_string(t.fromID.value()) : "N/A")
                  << " | To: "   << (t.toID.has_value()   ? std::to_string(t.toID.value())   : "N/A")
                  << std::endl;
    }
    return 0;
}
