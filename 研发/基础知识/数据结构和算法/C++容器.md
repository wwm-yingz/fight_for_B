# 容器

一个容器是特定类型对象的几个，在C\+\+标准库中包含了大部分常见的容器。STL 是“Standard Template Library”的缩写，中文译为“标准模板库”。STL 是 C\+\+ 标准库的一部分，不用单独安装。TSL核心包括3个组件。容器(containers)，算法(algorithms)，迭代器(iterators)。除此外还有仿函数，内存配置器和配接器。

按照容器的存储结构可以分为顺序容器与关联容器两类。元素在顺序容器的顺序与加入容器时的位置相对应。关联容器的元素位置由元素相关联的字值决定。

## 顺序容器

顺序容器一般都具有快速顺序访问元素的能力。在以下两点中有性能折中:

* 向容器添加或删除元素的代价
* 非顺序访问元素的代价

**标准库顺序容器**

* vector，   可变大小数组。可随机访问。在**除尾部外**插入删除元素慢
* deque，    双向队列。可随机访问。在头尾插入删除快。
* list，          双向链表。只支持双向顺序访问。任何位置插入删除速度都快。
* forward_list单向链表。只支持单向顺序访问。任何位置插入删除都快。
* array          固定大小数组。支持快速随机访问。不可添加删除元素。
* string         与vector相似的容器。专门用于保存字符。随机访问快。尾部插入删除快

**补充**

deque的随机读取实现方法为通过分段的连续内存加map实现。其中map将各段内存地址串起来，表现为一块连续的大内存，以此实现随机访问。

### 选取容器 

基本原则:

* 一般而言选用vector
* 如果程序小元素多(会产生大量碎片)，且空间额外开销很重要。不要使用list或者forward_list。
* 随机访问，必须vector或者deque
* 中间插入list,forward_list
* 头尾插中间不插，deque
* 读取时中间插，访问时随机访问
    * 确定是否真的有中插需要。例如vector的sort函数可以进行重排的操作sort(v.begin(),v.end(),优先级)。默认是升序。
    * 必须中插可考虑，输入阶段list。完成输入将list拷贝到vector

几乎最全能的容器，deque。一般而言，vector和list足以使用了。

### 容器类型成员

容器container中包含了一些类型,通过域运算符进行定义。container\<template\>::member_name

* iterator 对应容器类型的迭代器类型
* const_iterator 只读迭代器类型，可读不可写
* size_type 无符号整数类型，足够保存此类容器最大可能容器的大小
* difference_type 带符号整数类型，足够表达两个**迭代器**之间的距离
* value_type 元素类型
* referenct 元素左值类型;与value_type& 含义相同
* const_referenct 元素的const左值类型

由于大多数时候并不需要关心如容器的迭代器具体是什么类型，所以配合auto使用。代码可读性和泛化性更好。

#### 迭代器

迭代器是最主要使用的容器中的类型。支持以下操作

* *iter 使用解引用符返回迭代器iter所指向元素的**引用**
* iter->member 解引用iter并获得该元素名为men的成员的引用
* ++iter    iter指向容器中下一个元素
* --iter      iter指向容器中上一个元素
* iter1 == iter2，iter1 != iter2   判断2个迭代器是否相等

通过begin和end可以取得容器的首元素与尾后迭代器。带r版本返回反向迭代器，带c版本返回const迭代器。

反向迭代器对于各种操作相对正向迭代器相反，例如++操作对于反向迭代器会得到上一个迭代器。

不以c开头的函数是被重载过的，实际上有2个名为begin的成员。cbegin()是C++11新标准，显式指定类型为const接受begin()函数的返回值也可以取到只读迭代器如it5。

```C++
list<string> c = {"hello","world","!"};
auto it1 = c.begin();//list<string>::iterator
auto it2 = c.rbegin();//list<string>::reverse_iterator
auto it3 = c.cbegin();//list<string>::const_iterator
auto it4 = c.crbegin();//list<string>::const_reverse_iterator;
list<string>::const_iterator it5 = c.begin();//显式指定数据类型
```

