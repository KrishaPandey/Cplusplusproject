#ifndef LOGIN_H
#define LOGIN_H

#include "User.h"
#include <iostream>
#include <fstream>
using namespace std;

class Login : public User {
public:
    bool loginUser() {
        cout << "\n+----------------------------+\n"
             << "|          LOGIN             |\n"
             << "+----------------------------+\n";

       
        cout << "Username: ";
        cin  >> username;

        cout << "Password: ";
        cin  >> password;
        cout << "\n";

        ifstream file("users.txt");
        if (!file.is_open()) {
            cout << "No users.txt found.\n";
            return false;
        }

        string u, p, r;
        while (file >> u >> p >> r) {
            if (u == username && p == password) {
                role = r;
                return true;
            }
        }

        cout << "Invalid username or password.\n";
        return false;
    }
};

#endif 

