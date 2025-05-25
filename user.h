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
    bool   locked;

public:
    void setUsername(string u) { username = u; }
    void setPassword(string p) { password = p; }
    void setRole(string r)       { role = r; }
    void setLocked(bool l)     { locked = l; } 

    string getUsername() { return username; }
    string getPassword() { return password; }
    string getRole()     { return role; }
    bool   isLocked()    { return locked; }
};

#endif // USER_H
