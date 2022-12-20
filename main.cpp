#include <iostream>
#include "Tokenscanner.hpp"
#include "book.hpp"
#include "bookstore.hpp"
#include "diary.hpp"
#include "user.hpp"
#include "databaseplus.hpp"
#include "error.hpp"

void processLine(std::string &line,user &users);
int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    user users;
    while (true) {
    try {
        std::string input;
        getline(std::cin, input);
        if (input.empty())
            return 0;
        processLine(input,users);
        if(users.checkquit())return 0;
    } catch (error &ex) {
        std::cout << ex.toString();
    }
  }
}
void processLine(std::string &line,user &users) {
    Tokenscanner scanner;
    scanner.setInput(line);
    std::string s=scanner.nextToken();
    if(s.empty())return;
    if(s=="su")users.su(scanner);
    else if(s=="logout")users.logout();
    else if(s=="register")users.Register(scanner);
    else if(s=="passwd")users.passwd(scanner);
    else if(s=="useradd")users.useradd(scanner);
    else if(s=="delete")users.deleteuser(scanner);
    else if(s=="exit"||s=="quit"){users.quit();}
    else throw(error("Invalid\n"));
}
// # 基础指令
// quit
// exit

// # 帐户系统指令
// su [UserID] ([Password])?
// logout
// register [UserID] [Password] [Username]
// passwd [UserID] ([CurrentPassword])? [NewPassword]
// useradd [UserID] [Password] [Privilege] [Username]
// delete [UserID]

// # 图书系统指令
// show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
// buy [ISBN] [Quantity]
// select [ISBN]
// modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
// import [Quantity] [TotalCost]

// # 日志系统指令
// show finance ([Count])?
// log