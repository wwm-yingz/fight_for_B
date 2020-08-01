# 栈和队列

栈(stack)与队列(queue)在STL中是顺序容器适配器(adaptor)，底层容器基于deque实现。

## 栈

栈的特点，先进先出，常用于BFS广度搜索，类似一层层的搜索形式

### 155_最小栈

构建全局变量min并维护是一种方法，较好的方法是使用双栈，一个栈存储正常压入的数据，另一个栈存储对应的最小值。时间复杂度O(N)，空间复杂度O(N)。

```C++
class MinStack {
    stack<int> x_stack;
    stack<int> min_stack;
public:
    MinStack() {
        min_stack.push(INT_MAX);
    }
    
    void push(int x) {
        x_stack.push(x);
        min_stack.push(min(min_stack.top(), x));
    }
    
    void pop() {
        x_stack.pop();
        min_stack.pop();
    }
    
    int top() {
        return x_stack.top();
    }
    
    int getMin() {
        return min_stack.top();
    }
};
```

### 150_逆波兰表达式

常规题，顺着思路做即可。时间复杂度O(N)需要遍历一次给定字符串数组。空间复杂度O(N)，使用了栈缓存数值

```C++
    int evalRPN(vector<string>& tokens) {
        set<string> sym = {"+","-","*","/"};
        stack<int> nums = {};
        int res = 0;
        int t,s = 0;
        for(auto iter = tokens.begin(); iter != tokens.end() ; ++iter){
            auto judge = sym.find(*iter);
            if(judge == sym.end()){//非符号
                nums.push(stoi(*iter));
            } 
            else{
                t = nums.top();
                nums.pop();
                s = nums.top();
                nums.pop();
                if(*iter == "+")
                    res =  s + t;
                else if(*iter == "*")
                    res = s * t;
                else if(*iter == "-")
                    res = s-t;
                else if (*iter == "/")
                    res = s/t;
                nums.push(res);
       
```

### 394_字符串解码

构建一个vector或者deque用于存放string。存放内容有数字，需要复制的字母和'['。或者使用递归操作。**重做**



```
    string getDigits(string &s, size_t &ptr) {
        string ret = "";
        while (isdigit(s[ptr])) {
            ret.push_back(s[ptr++]);
        }
        return ret;
    }

    string getString(vector <string> &v) {
        string ret;
        for (const auto &s: v) {
            ret += s;
        }
        return ret;
    }

    string decodeString(string s) {
        vector <string> stk;
        size_t ptr = 0;

        while (ptr < s.size()) {
            char cur = s[ptr];
            if (isdigit(cur)) {
                // 获取一个数字并进栈
                string digits = getDigits(s, ptr);
                stk.push_back(digits);
            } else if (isalpha(cur) || cur == '[') {
                // 获取一个字母并进栈
                stk.push_back(string(1, s[ptr++])); 
            } else {
                ++ptr;
                vector <string> sub;
                while (stk.back() != "[") {
                    sub.push_back(stk.back());
                    stk.pop_back();
                }
                reverse(sub.begin(), sub.end());
                // 左括号出栈
                stk.pop_back();
                // 此时栈顶为当前 sub 对应的字符串应该出现的次数
                int repTime = stoi(stk.back()); 
                stk.pop_back();
                string t, o = getString(sub);
                // 构造字符串
                while (repTime--) t += o; 
                // 将构造好的字符串入栈
                stk.push_back(t);
            }
        }

        return getString(stk);
    }
```

### 94_中序遍历

最好将左子树一次性遍历到最深，而不是逐次if判断，否则逐个判断的话需要记录当前节点的左节点是否遍历过，容易死循环在cur->left。

```C++
    vector<int> inorderTraversal(TreeNode* root) {

        stack<TreeNode*> stk;
        vector<int> res;
        TreeNode* cur = root;
        while(!stk.empty()|| cur!=nullptr)
        {
            while(cur!=nullptr){
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top();
            stk.pop();
            res.push_back(cur->val);
            cur= cur->right;            
        }
        return res;
    }
```

### 138_克隆图

建立一个map映射旧点和新点，然后根据旧点进行深度遍历或者广度遍历实现复制过程。由于是联通图，所以从任一点出发均可遍历完所有节点，不必担心有节点没有遍历到。

一般深度遍历使用递归方法，广度遍历借助栈使用迭代方法。

```C++
//注意需要补上比较函数否则不能建立map
bool operator < (const Node &p1,const Node &p2){
    return p1.val<p2.val;
}
    Node* cloneGraph(Node* node) {
        //构造一个map  构建原图和新图的对应
        //起始点对应起来   新图链表空
        map<Node*,Node*> m;
        Node* cur = node;
        if(cur == nullptr)
            return nullptr;
        vector<Node*> curlst;
        m[node] = new Node(cur->val);
        cout<<cur->val;

        //深度遍历，把原图起始点node的链表入栈。
        stack<Node*> stknode;
        stack<vector<Node*>> stk;
        stknode.push(cur);
        stk.push(cur->neighbors);

        //当前点是cur是旧图起始点
        //curlst  是旧图链表
        while(!stk.empty()){//栈非空
            cur = stknode.top();
            stknode.pop();
            curlst = stk.top();//vector<Node*> 
            stk.pop();

            vector<Node*> newneighbour = {};
            //广度遍历  通过是否是nullptr的方式判断是否经历过
            for(auto lstiter = curlst.begin(); lstiter != curlst.end() ; lstiter++){
                //lstite是cur的链表的node*元素   类型应该是node*指针
                //*lstite 是node*   **lstite是node 
                if(m[*lstiter] == nullptr){//说明这个点没去过  //那么这个点需要被构建
                    m[*lstiter] = new Node((*lstiter)->val);
                    cout<<(*lstiter)->val;
                    stknode.push(*lstiter);
                    stk.push((*lstiter)->neighbors);
                }
                newneighbour.push_back(m[*lstiter]);
            }
            m[cur]->neighbors = newneighbour;
        }
        return m[node];
    }
```

