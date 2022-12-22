#ifndef DATABASE
#define DATABASE

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <fstream>
#include <vector>
const int size_of_block_=316;

class Start {
public:
    int num;
    int max_num_of_block;
    Start (){
        num=1;
        max_num_of_block=1;
    }
};
template <class T>
class Head {
public:
    int from;
    T from_value;
    int to;
    T to_value;
    int num=0;
    int next_Head_num=0;
    int last_Head_num=0;
    Head(){
        from=0;
        to=0;
        last_Head_num=0;
        next_Head_num=0;
    }
};
template <class T>
class Data {
public:
    int index;
    T value;
    friend bool operator <(const Data<T> &thi,const Data<T> &other){
        if(thi.index<other.index)return true;
        else if(thi.index==other.index && thi.value<other.value)return true;
        else return false;
    }
    Data(){
        index=0;
    }
    Data(int s,T &value_){
        index=s;
        value=value_;
    }
    friend bool operator >(const Data<T> &thi,const Data &other){
        if(thi.index>other.index)return true;
        else if(thi.index==other.index && thi.value>other.value)return true;
        else return false;
    }
    friend bool operator >=(const Data<T> &thi,const Data &other){return !(thi<other);}
    friend bool operator <=(const Data<T> &thi,const Data &other){return !(thi>other);}
    friend bool operator ==(const Data<T> &thi,const Data &other){return (!(thi>other) && !(thi<other));}
};
template <class T>
class Node{
public:
    Head<T> Head_;
    Data<T> Data_[size_of_block_];
};
// static inline void print()//调试专用
// {
//     int i=1;
//     while(true){
//         Head Head_;Head_=getHead(i);
//         std::cout<<"  "<<i<<"---------------\n";
//         std::cout<<"  "<<Head_.from_value<<' '<<Head_.to_value<<' '<<Head_.num<<'\n';
//         for(int j=1;j<=Head_.num;j++){
//             Data Data_;Data_=getData(i,j);
//             std::cout<<"  "<<Data_.index<<' '<<Data_.value<<'\n';
//         }
//         if(Head_.next_Head_num!=0)i=Head_.next_Head_num;
//         else break;
//     }
//     Start st;st=getStart();
//     std::cout<<"st.num:"<<st.num;
// }
template <class T>
static bool cmpp(Data<T> a,Data<T> b){
    return a.index<b.index;
}
template <class T>
class Database_
{
public:
    static std::fstream opfile;
    Database_(){}
    Database_(std::string name){
        std::ifstream in;
        in.open(name);
        if(!in){
            std::ofstream outfile(name);
            outfile.seekp(0);
            Start st;
            outfile.write(reinterpret_cast<char*>(&st),sizeof(Start));
            outfile.seekp(sizeof(Start));
            Node<T> h;
            outfile.write(reinterpret_cast<char*>(&h),sizeof(Node<T>));
        }
        opfile.open(name);
    }
    bool setfile(std::string name){
        std::ifstream in;
        in.open(name);
        bool flag=1;
        if(!in){
            flag=0;
            std::ofstream outfile(name);
            outfile.seekp(0);
            Start st;
            outfile.write(reinterpret_cast<char*>(&st),sizeof(Start));
            outfile.seekp(sizeof(Start));
            Node<T> h;
            outfile.write(reinterpret_cast<char*>(&h),sizeof(Node<T>));
        }
        opfile.open(name);
        return flag;
    }

