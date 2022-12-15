#include <iostream>
#include "book.hpp"
#include "bookstore.hpp"
#include "diary.hpp"
#include "user.hpp"
#include "databaseplus.hpp"
using namespace std;

int main ()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    database database_;
    string index,op;
    int n,value;
    int cnt=0;
    cin>>n;
    for(int i=0;i<n;i++){
        cnt++;
        cin>>op;
        if(op=="insert"){
            cin>>index>>value;
            std::cout<<cnt<<" ******"<<'\n';
            database_.insert(index,value);
            cout<<'\n';
        }
        if(op=="find"){
            cin>>index;
            database_.find(index);
        }
        if(op=="delete"){
            cin>>index>>value;
            database_.Delete(index,value);
        }
    }
    // start st;
    // std::cout<<st.num<<'\n';
}