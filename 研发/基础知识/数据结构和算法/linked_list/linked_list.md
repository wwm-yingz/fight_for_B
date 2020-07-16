# 链表

结构体

```C++
struct ListNode{
    int val;
    ListNode *next;
    ListNode(){}
    ListNode(int x): val(x),next(null){}
};
```

## 注意内容

*   null/nil 异常处理
*   dummy node 哑巴节点
*   快慢指针
*   插入一个节点到排序链表
*   从一个链表中移除一个节点
*   翻转链表
*   合并两个链表
*   找到链表的中间节点



## 例题

### 83_删除顺序链表重复元素

```C++
ListNode* deleteDuplicates(ListNode* head) {
    if(head == NULL)
        return head;
    ListNode * current = head;
    while(current->next!=NULL){
        if(current->val == current->next->val){//当前与下一个相同
            current->next = current->next->next;//跳过一个
        }
        else{
            current = current->next;//正常下一个
        }            


    }
    return head;
}
```

### 82_删除顺序链表重复元素2

链表中常会加一个**哑节点**(dummy)作为头结点，这样第一个有效节点则不需要进行单独的处理，可以视作与后续的节点相同。注意，行5处必须使用new或者malloc进行初始化，否则仅仅定义了野指针，没有分配空间，行6会报错。

运用了**快慢指针**的思想。代码中node即慢指针，p即快指针

```C++
    ListNode* deleteDuplicates(ListNode* head) {
        if(head == NULL)
            return head;

        ListNode* prehead = new ListNode;//空节点作为链表头
        prehead->next =head;
        ListNode* node = prehead;//当前的头
        ListNode* p;//当前指向的针
        int num;//当前的值
        while(node->next != NULL){//node就是一个头，已经确定她是唯一的了
            p = node->next;
            num = p->val;
            if(p->next == NULL || p->next->val != num ){//p的下一个节点是空节点，或者p与下一个节点不同  则直接过下一个
                node = p;
            }
            else{//p与下一个节点相同且p的下一个节点不是空节点
                p = p->next;
                while(p->next!=NULL)
                {
                    if(p->next->val != num)
                        break;
                    p = p->next;
                }
                node->next = p->next;//此时next为NULL  或者为值与num不同的东西  继续回while循环
            }
        }
        return prehead->next;
    }
```

### 206_反转链表

迭代与递归均是循环中的一种，区别如下

1.  递归通过重复调用函数本身实现循环。迭代通过函数某段代码实现循环，迭代与普通循环的区别是，循环代码中参与运算的变量同时是保存结果的变量，当前保存的结果作为下一次循环计算的初值。
2.  递归通过满足终止条件逐层返回结束，迭代通过使用计数器结束循环。

一般而言迭代是逐渐逼近，用新值覆盖旧值，直到满足条件后结束，不保存中间值，空间利用率高。
递归是将一个问题分解为若干相对小一点的问题，遇到递归出口再原路返回，因此必须保存相关的中间值，这些中间值压入栈保存，问题规模较大时会占用大量内存。

故能用迭代不用递归。以下均是**迭代方法**。

**新建全新链表**

```C++
    ListNode* reverseList(ListNode* head) {
        if(head == NULL)
            return head;
        ListNode* newhead = new ListNode;
        ListNode* ptr = head;
        ListNode* node = new ListNode(ptr->val);
        newhead->next = node;
        while(ptr->next !=NULL){
            ListNode* node = new ListNode(ptr->next->val);
            node->next = newhead->next;
            newhead->next = node;
            ptr = ptr->next;
        }
        return newhead->next;
    }
```

**原地转换**

```
ListNode* reverseList(ListNode* head) {
    ListNode* res = nullptr;
    ListNode* ptr = head;
    //原地转换
    while(ptr){
    ListNode* t = ptr->next;
    ptr->next = res;
    res = ptr;
    ptr = t;
    }
    return res;
}
```

**递归方法**

