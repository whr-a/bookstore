# Bookstore **开发文档**
written by 王浩然

## 程序功能概述
本次开发的主要目标是实现一个模拟的书店管理系统。实现功能如下：
### 一：利用命令行进行交互
使用命令行（Windows CMD 或 Linux Terminal）进行操作，输入数据以换行符或回车符为分隔为若干指令。

指令合法字符集为标准 ASCII 字符；允许的空白符仅为空格，单个指令被空格分割为多个部分。多个连续空格效果与一个空格等价；行首行末允许出现多余空格；如无特殊说明禁止省略或另增空格。

指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配。

本文档中以弱化的正则语法说明指令文法。合法指令的文法相关说明如下：

- `[x]` 表示一串有特定限制的用户自定义字符串；
- `(a|b|c)` 表示此处仅能出现 `a`, `b`, `c` 中其一；
- `(x)?` 表示此处可以出现零次或一次 `x`；
- `(x)+` 表示此处可以出现一次或多次 `x`。

合法的指令如下所示：（`#` 开头的行是注释）

```
# 基础指令
quit
exit

# 帐户系统指令
su [UserID] ([Password])?
logout
register [UserID] [Password] [Username]
passwd [UserID] ([CurrentPassword])? [NewPassword]
useradd [UserID] [Password] [Privilege] [Username]
delete [UserID]

# 图书系统指令
show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
buy [ISBN] [Quantity]
select [ISBN]
modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
import [Quantity] [TotalCost]

# 日志系统指令
show finance ([Count])?
log
```
在用户输入一条指令后，如果合法则执行对应操作，如果有则输出操作结果；如果指令非法或操作失败则输出 `Invalid\n`。仅有空格的指令合法且无输出内容。

除非有特殊说明，如果输入指令对应的输出内容非空，则结尾有 `\n` 字符；输出内容为空则不输出任何字符。

`quit` 和 `exit` 指令功能为正常退出系统。

### 二：维护一个账户系统，实现操作的多态性
书店管理系统服务于 **店主（超级管理员）**，**员工（管理员）以及顾客（用户）**。为满足其不同需求，故需帐户系统管理帐户及权限数据，并提供相应功能。

### 各权限可访问功能

- **店主 {7}**：使用 `root` 帐户，可以访问书店系统所有功能，包括日志和帐户管理功能；
- **员工 {3}**：可以访问修改图书数据相关功能等；
- **顾客 {1}**：可以注册帐户，查询、购买图书；
- **游客 {0}**：可以注册帐户。

### 登录栈

系统支持嵌套登录，即允许多个帐户同时处于登录状态；允许同一帐户同时多次登录。输入的指令视为最后登录的帐户操作。退出系统视为登出所有已登录帐户。

### 帐户信息

关于指令格式中的用户自定义字符串限定如下，其中「最大长度」含义为该字符串所占存储空间不能超过指定数目的英文字母字符所占存储空间。

- `[UserID]`, `[Password]`, `[CurrentPassword]`, `[NewPassword]`
  - 合法字符集：数字，字母，下划线；
  - 最大长度：30。
- `[Username]`
  - 合法字符集：除不可见字符以外 ASCII 字符；
  - 最大长度：30。
- `[Privilege]`
  - 合法字符集：数字；
  - 最大长度：1。


出现不符合上述限定的自定义字符串的指令视为非法，下同。

### 帐户相关指令

本文档中指令说明格式如下：

- **指令名称**
  - {指令权限}  `[指令格式]`
  - 指令功能
  - 指令逻辑（条数不定，各逻辑间优先顺序同列举顺序）

以下为指令说明。

- **登录帐户**
  - {0} `su [UserID] ([Password])?`
  - 使登录帐户变为已登录状态，当前帐户变为该帐户。
    - 如果密码错误则操作失败；
    - 如果当前帐户权限等级高于登录帐户则可以省略密码。
- **注销帐户**
  - {1} `logout`
  - 撤销最后一次成功执行的 `su` 指令效果。
    - 如无已登录帐户则操作失败。
- **注册帐户**
  - {0} `register [UserID] [Password] [Username]`
  - 注册信息如指令格式所示，权限等级为 {1} 的帐户。
    - 如果 `[UserID]` 与已注册帐户重复则操作失败。