### 容器操作

一般而言除array容器比较特殊外，几乎所有操作对于其他的顺序容器都是通用的。因为array容器需要在创建时初始化容器大小。

#### 初始化

除array容器外，其他容器的默认构造函数都会创建一个指定类型的空容器

定义与初始化:

对应于初始化的3种常见方式

1. 拷贝初始化，=，迭代器
2. 列表初始化，{}
3. 构造函数初始化，()最后两种

```C++
C c;                //默认构造函数，如果C是array则c中元素按照默认方式初始化，否则c为空。

C c1(c2);           //c1初始化为c2的拷贝。c1、c2必须是相同容器类型，且保存相同元素类型(对于array而
C c1 = c2;          //言，还要有相同大小)

C c{a,b,c..};       //c初始化为初始化列表中元素的拷贝。列表元素类型必须和c保存元素类型相同。
C C = {a,b,c};      //对于array而言，列表元素数目必须小于等于array大小，遗漏的元素将进行值初始化。

C c(b,e);           //c初始化为得带器be指定范围中的元素的拷贝。array不适用

只有顺序容器(除array)，构造函数可以接受大小参数
C seq(n);   //容器seq包含n个元素，对这些元素进行了值初始化。这个构造函数是explicit的;(string不适用)
C seq(n,t); //包含n个初始化值为t的元素

```

在创建一个容器为另一个容器拷贝的时候，容器类型和储存元素类型都必须相同。当使用迭代器表示拷贝时，储存元素类型可转化即可，但容器类型必须相同。

array的初始化，必须指定大小。注意，array支持容器**大小相同，储存元素类型相同的**2个array容器进行拷贝和赋值。但是内置数组不支持拷贝或者赋值操作。

```C++
array<int,42> //类型为保存42个int的数组
注意size_type也需要指定大小
array<int,10>::size_type i; //正确
array<int>::size_type j;    //错误

int digs[10] = {1,2,3,4,5,6,7,8,9,10};
int cpy[10] = digs;                     //错误，内置数组不支持拷贝
array数组支持拷贝初始化。
```

#### 赋值

赋值运算将左边容器中的元素替换成邮编容器中元素的拷贝。

```C++
c1 = c2                             //c1,c2必须具有相同类型
c = {a,b,c,....}                    //c1替换成初始列表的拷贝，**array不适用**
swap(c1,c2)                         //交换c1和c2的元素，c1，c2必须有相同的类型，
c1.swap(c2)                         //swap一般比从c2向c1拷贝要快

assign操作不适用于关联容器和array
seq.assign(b,e)                     //将seq中的元素替换迭代器b和e所表示的范围中的元素。
                                    //迭代器b和e不能指向seq中的元素
seq.assign(ilst)                        //将seq中的元素替换为初始化列表ilst中元素
seq,assign(n,t)                     //将seq替换为n个值为t的元素
```



assgin的意义在于**语序从一个不同但相容的类型往原容器赋值**。例如可以将vector中的char*赋值给list中的string。由于旧元素被替换，所以传递给assign 的迭代器不能指向调用assign的容器。

```C++
list<string> name;
vector<const char*> oldstyle;
names = oldstyle;               //错误类型不匹配！
names.assign(oldstyle.cbegin(),oldstyle.cend()); //可以将const char*转化为string
```



swap操作交换两个类型相同的容器。**注意，元素本身未被交换**。交换容器内的操作保证会很快，只是交换了容器内部数据结构(比如指针)。除array外，swap不会对元素进行拷贝删除插入的操作。**对于array的swap操作会交换他们的元素，开销与array元素数目成正比。**

假设iter指向svec1[3]的sting。那么swap(svec1, svec2)后，iter会指向svec2[3]的迭代器。(其中svec1和svec2类型为list\<string\>)**对一个string调用swap会导致迭代器引用指针失效。

