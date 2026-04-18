#pragma once
#include "Utils.h"
#include "AnimationWindow.h"
#include "widgets/Button.h"
#include "widgets/TextInput.h"
#include "widgets/DropdownList.h"
#include "../core/Ledger.h"
#include <string>
#include <vector>
#include <functional>

class CreateAccountScreen : public TDT4102::AnimationWindow {
public:
    CreateAccountScreen(Ledger& ledger, User& user);
    std::optional<std::string> nextScreen = "CreateAccount";
private:
    Ledger& ledger;
    User& user;
    std::vector<std::string> typeOptions;
    TDT4102::DropdownList typeDropdown;
    TDT4102::TextInput initialBalanceField;
    TDT4102::Button confirmButton;
    TDT4102::Button cancelButton;

    std::string statusMessage;
    TDT4102::Color statusColor;

    bool isSavingsSelected() const;
    void setStatus(const std::string& message, TDT4102::Color color);
    
    void handleConfirm(); // sends user to DashboardScreen
    void handleCancel(); // sends user to DashboardScreen
};
