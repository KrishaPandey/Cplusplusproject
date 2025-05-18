// User.h
#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:            
    string username;
    string password;
    string role;

public:
    void setUsername(string u) { username = u; }
    void setPassword(string p) { password = p; }
    void setRole(string r)       { role = r; }

    string getUsername() { return username; }
    string getPassword() { return password; }
    string getRole()     { return role; }
};

#endif // USER_H