```C++
ListNode* reverseList(ListNode* head) {
	//return
    if(!head || !head->next)
        return head;
    //p一直是最后一个非空节点   最后一节个节点返回自身，其他节点返回p，所以p被保留下来了，始终是最后一个节点，78则为单步操作。
    ListNode* p =reverseList(head->next);
    head->next->next =head;
    head->next = NULL;
    return p;
}

```

### 92_反转链表2

厘清思路，合理选择添加哑结点作为头，即可做出。(一般都需要考虑添加哑结点作为头结点)

对于链表进行反转操作，需要至少3个指针才能进行。

```C++
ListNode* reverseBetween(ListNode* head, int m, int n) {
    ListNode* slow,*fast,*prefast;
    ListNode* ptr ;

    ListNode* dummy = new ListNode(0);
    dummy->next = head;
    ListNode* curr=dummy;
    for(int i =1;i<m;i++){
        curr = curr->next;
    }
    slow = curr;
    curr = curr->next;//调转的第一个
    fast = curr->next;
    for(int i = m;i<n;i++){
        //curr = fast;
        ptr = curr;
        curr = fast;
        fast = curr->next;
        curr->next = ptr;
    }
    slow->next->next = fast;
    slow->next = curr;
    return dummy->next;
}
```

### 21_合并有序链表

```C++
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode* head = new ListNode();
    ListNode* ptr = head;
    while(l1 && l2){
        if(l1->val < l2->val){
            ptr->next = l1;
            ptr = ptr->next;
            l1 = l1->next;
        }
        else
        {
            ptr->next = l2;
            ptr = ptr->next;
            l2 = l2->next;
        }
    }
    if(l1){
        ptr->next = l1;
    }
    else
        ptr->next = l2;

    return head->next;
} 
```

### 86_分割链表

方法1: 直接把小于x和大于x的变成2个链表存出去(或者分出去一个链表，原链表存一个)，最后再合并。时间复杂度O(N)，空间复杂度O(1)，注意该方法可以直接挪动原有链表的节点，所以没有增加空间复杂度。

方法2:原地处理。时间复杂度O(N)，空间复杂度O(1)。 可进一步优化时间复杂度，20行与31行当连续小于或者连续大于时是无用的。该方法没有方法1简单，并且没有效率提升。

```C++
    ListNode* partition(ListNode* head, int x) {
        if(head == NULL){
            return head;
        }
        ListNode* bigs,*bigend,*smalls,*smallend;
        ListNode* node = NULL;
        ListNode* pummy = new ListNode(0);
        pummy->next = head;
        node = pummy->next;
        bool small = false , big = false;
        

        while(node != NULL){
            if(node->val < x){
                if(!small){
                    smalls = node;
                    smallend = node;
                    small = true;
                }else{
                    smallend ->next = node;
                    smallend = node;
                }

            }
            else{
                if(!big){
                    bigs = node;
                    bigend = node;
                    big = true;
                }else{
                    bigend->next = node;
                    bigend = node;
                }
            }
            node = node->next;

        }
        if(big && small){
            pummy->next = smalls;
            smallend->next = bigs;
            bigend->next = NULL;
        }
        else{}

        return pummy->next;
    }
```

### 148_排序链表

要求O(nlogn)时间复杂度，O(1)的空间复杂度。

快速排序多用于带索引的序列排序，因为每次必将一个数字恢复到正确的地方。假如不是带索引的序列，如链表。可以使用归并排序的思路，分治法拆分再合并。

分治法递归方法: 因为二分法递归的深度是log2n，最深处有n次需要处理，所以二分法常见的时间复杂度是n*logn。时间复杂度O(logn)。因为没有新开辟存储空间，所以空间复杂度O(logn)即递归深度。