- **修改密码**
  - {1} `passwd [UserID] ([CurrentPassword])? [NewPassword]`
  - 修改指定帐户的密码。
    - 如果该帐户不存在则操作失败；
    - 如果密码错误则操作失败；
    - 如果当前帐户权限等级为 {7} 则可以省略 `[CurrentPassword]`。
- **创建帐户**
  - {3} `useradd [UserID] [Password] [Privilege] [Username]`
  - 创建信息如指令格式所示的帐户。
    - 如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败；
    - 如果 `[UserID]` 与已注册帐户重复则操作失败。
- **删除帐户**
  - {7} `delete [UserID]`
  - 删除指定帐户。
    - 如果待删除帐户不存在则操作失败；
    - 如果待删除帐户已登录则操作失败。

### 三：维护一个图书系统

图书系统提供图书信息、库存信息和图书交易财务记录的相关服务及数据存储功能。

### 图书信息

- `[ISBN]`：Internal Strange Book Number；
  - 合法字符集：除不可见字符以外 ASCII 字符；
  - 最大长度：20；
  - **特殊说明：** 本系统中任何两本图书不应有相同 `[ISBN]` 信息。
- `[BookName]`, `[Author]`：图书名字，图书作者名字；
  - 合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
  - 最大长度：60。
- `[Keyword]`：图书关键词；
  - 合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
  - 最大长度：60；
  - **特殊说明**：`[keyword]` 内容以 `|` 为分隔可以出现多段信息。如 `math|magic|quantum` 表示该图书具有三个无顺序的关键词 `math`, `magic` 和 `quantum`，每段信息长度至少为 1。
- `[Quantity]`：购买数量；
  - 合法字符集：数字；
  - 最大长度：10；
  - 特殊说明：数值不超过 2'147'483'647。
- `[Price]`, `[TotalCost]`：图书单价，交易总额。
  - 合法字符集：数字和 `.`；
  - 最大长度：13；
  - 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位。

字符相关信息默认值为空，数字相关信息默认值为数值 0。

### 图书相关指令

- **检索图书**
  - {1} `show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?`
  - 以 `[ISBN]` 字典升序依次输出满足要求的图书信息，每个图书信息输出格式为 `[ISBN]\t[BookName]\t[Author]\t[Keyword]\t[Price]\t[库存数量]\n`，其中 `[Keyword]` 中关键词顺序为输入时的顺序。
    - 无满足要求的图书时输出空行；
    - 无附加参数时，所有图书均满足要求；
    - 附加参数内容为空则操作失败；
    - `[Keyword]` 中出现多个关键词则操作失败。

- **购买图书**
  - {1} `buy [ISBN] [Quantity]`
  - 购买指定数量的指定图书（由于本系统不包括支付系统，故仅需减少库存），以浮点数输出购买图书所需的总金额。
    - 没有符合条件的图书则操作失败；
    - 购买数量为非正整数则操作失败。

- **选择图书**
  - {3} `select [ISBN]`
  - 以**当前帐户**选中指定图书（帐户登出后无需保存选中图书情况）。
    - 没有符合条件的图书则创建仅拥有 `[ISBN]` 信息的新图书；
    - 退出系统视为取消选中图书。

- **修改图书信息**
  - {3} `modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+`
  - 以指令中的信息更新选中图书的信息。
    - 如未选中图书则操作失败；
    - 有重复附加参数为非法指令；
    - 附加参数内容为空则操作失败；
    - 不允许将 ISBN 改为原有的 ISBN，例如：
      ```
      select example
      modify -ISBN=example
      ```
      为非法操作；
    - `[keyword]` 包含重复信息段则操作失败。

- **图书进货**
  - {3} `import [Quantity] [TotalCost]`
  - 以指定交易总额购入指定数量的选中图书，增加其库存数。
    - 如未选中图书则操作失败；
    - 购入数量为非正整数则操作失败；
    - 交易总额为非正数则操作失败。

### 四：在操作的同时维护一个日志系统

日志系统负责记录书店管理系统运行过程中的各种数据，提供各类日志信息查询服务

### 日志信息

- `[Count]`：交易笔数
  - 合法字符集：数字；
  - 最大长度：10；
  - 特殊说明：数值不超过 2'147'483'647。

