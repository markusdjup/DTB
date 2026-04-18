#include "core/Ledger.h"
#include "gui/LoginScreen.h"
#include "gui/DashboardScreen.h"
#include "gui/CreateAccountScreen.h"
#include <iostream>

int main()
{
    Ledger l;
    User u;
    std::optional<std::string> nextScreen = "Login";

    while (nextScreen.has_value())
    {
        if (nextScreen == "Login")
        {
            LoginScreen login(l);
            nextScreen = login.nextScreen;
            if (login.loggedInUser.has_value())
                u = login.loggedInUser.value();
        }
        else if (nextScreen == "Dashboard")
        {
            DashboardScreen dashboard(l, u);
            nextScreen = dashboard.nextScreen;
        }
        else if (nextScreen == "CreateAccount")
        {
            CreateAccountScreen create(l, u);
            nextScreen = create.nextScreen;
        }
        else { break; }
    }

    return 0;
}
