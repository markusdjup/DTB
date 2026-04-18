#pragma once
#include "AnimationWindow.h"
#include "widgets/Button.h"
#include "widgets/TextInput.h"
#include "../core/Ledger.h"
#include <string>
#include <stdexcept>

class LoginScreen : public TDT4102::AnimationWindow
{
private:
    Ledger& ledger;
    TDT4102::TextInput usernameField;
    TDT4102::TextInput passwordField;
    TDT4102::Button registerButton;
    TDT4102::Button loginButton;
    std::string statusMessage;
    TDT4102::Color statusColor;

    void setStatus(std::string newStatusMessage, TDT4102::Color newStatusColor);
    void handleRegister();
    void handleLogin(); // sends user to DashboardScreen
public:
    LoginScreen(Ledger& ledger);
    std::optional<std::string> nextScreen = "Login";
    std::optional<User> loggedInUser = std::nullopt;
};
