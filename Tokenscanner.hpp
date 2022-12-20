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
    //0为数字，字母，下划线
    //1为除不可见字符以外 ASCII 字符
    //2为数字
    //3为除不可见字符和英文双引号以外 ASCII 字符
    //4数字和 .
    bool check(const std::string &s,int max_length,int mode){
        if(s.size()==0 || s.size()>max_length)return false;
        switch(mode){
            case 0:
                for(int i=0;i<s.size();i++)
                    if(!(s[i]>='0'&&s[i]<='9'||
                         s[i]>='A'&&s[i]<='Z'||
                         s[i]>='a'&&s[i]<='z'||
                         s[i]=='_'))return false;
                break;
            case 1:
                break;
            case 2:
                for(int i=0;i<s.size();i++)if(!(s[i]>='0'&&s[i]<='9'))return false;
                break;
        }
        return true;
    }
    bool check_privilege(int x){
        if(x==0 || x==1 || x==3 || x==7)return true;
        else return false;
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