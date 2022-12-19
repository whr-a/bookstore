#include <iostream>
#include "Tokenscanner.hpp"
#include "book.hpp"
#include "bookstore.hpp"
#include "diary.hpp"
#include "user.hpp"
#include "databaseplus.hpp"
#include "error.hpp"

void processLine(std::string &line);
int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    while (true) {
    try {
        std::string input;
        getline(std::cin, input);
        if (input.empty())
            return 0;
        processLine(input);
    } catch (error &ex) {
        std::cout << ex.toString() << std::endl;
    }
  }
}
void processLine(std::string &line) {
    Tokenscanner scanner;
    std::string s=scanner.nextToken();
    if(s=="su"){}
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