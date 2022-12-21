#ifndef TOKENSCANNER
#define TOKENSCANNER
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
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
        while(line[cur]==' '){i++;cur++;}
        if(line[cur]=='-'){i++;cur++;}
        else return "";
        while(line[cur]!=' ' && line[cur]!='\0' && line[cur]!='='){cur++;}
        if(line[cur]!='=')return "";
        else cur++;
        return line.substr(i,cur-i-1);
    }
    //0为数字，字母，下划线
    //1为除不可见字符以外 ASCII 字符
    //2为数字
    //3为除不可见字符和英文双引号以外 ASCII 字符
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
            case 3:
                for(int i=0;i<s.size();i++)if(s[i]=='\"')return false;
                break;
        }
        return true;
    }
    bool check_privilege(int x){
        if(x==1 || x==3 || x==7)return true;
        else return false;
    }
    int check_num(std::string &s){
        if(s.size()==0 || s.size()>10)return -1;
        if(s.size()==10 && s>"2147483647")return -1;
        int ans=0;
        for(int i=0;i<s.size();i++){
            if(s[i]>='0' && s[i]<='9')ans=ans*10+s[i]-'0';
            else return -1;
        }
        if(ans==0)return -1;
        else return ans;
    }
    
    bool check_keyword(std::string &s){
        std::string temp;
        std::vector<std::string> line;
        for(int i=0;i<s.size();i++){
            if(s[i]!='|')temp+=s[i];
            else{
                if(temp.empty())return false;
                if(find(line.begin(),line.end(),temp)!=line.end())return false;
                else{
                    line.push_back(temp);
                    temp.clear();
                }
            }
        }
        if(temp.empty())return false;
        if(find(line.begin(),line.end(),temp)!=line.end())return false;
        return true;
    }
    bool is_num(char &x){
        return (x>='0'&&x<='9');
    }
    double check_double(std::string &s){
        if(s.size()<=3 || s.size()>13)return -1.00;
        if(s[s.size()-3]!='.')return -1.00;
        double ans=0;
        for(int i=0;i<s.size()-3;i++){
            if(!is_num(s[i]))return -1.00;
            else ans=ans*10+s[i]-'0';
        }
        if(!(is_num(s[s.size()-2])&&is_num(s[s.size()-1])))return -1.00;
        ans+=(s[s.size()-2]-'0')*0.1+(s[s.size()-1]-'0')*0.01;
        return ans;
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