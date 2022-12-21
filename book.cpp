#include "book.hpp"
#include <iomanip>
void print(book_inf &x){
    std::cout<<x.ISBN<<'\t'
             <<x.bookname<<'\t'
             <<x.author<<'\t'
             <<x.keyword<<'\t';
    std::cout<<setiosflags(std::ios::fixed)<<std::setprecision(2)<<x.price;
    std::cout<<'\t'<<x.store<<std::endl;
}

void book::login(){
    std::string temp="";
    book_stack.push_back(temp);
}
void book::logout(){
    book_stack.pop_back();
}
void book::show(Tokenscanner &scanner,user &users){
    if(users.login_stack.empty())throw(error("Invalid"));
    bool flag=0;
    if(!scanner.haveMoreTokens()){
        int i=1;
        node<book_inf> h;
        while(true){
            database<book_inf>::getnode(h,i);
            for(int j=0;j<h.head_.num;j++){
                flag=1;
                print(h.data_[j].value);
            }
            if(h.head_.next_head_num!=0)i=h.head_.next_head_num;
            else break;
        }
    }
    else{
        std::string op=scanner.nextToken_separate();
        if(op=="")throw(error("Invalid"));
        if(op=="ISBN"){
            std::string ISBN_=scanner.nextToken();
            if(!scanner.check(ISBN_,20,1))throw(error("Invalid"));
            if(scanner.haveMoreTokens())throw(error("Invalid"));
            char index[20];strcpy(index,ISBN_.c_str());
            std::pair<book_inf,bool> result=books.find(index);
            if(result.second){
                print(result.first);
                flag=1;
            }
        }
        else if(op=="name"){
            std::string name_=scanner.nextToken_separate_plus();
            if(!scanner.check(name_,60,3))throw(error("Invalid"));
            if(scanner.haveMoreTokens())throw(error("Invalid"));
            int i=1;
            node<book_inf> h;
            while(true){
                database<book_inf>::getnode(h,i);
                for(int j=0;j<h.head_.num;j++){
                    if(book::check(name_,h.data_[j].value,0)){
                        print(h.data_[j].value);
                        flag=1;
                    }
                }
                if(h.head_.next_head_num!=0)i=h.head_.next_head_num;
                else break;
            }
        }
        else if(op=="author"){
            std::string name_=scanner.nextToken_separate_plus();
            if(!scanner.check(name_,60,3))throw(error("Invalid"));
            if(scanner.haveMoreTokens())throw(error("Invalid"));
            int i=1;
            node<book_inf> h;
            while(true){
                database<book_inf>::getnode(h,i);
                for(int j=0;j<h.head_.num;j++){
                    if(book::check(name_,h.data_[j].value,1)){
                        print(h.data_[j].value);
                        flag=1;
                    }
                }
                if(h.head_.next_head_num!=0)i=h.head_.next_head_num;
                else break;
            }
        }
        else if(op=="keyword"){
            std::string name_=scanner.nextToken_separate_plus();
            if(!scanner.check(name_,60,3))throw(error("Invalid"));
            for(int i=0;i<name_.size();i++)if(name_[i]=='|')throw(error("Invalid"));
            if(scanner.haveMoreTokens())throw(error("Invalid"));
            int i=1;
            node<book_inf> h;
            while(true){
                database<book_inf>::getnode(h,i);
                for(int j=0;j<h.head_.num;j++){
                    if(book::check(name_,h.data_[j].value,2)){
                        print(h.data_[j].value);
                        flag=1;
                    }
                }
                if(h.head_.next_head_num!=0)i=h.head_.next_head_num;
                else break;
            }
        }
        else throw(error("Invalid"));
        if(!flag)std::cout<<std::endl;
    }
}
void book::buy(Tokenscanner &scanner,user &users){
    //检查正确性
    if(users.login_stack.empty())throw(error("Invalid"));
    std::string ISBN_;
    ISBN_=scanner.nextToken();
    if(!scanner.check(ISBN_,20,1))throw(error("Invalid"));
    std::string num_=scanner.nextToken();
    int num=scanner.check_num(num_);
    if(num==-1)throw(error("Invalid"));
    if(scanner.haveMoreTokens())throw(error("Invalid"));
    char index[20];strcpy(index,ISBN_.c_str());
    std::pair<book_inf,bool> result=books.find(index);
    if(!result.second)throw(error("Invalid"));
    book_inf &t=result.first;
    if(t.store<num)throw(error("Invalid"));
    //操作
    double ans=t.price*num;
    std::cout<<ans<<'\n';
    t.store-=num;
    books.modify(index,t);
}

