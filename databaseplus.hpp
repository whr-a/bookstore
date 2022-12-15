#ifndef DATABASEPLUS
#define DATABASEPLUS

#include <iostream>
#include <stdexcept>
#include <string.h>
#include <fstream>
const int size_of_block=316;
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
class node{
public:
    head head_;
    data data_[size_of_block];
};
static node getnode(int num){
    std::fstream opfile("database");
    opfile.seekg(sizeof(start)+(num-1)*(sizeof(head)+size_of_block*sizeof(data)));
    node temp;
    opfile.read(reinterpret_cast<char*>(&temp),sizeof(node));
    return temp;
}
static void modify_node(int num,node node_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(num-1)*(sizeof(head)+size_of_block*sizeof(data)));
    opfile.write(reinterpret_cast<char*>(&node_),sizeof(node));
}
static head gethead(int num){
    std::fstream opfile("database");
    opfile.seekg(sizeof(start)+(num-1)*(sizeof(head)+size_of_block*sizeof(data)));
    head h;
    opfile.read(reinterpret_cast<char *>(&h),sizeof(head));
    return h;
}
static data getdata(int head_num,int data_num){
    std::fstream opfile("database");
    opfile.seekg(sizeof(start)+(head_num-1)*(sizeof(head)+size_of_block*sizeof(data))
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
        // head hh;hh=gethead(1);
        // std::cout<<hh.from_value<<"-=-=-=-"<<'\n';
        // head hhh;hhh=gethead(2);
        // std::cout<<hhh.from_value<<"-=-=-="<<'\n';
        int i=1;
        node h;
        while(true){
            //std::cout<<i<<"**--**"<<'\n';
            h=getnode(i);
            data from;
            strcpy(from.index,h.head_.from);
            from.value=h.head_.from_value;
            if(h.head_.next_head_num==0){//找到最后一个块，则比最后一个大
                add_position=i;
                break;
            }
            if(h.head_.last_head_num==0){
                if(temp<from){
                    add_position=i;
                    break;
                }
            }
            head h_next;h_next=gethead(h.head_.next_head_num);
            data to;
            strcpy(to.index,h_next.from);
            to.value=h_next.from_value;
            if(temp>=from && temp<to){
                add_position=i;
                break;
            }
            if(h.head_.next_head_num!=0)i=h.head_.next_head_num;
            else break;
        }
    }
    return add_position;
}
static void modify_data(int head_num,int data_num,data data_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(head_num-1)*(sizeof(head)+size_of_block*sizeof(data))
                    +sizeof(head)+(data_num-1)*sizeof(data));
    opfile.write(reinterpret_cast<char*>(&data_),sizeof(data));
}
static void modify_head(int head_num,head head_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(head_num-1)*(sizeof(head)+size_of_block*sizeof(data)));
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
    node this_head=getnode(head_num);
    start st=getstart(); st.num++;st.max_num_of_block++;
    modify_start(st);
    //下面的区块为建造一个新head_(node)
    node head_;
    data front=this_head.data_[size_of_block/2+1-1];
    data end=this_head.data_[size_of_block-1];
    strcpy(head_.head_.from,front.index);head_.head_.from_value=front.value;
    strcpy(head_.head_.to,end.index);    head_.head_.to_value=end.value;
    head_.head_.num=size_of_block/2;head_.head_.last_head_num=head_num;
    if(this_head.head_.next_head_num)head_.head_.next_head_num=this_head.head_.next_head_num;
    //modify_head(st.max_num_of_block,head_);
    //下面的区块为更新本节点
    data end_=this_head.data_[size_of_block/2-1];
    strcpy(this_head.head_.to,end_.index);
    this_head.head_.to_value=end_.value;
    this_head.head_.next_head_num=st.max_num_of_block;
    this_head.head_.num=size_of_block/2;
    modify_node(head_num,this_head);
    //下面的区块为拷贝元素
    for(int i=1;i<=size_of_block/2;i++){
        head_.data_[i-1]=this_head.data_[size_of_block/2+i-1];
    }
    modify_node(st.max_num_of_block,head_);
}
static void merge(int head_num)
{
//先判断后面有没有块，如果有块，则找后面借，如果后面的块的大小大于158，则借一个元素过来，
//如果后面的块的大小恰好为158，则并块，改head的num、区间、前驱后继、start的num
//如果没块，则往前借，同理
    node head_=getnode(head_num);
    if(head_.head_.next_head_num!=0){
        node next_head=getnode(head_.head_.next_head_num);
        if(next_head.head_.num>size_of_block/2){
            //借一个元素过来
            data next_first_data=next_head.data_[0];
            head_.data_[size_of_block/2-1]=next_first_data;
            //修改借后的头
            strcpy(head_.head_.to,next_first_data.index);
            head_.head_.to_value=next_first_data.value;
            head_.head_.num++;
            modify_node(head_num,head_);
            //后面的块去头
            for(int i=0;i<next_head.head_.num-1;i++){
                next_head.data_[i]=next_head.data_[i+1];
            }
            //修改后面的头
            data temp=next_head.data_[0];
            strcpy(next_head.head_.from,temp.index);
            next_head.head_.from_value=temp.value;
            next_head.head_.num--;
            modify_node(head_.head_.next_head_num,next_head);
            return;
        }
        else{
            //并块 现在的情况是head里有157个元素，后面的有158个元素
            //先移动data
            for(int i=0;i<size_of_block/2;i++){
                head_.data_[size_of_block/2-1+i]=next_head.data_[i];
            }
            //改这个的头
            head_.head_.num=size_of_block-1;
            head_.head_.next_head_num=next_head.head_.next_head_num;
            if(next_head.head_.next_head_num){
                head tem;tem=gethead(next_head.head_.next_head_num);
                tem.last_head_num=head_num;
                modify_head(next_head.head_.next_head_num,tem);
            }
            data temp_prime=head_.data_[size_of_block-2];
            strcpy(head_.head_.to,temp_prime.index);
            head_.head_.to_value=temp_prime.value;
            modify_node(head_num,head_);
            //千万不能忘了改start
            start st;st=getstart();
            st.num--;
            modify_start(st);
            return;
        }
    }
    else{//后面没有只好往前借
        node last_head=getnode(head_.head_.last_head_num);
        if(last_head.head_.num>size_of_block/2){
            //借一个过来
            data temp;
            for(int i=size_of_block/2-1;i>=1;i--){
                head_.data_[i]=head_.data_[i-1];
            }
            head_.data_[0]=last_head.data_[last_head.head_.num-1];
            //改这个头
            head_.head_.num++;
            strcpy(head_.head_.from,head_.data_[0].index);
            head_.head_.from_value=head_.data_[0].value;
            modify_node(head_num,head_);
            //改前一个头
            last_head.head_.num--;
            temp=last_head.data_[last_head.head_.num-1];
            strcpy(last_head.head_.to,temp.index);
            last_head.head_.to_value=temp.value;
            modify_node(head_.head_.last_head_num,last_head);
            return;
        }
        else{
            //向前并块 目前前面块有158个元素，后面块有157个元素
            //先移动元素
            data temp;
            for(int i=0;i<size_of_block/2-1;i++){
                last_head.data_[size_of_block/2+i]=head_.data_[i];
            }
            //改上一个的头
            last_head.head_.num=size_of_block-1;
            last_head.head_.next_head_num=0;
            temp=last_head.data_[size_of_block-2];
            strcpy(last_head.head_.to,temp.index);
            last_head.head_.to_value=temp.value;
            modify_node(head_.head_.last_head_num,last_head);
            start st;st=getstart();
            st.num--;
            modify_start(st);
            return;
        }
    }
}
static void print()//调试专用
{
    int i=1;
    while(true){
        head head_;head_=gethead(i);
        std::cout<<"  "<<i<<"---------------\n";
        std::cout<<"  "<<head_.from_value<<' '<<head_.to_value<<'\n';
        for(int j=1;j<=head_.num;j++){
            data data_;data_=getdata(i,j);
            std::cout<<"  "<<data_.index<<' '<<data_.value<<'\n';
        }
        if(head_.next_head_num!=0)i=head_.next_head_num;
        else break;
    }
}

