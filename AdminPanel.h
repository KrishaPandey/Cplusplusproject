// AdminPanel.h
#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <iostream>
#include <fstream>
#include <string>
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

    // 2) Delete account (now skipping the lockFlag field)
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
            out << u << ' '
                << p << ' '
                << r << ' '
                << lockFlag
                << '\n';
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

    // 3) Lock an account (set lockFlag = 1)
    void lockAccount() {
        cout << "Enter username to lock: ";
        string target;
        cin >> target;

        if (target == "admin") {
            cout << " Cannot lock superadmin.\n";
            return;
        }
        if (target == currentAdmin) {
            cout << " You cannot lock your own account.\n";
            return;
        }

        ifstream fin("users.txt");
        ofstream fout("users.tmp");
        if (!fin) {
            cout << " Cannot open users.txt\n";
            return;
        }

        bool   found = false;
        string u, p, r;
        int    lockFlag;
        while (fin >> u >> p >> r >> lockFlag) {
            if (u == target) {
                lockFlag = 1;
                found = true;
            }
            fout << u << ' '
                 << p << ' '
                 << r << ' '
                 << lockFlag
                 << '\n';
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

    // 4) Unlock an account (set lockFlag = 0)
        void unlockAccount() {
        cout << "Enter username to unlock: ";
        string target;
        cin >> target;

        // 1) Open and scan for the user and its lockFlag
        ifstream fin("users.txt");
        if (!fin) {
            cout << "? Cannot open users.txt\n";
            return;
        }

        bool   found = false;
        string u, p, r;
        int    lockFlag;
        while (fin >> u >> p >> r >> lockFlag) {
            if (u == target) {
                found = true;
                if (lockFlag == 0) {
                    // user exists but is not locked
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
            fout << u << ' '
                 << p << ' '
                 << r << ' '
                 << lockFlag << '\n';
        }
        fin2.close();
        fout.close();

        // 3) Swap in the updated file
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
};

#endif // ADMINPANEL_H