void book::select(Tokenscanner &scanner,user &users){
    if(users.login_stack.empty())throw(error("Invalid"));
    if(users.login_stack.back().privilege<3)throw(error("Invalid"));
    std::string ISBN_;
    ISBN_=scanner.nextToken();
    if(!scanner.check(ISBN_,20,1))throw(error("Invalid"));
    if(scanner.haveMoreTokens())throw(error("Invalid"));
    char index[20];strcpy(index,ISBN_.c_str());
    std::pair<book_inf,bool> result=books.find(index);
    if(!result.second){
        book_inf temp;strcpy(temp.ISBN,index);
        books.insert(index,temp);
        book_stack.pop_back();
        book_stack.push_back(ISBN_);
    }
    else{
        book_stack.pop_back();
        book_stack.push_back(ISBN_);
    }
}

void book::modify(Tokenscanner &scanner,user &users){
    if(users.login_stack.empty())throw(error("Invalid"));
    if(users.login_stack.back().privilege<3)throw(error("Invalid"));
    if(!scanner.haveMoreTokens())throw(error("Invalid"));
    if(book_stack.back()=="")throw(error("Invalid"));
    char index[20];strcpy(index,book_stack.back().c_str());
    std::pair<book_inf,bool> result=books.find(index);
    book_inf tmp=result.first;
    book_inf &t=result.first;
    bool vis[5];
    memset(vis,0,sizeof(vis));
    while(scanner.haveMoreTokens()){
        std::string s=scanner.nextToken_separate();
        if(s=="ISBN"){
            std::string ISBN=scanner.nextToken();
            if(!scanner.check(ISBN,20,1))throw(error("Invalid"));
            if(ISBN==book_stack.back())throw(error("Invalid"));
            if(vis[0])throw(error("Invalid"));
            strcpy(t.ISBN,ISBN.c_str());
            vis[0]=1;
        }
        else if(s=="name"){
            std::string name=scanner.nextToken_separate_plus();
            if(!scanner.check(name,60,3))throw(error("Invalid"));
            if(vis[1])throw(error("Invalid"));
            strcpy(t.bookname,name.c_str());
            vis[1]=1;
        }
        else if(s=="author"){
            std::string author=scanner.nextToken_separate_plus();
            if(!scanner.check(author,60,3))throw(error("Invalid"));
            if(vis[2])throw(error("Invalid"));
            strcpy(t.author,author.c_str());
            vis[2]=1;
        }
        else if(s=="keyword"){
            std::string keyword=scanner.nextToken_separate_plus();
            if(!scanner.check(keyword,60,3))throw(error("Invalid"));
            if(vis[3])throw(error("Invalid"));
            if(!scanner.check_keyword(keyword))throw(error("Invalid"));
            strcpy(t.keyword,keyword.c_str());
            vis[3]=1;
        }
        else if(s=="price"){
            std::string price_=scanner.nextToken();
            double price=scanner.check_double(price_);
            if(price==-1.00)throw(error("Invalid"));
            if(vis[4])throw(error("Invalid"));
            t.price=price;
            //std::cout<<"**"<<t.price<<"**";
            vis[4]=1;
        }
        else throw(error("Invalid"));
    }
    if(vis[0]==0){
        books.modify(index,t);
    }
    else{
        books.Delete(index,tmp);
        books.insert(t.ISBN,t);
        std::string tep=index;
        int x=find(book_stack.begin(),book_stack.end(),tep)-book_stack.begin();
        while(true){
            if(x==book_stack.size())break;
            book_stack[x]=t.ISBN;
            x=find(book_stack.begin()+x+1,book_stack.end(),tep)-book_stack.begin();
        }
    }
}

void book::import(Tokenscanner &scanner,user &users){
    if(users.login_stack.empty())throw(error("Invalid"));
    if(users.login_stack.back().privilege<3)throw(error("Invalid"));
    if(book_stack.back()=="")throw(error("Invalid"));
    std::string quantity_=scanner.nextToken();
    int quantity=scanner.check_num(quantity_);
    if(quantity==-1)throw(error("Invalid"));
    std::string totalcost_=scanner.nextToken();
    double totalcost=scanner.check_double(totalcost_);
    if(totalcost<=0)throw(error("Invalid"));
    char index[20];strcpy(index,book_stack.back().c_str());
    std::pair<book_inf,bool> result=books.find(index);
    result.first.store+=quantity;
    books.modify(index,result.first);
}