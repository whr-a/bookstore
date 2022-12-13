#include <iostream>
#include "book.hpp"
#include "bookstore.hpp"
#include "diary.hpp"
#include "user.hpp"
#include "database.hpp"
using namespace std;
int main ()
{
    database database_;
    string index,op;
    int n,value;
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>op;
        if(op=="insert"){
            cin>>index>>value;
            database_.insert(index,value);
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
}