### 日志相关指令
- **财务记录查询**
  - {7} `show finance ([Count])?`
  - 输出最后完成的指定笔数交易总额，格式为 `+ [收入] - [支出]\n`，例如 `+ 1.23 - 45678.90`。
    - 无 `Count` 参数时，输出所有交易之总额；
      - 不存在交易时认为收入支出均为 0.00。
    - `Count` 为 0 时输出空行；
    - `Count` 大于历史交易总笔数时操作失败。

- **生成日志**
  - {7} `log`
  - 返回赏心悦目的日志记录，包括系统操作类的谁干了什么，以及财务上每一笔交易情况。

## 主体逻辑说明

在main函数中读取控制台中的指令，并进行处理

用一个类对指令进行划分切割成小块，找到操作的关键词和相关参数。通过关键词调用对应的类以及类中的函数。

对每一类操作写一个类，处理所有的情况，并对错误信息进行抛出，在主程序中进行捕获并报错。

考虑到应用场景下帐户和图书数量较大，我们不将主体数据存储于内存，而实时读写文件数据。所以在类的处理中，我们以文件的形式进行读写。

## 代码文件结构


主要函数有：main函数、指令切分函数，登录状态维护函数、账户相关指令处理函数、图书相关指令处理函数、日志相关处理函数、错误类型处理函数。

主要类有：
- TokenScanner类（负责切分指令）
- account类 （负责处理一个账户的信息）
- user类 （负责维护账户、处理登陆状态以及处理账户相关指令）
- book_inf类 （负责处理一本书的信息）
- book类 （负责维护图书信息）
- diary类 （负责记录日志）
- error类 （负责处理抛出和接收错误信息）

## 各个类的接口和成员说明

### **TokenScanner类**

成员：
- std::string line 存储要处理的字符串

函数：
```
TokenScanner () 构造函数，初始化为空
~TokenScanner () 析构
std::string nextToken() 获得下一片切分
std::string see_nextToken() 查看下一片切分，但是不更改line
bool haveMoreTokens() 判断是否切分完毕
std::string nextToken_separate() 截取到=截止的字符串
std::string nextToken_separate_plus() 截取引号之间的字符串
bool check(const std::string &s,
      int max_length,int mode) 检查字符串是否合法
bool check_privilege(int x) 检查权限数字是否合法
int check_num(std::string &s) 将字符串转化为整数，不合法返回-1
int check_num_(std::string &s) 同上，但允许返回0
bool check_keyword(std::string &s) 检查关键词集合是否合法
bool is_num(char &x) 检查一个字符是否是数字
double check_double(std::string &s) 将字符串转化为实数，不合法返回-1
```
### **account类**
成员
```
char ID[31];
char password[31];
int privilege;
char name[31];
```
函数
```
account()
account(const char* ID_,const char* password_,const int privilege_,const char* name_)
account &operator= (const account &obj)
friend bool operator<(const account &a,const account &b)
friend bool operator>(const account &a,const account &b)
friend bool operator==(const account &a,const account &b)
friend bool operator>=(const account &a,const account &b)
friend bool operator<=(const account &a,const account &b)
```
### **user类**

成员

```
database<account> users; 存账户信息的数据库
std::vector<account> login_stack; 登录栈
bool exit; 退出条件
```

函数
```
user(); 初始化，若第一次打开数据库则加入超级账户root
void quit(); 设置即将退出
bool checkquit(); 是否已有退出标记
void su(Tokenscanner &scanner); 登录账户
void logout(); 登出账户
void Register(Tokenscanner &scanner); 注册账户
void passwd(Tokenscanner &scanner); 修改密码
void useradd(Tokenscanner &scanner); 创建账户
void deleteuser(Tokenscanner &scanner); 删除用户
```
### **book_inf类**
成员
```
char ISBN[21];
char bookname[61];
char author[61];
char keyword[61];
int store;
double price;
```
函数
```
book_inf()
book_inf(const char* ISBN_,const char* bookname_,const char* author_,const char* keyword_)
book_inf &operator= (const book_inf &obj)
friend bool operator<(const book_inf &a,const book_inf &b)
friend bool operator>(const book_inf &a,const book_inf &b)
friend bool operator<=(const book_inf &a,const book_inf &b)
friend bool operator>=(const book_inf &a,const book_inf &b)
friend bool operator==(const book_inf &a,const book_inf &b)
```
### **book类**

