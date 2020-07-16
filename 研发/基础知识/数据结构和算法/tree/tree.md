# 本文主要内容

* 二叉树

	*   二叉索引树
	*   搜索树
	*   平衡树

* 遍历

* 分治法

	*   代码结构（3步）
	*   处理情况（3种）
	*   逻辑判断

	

# 二叉树

常见的数据结构，因此衍生的数据结构有平衡二叉树，红黑树，Btree，B+tree。

## 二叉树结构体

```C++
struct treeNode{
    int val;
    treeNode *left;
    treeNode *right;
    treeNode(int x): val=x,left=NULL,right=NULL {};//结构体的构造函数，cpp特性。赋值语句在{}外相当于初始化，若在括号内则等同于赋值。
};
```
### 生成二叉树

由list转为二叉树数据结构，数字0标记为空点

```C++
TreeNode* listCreateTree (vector<int> treeList,int ptr){
    
    if (ptr > treeList.size()-1 || treeList[ptr] == 0)//当作空点
        return NULL;
    TreeNode* node;
    node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = treeList[ptr];
    node->left = listCreateTree(treeList,2*ptr+1);
    node->right = listCreateTree(treeList,2*ptr+2);
    return node;
}   
```



### 遍历

先序遍历是**根左右**的遍历顺序，中序遍历是**左根右**的遍历顺序，后序遍历是**左右根**的遍历顺序。先中后指示了根的位置，永远都是先左再右。其中递归方法的先中后序遍历，仅需要修改对应的迭代函数顺序即可。

#### **递归先序遍历**

```C++
void preOrderTravelsal (treeNode * root){
    if (root == NULL)
        return ;

    printf("%s",root->val);
    preOrderTravalsal(root->left);
    preOrderTravalsal(root->right);
}
```

#### **非递归先序遍历**

该方法需要用到栈对已遍历的内容进行暂存，需要占用较大的储存空间
```C++
void preOderTravelsal (treeNode * root){
    //a stack for saving treeNode
    Stack<treeNode*> treeNodeStack = new Stack<treeNode*>();
    treeNode * node = root;
    //if node is the last one and stack is empty , all treeNode have been passed
    while (node! = NULL || !treeNodeStack.empty()){
        while (node != NULL){//in this loop will move to the most deep left node
 			//.....operation.......
            printf("%s",node->val);
            //.....................
            // push node into stack for travel right node in the future
            treeNodeStack.push(node);
            // left node
            node = node->left;
        }
        if(!treeNodeStack.empty()){
            node = treeNodeStack.top();
            treeNodeStack.pop();
            node = node->right;
        }
    }
}
```

#### **非递归中序遍历**

左根右

```C++
void inorderTraversal(treeNode * root){
    Stack<treeNode*> treeNodeStack = new Stack<treeNode*>();
    treeNode * node = root;
    while(node!=NULL || !treeNodeStack.empty()){
        while(node!=NULL){
            treeNodeStack.push(node);//root
            node = node->left;            
        }
        node = treeNodeStack.top();
        treeNodeStack.pop();
        //...operate...
		printf("%s",node->val);
        node = node -> right;
    }
}
```

#### **非递归后序遍历**

左右根 注意先右再根，这一步必须打标记

```C++
void postorderTraversal(treeNode * root){
    stack<treeNode*> treeNodeStack = new Stack<treeNode*>();
    treeNode* node = root;
    treeNode* lastvisit;
    while(node!=NULL || !treeNodeStack.empty()){
        while(node!=NULL){
            treeNodeStack.push(node);
            node = node->left;
        }
            node = treeNodeStack.top();
            if( node->right == NULL || node->right == lastvisit){//the root node should be operate after right node
                treeNodeStack.pop();
                //..operate..
                printf("%s",node->val);
                lastvisit = node;
            }
            else{
                node = node->right;
            }
        }
    }
}
```

#### **DFS深度搜索-从上到下**

