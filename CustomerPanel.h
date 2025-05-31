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
    CustomerPanel(const string u) : username(u) {}

    // 1) View current balance
    void viewBalance() {
        ifstream in("balance.txt");
        if (!in) { cout << "? Cannot open balance.txt\n"; return; }
        string u; double bal;
        while (in >> u >> bal) {
            if (u == username) {
                cout << "\nYour balance is: Rs" << bal << "\n";
                return;
            }
        }
        cout << "? No balance record found.\n";
    }

    // 2) Deposit money
    void depositMoney() {
        cout << "Enter amount to deposit: Rs";
        double amt; cin >> amt;
        if (amt <= 0) {
            cout << "? Deposit amount must be positive.\n";
            return;
        }

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
            fout << u << ' ' << bal << "\n";
        }
        fin.close(); fout.close();

        if (!updated) {
            cout << "? Balance record not found.\n";
            remove("balance.tmp");
            return;
        }

        remove("balance.txt");
        rename("balance.tmp", "balance.txt");

        ofstream tf("transactions.txt", ios::app);
        tf << username << ' '
           << currentTimestamp() << ' '
           << "DEPOSIT" << ' '
           << amt << ' '
           << newBalance << "\n";
        tf.close();

        cout << "Deposited Rs" << fixed << setprecision(2) << amt
             << ". New balance: Rs" << newBalance << "\n";
    }

    // 3) Withdraw money
    void withdrawMoney() {
        cout << "Enter amount to withdraw: Rs";
        double amt; cin >> amt;
        if (amt <= 0) {
            cout << "? Withdrawal amount must be positive.\n";
            return;
        }

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
                    fout << u << ' ' << bal << "\n";
                    // copy rest unchanged
                    while (fin >> u >> bal)
                        fout << u << ' ' << bal << "\n";
                    fin.close(); fout.close();
                    remove("balance.tmp");
                    return;
                }
                bal -= amt;
                updated = true;
                newBalance = bal;
            }
            fout << u << ' ' << bal << "\n";
        }
        fin.close(); fout.close();

        if (!updated) {
            cout << "? Balance record not found.\n";
            remove("balance.tmp");
            return;
        }

        remove("balance.txt");
        rename("balance.tmp", "balance.txt");

        ofstream tf("transactions.txt", ios::app);
        tf << username << ' '
           << currentTimestamp() << ' '
           << "WITHDRAW" << ' '
           << amt << ' '
           << newBalance << "\n";
        tf.close();

        cout << "Withdrew Rs" << fixed << setprecision(2) << amt
             << ". New balance: Rs" << newBalance << "\n";
    }
    // 4) Transfer money
    void transferMoney() {
        cout << "Enter recipient username: ";
        string recipient; 
        cin >> recipient;
        if (recipient == username) {
            cout << "? Cannot transfer to yourself.\n";
            return;
        }
			
        ifstream uf("users.txt");
        if (!uf) { cout << "? Cannot open users.txt\n"; return; }
        bool found = false;
        string u, pw, role; int lf;
        while (uf >> u >> pw >> role >> lf) {
            if (u == recipient) {
                found = true;
                if (role != "Customer") {
                    cout << "? Recipient is not a Customer.\n";
                    uf.close();
                    return;
                }
                break;
            }
        }
        uf.close();
        if (!found) {
            cout << "? Recipient not found.\n";
            return;
        }

        // get amount
        cout << "Enter amount to transfer: Rs";
        double amt; cin >> amt;
        if (amt <= 0) {
            cout << "? Amount must be positive.\n";
            return;
        }

        // Pass 1: read balances
        ifstream fin1("balance.txt");
        if (!fin1) { cout << "? Cannot open balance.txt\n"; return; }
        double senderBal = -1, recipBal = -1, bal;
        while (fin1 >> u >> bal) {
            if (u == username)    senderBal = bal;
            if (u == recipient)   recipBal  = bal;
        }
        fin1.close();

        if (senderBal < 0 || recipBal < 0) {
            cout << "? Balance record missing.\n";
            return;
        }
        if (senderBal < amt) {
            cout << "? Insufficient funds.\n";
            return;
        }

        // Pass 2: rewrite balances
        ifstream fin2("balance.txt");
        ofstream fout("balance.tmp");
        while (fin2 >> u >> bal) {
            if (u == username)    bal = senderBal - amt;
            else if (u == recipient) bal = recipBal + amt;
            fout << u << ' ' << bal << "\n";
        }
        fin2.close(); fout.close();

        remove("balance.txt");
        rename("balance.tmp", "balance.txt");

        // log transactions
        ofstream tf("transactions.txt", ios::app);
        string ts = currentTimestamp();
        tf << username  << ' ' << ts << ' '
           << "TRANSFER_OUT" << ' ' << amt << ' ' << (senderBal - amt) << "\n";
        tf << recipient << ' ' << ts << ' '
           << "TRANSFER_IN"  << ' ' << amt << ' ' << (recipBal + amt)  << "\n";
        tf.close();

        cout << "Transferred Rs" << fixed << setprecision(2) << amt
             << " to " << recipient
             << ". New balance: Rs" << (senderBal - amt) << "\n";
    }

    // 5) View mini-statement (includes transfers)
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
                cout << "? From-date must be <= To-date.\n";
                return;
            }
        }

        cout << "Filter by type? (D=deposit, W=withdraw, "
             << "T=transfer, A=all): ";
        char typeFilter; cin >> typeFilter;
        if (typeFilter!='D' && typeFilter!='W' && 
            typeFilter!='T' && typeFilter!='A') {
            cout << "? Invalid type filter.\n";
            return;
        }

        // Pass 1: count matches
        ifstream in1("transactions.txt");
        if (!in1) { cout << "? Cannot open transactions.txt\n"; return; }
        int total = 0;
        string u, date, time, typ;
        double amt2, balAfter;
        while (in1 >> u >> date >> time >> typ >> amt2 >> balAfter) {
            if (u != username) continue;
            if ((byDate=='y'||byDate=='Y') && 
                (date<fromDate || date>toDate)) continue;
            if (typeFilter=='D' && typ!="DEPOSIT")      continue;
            if (typeFilter=='W' && typ!="WITHDRAW")     continue;
            if (typeFilter=='T' &&
               typ!="TRANSFER_IN" && typ!="TRANSFER_OUT") continue;
            total++;
        }
        in1.close();

        if (total == 0) {
            cout << "? No matching transactions found.\n";
            return;
        }
        int skip = total > N ? total - N : 0;

        // Pass 2: print last N
        ifstream in2("transactions.txt");
        int seen = 0;
        cout << "\nMini-Statement for " << username << "\n\n";
        cout << left
             << setw(12) << "Date"
             << setw(10) << "Time"
             << setw(15) << "Type"
             << setw(12) << "Amount"
             << setw(15) << "Balance\n";
        cout << string(64, '-') << "\n";

        while (in2 >> u >> date >> time >> typ >> amt2 >> balAfter) {
            if (u != username) continue;
            if ((byDate=='y'||byDate=='Y') && 
                (date<fromDate || date>toDate)) continue;
            if (typeFilter=='D' && typ!="DEPOSIT")      continue;
            if (typeFilter=='W' && typ!="WITHDRAW")     continue;
            if (typeFilter=='T' &&
               typ!="TRANSFER_IN" && typ!="TRANSFER_OUT") continue;

            if (seen++ >= skip) {
                cout << left
                     << setw(12) << date
                     << setw(10) << time
                     << setw(15) << typ
                     << setw(12) << fixed << setprecision(2) << amt2
                     << setw(15) << fixed << setprecision(2) << balAfter
                     << "\n";
            }
        }
        cout << "\n";
    }

    // 6) Change password
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
        string u, pw, role; int lf;
        while (fin >> u >> pw >> role >> lf) {
            if (u == username)
                fout << u << ' ' << newp << ' ' << role << ' ' << lf << "\n";
            else
                fout << u << ' ' << pw << ' ' << role << ' ' << lf << "\n";
        }
        fin.close(); fout.close();

        remove("users.txt");
        rename("users.tmp", "users.txt");

        cout << "Password changed successfully.\n";
    }
};

#endif // CUSTOMERPANEL_H
