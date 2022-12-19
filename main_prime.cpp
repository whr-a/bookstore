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
    database<int> database_;
    char index[64];
    string op;
    int n,value;
    int cnt=0;
    cin>>n;
    for(int i=0;i<n;i++){
        cnt++;
        cin>>op;
        if(op=="insert"){
            cin>>index>>value;
            //std::cout<<cnt<<" ******"<<'\n';
            database_.insert(index,value);
            //cout<<'\n';
        }
        if(op=="find"){
            cin>>index;
            database_.find(index);
        }
        if(op=="delete"){
            cin>>index>>value;
            //std::cout<<cnt<<" ******"<<'\n';
            database_.Delete(index,value);
            //cout<<'\n';
        }
    }
    // start st;
    // std::cout<<st.num<<'\n';
}