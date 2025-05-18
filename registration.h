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
            cout << "Passwords do not match.\n";
            return;
        }

        // 3) choose role
        cout << "Role (1=Admin, 2=Customer): ";
        int r; 
        cin >> r;
        role = (r == 1 ? "Admin" : "Customer");

        // 4) append to file (each record ends with '\n')
        ofstream file("users.txt", ios::app);
        if (!file) {
            cout << "Could not open users.txt for writing.\n";
            return;
        }

        file << username << ' '
             << password << ' '
             << role     << '\n';
        file.close();

        cout << "Registered " 
             << username << " as " << role << "\n";
    }
};

#endif // REGISTRATION_H