```C++
    ListNode* sortList(ListNode* head) {
        //退出条件
        if(head == NULL || head->next == NULL)
            return head;
        //divide
        ListNode* pummy = new ListNode(0);
        pummy ->next = head;        
        ListNode* mid,*slow,*fast;
        slow = pummy;
        fast = pummy;
        while(fast && fast->next){
            slow = slow->next;
            fast =fast->next->next;
        }
        //slow奇数个正中，偶数个中左
        mid = slow->next;
        slow->next = NULL;
        
        
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);
        //merge
        ListNode * node = pummy;

        while(left && right){
            if(left->val < right->val){
                node->next = left;
                left = left->next;
            }
            else{
                node ->next = right;
                right = right->next;
            }
            node = node->next;
        }
        if(left)
            node->next =left;
        else
            node->next = right;

        return pummy->next;
    }
```

递归改迭代，从底到顶直接合并。这样空间复杂度可降为O(1)

```C++
//准备内容
//链表常用函数，cut(head,n)，断链操作，将链表head切掉前n个节点返回后半部分链表头
//merge(l1,l2)双路归并
ListNode* cut(ListNode* head ,int n){
    auto p = head;
    while(--n&&p){
        p = p->next;
    }
    if(!p) return nullptr;
    auto next = p->next;
    p->next = nullptr;
    return next;
} 

ListNode* mergeList(ListNode* l1, ListNode* l2){
    ListNode pummy(0);
    ListNode* p = &pummy;   
    while(l1&&l2){
        if(l1->val <= l2->val){
            p->next = l1;
            l1 = l1->next; 
        }else{
            p->next = l2;
            l2 = l2->next;
        }
        p = p->next;
    }
    p->next = l1?l1:l2;        
    return pummy.next;
}
```

### 143_重排链表

较为简单，找到中间点，然后反转中间点往后的链表，然后拼接回去即可。

```C++
    void reorderList(ListNode* head) {
        if(!head || !head->next)
            return;
        //哑头
        ListNode* pummy = new ListNode(0);
        pummy->next = head;
        //找中点slow
        ListNode* p =pummy;
        ListNode *slow, *fast;
        slow = p;
        fast = p;
        while(fast && fast->next){
            slow = slow->next;
            fast = fast->next->next;
        }
        //slow是中间或者中左，排序后应该出现在最后
        //反转
        ListNode *p2 = slow->next;
        slow->next = NULL;
        ListNode *np2 = reverse(p2);//反转
        mergeList(head , np2);//合并
    }
    ListNode* reverse(ListNode *head){
        //反序链表
        ListNode *res = nullptr;
        ListNode *p = head;
        while(p){
            ListNode *t = p->next;
            p->next = res;
            res = p;
            p =t;
        }
        return res;
    }

    ListNode* mergeList(ListNode* l1, ListNode* l2){
        ListNode pummy(0);
        ListNode* p = &pummy;   
        bool flag = true;
        while(l1&&l2){
            if(flag){
                p->next = l1;
                l1 = l1->next; 
                flag = false;
            }else{
                p->next = l2;
                l2 = l2->next;
                flag = true;
            }
            p = p->next;
        }
        p->next = l1?l1:l2;        
        return pummy.next;
    }
```

### 141_环形链表

使用快慢节点解决，假如存在环则快慢节点一定会相遇。

**快慢指针常见解决的问题**:

*   取倒数第k个元素(先让快慢指针距离变为k，然后快指针到终点，慢指针就是倒数第k)
*   找中间结点
*   是否存在环
*   环的大小(2次相遇经过的节点数)

```C++
    bool hasCycle(ListNode *head) {
        if(!head||!head->next)
            return false;
        ListNode *slow = head, *fast = head->next;
        while(slow->val != fast->val){
            slow =slow->next;
            if(!fast->next || !fast->next->next)
                return false;
            fast = fast->next->next;
        }
        return true;
    }
```

### 142_环形链表2

判断是否有环并找到环起点。

方法一:使用set保存遍历过的所有节点，第一个已存在set中的节点就是循环头

方法二: **Floyd 算法**数学过程如下

|      |      |
| ---- | ---- |
|      |      |
|      |      |
|      |      |
|      |      |