使用指针标记位置，逐步填入数组，增加一个参数用于标记当前level可计算最大深度。

```C++
void dfs(treeNode* root, vector<int>& result){
    if root == NULL
        return result;
    result.append(root->val);
    dfs(root->left,result);
    dfs(root->right, result);
}

vector<int> dfsTraversal(treeNode* root){
    vector<int> result = {};
    dfs(root,result);
    return result;
}
```

#### **DFS深度搜索-从下到上**(分治法) 

**输出结果相同，运行顺序不同**

先经历到了最深的子树处，再返回合并成结果。

递归返回结果并合并。

​	例子

​	tree [1,2,3,4,5]

​	result [1,2,4,5,3]

​	1

​	245<-2  4  5 

​	3

```C++
vector<int> divideAndConquer(TreeNode* root)  {
    vector<int> result = {} ;
    // 返回条件(null & leaf)
    if (root == NULL) {
        return result;
    }
    // 分治(Divide)
    vector<int> left,right;
    left = divideAndConquer(root->left);
    right = divideAndConquer(root->right);
    // 合并结果(Conquer)
    result.push_back(root->val);
    result.insert(result.end(),left.begin(),left.end());
    result.insert(result.end(), right.begin(),right.end());
    return result;
}
```

#### **BFS广度搜索-层次遍历**

先入先出，借助队列辅助计算

```C++
vector<int> levelOrder(TreeNode* root){
    vector<int> result = {};
    queue<TreeNode*> treeQueue;
    TreeNode* node = NULL;
    if(root == NULL){
        return result;
    }
    treeQueue.push(root);
    //左进 右进 存 出队
    while(!treeQueue.empty()){
        node = treeQueue.front();
        if(node->left != NULL)
        	treeQueue.push(root->left);
        if(node->right != NULL)
            treeQueue.push(root->right);
        result.push_back(node->val);
        treeQueue.pop();
    }
}
    
```

### 线索二叉树数据结构

线索二叉树目的 : 充分利用指针的作用，防止出现空指针浪费内存空间。将空指针改为指向前驱或者后继的线索(依据中序遍历)。

在**经常需要遍历或查找节点需要按照某种顺序的时候**可以使用。

可以再补充一个头结点，头结点左指向根节点，头结点右指向尾节点(中序最后一个); 中序第一个节点的前驱(左)指向头结点，中序最后一个节点(尾)指向头结点。这样整个二叉树将会变成一个**双向链表**。

此时可以从头结点(新建的那个)进行遍历: 如果有左子树经过左，如果无左子树则往右子树走(后驱节点)。遍历结果是中序遍历。

也可以从尾节点(最后一个，右下)进行遍历

```C++
typedef enum{Child,Thread} pointTag; //Child==0  代表指针指向孩子
									 //Thread==1 代表指针指向中序遍历的前驱或者后驱节点(左指前，右指后)
struct clueTreeNode{
    int val;
    pointTag lTag,rTag;
    clueTreeNode *left;
    clueTreeNode *right;
};
```

#### **构造过程**

**构建二叉树时无法确定前驱后驱信息，故需要在中序遍历过程中进行线索化**需要定义全局变量记录访问的上一个节点用于指定后继线索。

```C++
clueTreeNode* pre;
void clueCreateTree (clueTreeNode * root)
{
    if(root == NULL)
        return ;
	clueCreateTree(root->left);
    if(!root->left){
        root->lTag = Thread;
        root->left = pre;//第一次这里是NULL
    }
    if(!pre->right){
        pre->rTag = Thread;
        pre->right = root;
    }
    pre = root;
    clueCreateTree(root->right);   	
}
```

#### **双链表方式遍历过程**