一般使用非成员版本的swap操作，便于兼容旧版本更好的泛化。

#### 比较

容器一定支持相等运算符操作，所有容器都支持==与!=操作。当内部元素拥有比较运算符，则可以使用关系运算符来比较容器。否则不支持。

判定标准:比较过程其实就是内部元素的逐对比较。

* 大小相同且两两对应相等。则容器相等，否则不等
* 大小不同，小容器每个元素等于大容器中对应元素。小容器小于大容器
* 大小不同，且小容器不是大容器前缀子序列。比较结果取决于第一个不等的元素的结果。

#### 增加

除array外，标准库容器都可以再运行时动态添加删除容器大小。array不支持这些操作。

注意事项:

* 向vector,string,deque插入元素会使所有指向容器的迭代器引用和指针失效。(该过程会重新申请分配内存空间。故失效)
* 使用过程中应按照不同策略分配元素空间，策略将直接影响性能。
    * 在vector，string尾部外任何位置，deque首尾外任何位置添加元素。都需要移动元素。
    * 像一个vector或者string添加元素可能引擎整个对象存储空间的重新分配。
* array不支持下列操作，因为会改变大小
* forward_list有专门版insert和emplace
* forward_list不支持push_back和emplace_back
* vector,string不支持push_front,emplace_front

```C++
c.push_back(t);             //在c的尾部创建一个值为t或者由args创建的元素
c.emplace_back(args);       //返回void

c.push_front(t);            //在c的头部创建元素
c,emplace_front(args);      //返回void

c.insert(p,t);              //在迭代器p指向的元素！！之前！！创建一个值为t或者由args创建的元素
c.emplace(p,args);          //返回指向新添加元素的迭代器

c.insert(p,n,t);            //在迭代器p之前插入n个值为t的元素
                            //返回指向第一个新添加元素的迭代器，n为0返回p
c.insert(p,b,e);            //在迭代器p之前插入迭代器b和e指定的范围内的元素(左闭右开)。b和e不能指向c
                            //返回指向新添加的第一个元素，若范围为空返回p
c.insert(p,ilst);               //!! ilst是一个花括号包围的元素列表!! 将这些值插入到迭代器p之前。
                            //返回插得第一个元素，若为空返回p
```

**push和emplace的区别**

push接受的是容器的储存元素的数据类型是一个对象。emplace接收的是容器储存元素的的参数。严格而言，emplace方法更省空间。push 方法会先将接收元素对象进行拷贝创建局部临时变量，再放入容器本身，而不是直接使用对象放入。而emplace方法会将参数传给元素类型的构造函数初始化为对象随后放入容器。注意，传递给emplace函数的参数必须与元素类型的构造函数相匹配

**insert**

注意该方法添加的元素是在迭代器之前，主要是因为迭代器可能指向尾后元素，这是一个不存在元素的位置。部分容器不支持push_front操作，但是对于insert无限制，可以通过指向首元素的迭代器把元素插入容器开始位置。注意，但此操作可能耗时较长。

通过插入1.n个相同的元素，2.两个迭代器指示范围，3.元素列表。可以实现一次性插入多个元素。

#### 访问

注意事项:

* at和下标操作只适用于string,vector,string,deque,array
* back不适用于forwark_list
* 顺序容器没有find操作，如需使用需要用全局find
* 队列是尾进头出，即front是最早入队的，back是最后入队的。


```C++
c.back();           //返回c尾元素的引用，若c为空函数行为未定义    
c.front();          //返回c首元素的引用，若空未定义
c[n];               //返回下标为n的元素的引用，n是一个无符号整型，若n>=c.size()，行为未定义
c.at(n);            //返回下标为n的元素的引用，若下标越界抛出out_of_range异常
```

**back()front()和begin()end()的区别**

