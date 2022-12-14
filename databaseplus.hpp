#ifndef DATABASEPLUS
#define DATABASEPLUS

#include <iostream>
#include <stdexcept>
#include <string.h>
#include <fstream>
const int size_of_bolck=316;
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
    opfile.seekg(sizeof(start)+(num-1)*(sizeof(head)+size_of_bolck*sizeof(data)));
    head h;
    opfile.read(reinterpret_cast<char *>(&h),sizeof(head));
    return h;
}
static data getdata(int head_num,int data_num){
    std::fstream opfile("database");
    opfile.seekg(sizeof(start)+(head_num-1)*(sizeof(head)+size_of_bolck*sizeof(data))
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
        head h;
        while(true){
            //std::cout<<i<<"**--**"<<'\n';
            h=gethead(i);
            data from;
            strcpy(from.index,h.from);
            from.value=h.from_value;
            if(h.next_head_num==0){//找到最后一个块，则比最后一个大
                add_position=i;
                break;
            }
            if(h.last_head_num==0){
                if(temp<from){
                    add_position=i;
                    break;
                }
            }
            head h_next;h_next=gethead(h.next_head_num);
            data to;
            strcpy(to.index,h_next.from);
            to.value=h_next.from_value;
            if(temp>=from && temp<to){
                add_position=i;
                break;
            }
            if(h.next_head_num!=0)i=h.next_head_num;
            else break;
        }
    }
    return add_position;
}
static void modify_data(int head_num,int data_num,data data_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(head_num-1)*(sizeof(head)+size_of_bolck*sizeof(data))
                    +sizeof(head)+(data_num-1)*sizeof(data));
    opfile.write(reinterpret_cast<char*>(&data_),sizeof(data));
}
static void modify_head(int head_num,head head_){
    std::fstream opfile("database");
    opfile.seekp(sizeof(start)+(head_num-1)*(sizeof(head)+size_of_bolck*sizeof(data)));
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
    data front; front=getdata(head_num,size_of_bolck/2+1);
    data end;   end=getdata(head_num,size_of_bolck);
    strcpy(head_.from,front.index);head_.from_value=front.value;
    strcpy(head_.to,end.index);    head_.to_value=end.value;
    head_.num=size_of_bolck/2;head_.last_head_num=head_num;
    if(this_head.next_head_num)head_.next_head_num=this_head.next_head_num;
    modify_head(st.max_num_of_block,head_);
    //下面的区块为更新本节点
    data end_;end_=getdata(head_num,size_of_bolck/2);
    strcpy(this_head.to,end_.index);
    this_head.to_value=end_.value;
    this_head.next_head_num=st.max_num_of_block;
    this_head.num=size_of_bolck/2;
    modify_head(head_num,this_head);
    //下面的区块为拷贝元素
    data temp;
    for(int i=1;i<=size_of_bolck/2;i++){
        temp=getdata(head_num,size_of_bolck/2+i);
        modify_data(st.max_num_of_block,i,temp);
    }
}
static void merge(int head_num)
{
//先判断后面有没有块，如果有块，则找后面借，如果后面的块的大小大于158，则借一个元素过来，
//如果后面的块的大小恰好为158，则并块，改head的num、区间、前驱后继、start的num
//如果没块，则往前借，同理
    head head_;head_=gethead(head_num);
    if(head_.next_head_num!=0){
        head next_head;next_head=gethead(head_.next_head_num);
        if(next_head.num>size_of_bolck/2){
            //借一个元素过来
            data next_first_data;next_first_data=getdata(head_.next_head_num,1);
            modify_data(head_num,size_of_bolck/2,next_first_data);
            //修改借后的头
            strcpy(head_.to,next_first_data.index);
            head_.to_value=next_first_data.value;
            head_.num++;
            modify_head(head_num,head_);
            //后面的块去头
            data temp;
            for(int i=1;i<next_head.num;i++){
                temp=getdata(head_.next_head_num,i+1);
                modify_data(head_.next_head_num,i,temp);
            }
            //修改后面的头
            temp=getdata(head_.next_head_num,1);
            strcpy(next_head.from,temp.index);
            next_head.from_value=temp.value;
            next_head.num--;
            modify_head(head_.next_head_num,next_head);
            return;
        }
        else{
            //并块 现在的情况是head里有157个元素，后面的有158个元素
            //先移动data
            for(int i=1;i<=size_of_bolck/2;i++){
                data temp;temp=getdata(head_.next_head_num,i);
                modify_data(head_num,size_of_bolck/2-1+i,temp);
            }
            //改这个的头
            head_.num=size_of_bolck-1;
            head_.next_head_num=next_head.next_head_num;
            if(next_head.next_head_num){
                head tem;tem=gethead(next_head.next_head_num);
                tem.last_head_num=head_num;
                modify_head(next_head.next_head_num,tem);
            }
            data temp_prime;temp_prime=getdata(head_num,size_of_bolck-1);
            strcpy(head_.to,temp_prime.index);
            head_.to_value=temp_prime.value;
            modify_head(head_num,head_);
            //千万不能忘了改start
            start st;st=getstart();
            st.num--;
            modify_start(st);
            return;
        }
    }
    else{//后面没有只好往前借
        head last_head;last_head=gethead(head_.last_head_num);
        if(last_head.num>size_of_bolck/2){
            //借一个过来
            data temp;
            for(int i=size_of_bolck/2;i>=2;i--){
                temp=getdata(head_num,i-1);
                modify_data(head_num,i,temp);
            }
            temp=getdata(head_.last_head_num,last_head.num);
            modify_data(head_num,1,temp);
            //改这个头
            head_.num++;
            strcpy(head_.from,temp.index);
            head_.from_value=temp.value;
            modify_head(head_num,head_);
            //改前一个头
            last_head.num--;
            temp=getdata(head_.last_head_num,last_head.num);
            strcpy(last_head.to,temp.index);
            last_head.to_value=temp.value;
            modify_head(head_.last_head_num,last_head);
            return;
        }
        else{
            //向前并块 目前前面块有158个元素，后面块有157个元素
            //先移动元素
            data temp;
            for(int i=1;i<=size_of_bolck/2-1;i++){
                temp=getdata(head_num,i);
                modify_data(head_.last_head_num,size_of_bolck/2+i,temp);
            }
            //改上一个的头
            last_head.num=size_of_bolck-1;
            last_head.next_head_num=0;
            temp=getdata(head_.last_head_num,size_of_bolck-1);
            strcpy(last_head.to,temp.index);
            last_head.to_value=temp.value;
            modify_head(head_.last_head_num,last_head);
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
        //std::cout<<"**"<<temp.index<<' '<<temp.value<<"**"<<'\n';
        start st;st=getstart();
        int add_position=find_add_position(temp,st);
        //std::cout<<"***"<<add_position<<"***"<<'\n';//==========================
        //std::cout<<add_position<<"***\n";
        head add_pos=gethead(add_position);
        data tem1,tem2;
        bool flag=1;//是否成功插入中间
        data from;
        strcpy(from.index,add_pos.from);
        from.value=add_pos.from_value;
        if(temp<from){//要插在头的前面的特殊情况。
            //std::cout<<114514;
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
                //std::cout<<"**"<<index_<<' '<<value_<<"*****666"<<' '<<i<<'\n';
                for(int j=add_pos.num+1;j>=i+2;j--){//后面的依次往后移一格
                    data tem;
                    tem=getdata(add_position,j-1);
                    modify_data(add_position,j,tem);
                }
                modify_data(add_position,i+1,temp);
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
            if(add_pos.num==1){
                strcpy(add_pos.from,index_.c_str());
                add_pos.from_value=value_;
            }
            modify_head(add_position,add_pos);
        }
        if(add_pos.num>=size_of_bolck){
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
        while(true){
            head temp;temp=gethead(i);
            std::string s1=temp.from,s2=temp.to;
            if(s1>index_)break;

            if(index_>=s1 && index_<=s2){
                for(int j=1;j<=temp.num;j++){
                    data tem;tem=getdata(i,j);
                    std::string s=tem.index;
                    if(s==index_){
                        std::cout<<tem.value<<' ';
                        flag=0;
                    }
                }
            }
            if(temp.next_head_num!=0)i=temp.next_head_num;
            else break;
        }
        if(flag)std::cout<<"null";
        std::cout<<'\n';
    }
    void Delete (std::string index_,int value_){
        start st;
        int i=1;
        while(true){
            head temp;temp=gethead(i);
            std::string s1=temp.from,s2=temp.to;
            if(s1>index_)return;
            if(index_>=s1 && index_<=s2){
                for(int j=1;j<=temp.num;j++){
                    data tem;tem=getdata(i,j);
                    std::string s=tem.index;
                    if(s==index_ && tem.value==value_){
                        //把后面的全往前移一格
                        for(int k=j;k<temp.num;k++){
                            data tem_prime;tem_prime=getdata(i,k+1);
                            modify_data(i,k,tem_prime);
                        }
                        //修改头
                        if(temp.num==1){
                            head empty;
                            modify_head(i,empty);
                        }//超级极端的情况
                        if(j==1){//j为1则要改头
                            data tem_prime;tem_prime=getdata(i,1);
                            strcpy(temp.from,tem_prime.index);
                            temp.from_value=tem_prime.value;
                            temp.num--;
                            modify_head(i,temp);
                        }
                        else if(j==temp.num){//尾
                            data tem_prime;tem_prime=getdata(i,temp.num-1);
                            strcpy(temp.to,tem_prime.index);
                            temp.to_value=tem_prime.value;
                            temp.num--;
                            modify_head(i,temp);
                        }
                        else{//中间
                            temp.num--;
                            modify_head(i,temp);//什么都不用改
                        }
                        //先判断是不是只有一个块，只有一个块就返回，否则判断是否小于158，
                        //不小于158则继续，小于则借元素或并块。
                        if(st.num==1 || temp.num>=size_of_bolck/2)return;
                        else{
                            merge(i);
                        }
                    }
                }
            }
            if(temp.next_head_num!=0)i=temp.next_head_num;
            else break;
        }
    }
};

#endif