```C++
//T是头结点，左指向根节点，右指向中序遍历的最后一个节点
void linkInOrderTraversal(clueTreeNode* T){
    vector<int> result;
    clueTreeNode* ptr;
    ptr = T->left;//根节点
    while(ptr!= T){
        while(ptr->lTag == Child)
            ptr = ptr->left;
        //operate...
        result.push_back(ptr->val);
        while(ptr->rTag == Thread && p->right!= T)//右节点指向的都是某个根节点或者右子树，如果是根节点则输出，子树则重新进迭代。
        {
            ptr = ptr->right;
            result.push_back(ptr->val);
        }
        ptr = ptr->right;//终点或者有右子树的情况 出了上一个循环。
    }
    return;
}
```

### **哈夫曼树**(带权路径的二叉树)

叶子节点带权，根节点到叶子节点的距离称为路径长度。哈弗曼(huffman)树为带权路径长度(WPL)最小的二叉树。

算法过程:

1.  n个权值{w1,...,wn}构成n个二叉树集合F={T1,...Tn}此时每个二叉树Ti只有一个带权wi的节点

2.  从F中选取权值最小的2个节点构造一棵新树，左小右大，根节点权重为左右子树权重和。

3.  删除这两个树，构成的新树加入。

4.  重复2，3直到F只剩一棵树。

    #### **哈夫曼编码**

    常用于数据压缩。根据使用字符的频率构造01编码。只需要保证所有数据点都在叶子节点上(即不会发生某一字符的编码是另一字符的前缀的情况，前缀编码)。即可使用哈夫曼编码表达数据。并且由于使用01编码代替原数据结构，达到了压缩的目的。**一般左0右1**

    | 字母       | A    | B    | C    | D    | E    | F    |
    | ---------- | ---- | ---- | ---- | ---- | ---- | ---- |
    | 二进制字符 | 01   | 1001 | 101  | 00   | 11   | 1000 |



## **分治法**

思想: 先处理局部，再合并结果:

### 常用场景

*   快速排序
*   归并排序
*   二叉树问题

### 分治法模板

*   递归返回条件
*   分段处理
*   合并结果

DFS深度搜索-从下到上(分治法) ，中就是一个典型的分治法模型。由返回条件，拆分情况处理，合并结果3部分组成。

### 分治法遍历二叉树

```C++
vector<int> divideAndConquTra(TreeNode* root){
    vector<int> result = {};
    //返回条件
    if(root = NULL){
        return result;
    }
    //分治
    vector<int> left,right;
    left = divideAndConquTra(root->left);
    right = divideAndConquTra(root->right);
    //合并  (操作一般在在这部分)
    result.push_back(root->val);//操作
    result.insert(result.end(),left.begin(),left.end());
	result.insert(result.end(),right.begin(),right.end());
    return result;
    
}
```

### 归并排序(分治法)

数组拆分成多个部分，各个部分逐个排序，然后再两两合并复原。

```C++
vector<int> mergeSort(vecrtor<int> nums){
    //stop condition
    if nums.size() <= 1{
        return nums;
    }
    //divide 
    int mid = nums.size() / 2;
    vector<int> temp_l(nums.begin(),nums.begin() + mid);
    vector<int> left = mergeSort(temp_l);
    
    vector<int> temp_r(nums.begin()+mid,nums.end());
    vector<int> right = mergeSort(temp_r);
    //merge  operate in this part
    vector<int> result = merge(left, right);
    return result;
}
vector<int> merge(vector<int> left,vector<int> right){
    int ind_l = 0, ind_r = 0;
    vector<int> result;
    for (ind_l<left.size() && ind_r < right.size()){
        //谁的小谁先进
        if (left[ind_l]<=right[ind_r]){//所以是稳定的算法
            result.push_back(left[ind_l]);
            ind_l--;
        }
        else{
            result.push_back(right[ind_r]);
            ind_r--;
        }
	}    result.insert(result.end(),left.begin()+ind_l,left.end());
    result.insert(result.end(),right.begin()+ind_r,right.end());
}
```

### 快速排序(分治法)

