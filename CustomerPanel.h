// CustomerPanel.h
#ifndef CUSTOMERPANEL_H
#define CUSTOMERPANEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "TimestampHelper.h"
using namespace std;

class CustomerPanel {
    string username;
public:
    CustomerPanel(const string u) {
        username = u;
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
            cout << "? Deposit amount must be positive.\n";
            return;
        }

        // Update balance.txt
        ifstream fin("balance.txt");
        ofstream fout("balance.tmp");
        if (!fin) { cout << "? Cannot open balance.txt\n"; return; }

        string u; double bal;
        bool updated = false;
        double newBalance = 0.0;

        while (fin >> u >> bal) {
            if (u == username) {
                bal += amt;
                updated = true;
                newBalance = bal;
            }
            fout << u << ' ' << bal << '\n';
        }
        fin.close(); fout.close();

        if (!updated) {
            cout << "? Balance record not found.\n";
            remove("balance.tmp");
            return;
        }

        remove("balance.txt");
        rename("balance.tmp", "balance.txt");

        // Log transaction
        ofstream tf("transactions.txt", ios::app);
        tf << username << ' '
           << currentTimestamp() << ' '
           << "DEPOSIT" << ' '
           << amt << ' '
           << newBalance
           << '\n';
        tf.close();

        // Report
        cout << "Deposited Rs" << fixed << setprecision(2) << amt
             << ". New balance: Rs" << newBalance << "\n";
    }

    void withdrawMoney() {
        cout << "Enter amount to withdraw: Rs";
        double amt;
        cin >> amt;
        if (amt <= 0) {
            cout << "? Withdrawal amount must be positive.\n";
            return;
        }

        // Update balance.txt
        ifstream fin("balance.txt");
        ofstream fout("balance.tmp");
        if (!fin) { cout << "? Cannot open balance.txt\n"; return; }

        string u; double bal;
        bool updated = false;
        double newBalance = 0.0;

        while (fin >> u >> bal) {
            if (u == username) {
                if (bal < amt) {
                    cout << "? Insufficient funds.\n";
                    // copy rest unchanged
                    fout << u << ' ' << bal << '\n';
                    while (fin >> u >> bal)
                        fout << u << ' ' << bal << '\n';
                    fin.close(); fout.close();
                    remove("balance.tmp");
                    return;
                }
                bal -= amt;
                updated = true;
                newBalance = bal;
            }
            fout << u << ' ' << bal << '\n';
        }
        fin.close(); fout.close();

        if (!updated) {
            cout << "? Balance record not found.\n";
            remove("balance.tmp");
            return;
        }

        remove("balance.txt");
        rename("balance.tmp", "balance.txt");

        // Log transaction
        ofstream tf("transactions.txt", ios::app);
        tf << username << ' '
           << currentTimestamp() << ' '
           << "WITHDRAW" << ' '
           << amt << ' '
           << newBalance
           << '\n';
        tf.close();

        // Report
        cout << "Withdrew Rs" << fixed << setprecision(2) << amt
             << ". New balance: Rs" << newBalance << "\n";
    }

    void viewMiniStatement() {
        cout << "Show last how many transactions? ";
        int N; cin >> N;
        if (N <= 0) {
            cout << "? Number must be positive.\n";
            return;
        }

        cout << "Filter by date? (y/n): ";
        char byDate; cin >> byDate;
        string fromDate, toDate;
        if (byDate=='y' || byDate=='Y') {
            cout << "  From (yyyy-mm-dd): "; cin >> fromDate;
            cout << "  To   (yyyy-mm-dd): "; cin >> toDate;
            if (fromDate > toDate) {
                cout << "? From-date must be = To-date.\n";
                return;
            }
        }

        cout << "Filter by type? (D=deposit, W=withdraw, A=all): ";
        char typeFilter; cin >> typeFilter;
        if (typeFilter!='D' && typeFilter!='W' && typeFilter!='A') {
            cout << "? Invalid type filter.\n";
            return;
        }

        // Pass 1: count matches
        ifstream in1("transactions.txt");
        if (!in1) { cout << "? Cannot open transactions.txt\n"; return; }

        int total = 0;
        string u, date, time, typ;
        double amt, balAfter;
        while (in1 >> u >> date >> time >> typ >> amt >> balAfter) {
            if (u != username) continue;
            if ((byDate=='y'||byDate=='Y') && (date<fromDate || date>toDate)) continue;
            if (typeFilter=='D' && typ!="DEPOSIT")  continue;
            if (typeFilter=='W' && typ!="WITHDRAW") continue;
            total++;
        }
        in1.close();

        if (total == 0) {
            cout << "? No matching transactions found.\n";
            return;
        }
        int skip = total > N ? total - N : 0;

        // Pass 2: re-open & print only last N
        ifstream in2("transactions.txt");
        int seen = 0;
        cout << "\nMini-Statement for " << username << "\n\n";
        cout << left
             << setw(12) << "Date"
             << setw(10) << "Time"
             << setw(10) << "Type"
             << setw(12) << "Amount"
             << setw(15) << "Balance\n";
        cout << string(59, '-') << "\n";

        while (in2 >> u >> date >> time >> typ >> amt >> balAfter) {
            if (u != username) continue;
            if ((byDate=='y'||byDate=='Y') && (date<fromDate || date>toDate)) continue;
            if (typeFilter=='D' && typ!="DEPOSIT")  continue;
            if (typeFilter=='W' && typ!="WITHDRAW") continue;

            if (seen++ >= skip) {
                cout << left
                     << setw(12) << date
                     << setw(10) << time
                     << setw(10) << typ
                     << setw(12) << fixed << setprecision(2) << amt
                     << setw(15) << fixed << setprecision(2) << balAfter
                     << "\n";
            }
        }
        in2.close();
        cout << "\n";
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

        ifstream fin("users.txt");
        ofstream fout("users.tmp");
        string u, p, r,lg;
        while (fin >> u >> p >> r>>lg) {
            if (u == username) fout << u << ' ' << newp << ' ' << r << lg <<'\n';
            else               fout << u << ' ' << p    << ' ' << r << lg<< '\n';
        }
        fin.close(); fout.close();

        remove("users.txt");
        rename("users.tmp", "users.txt");

        cout << "Password changed successfully.\n";
    }
};

#endif // CUSTOMERPANEL_H