```C++
    ListNode *detectCycle(ListNode *head) {
        if(!head||!head->next)
            return nullptr;
        ListNode *slow = head, *fast = head;
        while(fast && fast->next){ 
            slow = slow->next;
            fast = fast->next->next;
            if(slow == fast){
                break;
            }
        }
        if(!fast || !fast->next)            
            return nullptr;

        //找入环节点
        ListNode *ptr1 = head;
        ListNode *ptr2 = fast;
        while(ptr1 != ptr2){
            
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
        
        return ptr1;
    }
```

### 234_回文链表

方法1: 存进数组，双指针向中间扫。时间复杂O(N)，空间复杂度O(N)

方法2:递归

​	如果使用递归反向迭代节点，同时使用递归函数外的变量向前迭代，就可以判断链表是否为回文。思路可参考如下伪代码。

```python
global front_ptr = head;
function print_val_in_reverst(ListNode head)
	if head is NOT null
		print_val_in_reverst(head.next)
        # 此处的head其实就是逆序head，只要在加一个全局变量从头进行索引，则可以实现头尾比较
		print head.val
```



方法3:获取中间点+翻转+逐个对比。时间复杂度O(N)，空间复杂度O(1)。

```C++
//方法3
    bool isPalindrome(ListNode* head) {
        if (head == NULL)
            return true;
        ListNode *pummy = new ListNode(0);
        pummy->next = head;
        //找中间        
        ListNode *slow = pummy, *fast = pummy;
        while(fast&&fast->next){
            fast = fast->next->next;
            slow = slow->next;
        } 
        //翻转后半部分
        ListNode* p1 = head;
        ListNode* p2 = slow->next;
        p2 = reserve(p2);
        while(p2){
            if(p1->val != p2->val)
                return false;
            p1 = p1->next;
            p2 = p2->next;
        }
        //此时p1的位置( 奇数p1==slow || 偶数p1==slow->next)
        //有需要可以借助slow翻转后半部分恢复链表
        return true;
    }
    ListNode* reserve(ListNode* head){
        ListNode* res = nullptr;
        ListNode* p = head;
        ListNode* t;
        while(p){
            t = p->next;
            p->next = res;
            res = p;
            p = t;
        }
        return res;
    }
```

### 138_复制链表

难点在于random指向的节点在深拷贝过程中可能还没有建立。

方法1: 建立一个hashmap存node。第一次遍历建立原链表和新链表映射关系，第二次解决next指针和random指针。时间复杂度O(N)，空间复杂度O(N);

方法2: 每建立一个新节点则建立一份指针拷贝存进数组。list和random指针同时进行，只要在数组中找不到拷贝就新建一个节点放进去。

方法3: 在原链表中各个节点旁边插入新建立的节点。(例如原链表A->B->C变为A->A'->B->B'->C->C')。二次遍历时可以直接通过B->random找到B'->random对应的节点(即B->random->next)。第三次遍历重新链接节点即可。时间复杂度O(N)，空间复杂度O(1)。不需要额外空间

```C++
//方法1
    Node* copyRandomList(Node* head)
    {
        if (head == nullptr)
            return head;

        //先建立映射关系
        unordered_map<Node*, Node*> map; //<原链表节点，对应位置的新链表节点>

        Node* cur = head;
        while (cur)
        {
            map[cur] = new Node(cur->val);
            cur = cur->next;
        }

        //遍历原链表 一次性解决next和random
        cur = head;
        while (cur)
        {
            Node* node = map[cur];
            node->next = map[cur->next];
            node->random = map[cur->random];
            cur = cur->next;
        }
        return map[head];
    }
```

## 补充

C++ 红黑树

C++中map容器用法

C++中小根堆容器

priority_queue<int, vector<int>, less<int>>s;//less表示按照递减(从大到小)的顺序插入元素
priority_queue<int, vector<int>, greater<int>>s;//greater表示按照递增（从小到大）的顺序插入元素

