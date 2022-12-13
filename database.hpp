#ifndef DATABASE
#define DATABASE
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <fstream>
class head
{
public:
    char from[64];
    int from_value;
    char to[64];
    int to_value;
    int num=0;
    head(){
        memset(from,0,sizeof(from));
        memset(to,0,sizeof(to));
        from_value=0;
        to_value=0;
    }
};

class data
{
public:
    char index[64];
    int value;
    bool operator <(data &other){
        std::string s1=index,s2=other.index;
        if(s1<s2)return true;
        else if(s1==s2 && value<other.value)return true;
        else return false;
    }
    bool operator >(data &other){return other<*this;}
    bool operator >=(data &other){return !(*this<other);}
    bool operator <=(data &other){return !(*this>other);}
    bool operator ==(data &other){return (!(*this>other) && !(*this<other));}
};
class database
{
public:
    database(){
        std::ofstream outfile("database");
        head h;
        outfile.write(reinterpret_cast<char*>(&h),sizeof(head));
    }
    void insert(std::string index_,int value_){
        data temp;
        strcpy(temp.index,index_.c_str());
        temp.value=value_;//完成temp节点的构造
        std::fstream outfile("database");
        outfile.seekg(0);
        head h;
        outfile.read(reinterpret_cast<char *>(&h),sizeof(head));//取出头节点
        std::string s1=index_,s2=h.from,s3=h.to;
        if(s1<s2 || s1==s2 && value_<h.from_value){
            data tem;
            for(int j=h.num+1;j>=2;j--){//后面的依次往后移一格
                outfile.seekg((j-2)*sizeof(data)+sizeof(head));
                outfile.read(reinterpret_cast<char *>(&tem),sizeof(data));
                outfile.seekp((j-1)*sizeof(data)+sizeof(head));
                outfile.write(reinterpret_cast<const char *>(&tem),sizeof(data));
            }
            outfile.seekp(sizeof(head));
            outfile.write(reinterpret_cast<char *>(&temp),sizeof(data));//填入最前面
            strcpy(h.from,index_.c_str());//更新from
            h.from_value=value_;
            h.num++;
            outfile.seekp(0);
            outfile.write(reinterpret_cast<const char *>(&h),sizeof(head));//更新头节点
            return;
        }
        data tem1,tem2;
        for(int i=1;i<h.num;i++){//插在中间
            outfile.seekg((i-1)*sizeof(data)+sizeof(head));
            outfile.read(reinterpret_cast<char *>(&tem1),sizeof(data));
            outfile.seekg((i)*sizeof(data)+sizeof(head));
            outfile.read(reinterpret_cast<char *>(&tem2),sizeof(data));
            if(temp>=tem1 && temp<=tem2){
                //std::cout<<114<<' '<<index_;
                for(int j=h.num+1;j>=i+1;j--){//后面的依次往后移一格
                    data tem;
                    outfile.seekg((j-2)*sizeof(data)+sizeof(head));
                    outfile.read(reinterpret_cast<char *>(&tem),sizeof(data));
                    outfile.seekp((j-1)*sizeof(data)+sizeof(head));
                    outfile.write(reinterpret_cast<const char *>(&tem),sizeof(data));
                }
                outfile.seekp((i)*sizeof(data)+sizeof(head));
                outfile.write(reinterpret_cast<const char *>(&temp),sizeof(data));
                h.num++;
                outfile.seekp(0);
                outfile.write(reinterpret_cast<const char *>(&h),sizeof(head));
                return;
            }
        }
        if(s1>s3 || s1==s3 && value_<h.to_value){
            outfile.seekp(h.num*sizeof(data)+sizeof(head));
            outfile.write(reinterpret_cast<const char *>(&temp),sizeof(data));
            strcpy(h.to,index_.c_str());
            h.to_value=value_;
            h.num++;
            if(h.num==1){
                h.from_value=value_;
                strcpy(h.from,index_.c_str());
            }
            outfile.seekp(0);
            outfile.write(reinterpret_cast<const char *>(&h),sizeof(head));
            return;
        }
    }

    void Delete (std::string index_,int value_){
        std::fstream outfile("database");
        head h;
        data goal;
        strcpy(goal.index,index_.c_str());
        goal.value=value_;
        outfile.seekg(0);
        outfile.read(reinterpret_cast<char *>(&h),sizeof(head));
        for(int i=1;i<=h.num;i++){
            data temp;
            outfile.seekg((i-1)*sizeof(data)+sizeof(head));
            outfile.read(reinterpret_cast<char *>(&temp),sizeof(data));
            if(goal==temp){
                data tem;
                for(int j=i;j<=h.num-1;j++){
                    outfile.seekg((j)*sizeof(data)+sizeof(head));
                    outfile.read(reinterpret_cast<char *>(&tem),sizeof(data));
                    outfile.seekp((j-1)*sizeof(data)+sizeof(head));
                    outfile.write(reinterpret_cast<char*>(&tem),sizeof(data));
                }
                h.num--;
                if(h.num==0){
                    memset(h.from,0,sizeof(h.from));
                    memset(h.to,0,sizeof(h.to));
                    h.from_value=0;h.to_value=0;
                }
                else{
                    if(i==1){
                        outfile.seekg(sizeof(head));
                        outfile.read(reinterpret_cast<char*>(&tem),sizeof(data));
                        strcpy(h.from,tem.index);
                        h.from_value=tem.value;
                    }
                    if(i==h.num+1){
                        outfile.seekg((h.num-1)*sizeof(data)+sizeof(head));
                        outfile.read(reinterpret_cast<char *>(&tem),sizeof(data));
                        strcpy(h.to,tem.index);
                        h.to_value=tem.value;
                    }
                }
                outfile.seekp(0);
                outfile.write(reinterpret_cast<char*>(&h),sizeof(head));
                return;
            }
        }
        return;
    }
    void find (std::string index_){
        std::fstream outfile("database");
        head h;
        outfile.seekg(0);
        outfile.read(reinterpret_cast<char *>(&h),sizeof(head));
        //std::cout<<h.num;
        bool flag=1;
        for(int i=1;i<=h.num;i++){
            data temp;
            outfile.seekg((i-1)*sizeof(data)+sizeof(head));
            outfile.read(reinterpret_cast<char *>(&temp),sizeof(data));
            //std::cout<<temp.index<<' '<<temp.value<<'\n';
            if(temp.index==index_){
                std::cout<<temp.value<<' ';
                flag=0;
            }
        }
        if(flag)std::cout<<"null";
        std::cout<<'\n';
    }
    
};

#endif