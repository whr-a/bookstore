#ifndef DIARY
#define DIARY
#include <iostream>
#include "Tokenscanner.hpp"
#include "database.hpp"
#include <string>
#include "error.hpp"
#include <iomanip>
#include "log.hpp"

class diary{
public:
    int total;
    Database_<double> diarys;
    logs<int> log_inf;
    diary(){
        bool flag=diarys.setfile("diary");
        if(!flag){
            double x=0;
            diarys.insert(0,x);
            total=0;
        }
        else{
            double result=diarys.find();
            total=(int)result;
            //std::cout<<"**"<<total<<"**"<<'\n';
        }
    }
    void add(double get){
        total++;
        diarys.insert(total,get);
        diarys.add_one();
    }
    void show(Tokenscanner &scanner){
        scanner.nextToken();
        if(!scanner.haveMoreTokens()){
            std::vector<double> ans=diarys.search(1);
            double positive=0.00,nagetive=0.00;
            for(int i=0;i<ans.size();i++){
                if(ans[i]>0)positive+=ans[i];
                else nagetive+=0-ans[i];
            }
            std::cout<<"+ ";
            std::cout<<setiosflags(std::ios::fixed)<<std::setprecision(2)<<positive;
            std::cout<<" - ";
            std::cout<<setiosflags(std::ios::fixed)<<std::setprecision(2)<<nagetive;
            std::cout<<std::endl;
            return;
        }
        else{
            std::string count_=scanner.nextToken();
            int count=scanner.check_num_(count_);
            if(count==-1)throw(error("Invalid"));
            if(scanner.haveMoreTokens())throw(error("Invalid"));
            if(count==0){std::cout<<std::endl;return;}
            if(count>total)throw(error("Invalid"));
            std::vector<double> ans=diarys.search(total-count+1);
            // std::cout<<"***"<<'\n';
            // for(int i=0;i<ans.size();i++)std::cout<<setiosflags(std::ios::fixed)<<std::setprecision(2)<<ans[i]<<'\n';
            // std::cout<<"***"<<'\n';
            double positive=0.00,nagetive=0.00;
            for(int i=0;i<ans.size();i++){
                if(ans[i]>0)positive+=ans[i];
                else nagetive+=0-ans[i];
            }
            std::cout<<"+ ";
            std::cout<<setiosflags(std::ios::fixed)<<std::setprecision(2)<<positive;
            std::cout<<" - ";
            std::cout<<setiosflags(std::ios::fixed)<<std::setprecision(2)<<nagetive;
            std::cout<<std::endl;
            return;
        }
    }
    void add_log(std::string &&s){
        log_inf.add(s);
    }
    void log(){
        log_inf.print();
    }
};
#endif