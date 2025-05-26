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
         << "5) Unlock account\n"
         << "6) Lock account\n"
         << "7) View transactions\n"
         << "0) Logout\n"
         << "Choice: ";
}

void showCustomerMenu() {
    cout << "\n[CUSTOMER MENU]\n"
         << "1) Dashboard (stub)\n"
         << "2) View balance\n"
         << "3) Deposit money\n"
         << "4) Withdraw money\n"
         << "5) Change password\n"
         << "6) View mini-statement\n"
         << "0) Logout\n"
         << "Choice: ";
}

int main() {
    // ensure transactions.txt exists
    { ofstream touch("transactions.txt", ios::app); }

    while (true) {
        showMainMenu();
        int choice; cin >> choice;

        if (choice == 0) {
            cout << "Goodbye!\n";
            break;
        }

        if (choice == 1) {
            // Registration (Admins only)
            cout << "\nPlease login as Admin to register new users.\n";
            Login lg;
            if (!lg.loginUser() || lg.getRole() != "Admin") {
                cout << "? Only Admins can register.\n";
                continue;
            }
            while (true) {
                showAdminMenu();
                int a; cin >> a;
                if (a == 0) break;
                else if (a == 1) { Registration rg; rg.registerUser(); }
                else if (a == 2) cout << "[Dashboard stub]\n";
                else if (a == 3) { AdminPanel ap(lg.getUsername()); ap.viewAllAccounts(); }
                else if (a == 4) { AdminPanel ap(lg.getUsername()); ap.deleteAccount(); }
                else if (a == 5) { AdminPanel ap(lg.getUsername()); ap.unlockAccount(); }
                else if (a == 6) { AdminPanel ap(lg.getUsername()); ap.lockAccount(); }
                else if (a == 7) { AdminPanel ap(lg.getUsername()); ap.viewTransactions(); }
                else cout << "? Invalid choice.\n";
            }
        }
        else if (choice == 2) {
            // Login (Admin or Customer)
            Login lg;
            if (!lg.loginUser()) continue;

            if (lg.getRole() == "Admin") {
                AdminPanel ap(lg.getUsername());
                while (true) {
                    showAdminMenu();
                    int a; cin >> a;
                    if (a == 0) break;
                    else if (a == 1) { Registration rg; rg.registerUser(); }
                    else if (a == 2) cout << "[Dashboard stub]\n";
                    else if (a == 3) ap.viewAllAccounts();
                    else if (a == 4) ap.deleteAccount();
                    else if (a == 5) ap.unlockAccount();
                    else if (a == 6) ap.lockAccount();
                    else if (a == 7) ap.viewTransactions();
                    else cout << "? Invalid choice.\n";
                }
            }
            else {
                CustomerPanel cp(lg.getUsername());
                while (true) {
                    showCustomerMenu();
                    int c; cin >> c;
                    if (c == 0) break;
                    else if (c == 1) cout << "[Customer dashboard stub]\n";
                    else if (c == 2) cp.viewBalance();
                    else if (c == 3) cp.depositMoney();
                    else if (c == 4) cp.withdrawMoney();
                    else if (c == 5) cp.changePassword();
                    else if (c == 6) cp.viewMiniStatement();
                    else cout << "? Invalid choice.\n";
                }
            }
        }
        else {
            cout << "? Invalid choice.\n";
        }
    }

    return 0;
}
