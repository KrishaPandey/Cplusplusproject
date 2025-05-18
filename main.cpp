// main.cpp
#include <iostream>
#include <fstream>
#include "Login.h"         // still included for when you bring login back
#include "Registration.h"
using namespace std;

void showAdminMenu() {
    cout << "\n[ADMIN MENU]\n"
         << "1) Register new user\n"
         << "2) Dashboard (stub)\n"
         << "0) Logout\n"
         << "Choice: ";
}

void showCustomerMenu() {
    cout << "\n[CUSTOMER MENU]\n"
         << "1) Dashboard (stub)\n"
         << "0) Logout\n"
         << "Choice: ";
}

int main() {
    while (true) {
        cout << "\n+----------------------------+\n"
             << "|     USER REGISTRATION      |\n"
             << "+----------------------------+\n"
             << "| 1) Register                |\n"
             << "| 0) Exit                    |\n"
             << "+----------------------------+\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            break;
        }
        else if (choice == 1) {
            Registration rg;
            rg.registerUser();
        }
        else {
            cout << "? Invalid choice.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}