begin和end返回的是迭代器，需要解引用符号才可以获取元素值，front和back返回的直接是元素对象的引用。**注意**end返回的是尾后元素，需要*(--iter)才能正确获得尾元素的引用。

#### 删除

注意事项:

* 删除操作会改变大小，所以array不适用
* forward_list有特殊的erase
* forward_list不支持pop_back
* vector和front不支持pop_front

```C++
c.pop_back();           //删除c的尾元素。返回void。若c为空函数未定义
c.pop_front();          //删除c的首元素。返回void。若空未定义
c.erase(p);             //删除迭代器p所指向元素，返回指向被删元素之后的迭代器。
                        //若p是尾元素，返回尾后元素。    若p是尾后，未定义
c.erase(b,e);           //删除迭代器b和e所指定范围捏的元素，返回一个指向最后一个被删除元素之后的迭代器
                        //若e是尾后迭代器，也返回尾后迭代器.(计数范围使用左闭右开原则)
c.clear();              //删除c所有元素，返回void
```

**注意**erase(b,e)依旧是左闭右开原则，e应当指向希望删除的最后一个元素之后的位置，以下两者是等效操作，

>  c.erase(c.begin(),c.end())
>
> c.clear()

#### 单链表容器的添加与删除

单链表容器forward_list有着特殊的添加删除操作。因为单链表的数据结构从插入行为而言需要访问前驱，但是单链表的结构导致迭代器无法知道其前驱节点。故未定义类似其他顺序容器的添加删除函数，定义了insert_afer,rmplace_after,erase_after的函数，插入删除操作也是针对给定迭代器的下一个元素元素进行。类似尾后元素定义了before_begin函数取得首前迭代器，类似哑头结点。

```C++
lst.before_begin()              //返回指向链表首元素钱不存在的元素的迭代器，此迭代器不能解引用
lst.cbefore_begin()             //c前缀返回一个const_iterator
    
lst.insert_after(p,t)           //在迭代器p后插入元素，t是一个元素对象
lst.insert_after(p,n,t)         //n代表数量
lst.insert_after(p,b,e)         //b和e代表范围内的一对迭代器，不能指向lst。范围为空返回p
lst.inster_after(p,ilst)            //ilst是一个花括号列表
                                //返回值指向最后一个插入元素的迭代器
                                //若p为尾后迭代器，则行为未定义。
    
emplace_after(p,args)           //使用args在迭代器p后传建一个元素，返回一个指向这个元素的指针
                                //若p为尾后，行为未定义
    
lst.erase_after(p)              //删除迭代器!!p之后!!的位置指向的元素
lst.erase_after(b,e)            //删除从b到e之间的元素(左闭右开不包括e)
                                //返回指向最后一个被删除元素之后的迭代器
                                //    若不存在这样的元素返回尾后迭代器，p为尾后行为未定义
```

#### 改变容器大小

可以使用resize增大或缩小容器，array不支持resize。若当前大小>新大小，容器后部元素被删除。若当前大小<新大小，会将新元素添加到容器后部。

```C++
c.resize(n)             //调整c的大小为n。若c.size()>n，多出来的元素被删除。
c.resize(n,t)           //若c.size()<n，变大，则对新元素进行值初始化，如果有对象t，则初始化为值t。
```

#### 容器操作导致迭代器失效

对容器进行添加和删除元素操作均可能导致指向容器元素的指针，引用或者迭代器失效。使用失效的指针引用与迭代器是严重程序设计错误。

**添加元素**后:

* vector或者string，若储存空间被重新分配(发生在原空间不足够的情况下，会向操作系统申请更大的内存空间并迁移)。则指针迭代器引用全部失效。若未重新分配，指向插入位置前的指针等有效，**但指向插入位置后的**指针等无效。
* 对于deque，插入**除首尾位之外任何位置**会导致指针等失效。在首尾添加元素，**迭代器失效**，指针引用不会失效。(例如D={1,2,3},cite1是D[1]的引用,iter1是指向D[1]的迭代器。此时在首插入元素,D={0,1,2,3}。cite1此时是D[2]的引用了iter1也是指向D[2]的迭代器，含义不同了。)
* 对于list和forward_list，指向容器的迭代器(包括尾后和首前)指针引用**均有效**。(因为本身就是随机写)

