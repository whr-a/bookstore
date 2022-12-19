#ifndef TOKENSCANNER
#define TOKENSCANNER
#include <cstring>
#include <iostream>
class Tokenscanner
{
private:
    std::string line;
    int cur;
public:
    Tokenscanner(){
        cur=0;
    }
    ~Tokenscanner(){}
    void setInput(const std::string &s){
        line=s;
        cur=0;
    }
    std::string nextToken(){
        int i=cur;
        if(line[cur]=='\0')return "";
        while(line[cur]==' '){i++;cur++;}
        while(line[cur]!=' ' && line[cur]!='\0'){cur++;}
        return line.substr(i,cur-i);
    }
    bool haveMoreTokens(){
        while(line[cur]==' ')cur++;
        if(line[cur]=='\0')return false;
        else return true;
    }
    std::string nextToken_separate(){
        int i=cur;
        if(line[cur]=='\0')return "";
        while(line[cur]==' ' || line[cur]=='-'){i++;cur++;}
        while(line[cur]!=' ' && line[cur]!='\0' && line[cur]!='='){cur++;}
        cur++;
        return line.substr(i,cur-i-1);
    }
};

#endif

// #include "Tokenscanner.hpp"

// int main ()
// {
// 	Tokenscanner scanner;
// 	std::string s;
// 	getline(std::cin,s);
// 	scanner.setInput(s);
// 	std::cout<<scanner.nextToken()<<'\n';
// 	while(true){
// 		std::string s1=scanner.nextToken_separate();
// 		if(s1=="")break;
// 		std::string s2=scanner.nextToken();
// 		std::cout<<s1<<'\n'<<s2<<'\n';
// 	}
// 	return 0;
// }