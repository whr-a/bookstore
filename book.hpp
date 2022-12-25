#ifndef BOOK
#define BOOK
#include "databaseplus.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "Tokenscanner.hpp"
#include "user.hpp"
#include "error.hpp"
#include "diary.hpp"
class book_inf{
public:
    char ISBN[21];
    char bookname[61];
    char author[61];
    char keyword[61];
    int store;
    double price;
    book_inf(){
        memset(ISBN,0,sizeof(ISBN));
        memset(bookname,0,sizeof(bookname));
        memset(author,0,sizeof(author));
        memset(keyword,0,sizeof(keyword));
        store=0;price=0;
    }
    book_inf(const char* ISBN_,const char* bookname_,const char* author_,const char* keyword_){
        strcpy(ISBN,ISBN_);
        strcpy(bookname,bookname_);
        strcpy(author,author_);
        strcpy(keyword,keyword_);
        store=0;
        price=0;
    }
    book_inf &operator= (const book_inf &obj){
        strcpy(ISBN,obj.ISBN);
        strcpy(bookname,obj.bookname);
        strcpy(author,obj.author);
        strcpy(keyword,obj.keyword);
        store=obj.store;
        price=obj.price;
        return *this;
    }
    friend bool operator<(const book_inf &a,const book_inf &b){return strcmp(a.ISBN,b.ISBN)<0;}
    friend bool operator>(const book_inf &a,const book_inf &b){return strcmp(a.ISBN,b.ISBN)>0;}
    friend bool operator<=(const book_inf &a,const book_inf &b){return strcmp(a.ISBN,b.ISBN)<=0;}
    friend bool operator>=(const book_inf &a,const book_inf &b){return strcmp(a.ISBN,b.ISBN)>=0;}
    friend bool operator==(const book_inf &a,const book_inf &b){return strcmp(a.ISBN,b.ISBN)==0;}
};

class book
{
public:
    database<book_inf> books;
    std::vector<std::string> book_stack; 
    book(){
        books.setfile("books");
    }
    //0表示bookname，1表示author，2表示keyword
    static bool check(std::string s,book_inf book_,int mode){
        switch(mode){
            case 0:
                if(strcmp(s.c_str(),book_.bookname)==0)return true;
                else return false;
                break;
            case 1:
                if(strcmp(s.c_str(),book_.author)==0)return true;
                else return false;
            case 2:
                std::string s_;
                for(int i=0;i<60;i++){
                    if(book_.keyword[i]==0)break;
                    if(book_.keyword[i]!='|')s_.push_back(book_.keyword[i]);
                    else{
                        if(s==s_)return true;
                        else s_.clear();
                    }
                }
                if(s==s_)return true;
                else return false;
        }
        return false;
    }
    void login();
    void logout();
    void show(Tokenscanner &scanner,user &users);
    void buy(Tokenscanner &scanner,user &users,diary &diarys);
    void select(Tokenscanner &scanner,user &users,diary &diarys);
    void modify(Tokenscanner &scanner,user &users,diary &diarys);
    void import(Tokenscanner &scanner,user &users,diary &diarys);
};

#endif