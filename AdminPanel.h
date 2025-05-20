// AdminPanel.h
#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class AdminPanel {
    // remember who’s running this panel
    string currentAdmin;

public:
    // take the logged-in admin’s username by value
    AdminPanel( string adminUser){
    	currentAdmin=adminUser;
	}
      

    void viewAllAccounts() {
        ifstream in("users.txt");
        if (!in) { 
            cout << "? Cannot open users.txt\n"; 
            return; 
        }
        cout << "\n-- All Accounts --\n";
        string u, p, r;
        while (in >> u >> p >> r) {
            cout << "User: " << u << " | Role: " << r << "\n";
        }
    }

    void deleteAccount() {
        cout << "Enter username to delete: ";
        string target; 
        cin >> target;

        // 1) never delete First admin
        if (target == "admin") {
            cout << "? Cannot delete superadmin.\n";
            return;
        }
        // 2) never delete yourself
        if (target == currentAdmin) {
            cout << "? You cannot delete your own account.\n";
            return;
        }

        // 3) perform the temp-file swap
        ifstream in("users.txt");
        ofstream out("users.tmp");
        bool found = false;
        string u, p, r;
        while (in >> u >> p >> r) {
            if (u == target) {
                found = true;
                continue;      // skip writing this user
            }
            out << u << ' ' << p << ' ' << r << '\n';
        }
        in.close();  out.close();

        if (!found) {
            cout << "? User not found.\n";
            remove("users.tmp");
            return;
        }

        // 4) swap files
        remove("users.txt");
        rename("users.tmp", "users.txt");

        // 5) also remove their balance record
        removeBalance(target);
        cout << "Deleted user: " << target << "\n";
    }

private:
    void removeBalance(const string& user) {
        ifstream fin("balance.txt");
        ofstream fout("balance.tmp");
        string u; double bal;
        while (fin >> u >> bal) {
            if (u != user)
                fout << u << ' ' << bal << '\n';
        }
        fin.close();  fout.close();
        remove("balance.txt");
        rename("balance.tmp", "balance.txt");
    }
};

#endif // ADMINPANEL_H

