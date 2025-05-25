// Registration.h
#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "User.h"
#include <iostream>
#include <fstream>
using namespace std;

class Registration : public User {
public:
    void registerUser() {
        cout << "\n+----------------------------+\n"
             << "|         REGISTER           |\n"
             << "+----------------------------+\n";

        // 1) get username
        cout << "Username: ";
        cin  >> username;

        // 2) get & confirm password 
        cout << "Password: ";
        cin  >> password;
        cout << "Confirm password: ";
        string confirm;
        cin  >> confirm;
        if (password != confirm) {
            cout << "? Passwords do not match.\n";
            return;
        }

        // 3) choose role
        cout << "Role (1=Admin, 2=Customer): ";
        int r; 
        cin >> r;
        role = (r == 1 ? "Admin" : "Customer");

        // 4) append to users.txt
        ofstream uf("users.txt", ios::app);
        if (!uf) {
            cout << "? Could not open users.txt for writing.\n";
            return;
        }
        uf << username << ' '
           << password << ' '
           << role     << ' '
           <<0
           <<'\n';
           
        uf.close();

        // 5) append to balance.txt **only if Customer**
        if (role == "Customer") {
            ofstream bf("balance.txt", ios::app);
            if (!bf) {
                cout << "Warning: could not open balance.txt. Balance not initialized.\n";
            } else {
                bf << username << ' ' << 0.0 << '\n';
                bf.close();
            }
            cout << "Registered " << username 
                 << " as Customer with balance ?0.0\n";
        }
        else {
            cout << "Registered " << username << " as Admin.\n";
        }
    }
};

#endif // REGISTRATION_H
