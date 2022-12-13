#ifndef DATABASEPLUS
#define DATABASEPLUS

#include <iostream>
#include <stdexcept>
#include <string.h>
#include <fstream>
class start {
public:
    int num;
    int max_num_of_block;
    start (){
        num=1;
        max_num_of_block=1;
    }
};
class head {
public:
    char from[64];
    int from_value;
    char to[64];
    int to_value;
    int num=0;
    int next_head_num;
    int last_head_num;
    head(){
        memset(from,0,sizeof(from));
        memset(to,0,sizeof(to));
        from_value=0;
        to_value=0;
        last_head_num=0;
        next_head_num=0;
    }
};
class data {
public:
    char index[64];
    int value;
    bool operator <(data &other){
        std::string s1=index,s2=other.index;
        if(s1<s2)return true;
        else if(s1==s2 && value<other.value)return true;
        else return false;
    }
    data(){
        memset(index,0,sizeof(index));
        value=0;
    }
    data(std::string s,int value_){
        strcpy(index,s.c_str());
        value=value_;
    }
    void build(std::string s,int value_){
        strcpy(index,s.c_str());
        value=value_;
    }
    bool operator >(data &other){return other<*this;}
    bool operator >=(data &other){return !(*this<other);}
    bool operator <=(data &other){return !(*this>other);}
    bool operator ==(data &other){return (!(*this>other) && !(*this<other));}
};
static head gethead(int num){
    std::fstream opfile("database");
    opfile.seekg(sizeof(start)+(num-1)*(sizeof(head)+316*sizeof(data)));
    head h;
    opfile.read(reinterpret_cast<char *>(&h),sizeof(head));
    return h;
}
static data getdata(int head_num,int data_num){
    std::fstream opfile("database");
    opfile.seekg(sizeof(start)+(head_num-1)*(sizeof(head)+316*sizeof(data))
                    +sizeof(head)+(data_num-1)*sizeof(data));
    data d;
    opfile.read(reinterpret_cast<char *>(&d),sizeof(data));
    return d;
}
static start getstart(){
    std::fstream opfile("database");
    opfile.seekg(0);
    start st;
    opfile.read(reinterpret_cast<char *>(&st),sizeof(start));
    return st;
}
static int find_add_position(data temp,start st){
    int add_position;
    if(st.num==1)add_position=1;
    else{
        int i=1;
        while(true){
            head h;
            h=gethead(i);
            data from(h.from,h.from_value);
            if(h.next_head_num==0){//找到最后一个块，还比最后一个大
                if(temp>=from){
                    add_position=i;
                    break;
                }
            }
            else if(h.last_head_num==0){
                if(temp<from){
                    add_position=i;
                    break;
                }
            }
            else{
                head h_next=gethead(h.next_head_num);
                data to(h_next.from,h_next.to_value);
                if(temp>=from && temp<to){
                    add_position=i;
                    break;
                }
            }
            if(h.next_head_num!=0)i=h.next_head_num;
            else break;
        }
    }
    return add_position;
}
static void modify_data(int head_num,int data_num,data data_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(head_num-1)*(sizeof(head)+316*sizeof(data))
                    +sizeof(head)+(data_num-1)*sizeof(data));
    opfile.write(reinterpret_cast<char*>(&data_),sizeof(data));
}
static void modify_head(int head_num,head head_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(head_num-1)*(sizeof(head)+316*sizeof(data)));
    opfile.write(reinterpret_cast<char*>(&head_),sizeof(head));
}
static void modify_start(start st){
    std::fstream opfile("database");
    opfile.seekp(0);
    opfile.write(reinterpret_cast<char*>(&st),sizeof(start));
}
static void devide(int head_num)
{
//裂开一个块，首先到文件最后开一个块，然后把159——316号元素存到这个块里，更新两块的区间
//判断这个块，如果没有后继，则将后继设为这个新块（设置新块的前驱）
//若有，则更新本块的后继、新块的前驱后继、后继块的前驱
    head this_head;this_head=gethead(head_num);
    start st=getstart(); st.num++;st.max_num_of_block++;
    modify_start(st);
    //下面的区块为建造一个新head_
    head head_;
    data front; front=getdata(head_num,159);
    data end;   end=getdata(head_num,316);
    strcpy(head_.from,front.index);head_.from_value=front.value;
    strcpy(head_.to,end.index);    head_.to_value=end.value;
    head_num=158;head_.last_head_num=head_num;
    if(!this_head.next_head_num)head_.next_head_num=this_head.next_head_num;
    modify_head(st.max_num_of_block,head_);
    //下面的区块为更新本节点
    data end_;end_=getdata(head_num,158);
    strcpy(this_head.to,end_.index);
    this_head.to_value=end_.value;
    this_head.next_head_num=st.max_num_of_block;
    this_head.num=158;
    modify_head(head_num,this_head);
    //下面的区块为拷贝元素
    data temp;
    for(int i=1;i<=158;i++){
        temp=getdata(head_num,158+i);
        modify_data(st.max_num_of_block,i,temp);
    }
}
class database
{
public:
    database(){
        std::ofstream outfile("database");
        outfile.seekp(0);
        start st;
        outfile.write(reinterpret_cast<char*>(&st),sizeof(start));
        outfile.seekp(sizeof(start));
        head h;
        outfile.write(reinterpret_cast<char*>(&h),sizeof(head));
    }
    void insert(std::string index_,int value_){
        data temp(index_,value_);//完成temp节点的构造
        std::fstream outfile("database");
        start st;st=getstart();
        int add_position=find_add_position(temp,st);
        head add_pos=gethead(add_position);
        data tem1,tem2;
        bool flag=1;//是否成功插入中间
        data from(add_pos.from,add_pos.from_value);
        if(temp<from){//要插在头的前面的特殊情况。
            for(int j=add_pos.num+1;j>=2;j--){//后面的依次往后移一格
                data tem;
                tem=getdata(add_position,j-1);
                modify_data(add_position,j,tem);
            }
            modify_data(add_position,1,temp);//添加数据
            add_pos.num++;//1
            strcpy(add_pos.from,index_.c_str());//2
            add_pos.from_value=value_;//3
            modify_head(add_position, add_pos);//修改头
            flag=0;
        }
        if(flag)for(int i=1;i<add_pos.num;i++){//插在中间
            tem1=getdata(add_position,i);
            tem2=getdata(add_position,i+1);
            if(temp>tem1 && temp<tem2){
                for(int j=add_pos.num+1;j>=i+1;j--){//后面的依次往后移一格
                    data tem;
                    tem=getdata(add_position,j-1);
                    modify_data(add_position,j,tem);
                }
                modify_data(add_position,i,temp);
                add_pos.num++;
                modify_head(add_position,add_pos);
                flag=0;
                break;
            }
        }
        if(flag){//插于尾部
            modify_data(add_position,add_pos.num+1,temp);
            add_pos.num++;//1
            strcpy(add_pos.to,index_.c_str());//2
            add_pos.to_value=value_;//3 改头三步不能忘！改num改from改value
            modify_head(add_position,add_pos);
        }

        if(add_pos.num==316)devide(add_position);
    }
    void find (std::string index_){
        int i=1;
        while(true){
            head temp;temp=gethead(i);
            std::string s1=temp.from,s2=temp.to;
            if(s1>index_)break;

            if(index_>=s1 && index_<=s2){
                for(int j=1;j<=temp.num;j++){
                    data tem;tem=getdata(i,j);
                    std::string s=tem.index;
                    if(s==index_)std::cout<<tem.value<<' ';
                }
            }
            if(temp.next_head_num!=0)i=temp.next_head_num;
            else break;
        }
        std::cout<<'\n';
    }
};

#endif