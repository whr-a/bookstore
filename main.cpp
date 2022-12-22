#include <iostream>
#include "Tokenscanner.hpp"
#include "book.hpp"
#include "diary.hpp"
#include "user.hpp"
#include "databaseplus.hpp"
#include "error.hpp"

void processLine(std::string &line,user &users,book &books,diary &diarys);
bool check_check(std::string s){
    for(int i=0;i<s.size();i++){
        if(!(s[i]>=32 && s[i]<127))return false;
    }
    return true;
}
int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    user users;
    book books;
    diary diarys;
    while (true) {
    try {
        std::string input;
        if(!getline(std::cin, input))return 0;
        if(!check_check(input))throw(error("Invalid"));
        //std::cout<<input<<std::endl;
        processLine(input,users,books,diarys);
        //std::cout<<"successfully done"<<'\n';
        if(users.checkquit())return 0;
    } catch (error &ex) {
        std::cout << ex.toString() << std::endl;
    }
  }
}
void processLine(std::string &line,user &users,book &books,diary &diarys) {
    Tokenscanner scanner;
    scanner.setInput(line);
    std::string s=scanner.nextToken();
    if(s.empty())return;
    if(s=="su"){users.su(scanner);books.login();}
    else if(s=="logout"){users.logout();books.logout();}
    else if(s=="register")users.Register(scanner);
    else if(s=="passwd")users.passwd(scanner);
    else if(s=="useradd")users.useradd(scanner);
    else if(s=="delete")users.deleteuser(scanner);
    else if(s=="exit"||s=="quit"){users.quit();}
    else if(s=="show"){
        std::string next=scanner.see_nextToken();
        if(next=="finance")diarys.show(scanner,users);
        else books.show(scanner,users);
    }
    else if(s=="buy")books.buy(scanner,users,diarys);
    else if(s=="select")books.select(scanner, users);
    else if(s=="modify")books.modify(scanner,users);
    else if(s=="import")books.import(scanner,users,diarys);
    else throw(error("Invalid"));
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