class database
{
public:
    database(){
        std::ifstream in;
        in.open("database");
        if(!in){
            std::ofstream outfile("database");
            outfile.seekp(0);
            start st;
            outfile.write(reinterpret_cast<char*>(&st),sizeof(start));
            outfile.seekp(sizeof(start));
            head h;
            outfile.write(reinterpret_cast<char*>(&h),sizeof(head));
        }
    }
    void insert(std::string index_,int value_){
        data temp(index_,value_);//完成temp节点的构造
        //std::cout<<"**"<<temp.index<<' '<<temp.value<<"**"<<'\n';
        start st;st=getstart();
        int add_position=find_add_position(temp,st);
        //std::cout<<"***"<<add_position<<"***"<<'\n';//==========================
        //std::cout<<add_position<<"***\n";
        node add_pos=getnode(add_position);
        data tem1,tem2;
        bool flag=1;//是否成功插入中间
        data from;
        strcpy(from.index,add_pos.head_.from);
        from.value=add_pos.head_.from_value;
        if(temp<from){//要插在头的前面的特殊情况。
            //std::cout<<114514;
            for(int j=add_pos.head_.num;j>=1;j--){//后面的依次往后移一格
                add_pos.data_[j]=add_pos.data_[j-1];
            }
            add_pos.data_[0]=temp;//添加数据
            add_pos.head_.num++;//1
            strcpy(add_pos.head_.from,index_.c_str());//2
            add_pos.head_.from_value=value_;//3
            modify_node(add_position,add_pos);//修改头
            flag=0;
        }
        if(flag)for(int i=1;i<add_pos.head_.num;i++){//插在中间
            tem1=add_pos.data_[i-1];
            tem2=add_pos.data_[i];
            if(temp>tem1 && temp<tem2){
                //std::cout<<"**"<<index_<<' '<<value_<<"*****666"<<' '<<i<<'\n';
                for(int j=add_pos.head_.num;j>=i+1;j--){//后面的依次往后移一格
                    add_pos.data_[j]=add_pos.data_[j-1];
                }
                add_pos.data_[i]=temp;
                add_pos.head_.num++;
                modify_node(add_position,add_pos);
                flag=0;
                break;
            }
        }
        if(flag){//插于尾部
            add_pos.data_[add_pos.head_.num]=temp;
            add_pos.head_.num++;//1
            strcpy(add_pos.head_.to,index_.c_str());//2
            add_pos.head_.to_value=value_;//3 改头三步不能忘！改num改from改value
            if(add_pos.head_.num==1){
                strcpy(add_pos.head_.from,index_.c_str());
                add_pos.head_.from_value=value_;
            }
            modify_node(add_position,add_pos);
        }
        if(add_pos.head_.num>=size_of_block){
            devide(add_position);
            //std::cout<<114514;
        }
        //print();
        // // start stt;stt=getstart();
        // // std::cout<<"**"<<stt.num<<' '<<stt.max_num_of_block<<"**"<<'\n';
        // head temmp;temmp=gethead(add_position);
        // // std::cout<<"==="<<temmp.num<<"==="<<'\n';
        // std::cout<<"ooooo"<<add_position<<"ooooo"<<'\n';
        // for(int i=1;i<=temmp.num;i++){
        //     data temm;temm=getdata(add_position,i);
        //     std::cout<<temm.index<<' '<<temm.value<<'\n';
        // }//***********************
    }
    void find (std::string index_){
        int i=1;
        bool flag=1;
        node temp;
        while(true){
            temp=getnode(i);
            std::string s1=temp.head_.from,s2=temp.head_.to;
            if(s1>index_)break;

            if(index_>=s1 && index_<=s2){
                for(int j=0;j<temp.head_.num;j++){
                    if(temp.data_[j].index==index_){
                        std::cout<<temp.data_[j].value<<' ';
                        flag=0;
                    }
                }
            }
            if(temp.head_.next_head_num!=0)i=temp.head_.next_head_num;
            else break;
        }
        if(flag)std::cout<<"null";
        std::cout<<'\n';
    }
    void Delete (std::string index_,int value_){
        start st;
        int i=1;
        while(true){
            node temp;temp=getnode(i);
            std::string s1=temp.head_.from,s2=temp.head_.to;
            if(s1>index_)return;
            if(index_>=s1 && index_<=s2){
                for(int j=0;j<temp.head_.num;j++){
                    data tem=temp.data_[j];
                    std::string s=tem.index;
                    if(s==index_ && tem.value==value_){
                        //把后面的全往前移一格
                        for(int k=j;k<temp.head_.num-1;k++){
                            temp.data_[k]=temp.data_[k+1];
                        }
                        //修改头
                        if(temp.head_.num==1){
                            head empty;
                            temp.head_=empty;
                        }//超级极端的情况
                        if(j==0){//j为1则要改头
                            data tem_prime=temp.data_[0];
                            strcpy(temp.head_.from,tem_prime.index);
                            temp.head_.from_value=tem_prime.value;
                            temp.head_.num--;
                            modify_node(i,temp);
                        }
                        else if(j==temp.head_.num-1){//尾
                            data tem_prime=temp.data_[j];
                            strcpy(temp.head_.to,tem_prime.index);
                            temp.head_.to_value=tem_prime.value;
                            temp.head_.num--;
                            modify_node(i,temp);
                        }
                        else{//中间
                            temp.head_.num--;
                            modify_node(i,temp);//什么都不用改
                        }
                        //先判断是不是只有一个块，只有一个块就返回，否则判断是否小于158，
                        //不小于158则继续，小于则借元素或并块。
                        if(st.num==1 || temp.head_.num>=size_of_block/2)return;
                        else{
                            merge(i);
                        }
                    }
                }
            }
            if(temp.head_.next_head_num!=0)i=temp.head_.next_head_num;
            else break;
        }
    }
};

#endif