### [200. 岛屿数量](https://leetcode-cn.com/problems/number-of-islands/)

使用DFS方法搜索岛，借助栈储存DFS信息。时间复杂度O(MN)，空间复杂度O(MN)。

使用BFS方法搜索岛，借助队列存储BFS信息。时间复杂度O(MN)，空间复杂度O(MN)。

```C++
int move[4][2] = {{1,0},{-1,0},{0,-1},{0,1}};

int numIslands(vector<vector<char>>& grid) {
    int res = 0;
    if (grid.empty()) return 0;
    int m = grid.size() , n = grid[0].size();

    vector<vector<int>> visit(m, vector<int> (n));
    queue<pair<int,int>> landque ;

    for(int i = 0 ; i < m ; i++){
        for(int j = 0 ; j < n ; j++){
            if(visit[i][j] == 1)    continue;
            visit[i][j] = 1;
            if(grid[i][j] == '0')   continue;

            //是陆地  bfs搜索
            {
                int ni , nj;
                int ci , cj;
                landque.push(pair<int,int> {i,j});
                while(!landque.empty()){
                    auto l = landque.front();
                    landque.pop();
                    ci = l.first;
                    cj = l.second;
                    for(int t = 0 ; t < 4 ; t++){
                        ni = ci + move[t][0];
                        nj = cj + move[t][1];
                        if(ni>=0 && ni < m && nj >= 0 && nj < n && visit[ni][nj] == 0){
                            visit[ni][nj] = 1;
                            if(grid[ni][nj] == '1') landque.push(pair<int,int> {ni,nj});
                        }
                    }
                }
                res++;
            }
        }
    }
    return res;
}
```

并查集方法，扫描整个二维网格。如果一个位置为 11，则将其与相邻四个方向上的 11 在并查集中进行合并。

最终岛屿的数量就是并查集中连通分量的数目。

# 需要关注

```C++
class UnionFind {
public:
    UnionFind(vector<vector<char>>& grid) {
        count = 0;
        int m = grid.size();
        int n = grid[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == '1') {
                    parent.push_back(i * n + j);
                    ++count;
                }
                else {
                    parent.push_back(-1);
                }
                rank.push_back(0);
            }
        }
    }

    int find(int i) {
        if (parent[i] != i) {
            parent[i] = find(parent[i]);
        }
        return parent[i];
    }

    void unite(int x, int y) {
        int rootx = find(x);
        int rooty = find(y);
        if (rootx != rooty) {
            if (rank[rootx] < rank[rooty]) {
                swap(rootx, rooty);
            }
            parent[rooty] = rootx;
            if (rank[rootx] == rank[rooty]) rank[rootx] += 1;
            --count;
        }
    }

    int getCount() const {
        return count;
    }

private:
    vector<int> parent;
    vector<int> rank;
    int count;
};

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int nr = grid.size();
        if (!nr) return 0;
        int nc = grid[0].size();

        UnionFind uf(grid);
        int num_islands = 0;
        for (int r = 0; r < nr; ++r) {
            for (int c = 0; c < nc; ++c) {
                if (grid[r][c] == '1') {
                    grid[r][c] = '0';
                    if (r - 1 >= 0 && grid[r-1][c] == '1') uf.unite(r * nc + c, (r-1) * nc + c);
                    if (r + 1 < nr && grid[r+1][c] == '1') uf.unite(r * nc + c, (r+1) * nc + c);
                    if (c - 1 >= 0 && grid[r][c-1] == '1') uf.unite(r * nc + c, r * nc + c - 1);
                    if (c + 1 < nc && grid[r][c+1] == '1') uf.unite(r * nc + c, r * nc + c + 1);
                }
            }
        }

        return uf.getCount();
    }
};

作者：LeetCode
链接：https://leetcode-cn.com/problems/number-of-islands/solution/dao-yu-shu-liang-by-leetcode/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```



时间复杂度：O(MN∗α(MN))，其中 MM 和 NN 分别为行数和列数。注意当使用路径压缩（见 find 函数）和按秩合并（见数组 rank）实现并查集时，单次操作的时间复杂度为 α(MN)，其中 α(x) 为反阿克曼函数，当自变量 xx 的值在人类可观测的范围内（宇宙中粒子的数量）时，函数α(x) 的值不会超过 55，因此也可以看成是常数时间复杂度。

空间复杂度：O(MN)，这是并查集需要使用的空间

### [84. 柱状图中最大的矩形](https://leetcode-cn.com/problems/largest-rectangle-in-histogram/)







