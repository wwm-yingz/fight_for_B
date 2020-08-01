#include <stack>
#include <vector>
using namespace std;
class CQueue {
public:
    CQueue() {

    }
    
    void appendTail(int value) {
        pushStock.push(value);
    }
    
    int deleteHead() {
        if (pushStock.empty())
            return -1;
        while(!pushStock.empty()){//非空
            popStock.push(pushStock.top());
            pushStock.pop();
        }
        int result;
        result = popStock.top();
        popStock.pop();
        while(!popStock.empty()){//非空
            pushStock.push(popStock.top());
            popStock.pop();
        }
        return result;

    }

private:
    stack<int> pushStock ;
    stack<int> popStock;
    
};