```C++
    //调用语句
    quickSort(nums,0,nums.size()-1);

vecrot<int> quickSort(vector<int>&nums,int start, int end){
    //退出条件
	if(start < end){
        int pivot = partition(nums, 0,end);//确定分割点
        //分割后左右操作
        quickSort(nums,0,pivot-1);
        quickSort(nums,pivot+1,end);
        //因为是原地操作所以无需合并
    }
    //因为在数组中直接进行交换所以合并变成自动的了
}
int partition(vector<int>&nums,int start , int end){
    //将比基准值小的放在左边即可，不需要从两边向中间搜索
    int baseVal = nums[end];
    int indi = start;
    for(int indj=start; indj<end;intj++){
        if (nums[indj] < baseVal){
            swap(nums,indi ,indj);
            indi++;
        }
    }
    //此时baseVal归为到i
    swap(nums,i,end);
}

//工具函数
void swap(vector<int> nums,int i ,int j){
    int t = nums[i];
    num[i] = nums[j];
    nums[j] = t;
}
```





## 例题

### 104_二叉树最大深度

希望求得的类型值int,中间过程需要的类型值Int。可以一个函数搞定

```C++
int maxDepth(TreeNode* root) {
    //返回条件
    if(root == NULL){
        return 0;
    }
    //divide 
    int left = maxDepth(root->left) + 1;
    int right = maxDepth(root->right) +1 ;
    //加一其实就是operation
    //合并结果
    return left>right?left:right;
}
```

### 110_平衡二叉树

平衡二叉树的判定标准是，每个节点的平衡因子的绝对值小于等于1。其中平衡因子=左子树最大深度-右子树最大深度。根据此条件，我们只需要计算每个节点的左右子树深度差是否符合条件即可。

其中计算当前节点是否平衡这个过程是最小操作单元。

操作单元:

1.  左子树是不是平衡
2.  右子树是不是平衡
3.  该节点是不是平衡的(差是不是小于<=1)

**优化思路**: 仔细观察isBalanced和maxDepth函数，输入对象都是TreeNode对象。其中一个返回bool变量，另一个返回正数。因此其实可以将2者融合到一起。以-1代表子树是不平衡的，返回值为正数则代表深度。

不过一般工程不使用一个变量表示两种含义

```C++
bool isBalanced(TreeNode* root){
	//return
    if(root == NULL){
        return true;//空节点肯定是平衡的
    }
	//divide
    //左子树  右子树  分别计算深度
    int depL = maxDepth(root->left);
    int depR = maxDepth(root->right);
	//merge
    return ((isBalanced(root->left) && isBalanced(root->right)) &&((depL-depR<2)&&(depL-depR>-2)));
}
int maxDepth(TreeNode* root) {
    //返回条件
    if(root == NULL){
        return 0;
    }
    //divide 
    int left = maxDepth(root->left) + 1;
    int right = maxDepth(root->right) +1 ;
    //加一其实就是operation
    //合并结果
    return left>right?left:right;
}
```

### 124_二叉树最大路径和

首先分析最大路径会是什么样子的，只有3种情况。

1.  类似左斜树与其变形
2.  类似右斜树与其变形
3.  倒V字型的树

对于一个有子树的节点而言，他应该把与分别与左右子树结合后中较大的那个数值传上去。同时检查以它自身为根节点与左右子树结合所形成的的路径是否会更大。由此可以使用分治法。

操作单元:

1.  获取左子树最大路径
2.  获取右子树最大路径
3.  左+右+根对比当前最大路径是否超越，如果超越则更新。返回根，跟+左，根+右中较大的那个数值同时以这个最大数值去刷最大值。

tips : 当divide中需要返回的数值意义与当前函数意义相同(本题中本质都是求深度)，则可以直接使用当前函数迭代，否则需要写新函数并调用(上题中深度与平衡即不同。)

