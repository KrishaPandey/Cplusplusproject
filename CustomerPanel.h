// CustomerPanel.h
#ifndef CUSTOMERPANEL_H
#define CUSTOMERPANEL_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class CustomerPanel {
    string username;
public:
    CustomerPanel(const string  u) {
	username=u;
	}

    void viewBalance() {
        ifstream in("balance.txt");
        if (!in) { cout << "? Cannot open balance.txt\n"; return; }
        string u; double bal;
        while (in >> u >> bal) {
            if (u == username) {
                cout << "\nYour balance is: Rs" << bal << "\n";
                in.close();
                return;
            }
        }
        cout << "? No balance record found.\n";
        in.close();
    }

    void depositMoney() {
        cout << "Enter amount to deposit: Rs";
        double amt; 
        cin >> amt;
        if (amt <= 0) {
            cout << "Rs Deposit amount must be positive.\n";
            return;
        }

        ifstream fin("balance.txt");
        ofstream fout("balance.tmp");
        string u; double bal;
        bool updated = false;

        while (fin >> u >> bal) {
            if (u == username) {
                bal += amt;
                updated = true;
            }
            fout << u << ' ' << bal << '\n';
        }
        fin.close(); fout.close();

        if (!updated) {
            cout << "Rs Balance record not found.\n";
            remove("balance.tmp");
        } else {
            remove("balance.txt");
            rename("balance.tmp", "balance.txt");
            // show new balance
            ifstream rin("balance.txt");
            while (rin >> u >> bal) {
                if (u == username) {
                    cout << "Deposited ?" << amt 
                         << ". New balance: Rs" << bal << "\n";
                    break;
                }
            }
            rin.close();
        }
    }

    void changePassword() {
        cout << "Enter new password: ";
        string newp, confirm; 
        cin >> newp;
        cout << "Confirm new password: ";
        cin >> confirm;
        if (newp != confirm) {
            cout << "? Passwords do not match.\n";
            return;
        }

        // rewrite users.txt
        ifstream fin("users.txt");
        ofstream fout("users.tmp");
        string u, p, r;
        while (fin >> u >> p >> r) {
            if (u == username) fout << u << ' ' << newp << ' ' << r << '\n';
            else               fout << u << ' ' << p    << ' ' << r << '\n';
        }
        fin.close(); fout.close();
        remove("users.txt");
        rename("users.tmp", "users.txt");

        cout << "Password changed successfully.\n";
    }
};

#endif 