    ~Database_(){
        opfile.close();
    }
    static inline void getNode(Node<T>& temp,int &num){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekg(sizeof(Start)+(num-1)*(sizeof(Head<T>)+size_of_block_*sizeof(Data<T>)));
        Database_<T>::opfile.read(reinterpret_cast<char*>(&temp),sizeof(Node<T>));
        return;
    }
    static inline void modify_Node(int &num,Node<T> &Node_){
        // std::cout<<114514;
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekp(sizeof(Start)+(num-1)*(sizeof(Head<T>)+size_of_block_*sizeof(Data<T>)));
        Database_<T>::opfile.write(reinterpret_cast<char*>(&Node_),sizeof(Node<T>));
    }
    static inline Head<T> getHead(int &num){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekg(sizeof(Start)+(num-1)*(sizeof(Head<T>)+size_of_block_*sizeof(Data<T>)));
        Head<T> h;
        Database_<T>::opfile.read(reinterpret_cast<char *>(&h),sizeof(Head<T>));
        return h;
    }
    static inline Data<T> getData(int &Head_num,int &Data_num){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekg(sizeof(Start)+(Head_num-1)*(sizeof(Head<T>)+size_of_block_*sizeof(Data<T>))
                        +sizeof(Head<T>)+(Data_num-1)*sizeof(Data<T>));
        Data<T> d;
        Database_<T>::opfile.read(reinterpret_cast<char *>(&d),sizeof(Data<T>));
        return d;
    }
    static inline Start getStart(){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekg(0);
        Start st;
        Database_<T>::opfile.read(reinterpret_cast<char *>(&st),sizeof(Start));
        return st;
    }
    static inline void modify_Data(int &Head_num,int &Data_num,Data<T> &Data_){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekp(sizeof(Start)+(Head_num-1)*(sizeof(Head<T>)+size_of_block_*sizeof(Data<T>))
                        +sizeof(Head<T>)+(Data_num-1)*sizeof(Data<T>));
        Database_<T>::opfile.write(reinterpret_cast<char*>(&Data_),sizeof(Data<T>));
    }
    static inline void modify_Head(int &Head_num,Head<T> &Head_){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekp(sizeof(Start)+(Head_num-1)*(sizeof(Head<T>)+size_of_block_*sizeof(Data<T>)));
        Database_<T>::opfile.write(reinterpret_cast<char*>(&Head_),sizeof(Head<T>));
    }
    static inline void modify_Start(Start &st){
        //std::fstream opfile("Database_");
        Database_<T>::opfile.seekp(0);
        Database_<T>::opfile.write(reinterpret_cast<char*>(&st),sizeof(Start));
    }
    static inline int find_add_position(Data<T> &temp,Start &st){
        int add_position;
        if(st.num==1)add_position=1;
        else{
            int i=1;
            Node<T> h;
            while(true){
                //std::cout<<i<<"**--**"<<'\n';
                getNode(h,i);
                Data<T> from;
                from.index=h.Head_.from;
                from.value=h.Head_.from_value;
                if(h.Head_.next_Head_num==0){//找到最后一个块，则比最后一个大
                    add_position=i;
                    break;
                }
                if(h.Head_.last_Head_num==0){
                    if(temp<from){
                        add_position=i;
                        break;
                    }
                }
                Head<T> h_next;h_next=getHead(h.Head_.next_Head_num);
                Data<T> to;
                to.index=h_next.from;
                to.value=h_next.from_value;
                if(temp>=from && temp<to){
                    add_position=i;
                    break;
                }
                if(h.Head_.next_Head_num!=0)i=h.Head_.next_Head_num;
                else break;
            }
        }
        return add_position;
    }
    static inline void devide(int &Head_num)
    {
    //裂开一个块，首先到文件最后开一个块，然后把159——316号元素存到这个块里，更新两块的区间
    //判断这个块，如果没有后继，则将后继设为这个新块（设置新块的前驱）
    //若有，则更新本块的后继、新块的前驱后继、后继块的前驱
        Node<T> this_Head;getNode(this_Head,Head_num);
        Start st=getStart(); st.num++;st.max_num_of_block++;
        modify_Start(st);
        //下面的区块为建造一个新Head_(Node)
        Node<T> Head_;
        Data<T> front=this_Head.Data_[size_of_block_/2+1-1];
        Data<T> end=this_Head.Data_[size_of_block_-1];
        Head_.Head_.from=front.index;    Head_.Head_.from_value=front.value;
        Head_.Head_.to=end.index;    Head_.Head_.to_value=end.value;
        Head_.Head_.num=size_of_block_/2;  Head_.Head_.last_Head_num=Head_num;
        if(this_Head.Head_.next_Head_num)Head_.Head_.next_Head_num=this_Head.Head_.next_Head_num;
        //modify_Head(st.max_num_of_block,Head_);
        //下面的区块为更新本节点
        Data<T> end_=this_Head.Data_[size_of_block_/2-1];
        this_Head.Head_.to=end_.index;
        this_Head.Head_.to_value=end_.value;
        this_Head.Head_.next_Head_num=st.max_num_of_block;
        this_Head.Head_.num=size_of_block_/2;
        modify_Node(Head_num,this_Head);
        //下面的区块为拷贝元素
        for(int i=1;i<=size_of_block_/2;i++){
            Head_.Data_[i-1]=this_Head.Data_[size_of_block_/2+i-1];
        }
        modify_Node(st.max_num_of_block,Head_);
    }
    inline T find (){
        Node<T> temp;
        int i=1;
        getNode(temp,i);
        return temp.Data_[0].value;
    }
    inline void insert(int index_,T &value_){
        Data<T> temp(index_,value_);//完成temp节点的构造
        Start st;st=getStart();
        int add_position=find_add_position(temp,st);
        Node<T> add_pos;getNode(add_pos,add_position);
        bool flag=1;//是否成功插入中间
        Data<T> from;
        from.index=add_pos.Head_.from;
        from.value=add_pos.Head_.from_value;
        if(temp<from){//要插在头的前面的特殊情况。
            for(int j=add_pos.Head_.num;j>=1;j--){//后面的依次往后移一格
                add_pos.Data_[j]=add_pos.Data_[j-1];
            }
            add_pos.Data_[0]=temp;//添加数据
            add_pos.Head_.num++;//1
            add_pos.Head_.from=index_;//2
            add_pos.Head_.from_value=value_;//3
            modify_Node(add_position,add_pos);//修改头
            flag=0;
        }
        if(flag)for(int i=1;i<add_pos.Head_.num;i++){//插在中间
            if(temp>add_pos.Data_[i-1] && temp<add_pos.Data_[i]){
                //std::cout<<"**"<<index_<<' '<<value_<<"*****666"<<' '<<i<<'\n';
                for(int j=add_pos.Head_.num;j>=i+1;j--){//后面的依次往后移一格
                    add_pos.Data_[j]=add_pos.Data_[j-1];
                }
                add_pos.Data_[i]=temp;
                add_pos.Head_.num++;
                modify_Node(add_position,add_pos);
                flag=0;
                break;
            }
        }
        if(flag){//插于尾部
            add_pos.Data_[add_pos.Head_.num]=temp;
            add_pos.Head_.num++;//1
            add_pos.Head_.to=index_;//2
            add_pos.Head_.to_value=value_;//3 改头三步不能忘！改num改from改value
            if(add_pos.Head_.num==1){
                add_pos.Head_.from=index_;
                add_pos.Head_.from_value=value_;
            }
            modify_Node(add_position,add_pos);
        }
        if(add_pos.Head_.num>=size_of_block_){
            devide(add_position);
        }
    }
    inline std::vector<T> search(int index){
        std::vector<T> ans;
        int i=1;
        bool flag=0;
        Node<T> temp;
        while(true){
            getNode(temp,i);
            if(flag){
                for(int k=0;k<temp.Head_.num;k++)ans.push_back(temp.Data_[k].value);
            }
            else if(index>=temp.Head_.from && index<=temp.Head_.to){
                Data<T> t;t.index=index;
                int x=std::lower_bound(temp.Data_,temp.Data_+temp.Head_.num,t,cmpp<T>)-temp.Data_;
                if(t.index==temp.Data_[x].index){
                    flag=1;
                    for(int k=x;k<temp.Head_.num;k++)ans.push_back(temp.Data_[k].value);
                }
            }
            if(temp.Head_.next_Head_num!=0)i=temp.Head_.next_Head_num;
            else break;
        }
        return ans;
    }
    inline void add_one(){
        Node<T> temp;
        int i=1;
        getNode(temp,i);
        temp.Data_[0].value++;
        modify_Node(i,temp);
    }
};
template <class T>
std::fstream Database_<T>::opfile;

#endif