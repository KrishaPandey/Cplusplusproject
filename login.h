#ifndef LOGIN_H
#define LOGIN_H

#include "User.h"
#include <iostream>
#include <fstream>
using namespace std;

class Login : public User {
    // helper to set lockFlag in users.txt
    void lockAccount(string target) {
        ifstream fin("users.txt");
        ofstream fout("users.tmp");
        string u, p, r;
        int    lockFlag;
        while (fin >> u >> p >> r >> lockFlag) {
            if (u == target) lockFlag = 1;
            fout << u << ' ' << p << ' ' << r << ' ' << lockFlag << '\n';
        }
        fin.close();
        fout.close();
        remove("users.txt");
        rename("users.tmp", "users.txt");
    }

public:
    bool loginUser() {
        cout << "\n+----------------------------+\n"
             << "|          LOGIN             |\n"
             << "+----------------------------+\n";

        // 1) prompt username & check if locked
        cout << "Username: ";
        cin  >> username;

        ifstream file("users.txt");
        if (!file.is_open()) {
            cout << "? No users.txt found.\n";
            return false;
        }

        // scan once to find lockFlag for this user
        bool   userFound = false;
        int    lockFlag  = 0;
        {
            string u, p, r;
            while (file >> u >> p >> r >> lockFlag) {
                if (u == username) {
                    userFound = true;
                    break;
                }
            }
        }
        file.close();

        if (!userFound) {
            cout << "? Username not found.\n";
            return false;
        }
        if (lockFlag == 1) {
            cout << "? Your account is locked. Contact an administrator.\n";
            return false;
        }

        // 2) allow up to 3 password tries
        int maxAttempts = 3;
        int attempts = 0;
        while (attempts < maxAttempts) {
            cout << "Password: ";
            cin  >> password;

            ifstream chk("users.txt");
            string u, p, r;
            int    lf;
            bool   success = false;
            while (chk >> u >> p >> r >> lf) {
                if (u == username && p == password) {
                    role = r;
                    success = true;
                    break;
                }
            }
            chk.close();

            if (success) {
                return true;
            }

            cout << "? Invalid password.\n";
            attempts++;
        }

        // 3) too many failures ? lock
        lockAccount(username);
        cout << "? Too many failed attempts. Your account is now locked.\n";
        return false;
    }
};

#endif // LOGIN_H