中间过程值int,目标类型int。但是内在含义不同，所以设定全局变量。或者需要通过辅助操作函数完成。此题中注意需要的结果存在全局变量maxPath中，而不是返回值maxPathNode中，所以需要多一个主函数main。

```C++
int maxPath  = -100000;//设置一个全局变量用于储存最大路径，需要是足够小的负数
int maxPathSum(TreeNode* root){
    if(root == NULL){
        return 0;
    }
    //divide
    int pathLeft = maxPath(root->left);
    int pathRight = maxPath(root->left);
    //merge
    int pathLMR = pathLeft + pathRight + root->val;
    if (pathLMR > maxPath){
        maxPath = pathLMR;
    }
    int maxPathNode = root->val;
    if(pathLeft>0 && pathLeft>pathRight){
        maxPathNOde+=pathLeft;
    }else if(pathRight>0){
        maxPathNOde+=pathRight;
    }
    if (maxPathNode > maxPath){
        maxPath = maxPathNode;
    }
    
    return maxPathNode;    
}
```

### 236_二叉树最近公共祖先

操作单元:

1.  检测左子树是否包含目标节点
2.  检查右子树是否包含目标节点
3.  自己是否是目标节点

判断逻辑，满足这个条件的节点实际上只有目标公共祖先节点
$$
(f_lson \and  f_rson)\or[(root==p \or root==q)\and(f_lson \or  f_rson)]
$$
有了判断逻辑则可以简单的写出来merge后的返回值具体内容。

本题中，主函数返回值类型，树指针。中间值类型，bool类型。如果不使用同变量多意义，则必须要使用辅助函数。

如果使用同变量多意义则可以使指针为NULL代表false，否则代表true

```C++
TreeNode* targetNode = NULL;

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
	//主函数写个入口即可
    dfs(root,p,q);
    return targetNode;
}
bool dfs(TreeNode* root, TreeNode* p, TreeNode* q){
    //check
    if(root == NULL)
        return false;
    //divide
    bool left = dfs(root->left,p,q);
    bool right = dfs(root->right,p,q);
	//merge and return
    if( (left&&right) || ((left||right)&&(sameNode(root,p)||sameNode(root,q))  ){
        targetNode = root;
    }
    return left||right||sameNode(root,p)||sameNode(root,q);
}

//通用函数
bool sameNode(TreeNode* p,TreeNode* q){
    if(p->val == q->val)
        return true;
    else 
        return false;
}
```

## **分治法总结**

*   如果分治后中间数据类型与主函数返回值类型相同，意义相同。一个函数即可搞定。

*   如果分治中间数据类型与函数返回值类型相同，意义不同。可以根据情况使用全局变量，辅助函数，使用同一变量表达多种意思(例如负数表示false，正数表示true同时表示长度)

*   分治中数据类型与函数返回值类型意义均不同，则使用辅助函数+全局变量，使用同一变量表达多种意思。

**分治法做题步骤**

1.  形成操作单元，确定中间计算过程的数据类型

2.  确定根据左右分支形成的逻辑
3.  确定是否需要辅助函数





## BFS层次遍历应用例题

### 102_二叉树逐层遍历

```C++
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result = {};
    if(root==NULL)
        return result;
    queue<TreeNode*> levelQueue;
    levelQueue.push(root);
    while(!levelQueue.empty()){
        int len = levelQueue.size();
        vector<int> levelVec = {};
        for(int i=0;i<len;i++){
            //左存 右存 取 出
            TreeNode* node=levelQueue.front();
            if(node->left!=NULL)
                levelQueue.push(node->left);
            if(node->right!=NULL)
                levelQueue.push(node->right);
            levelVec.push_back(node->val);
            levelQueue.pop();
        }
        result.push_back(levelVec);
    }
    return result;
}
```

### 107_层次遍历自底向上

借用辅助栈