**删除元素**后，指向被删除元素的肯定无效，其余:

* vector或者string，指向被删除元素之前的迭代器引用指针有效。注意，**只要删除尾后迭代器总会失效**
* 对于deque，插入**除首尾位之外任何位置删除**会导致指针等失效。在尾删除元素，**尾后迭代器失效**，其他指针引用不会失效。删除首元素，其他不收影响。
* 对于list和forward_list，指向容器其他位置的迭代器(包括尾后和首前)指针引用**均有效**。(因为本身就是随机写)



故必须保证每次改变容器操作后正确重新定位迭代器。对于vector,string,deque需要更加注意迭代器是否有效。

常见场景:

* 使用循环时在循环体内更新迭代器，以确保有效
* 不要保存end返回的迭代器(对于vector,string,deque尾后迭代器删除操作后一般会失效，故C++实现end()操作都很快)

#### string的额外用法

主要函数:

* s.substr(pos,n)
* s.insert(pos,args)
    * 支持直接插入另一个字符串
* s.erase(pos,len)
* s.assign(args)
* s.append(args)
* s.replace(range,args)
* s.find(args)/s.rfind(args)   失败返回**string::npos**下同
    * arg的形式，pos一般默认0，s.find_first_of等类似
    * c,pos。从s中位置pos开始找c
    * s2,pos。从s中pos位置开始找字符串s2
    * cp,pos。从s中pos开始找以空字符结尾的C字符串。
    * cp,pos,n。从s中pos开始找cp指向的数组的前n个字符串。pos,n无默认值
* s.find_first_of(args)/ s.find_last_of(args)
* s.find_firse_not_of(args)/ s.find_last_not_of(args)
* s.compare(s2)
* to_string(val)
* stoi(s,p,b)       其中p是size_t类型的指针，默认为0，运行后会指向s中第一个非数值字符的下标(注意起点从s开始算)默认为0。b是转换基数(进制)，默认为10。
* 字符串转C风格字符串。c_str(s)

## 顺序容器适配器

除上述顺序容器外，标准库定义了3个顺序容器适配器: stack/queue/priority_queue。适配器(adaptor)是标准库通用概念，容器迭代器函数都有适配器。本质适配器是一种机制，使对象行为看起来符合某种事物要求。例如stack适配器接受一种顺序容器(除了array和forward_list)，并使其操作看起来像stack。

### 通用操作

所有适配器都支持的操作。

* size_type                           一种类型，足以保存当前类型最大对象的大小
* value_type                        元素类型
* container_type                适配器底层容器类型
* A a;                                    初始化
* A a(c);                                创建适配器a，带有容器c的拷贝
* a.empty()
* a.size()
* swap(a,b)/ a.swap(b)

### 初始化

默认情况下，stack和queue基于deque实现。priority_queue基于vector实现。可以再创建时指定底层容器类型。

```C++
//默认创建
stack<int> stk(deq);    //从deq拷贝元素到stk
//指定创建
stack<string, vector<string>> str_stk;          //指定了一个在vector上实现的空栈
stack<string, vector<string>> str_stk2(svec);   //用svec去初始化str_stk2
```

**底层容器限定**

* stack除array和forward_list之外任何容器类型均可构造。(要求push_back, pop_back, pop操作)
* queue可用list或者queue。不能基于vector。(要求push_front, push_back, front, back)
* priority_queue可用vector或queue，不能基于list。(要求随机访问，front, push_back, pop_back)

注意**不能在适配器中调用底层容器的操作**，例如在栈中调用push_back。即使栈基于deque实现。

### 栈操作

```C++
s.pop();
s.push(item);
s.emplace(args);
s.top()
```

