#pragma once
#include "AnimationWindow.h"
#include "widgets/Button.h"
#include "widgets/TextInput.h"
#include "widgets/DropdownList.h"
#include "../core/Ledger.h"
#include <string>
#include <vector>
#include <functional>
#include <cmath>

class DashboardScreen : public TDT4102::AnimationWindow
{
public:
    DashboardScreen(Ledger& ledger, const std::string username);
private:
    Ledger& ledger;
    std::string username;

    std::vector<BankAccount*> userAccounts;
    std::vector<std::string> accountLabels;

    TDT4102::DropdownList accountDropdown;
    TDT4102::TextInput amountField;
    TDT4102::TextInput toAccountIDField;
    TDT4102::Button depositButton;
    TDT4102::Button withdrawButton;
    TDT4102::Button transferButton;
    
    // sends user to a different screen
    TDT4102::Button newAccountButton;
    TDT4102::Button logoutButton;

    std::string statusMessage;
    TDT4102::Color statusColor;

    std::vector<std::string> buildAccountLabels() const;
    void refreshAccounts();
    BankAccount* getSelectedAccount() const;
    long long parseAmount() const;
    static std::string formatAmount(long long amount);
    void setStatus(const std::string& message, TDT4102::Color color);

    void handleDeposit();
    void handleWithdraw();
    void handleTransfer();
    void handleNewAccount();
    void handleLogout();

    void drawAccountList();
};
