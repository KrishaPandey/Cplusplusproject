// main.cpp
#include <iostream>
#include <fstream>
#include "Login.h"
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
             << "|         MAIN MENU          |\n"
             << "+----------------------------+\n"
             << "| 1) Register (Admins only)  |\n"
             << "| 2) Login                   |\n"
             << "| 0) Exit                    |\n"
             << "+----------------------------+\n"
             << "Choice: ";

        int choice; cin >> choice;
        if (choice == 0) break;

        // --- Option 1: Registration (requires Admin-login) ---
        if (choice == 1) {
            cout << "\nPlease login as Admin to register new users.\n";
            Login lg;
            if (!lg.loginUser() ||
               (lg.getRole() != "Admin" )) {
                cout << "? Only Admins can register.\n";
                continue;
            }

            // Admin logged in successfully ? show Admin menu
            while (true) {
                showAdminMenu();
                int a; cin >> a;
                if (a == 0) break;                 // logout ? back to main
                if (a == 1) {                      // register
                    Registration rg;
                    rg.registerUser();
                }
                else if (a == 2) {
                    cout << "[Dashboard stub]\n";
                }
                else {
                    cout << "? Invalid choice.\n";
                }
            }
        }
        // --- Option 2: Login (Admin or Customer) ---
        else if (choice == 2) {
            Login lg;
            if (!lg.loginUser()) continue;

            // branch by role
            if (lg.getRole() == "Admin" ) {
                while (true) {
                    showAdminMenu();
                    int a; cin >> a;
                    if (a == 0) break;       
                    if (a == 1) {
                        Registration rg;
                        rg.registerUser();
                    }
                    else if (a == 2) {
                        cout << "[Dashboard stub]\n";
                    }
                    else {
                        cout << "? Invalid choice.\n";
                    }
                }
            } else {
                // customer
                while (true) {
                    showCustomerMenu();
                    int c; cin >> c;
                    if (c == 0) break;       // logout
                    if (c == 1) {
                        cout << "[Customer dashboard stub]\n";
                    }
                    else {
                        cout << "? Invalid choice.\n";
                    }
                }
            }
        }
        else {
            cout << "? Invalid choice.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
