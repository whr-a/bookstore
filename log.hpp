#ifndef LOG
#define LOG
#include <fstream>
#include <cstring>
#include <iostream>
template <class T>
class logs
{
public:
    static std::fstream oplogs;
    int amount;
    static void add_one(int &x){
        oplogs.seekp(0);
        oplogs.write(reinterpret_cast<char*>(&x),sizeof(int));
    }
    static void pushback(int num,std::string inf){
        char temp[128];
        oplogs.seekp(sizeof(int)+(num-1)*sizeof(temp));
        strcpy(temp,inf.c_str());
        oplogs.write(reinterpret_cast<char*>(&temp),sizeof(temp));
    }
    logs(){
        std::ifstream in;
        in.open("log");
        if(!in){
            amount=0;
            std::ofstream outfile("log");
            outfile.seekp(0);
            outfile.write(reinterpret_cast<char*>(&amount),sizeof(int));
        }
        oplogs.open("log");
    }
    void add(std::string &s){
        amount++;
        add_one(amount);
        pushback(amount,s);
    }
    void print(){
        for(int i=1;i<=amount;i++){
            char inf[128];
            oplogs.seekg(sizeof(int)+(i-1)*sizeof(inf));
            oplogs.read(reinterpret_cast<char*>(&inf),sizeof(inf));
            std::cout<<inf<<'\n';
        }
    }
};
template <class T>
std::fstream logs<T>::oplogs;
#endif