```C++
vector<vector<int>> levelOrderBottom(TreeNode* root) {
    vector<vector<int>> result = {};
    stack<vector<int>> tempres = {};
    if(root==NULL)
        return result;
    queue<TreeNode*> levelQueue;
    levelQueue.push(root);
    while(!levelQueue.empty()){
        int len = levelQueue.size();
        vector<int> levelVec = {};
        for(int i=0;i<len;i++){
            //左存 右存 取 出
            TreeNode* node=levelQueue.front();
            if(node->left!=NULL)
                levelQueue.push(node->left);
            if(node->right!=NULL)
                levelQueue.push(node->right);
            levelVec.push_back(node->val);
            levelQueue.pop();
        }
        tempres.push(levelVec);
    }
    while(!tempres.empty()){
        result.push_back(tempres.top());
        tempres.pop();
    }
    return result;
}
```

### 103_二叉树Z形遍历

```C++
void reverse(vector<int>& nums) {
    for (int i=0,j=nums.size()-1 ; i<j;i = i+1,j=j-1){
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }
}

vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> result = {};
    if(root==NULL)
        return result;
    queue<TreeNode*> levelQueue;
    levelQueue.push(root);
    int level = 0;//代表层数
    while(!levelQueue.empty()){
        level++;
        int len = levelQueue.size();        
        vector<int> levelVec = {};
        for(int i=0;i<len;i++){
            //左存 右存 取 出
            TreeNode* node=levelQueue.front();
            if(node->left!=NULL)
                levelQueue.push(node->left);
            if(node->right!=NULL)
                levelQueue.push(node->right);
            levelVec.push_back(node->val);
            levelQueue.pop();
        }
        if (level&1)
            result.push_back(levelVec);
        else
        {
            reverse(levelVec);
            result.push_back(levelVec);
        }
    }
    return result;
}
```

### 98_验证二叉搜索树

二叉搜索树特征:

1.  左子树都小于当前节点
2.  右子树都大于当前节点
3.  左右子树分别也是二叉搜索树

中间数据类型int+bool,所以需要辅助函数+全局变量/创建结构体的形式。

逻辑判断式
$$
(f_lson \and f_rson)\and(root>lson)\and(root>rson)
$$
方法二:中序遍历储存，然后检查是否有序即可

```C++
struct resType{
    bool isval;
    TreeNode* maxval;
    TreeNode* minval;
};
bool isValidBST(TreeNode* root) {
	resType rootres = isvalue(root);
    if (rootres.isval)
        return true;
    else
        return false;
}
//核心函数
resType isvalue(TreeNode* root){
    resType res;//最好设定一个初始化函数
    //check
    if(root==NULL){
        res.isval = true;
        res.maxval = NULL;
        res.minval = NULL;
        return res;
    }        
    //divide
    resType left = isvalue(root->left);
    resType right = isvalue(root->right);
    //merge
    if((left.isval)&&(right.isval))
    {
        if((left.maxval ==NULL||left.maxval->val < root->val) &&
        	(right.minval == NULL||right.minval->val > root->val))
        {
            res.isval = true;
        }
        else{          
            res.isval = false;
        }
    }
    else
    {
        res.isval = false;
    }
    //此时只要是false就可以退出了
    if(res.isval){
        if(right.maxval == NULL)
            res.maxval = root;
        else
            res.maxval = right.maxval;
        if(left.minval == NULL)
            res.minval = root;
        else
            res.minval = left.minval;
    }
    return res;
}
```

### 701_搜索树插入

//只需要找到符合条件的叶子节点插入即可。可以论证，一定存在某个叶子节点可以将所需二叉树插入。

```C++
TreeNode* insertIntoBST(TreeNode* root, int val) {
        if(!root)   
            return new TreeNode(val);

        if(val > root -> val)
            root -> right = insertIntoBST(root -> right, val);
        else
            root -> left = insertIntoBST(root -> left, val);
        return root;
}
```

### 延申内容

**B树**

**B+树**

大根堆

小根堆





