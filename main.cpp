#include "core/Ledger.h"
#include "gui/LoginScreen.h"
#include "gui/DashboardScreen.h"
#include "gui/CreateAccountScreen.h"
#include <iostream>

int main()
{

    Ledger l;
    LoginScreen login(l);
    login.wait_for_close();
    std::string username = "";

    DashboardScreen dashboard(l, username);
    dashboard.wait_for_close();

    CreateAccountScreen create(l, username);
    create.wait_for_close();

    DashboardScreen dashboard(l, username);
    dashboard.wait_for_close();

    return 0;
}
