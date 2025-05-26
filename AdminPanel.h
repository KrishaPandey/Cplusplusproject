// AdminPanel.h
#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class AdminPanel {
    string currentAdmin;

public:
    AdminPanel(string adminUser) {
        currentAdmin = adminUser;
    }

    // 1) Show all accounts, including locked flag
    void viewAllAccounts() {
        ifstream in("users.txt");
        if (!in) {
            cout << "? Cannot open users.txt\n";
            return;
        }
        cout << "\n-- All Accounts --\n";
        string u, p, r;
        int    lockFlag;
        while (in >> u >> p >> r >> lockFlag) {
            cout << "User: "   << u
                 << " | Role: " << r
                 << " | Locked: " << (lockFlag ? "Yes" : "No")
                 << "\n";
        }
    }

    // 2) Delete account (and its balance entry)
    void deleteAccount() {
        cout << "Enter username to delete: ";
        string target;
        cin >> target;

        if (target == "admin") {
            cout << "? Cannot delete superadmin.\n";
            return;
        }
        if (target == currentAdmin) {
            cout << "? You cannot delete your own account.\n";
            return;
        }

        ifstream in("users.txt");
        ofstream out("users.tmp");
        bool   found = false;
        string u, p, r;
        int    lockFlag;

        while (in >> u >> p >> r >> lockFlag) {
            if (u == target) {
                found = true;
                continue;  // skip this user
            }
            out << u << ' ' << p << ' ' << r << ' ' << lockFlag << '\n';
        }
        in.close(); out.close();

        if (!found) {
            cout << "? User not found.\n";
            remove("users.tmp");
            return;
        }

        remove("users.txt");
        rename("users.tmp", "users.txt");
        removeBalance(target);
        cout << "Deleted user: " << target << "\n";
    }

    // 3) Lock an account
    void lockAccount() {
        cout << "Enter username to lock: ";
        string target;
        cin >> target;

        if (target == "admin") {
            cout << "? Cannot lock superadmin.\n";
            return;
        }
        if (target == currentAdmin) {
            cout << "? You cannot lock your own account.\n";
            return;
        }

        ifstream fin("users.txt");
        ofstream fout("users.tmp");
        if (!fin) {
            cout << "? Cannot open users.txt\n";
            return;
        }

        bool found = false;
        string u, p, r;
        int    lockFlag;
        while (fin >> u >> p >> r >> lockFlag) {
            if (u == target) {
                lockFlag = 1;
                found = true;
            }
            fout << u << ' ' << p << ' ' << r << ' ' << lockFlag << '\n';
        }
        fin.close(); fout.close();

        if (!found) {
            cout << "? User not found.\n";
            remove("users.tmp");
            return;
        }

        remove("users.txt");
        rename("users.tmp", "users.txt");
        cout << "User " << target << " has been locked.\n";
    }

    // 4) Unlock an account
    void unlockAccount() {
        cout << "Enter username to unlock: ";
        string target;
        cin >> target;

        ifstream fin("users.txt");
        if (!fin) {
            cout << "? Cannot open users.txt\n";
            return;
        }

        bool found = false;
        string u, p, r;
        int    lockFlag;
        while (fin >> u >> p >> r >> lockFlag) {
            if (u == target) {
                found = true;
                if (lockFlag == 0) {
                    cout << "? User \"" << target << "\" is not locked.\n";
                    fin.close();
                    return;
                }
                break;
            }
        }
        fin.close();

        if (!found) {
            cout << "? User not found.\n";
            return;
        }

        ifstream fin2("users.txt");
        ofstream fout("users.tmp");
        while (fin2 >> u >> p >> r >> lockFlag) {
            if (u == target) lockFlag = 0;
            fout << u << ' ' << p << ' ' << r << ' ' << lockFlag << '\n';
        }
        fin2.close(); fout.close();

        remove("users.txt");
        rename("users.tmp", "users.txt");
        cout << "User \"" << target << "\" has been unlocked.\n";
    }

    // 5) Remove balance entry when deleting
    void removeBalance(string user) {
        ifstream fin("balance.txt");
        ofstream fout("balance.tmp");
        string u; double bal;
        while (fin >> u >> bal) {
            if (u != user)
                fout << u << ' ' << bal << '\n';
        }
        fin.close(); fout.close();
        remove("balance.txt");
        rename("balance.tmp", "balance.txt");
    }

    // 6) View a Customer’s full transaction history
    void viewTransactions() {
        cout << "Enter customer username to view transactions: ";
        string target;
        cin  >> target;

        // Verify user exists and is Customer
        ifstream uf("users.txt");
        if (!uf) {
            cout << "? Cannot open users.txt\n";
            return;
        }
        bool found = false;
        string u, p, r;
        int    lf;
        while (uf >> u >> p >> r >> lf) {
            if (u == target) {
                found = true;
                if (r != "Customer") {
                    cout << "? User \"" << target << "\" is not a Customer.\n";
                    uf.close();
                    return;
                }
                break;
            }
        }
        uf.close();
        if (!found) {
            cout << "? User not found.\n";
            return;
        }

        // Read and display transactions.txt
        ifstream tf("transactions.txt");
        if (!tf) {
            cout << "? No transactions logged yet.\n";
            return;
        }

        cout << "\n-- Transactions for " << target << " --\n\n"
             << left
             << setw(12) << "Date"
             << setw(10) << "Time"
             << setw(10) << "Type"
             << setw(12) << "Amount"
             << setw(15) << "Balance\n"
             << string(59, '-') << "\n";

        bool any = false;
        string date, time, typ;
        double amount, balAfter;
        while (tf >> u >> date >> time >> typ >> amount >> balAfter) {
            if (u != target) continue;
            any = true;
            cout << left
                 << setw(12) << date
                 << setw(10) << time
                 << setw(10) << typ
                 << setw(12) << fixed << setprecision(2) << amount
                 << setw(15) << fixed << setprecision(2) << balAfter
                 << "\n";
        }
        tf.close();

        if (!any) {
            cout << "? No transactions found for this customer.\n";
        }
        cout << "\n";
    }
};

#endif // ADMINPANEL_H
