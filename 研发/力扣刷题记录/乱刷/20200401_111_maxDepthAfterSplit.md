# 力扣1111
题目链接：https://leetcode-cn.com/problems/maximum-nesting-depth-of-two-valid-parentheses-strings

简单而言就是对括号string进行分类，拆分为组A和组B，每组内不需要连续，但每组独立看都必须是完成对应的括号对。同时使两组的最大深度尽可能小。
```
错误分类:不允许交错括号出现
(())
0101
允许嵌套
()(())
000110
```
只要提到括号这类成对出现的数据类型，必然使用栈的思路进行数据存放，为了使栈的深度最小，当出现多层嵌套时，一层分给A一层分给B即可，按照奇偶性分类即可。

第一版代码
```C++
vector<int> maxDepthAfterSplit(string seq) {
    vector<int> store;//当作一个栈存放括号
    vector<int> result;
    for (auto s : seq){
        if (s=='('){
            store.push_back(0);//0代表存放一个括号
            //如果栈深是一个奇数，归类到A
            //栈深是一个偶数，归类到B
            if(store.size()%2==1)
                result.push_back(0);
            else
                result.push_back(1);
        }
        else {
            if(store.size()%2==1)
                result.push_back(0);
            else
                result.push_back(1);
            store.pop_back();
        }
    }
    return result;
}
```
分析:很明显这个题必须要遍历完整个string，只需要进行逐个判断，时间复杂度是O(n)。

很明显其中发生了代码重复，那么只要是代码重复就是有优化空间的。

首先是没必要对result逐次输入0和1，可以初始化为全0容器，随后再修改部分的位数进行置1。但是这样就不能在使用范围for循环了，要记录迭代器的位置对结果进行标记。

取余过程可以变为n&1。在C++中，a&b表示a和b在计算机中储存的对应2进制按位取与。按照规则0&*=0，1&1=1
```
十进制7&3=3
 0111
&0011
 0011
```
故n&1=0当n为偶数，n&1=1当n为奇数。

思路参考:前额叶没长好的题解<讨论栈顶位置的奇偶，击败C++双百!>

修改后代码如下:
```
vector<int> maxDepthAfterSplit(string seq) {
    vector<int> store;//当作一个栈存放括号
    vector<int> result(seq.size(),0);
    
    for (int i =0;i<seq.size();i++){
        if (seq[i]=='('){
            store.push_back(0);//0代表存放一个括号
            //如果栈深是一个奇数，归类到A
            //栈深是一个偶数，归类到B
            if((store.size()&1)==0)
                result[i]=1;
        }
        else {
            if((store.size()&1)==0)
                result[i]=1;
            store.pop_back();
        }
    }
    return result;
}
```
需要注意的是if((store.size()&1)==0)必须加括号，否则运算顺序出错，然后就发现结果更慢了。whatever。