### 队列适配器

```C++
q.pop();
q.front();      //最早入队元素
q.back();       //仅适用队列，最晚入队元素
q.push(item);
q.emplace(args);
```

### 优先级队列

完整创建方式:

其中参数2指定底层容器，默认vector。参数3指定优先级。默认情况排序顺序是less\<t\>内部图如下图。元素最大的会被放在队列前面。(一般丢列入队在队尾，出队在队头)

也可这样理解 (入) 队尾  1 < 2 < 3 < 4    队头  (出)。 这串序列即按照优先级排列了，且新加入元素会排在优先级比他低的元素之前。从队头出队。**less大顶堆，great小顶堆**

![image-20200719190147301](https://pic-bed-1256813635.cos.ap-nanjing.myqcloud.com/markdownImg/image-20200719190147301.png)

优先级队列在内部是不会随时维护一个顺序数组的，所以不能使用[i]的下标形式读取，只能使用top形式


```C++
//升序队列  小顶堆
priority_queue <int,vector<int>,greater<int> > pq;//升序
//降序队列  大顶堆 默认
priority_queue <int,vector<int>,less<int> > pq;//降序

记忆**less对应降序**。注意与别的比较函数区分！

实际上是保持优先级最高的元素在[0]的位置，每次pop或者push操作会更新
```



```C++
q.pop();        //删除最高优先级元素
q.top();        //仅适用优先级队列，返回优先级最高的元素
q.push(item);   //在恰当位置创建元素
q.emplace(args);
```



## 关联容器

关联容器支持**高效关键字查找和访问**。主要的关联容器(associative-container)类型是map(映射类型)和set(集合类型)。map中的元素是关键字-值(key-value)对:关键字起索引作用，值则表示与索引相关联的数据。set中每个元素质保函一个关键字;set支持高效的关键字查询操作----检查一个给定关键字是否在set中。常用于例如文本处理中需要忽略的单词。

标准库提供8种关联容器，主要在以下3点有所选择。允许重复关键字容器名字包含multi，不保持关键字按顺序存储的容器名字以unordered开头。无需容器使用哈希函数组织元素。

* 每个容器或是一个set，或是一个map
* 每个容器或要求不重复关键字，或允许关键字
* 按顺序保存元素，或者无序保存

**标准库关联容器:**

按关键字有序保存

* map
* set
* multimap
* multiset

按关键字无序保存

* unordered_map
* unordered_set
* unordered_multimap
* unordered_multiset

### 选取关联容器

map容器又被称为关联数组(associative array)，与正常数组类似，区别在于下标不必是整数。常见应用场景如需要对应的字典。

set容器是关键字的简单集合，应用于当需要知道一个值是否存在。 常见场景如判断是否是新用户。



在关键字没有明显序关系，选用无序容器。或者维护元素序代价非常高，选用无序容器。**理论上哈希技术能获得更好的平均性能。**

### 与顺序容器区别

关联容器不支持顺序容器与位置相关的操作，例如push_back，因为关联容器元素依据关键字存储，位置没有意义。

关联容器不支持构造函数或插入操作这类接受一个元素值和一个数量值的操作。不支持(n,t)这类构造函数，(b,e)这类构造函数仍支持。

**关联容器的迭代器都是双向的**



### 容器类型成员

#### 迭代器

**关联容器的迭代器都是双向的**。

map的迭代器得到的是容器的value_type的值得引用(实际上类似pair)，first保存的是const关键字，second成员保存值。

set的迭代器得到的是容器的value_type的值得引用，迭代器都是只读的，不能修改只能访问。

#### pair类型

可以使用first和second访问成员。**map的元素类型就是pair**。first保存的是const关键字，second成员保存值。

**注意区分迭代器和pair对象的引用，前者使用->解引用，后者使用. 运算符 **



### 容器操作

#### 初始化

关联容器与顺序容器初始化方式类似，联容器的默认构造函数仅创建一个指定类型的空容器，可以使用同类型容器的拷贝初始化关联容器。map容器需要指明关键字类型和值类型，set容器只需指明关键字类型

关联容器初始化的3种常见方式

1. 拷贝初始化，=，迭代器
2. 列表初始化，{}
3. 构造函数(b,e)

```C++
map<string, size_t> word_count; //空容器
//列表初始化
map<string, int> salary = {{"john", 100},
                           {"bob", 500},
                           {"austen", 1000}};
set<string> exclude = {"the","but","and","or"};
//构造函数初始化
vector<int> ivec = {1,1,2,2,3,3};
set<int> iset(ivec.begin(),ivec.end());             //仅包含来自ivec不重复的元素，大小为3
multiset<int> miset(ivec.begin(),ivec.end());       //包含所有元素即使重复，大小为6
```

列表初始化器的内容必须能转换为容器中的元素类型。初始化map时需要提供关键字-值对，使用花括号包围。

##### **关键字类型要求**

**对于有序容器**，关键字类型必须定义元素比较的方法。默认情况下，标准库使用关键字类型的<运算符比较关键字。可以自定义比较操作，但是必须定义**严格弱序**，类似于小于等于。

**对于自建类，需要重载<运算符或者显式指定比较运算符。(map必须重载运算符)**如下例，提供了自建类型和比较操作类型----这应该是函数指针。使用decltype获得函数指针类型，必须加上*支出要使用一个给定函数类型的指针。

```
class Sales_data{
    string no;
public:
    string getno();
}
Sales_data::getno(){
    return this.no;
}
//比较函数
bool compareNO(const Sales_data &p1,const Sales_data &p2){
    return p1.getno()<p2.getno();
}
//定义有序集合
multiset<Sales_data, decltype(compareNO)*> Salesstore(compareNO);
```

**对于无序容器**

由于需要hash计算，所以不能直接定义关键字类型为自定义类类型的无序容器，不能直接使用hash模板，需要自己创建hash模板。元素的哈希值类型为hash<key_type>。默认情况下，无序容器通过关键字==运算符比较元素。

```C++
size_t hasher(const Sales_data &sd){
    return hash<string>() (sd.getno());
}
bool eqOp(const Sales_data &p1,const Sales_data &p2){
    return  p1.getno()==p2.getno();
}
using SD_multiset = unordered_multiset<Sales_data, decltype(hasher)*, decltype(eqOp)*>;
SD_multiset Salesstore2(42,hasher,eqOp);//参数是 桶大小、哈希函数指针、相等性判断运算符指针。
//如果定义了==则可以值重载哈希函数
```



#### 添加

* insert(v)
* emplace(args)
* inse(b,e)
* insert(ilst)
* insert(p,v)          p是迭代器，指出从哪里开始搜索新元素存储位置。
* inse(p, args)

对于单个插入map/set返回一个pair。first是迭代器指向具有给定关键字的元素，second成员是bool，指出插入成功(true)或已存在(false)。对于multimap/multiset因不存在插入失败仅返回迭代器。多个插入返回void。

#### 删除

erase

#### 访问

map可用下标访问。**如果使用一个不在容器中的关键字作为下标，会添加一个具有此关键字的元素到map中**。**自动添加并执行默认初始化只要访问就会发生，并且读取的值就是默认初始化的值。**

```C++
//例子
map<int,int> m;
m[1] = 2;
if(m[2] == 0)//判断true，默认初始化就是0
    m[2] = 3;
cout<<m[5];//输出0
```



下标和at操作只适用于非const的map和unorder_map。

为了防止不必要的添加可以使用find操作。

其中

* find(k)                     返回指向k的迭代器，若不存在返回.end()
* count(k)                   返回int

* lower_bound(k)     不小于(可能是本身的第一个)     无序容器不适用
* upper_bound(k)     大于                                                无序容器不适用
* equal_range(k)       返回一个迭代器pair，指示关键字等于k的元素范围(左闭右开)。若不存在两者均为c.end()或指向关键字可插入的位置。

#### 桶管理(无序容器)

无序容器在存储上本质组织为一组桶，每个桶保存零个或多个元素。通过哈希函数将元素映射到桶。为了访问元素，容器先计算哈希值，指出搜索桶。搜索桶将具有相同哈希值的所有元素保存在相同的桶中。如果容器允许重复关键字，所有具有相同关键字的元素也会在同一个桶中。所以无序容器性能依赖于哈希函数质量和桶的数量和大小。一般而言计算元素哈希值和在桶中搜索都很快，除非一个桶保存了很多元素，那么查找特定元素需要大量比较操作。

容器会在需要时添加新的桶，以使得load_factor()<=max_load_factor重组存储。

桶管理的相关函数。

```C++
//桶接口
c.bucket_count();           //正在使用的桶数目
c.max_bucket_count();       //容器能容纳的最多的桶的数量
c.bucket_size(n);           //第n个桶有多少元素
c.bucket(k);                //关键字k的元素在哪个桶
//桶迭代
local_iterator              //用来访问桶中元素的迭代器类型(equal_range()的返回值类型)
const_local_iterator        //const版本
c.begin(n),c.end(n)         //桶n的首元素迭代器和尾后迭代器
c.cbegin(n),c.cend(n)       //const
//哈希策略
h = c.hash_function();      //返回c的哈希函数
eq = c.key_eq();            //eq是c的相等检测函数
c.load_factor();            //装载因子。元素除以桶数，double(c.size())/c.bucket_count()每个桶平均元素数量，float
c.max_load_factor();        //最大装载因子。c试图维护的平均桶大小，返回float。
//以下函数代价可能会触发重新hash所有元素，代价可能非常高，最坏情况O(N^2)
c.max_load_factor(d);       //输入参数float d，将最大装载因子设定为d，若装载因子已接近最大，c将改变哈希表大小
c.rehash(n);                //重组储存，使得bucket_count>=n且bucket_count>size/max_load_factor
c.reserve(n);               //重组储存，使得c可以保存n个元素并且不用rehash.c.rehash(ceil(n/c.max_load_factor()))
```

无序关联容器装载因子定义为已用空间的比例，也是size()/capacity()。

加载因子指的是hash表元素填满的程度，越满空间利用率越高，但是冲突的几率增加了。越小填满的元素越少，但是空间浪费多了





## 总览

##### 主要容器一览

目前，STL 中已经提供的容器主要如下：

- vector <T>：一种向量。
- list <T>：一个双向链表容器，完成了标准 C++ 数据结构中链表的所有功能。
- queue <T>：一种队列容器，完成了标准 C++ 数据结构中队列的所有功能。
- stack <T>：一种栈容器，完成了标准 C++ 数据结构中栈的所有功能。
- deque <T>：双端队列容器，完成了标准 C++ 数据结构中栈的所有功能。
- priority_queue <T>：一种按值排序的队列容器。
- set <T>：一种集合容器。
- multiset <T>：一种允许出现重复元素的集合容器。
- map <key, val>：一种关联数组容器。
- multimap <key, val>：一种允许出现重复 key 值的关联数组容器。

##### 主要算法一览

STL 提供了非常多的数据结构算法。这些算法在命名空间 std 的范围内定义，通过包含头文件 <algorithm> 来获得使用权。
 常见的部分算法如下：

关于sort的用法:

>     sort(v.begin(),v.end(),greater<int>());//降序
>
>     sort(v.begin(),v.end(),less<int>());//升序   默认！   小的在前  大的在后  

- for_each()；
- find()；
- find_if()；
- count()；
- count_if()；
- replace()；
- replace_if()；
- copy()；
- unique_copy()；
- sort()；
- equal_range()；
- merge()；



## 参考资料:

1. <C++ PRIME>