成员
```
database<book_inf> books;
std::vector<std::string> book_stack; 
```

函数
```
static bool check(std::string s,book_inf book_,int mode) 检查是否合法
void login() 登录的同时选一本空书
void logout() 登出的同时取消选书
void show(Tokenscanner &scanner,user &users) 查询书籍
void buy(Tokenscanner &scanner,user &users,diary &diarys)买书
void select(Tokenscanner &scanner,user &users) 选书
void modify(Tokenscanner &scanner,user &users) 修改图书信息
void import(Tokenscanner &scanner,user &users,diary &diarys) 进货
```

### **diary类**

成员
```
int total;
Database_<double> diarys;
```

函数
```
diary() 初始化，从文件中获取笔数
void add(double get) 加一笔账
void show(Tokenscanner &scanner,user &users) 输出最后完成的指定笔数交易总额
void log() 返回赏心悦目的日志记录
```

### **error类**

成员
```
std::string msg;
```
 函数
 ```
explicit error(const char *_msg_) 记录错误信息
const char *toString() 抛出错误信息
 ```


 ## 文件存储说明
 - 所有运行过程中产生的信息都存储在二进制文档中
 - 本程序创建了两种database类，一种以字符串作为index，服务于user类和book类，另一种以int作为index，服务于diary类

### **Database_类**
- 该类以int作为index，服务于diary类。
函数接口如下：
```
Database_()
Database_(std::string name) 初始化文件名
bool setfile(std::string name) 设置文件名
~Database_() 关闭文件
static inline void getNode(Node<T>& temp,int &num) 获得第num块链表
static inline void modify_Node(int &num,Node<T> &Node_) 改变第num块链表
static inline Head<T> getHead(int &num) 获得第num块链表的头
static inline Data<T> getData(int &Head_num,int &Data_num) 获得第Head_num块链表的第Data_num个数据
static inline Start getStart() 获得全文件的头
static inline void modify_Data(int &Head_num,int &Data_num,Data<T> &Data_) 改变第Head_num块链表的第Data_num个数据
static inline void modify_Head(int &Head_num,Head<T> &Head_) 改变第Head_num块链表的头
static inline void modify_Start(Start &st) 更改全文件的头
static inline int find_add_position(Data<T> &temp,Start &st) 找到temp应该插在第几块链表
static inline void devide(int &Head_num) 裂开第Head_num块
inline T find () 找到第一块的第一个数据，（下标为0）,用于找到目前有多少条日志记录
inline void insert(int index_,T &value_) 插入元素
inline std::vector<T> search(int index) 查找元素
inline void add_one() 让日志总条数加一
```
### database类
- 该类以字符串为index，服务于user类和book类
函数接口
```
database()
database(std::string name) 初始化文件名
bool setfile(std::string name) 设置文件名
~database() 关闭文件
static inline void getnode(node<T>& temp,int &num) 获得第num块链表
static inline void modify_node(int &num,node<T> &Node_) 改变第num块链表
static inline head<T> gethead(int &num) 获得第num块链表的头
static inline data<T> getdata(int &head_num,int &data_num) 获得第head_num块链表的第data_num个数据
static inline start getstart() 获得全文件的头
static inline void modify_data(int &head_num,int &data_num,data<T> &data_) 改变第head_num块链表的第data_num个数据
static inline void modify_head(int &head_num,head<T> &head_) 改变第head_num块链表的头
static inline void modify_start(start &st) 更改全文件的头
static inline int find_add_position(data<T> &temp,start &st) 找到temp应该插在第几块链表
static inline void devide(int &head_num) 裂开第head_num块
static inline void merge(int &head_num) 对第head_num块进行并块
inline void insert(char* index_,T &value_) 插入元素
inline std::pair<T,bool> find (char* index_) 查找元素
inline void Delete (char* index_,T &value_) 删除元素
inline bool modify(char* index_,T change_to) 修改元素
inline std::vector<T> search(char* index) 查找从index往后的所有元素
```
 ## 其它补充说明
 **暂无**