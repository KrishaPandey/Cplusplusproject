// main.cpp
#include <iostream>
#include "Login.h"
#include "Registration.h"
#include "AdminPanel.h"
#include "CustomerPanel.h"
using namespace std;

void showMainMenu() {
    cout << "\n+----------------------------+\n"
         << "|         MAIN MENU          |\n"
         << "+----------------------------+\n"
         << "| 1) Register (Admins only)  |\n"
         << "| 2) Login                   |\n"
         << "| 0) Exit                    |\n"
         << "+----------------------------+\n"
         << "Choice: ";
}

void showAdminMenu() {
    cout << "\n[ADMIN MENU]\n"
         << "1) Register new user\n"
         << "2) Dashboard (stub)\n"
         << "3) View all accounts\n"
         << "4) Delete account\n"
         << "0) Logout\n"
         << "Choice: ";
}

void showCustomerMenu() {
    cout << "\n[CUSTOMER MENU]\n"
         << "1) Dashboard (stub)\n"
         << "2) View balance\n"
         << "3) Deposit money\n"
         << "4) Change password\n"
         << "0) Logout\n"
         << "Choice: ";
}

int main() {
    while (true) {
        showMainMenu();
        int choice; 
        cin >> choice;
        if (choice == 0) {
            cout << "Goodbye!\n";
            break;
        }

        // --- Option 1: Registration (Admins only) ---
        if (choice == 1) {
            cout << "\nPlease login as Admin to register new users.\n";
            Login lg;
            if (!lg.loginUser() || lg.getRole() != "Admin") {
                cout << "? Only Admins can register.\n";
                continue;
            }

            // Admin is now authenticated
            while (true) {
                showAdminMenu();
                int a; 
                cin >> a;
                if (a == 0) break;            // logout ? back to main
                else if (a == 1) {
                    Registration rg;
                    rg.registerUser();
                }
                else if (a == 2) {
                    cout << "[Dashboard stub]\n";
                }
                else if (a == 3) {
                    // pass current admin’s username
                    AdminPanel ap(lg.getUsername());
                    ap.viewAllAccounts();
                }
                else if (a == 4) {
                    // pass current admin’s username
                    AdminPanel ap(lg.getUsername());
                    ap.deleteAccount();
                }
                else {
                    cout << "? Invalid choice.\n";
                }
            }
        }
        // --- Option 2: Login (Admin or Customer) ---
        else if (choice == 2) {
            Login lg;
            if (!lg.loginUser()) {
                continue;
            }

            if (lg.getRole() == "Admin") {
                // pass current admin’s username
                AdminPanel ap(lg.getUsername());
                while (true) {
                    showAdminMenu();
                    int a; 
                    cin >> a;
                    if (a == 0) break;        // logout
                    else if (a == 1) {
                        Registration rg;
                        rg.registerUser();
                    }
                    else if (a == 2) {
                        cout << "[Dashboard stub]\n";
                    }
                    else if (a == 3) {
                        ap.viewAllAccounts();
                    }
                    else if (a == 4) {
                        ap.deleteAccount();
                    }
                    else {
                        cout << "? Invalid choice.\n";
                    }
                }
            } else {
                // Customer path
                CustomerPanel cp(lg.getUsername());
                while (true) {
                    showCustomerMenu();
                    int c; 
                    cin >> c;
                    if (c == 0) break;        // logout
                    else if (c == 1) {
                        cout << "[Customer dashboard stub]\n";
                    }
                    else if (c == 2) {
                        cp.viewBalance();
                    }
                    else if (c == 3) {
                        cp.depositMoney();
                    }
                    else if (c == 4) {
                        cp.changePassword();
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

    return 0;
}
