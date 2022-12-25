#include "user.hpp"
#include <cstring>

void user::su(Tokenscanner &scanner,diary &diarys){
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid"));
    std::string in_password=scanner.nextToken();
    if(scanner.haveMoreTokens())throw(error("Invalid"));
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second==0)throw(error("Invalid"));
    account &t=result.first;
    //std::cout<<t.password<<' '<<in_password<<std::endl;
    if(!in_password.empty()){
        if(!scanner.check(in_password,30,0))throw(error("Invalid"));
        if(strcmp(in_password.c_str(),t.password)==0){
            login_stack.push_back(t);
            //std::cout<<"su successfully"<<'\n';//**********
            diarys.add_log(userid+" login.");
        }
        else throw(error("Invalid"));
    }
    else{
        if(login_stack.empty())throw(error("Invalid"));
        account &cur_account=login_stack.back();
        if(cur_account.privilege>t.privilege){
            login_stack.push_back(t);
            //std::cout<<"su successfully"<<'\n';//**********
            diarys.add_log(userid+" login.");
        }
        else throw(error("Invalid"));
    }
}
void user::logout(diary &diarys){
    if(login_stack.empty())throw(error("Invalid"));
    std::string s=login_stack.back().ID;
    diarys.add_log(s+" logout.");
    login_stack.pop_back();
    //std::cout<<"logout successfully"<<'\n';//**********
}

void user::Register(Tokenscanner &scanner,diary &diarys){
    account temp;
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid"));
    std::string password=scanner.nextToken();
    if(!scanner.check(password,30,0))throw(error("Invalid"));
    std::string username=scanner.nextToken();
    if(!scanner.check(username,30,1))throw(error("Invalid"));
    if(scanner.haveMoreTokens())throw(error("Invalid"));
    char index_[31];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second)throw(error("Invalid"));
    strcpy(temp.ID,userid.c_str());
    strcpy(temp.password,password.c_str());
    temp.privilege=1;
    strcpy(temp.name,username.c_str());
    users.insert(index_,temp);
    diarys.add_log("registered "+userid+'.');
    //std::cout<<"Register successfully"<<'\n';//**********
}

void user::passwd(Tokenscanner &scanner,diary &diarys){
    if(login_stack.empty())throw(error("Invalid"));
    else if(login_stack.back().privilege==0)throw(error("Invalid"));
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid"));
    std::string CurrentPassword=scanner.nextToken();
    if(!scanner.check(CurrentPassword,30,0))throw(error("Invalid"));
    if(!scanner.haveMoreTokens()){
        account &cur_account=login_stack.back();
        if(cur_account.privilege==7){
            char index_[31];strcpy(index_,userid.c_str());
            std::pair<account,bool> result=users.find(index_);
            if(result.second){
                strcpy(result.first.password,CurrentPassword.c_str());
                users.modify(index_,result.first);
                //std::cout<<"passwd successfully"<<'\n';//**********
                diarys.add_log(userid+"\'s password is changed.");
                return;
            }
            else throw(error("Invalid"));
        }
        else throw(error("Invalid"));
    }
    std::string NewPassword=scanner.nextToken();
    if(!scanner.check(NewPassword,30,0))throw(error("Invalid"));
    char index_[31];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second){
        if(strcmp(result.first.password,CurrentPassword.c_str())==0){
            strcpy(result.first.password,NewPassword.c_str());
            users.modify(index_,result.first);
            //std::cout<<"passwd successfully"<<'\n';//**********
            diarys.add_log(userid+"\'s password is changed.");
        }
        else throw(error("Invalid"));
    }
    else throw(error("Invalid"));
}

void user::useradd(Tokenscanner &scanner,diary &diarys){
    //正确性判断
    if(login_stack.empty())throw(error("Invalid"));
    else if(login_stack.back().privilege<3)throw(error("Invalid"));
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid"));
    std::string password=scanner.nextToken();
    if(!scanner.check(password,30,0))throw(error("Invalid"));
    std::string privilege_=scanner.nextToken();
    if(!scanner.check(privilege_,1,2))throw(error("Invalid"));
    int privilege=privilege_[0]-'0';
    if(!scanner.check_privilege(privilege))throw(error("Invalid"));
    if(privilege>=login_stack.back().privilege)throw(error("Invalid"));
    std::string username=scanner.nextToken();
    if(!scanner.check(username,30,1))throw(error("Invalid"));
    if(scanner.haveMoreTokens())throw(error("Invalid"));
    char index_[31];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second)throw(error("Invalid"));
    //操作
    account temp(userid.c_str(),password.c_str(),privilege,username.c_str());
    users.insert(index_,temp);
    //std::cout<<"useradd successfully"<<'\n';//**********
    diarys.add_log("user: "+userid+" added.");
}

void user::deleteuser(Tokenscanner &scanner,diary &diarys){
    if(login_stack.empty())throw(error("Invalid"));
    else if(login_stack.back().privilege!=7)throw(error("Invalid"));
    std::string userid=scanner.nextToken();
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(!result.second)throw(error("Invalid"));
    account &t=result.first;
    if(find(login_stack.begin(),login_stack.end(),t)!=login_stack.end())
        throw(error("Invalid"));
    users.Delete(index_,t);
    //std::cout<<"delete successfully"<<'\n';//**********
    diarys.add_log("user: "+userid+" deleted.");
}