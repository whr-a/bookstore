#include "user.hpp"
#include <cstring>

void user::su(Tokenscanner &scanner){
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid\n"));
    std::string in_password=scanner.nextToken();
    if(scanner.haveMoreTokens())throw(error("Invalid\n"));
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second==0)throw(error("Invalid\n"));
    account &t=result.first;
    if(!in_password.empty()){
        if(strcmp(in_password.c_str(),t.password)==0){
            login_stack.push_back(t);
            //std::cout<<"su successfully"<<'\n';//**********
        }
        else throw(error("Invalid\n"));
    }
    else{
        if(login_stack.empty())throw(error("Invalid\n"));
        account &cur_account=login_stack.back();
        if(cur_account.privilege>t.privilege){
            login_stack.push_back(t);
            //std::cout<<"su successfully"<<'\n';//**********
        }
        else throw(error("Invalid\n"));
    }
}
void user::logout(){
    if(login_stack.empty())throw(error("Invalid\n"));
    if(login_stack.back().privilege==0)throw(error("Invalid\n"));
    else login_stack.pop_back();
    //std::cout<<"logout successfully"<<'\n';//**********
}

void user::Register(Tokenscanner &scanner){
    account temp;
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid\n"));
    std::string password=scanner.nextToken();
    if(!scanner.check(password,30,0))throw(error("Invalid\n"));
    std::string username=scanner.nextToken();
    if(!scanner.check(username,30,1))throw(error("Invalid\n"));
    if(scanner.haveMoreTokens())throw(error("Invalid\n"));
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second)throw(error("Invalid\n"));
    strcpy(temp.ID,userid.c_str());
    strcpy(temp.password,password.c_str());
    temp.privilege=1;
    strcpy(temp.name,username.c_str());
    users.insert(index_,temp);
    //std::cout<<"Register successfully"<<'\n';//**********
}

void user::passwd(Tokenscanner &scanner){
    if(login_stack.empty())throw(error("Invalid\n"));
    else if(login_stack.back().privilege==0)throw(error("Invalid\n"));
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid\n"));
    std::string CurrentPassword=scanner.nextToken();
    if(!scanner.check(CurrentPassword,30,0))throw(error("Invalid\n"));
    if(!scanner.haveMoreTokens()){
        account &cur_account=login_stack.back();
        if(cur_account.privilege==7){
            char index_[30];strcpy(index_,userid.c_str());
            std::pair<account,bool> result=users.find(index_);
            if(result.second){
                strcpy(result.first.password,CurrentPassword.c_str());
                users.modify(index_,result.first);
                //std::cout<<"passwd successfully"<<'\n';//**********
                return;
            }
            else throw(error("Invalid\n"));
        }
        else throw(error("Invalid\n"));
    }
    std::string NewPassword=scanner.nextToken();
    if(!scanner.check(NewPassword,30,0))throw(error("Invalid\n"));
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second){
        if(strcmp(result.first.password,CurrentPassword.c_str())==0){
            strcpy(result.first.password,NewPassword.c_str());
            users.modify(index_,result.first);
            //std::cout<<"passwd successfully"<<'\n';//**********
        }
        else throw(error("Invalid\n"));
    }
    else throw(error("Invalid\n"));
}

void user::useradd(Tokenscanner &scanner){
    //正确性判断
    if(login_stack.empty())throw(error("Invalid\n"));
    else if(login_stack.back().privilege<3)throw(error("Invalid\n"));
    std::string userid=scanner.nextToken();
    if(!scanner.check(userid,30,0))throw(error("Invalid\n"));
    std::string password=scanner.nextToken();
    if(!scanner.check(password,30,0))throw(error("Invalid\n"));
    std::string privilege_=scanner.nextToken();
    if(!scanner.check(privilege_,1,2))throw(error("Invalid\n"));
    int privilege=privilege_[0]-'0';
    if(!scanner.check_privilege(privilege))throw(error("Invalid\n"));
    if(privilege>=login_stack.back().privilege)throw(error("Invalid\n"));
    std::string username=scanner.nextToken();
    if(!scanner.check(username,30,1))throw(error("Invalid\n"));
    if(scanner.haveMoreTokens())throw(error("Invalid\n"));
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(result.second)throw(error("Invalid\n"));
    //操作
    account temp(userid.c_str(),password.c_str(),privilege,username.c_str());
    users.insert(index_,temp);
    //std::cout<<"useradd successfully"<<'\n';//**********
}

void user::deleteuser(Tokenscanner &scanner){
    if(login_stack.empty())throw(error("Invalid\n"));
    else if(login_stack.back().privilege!=7)throw(error("Invalid\n"));
    std::string userid=scanner.nextToken();
    char index_[30];strcpy(index_,userid.c_str());
    std::pair<account,bool> result=users.find(index_);
    if(!result.second)throw(error("Invalid\n"));
    account &t=result.first;
    if(find(login_stack.begin(),login_stack.end(),t)!=login_stack.end())
        throw(error("Invalid\n"));
    users.Delete(index_,t);
    //std::cout<<"delete successfully"<<'\n';//**********
}