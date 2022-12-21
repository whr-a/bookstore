#ifndef USER
#define USER
#include "databaseplus.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "Tokenscanner.hpp"
#include "error.hpp"
class account
{
public:
    char ID[30];
    char password[30];
    int privilege;
    char name[30];
    account(){}
    account(const char* ID_,const char* password_,const int privilege_,const char* name_){
        strcpy(ID,ID_);
        strcpy(password,password_);
        privilege=privilege_;
        strcpy(name,name_);
    }
    // account(const char* ID_,const char* &password_,const int &privilege_,const char* &name_){
    //     strcpy(ID,ID_);
    //     strcpy(password,password_);
    //     privilege=privilege_;
    //     strcpy(name,name_);
    // }
    account &operator= (const account &obj){
        strcpy(ID,obj.ID);
        strcpy(password,obj.password);
        privilege=obj.privilege;
        strcpy(name,obj.name);
        return *this;
    }
    friend bool operator<(const account &a,const account &b){return strcmp(a.ID,b.ID)<0;}
    friend bool operator>(const account &a,const account &b){return strcmp(a.ID,b.ID)>0;}
    friend bool operator==(const account &a,const account &b){return strcmp(a.ID,b.ID)==0;}
    friend bool operator>=(const account &a,const account &b){return strcmp(a.ID,b.ID)>=0;}
    friend bool operator<=(const account &a,const account &b){return strcmp(a.ID,b.ID)<=0;}
};
class user
{
public:
    database<account> users;
    std::vector<account> login_stack;
    bool exit;
    user(){
        exit=0;
        bool flag=users.setfile("accounts");
        if(!flag){
            account temp("root","sjtu",7,"");
            char tem[]="root";
            users.insert(tem,temp);
        }
    }
    void quit(){exit=1;}
    bool checkquit(){return exit;}
    void su(Tokenscanner &scanner);
    void logout();
    void Register(Tokenscanner &scanner);
    void passwd(Tokenscanner &scanner);
    void useradd(Tokenscanner &scanner);
    void deleteuser(Tokenscanner &scanner);
};
#endif