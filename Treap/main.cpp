#include <iostream>
#include <random>
#include <limits>
#include <vector>
#include "test_runner.h"
using namespace std;

struct TreapNode{
    int key; // Keys are ordered as in the heap
    int value; // Values are ordered as in the binary search tree
    int cnt;
    TreapNode * left;
    TreapNode * right;
    TreapNode(int value, int key = rand()){
        this -> value = value;
        this -> key = key;
        this -> cnt = 1;
        left = right = nullptr;
    }
    ~TreapNode(){
        delete left;
        delete right;
    }
};

class Treap{
public:
    void insert(int value){
        Insert(this -> head, value);
    }
    void erase(int value){
        Erase(this -> head, value);
    }
    int lowerBound(int value){
        return lowerBound(this -> head, value);
    }
    int findNth(int number){
        return findNth(this -> head, number);
    }
    int pr(){
        cout << this -> head -> cnt << endl;
    }
    void print(){
        print(head);
    }
    ~Treap(){
       delete this -> head;
    }
private:
    TreapNode * head = nullptr;
    void Split(TreapNode * head, TreapNode * & l, TreapNode * & r, int value){
        if(!head){
            return;
        }
        if(head -> value <= value){
            l = head;
            Split(l -> right, head -> right, r, value);
        }
        else{
            r = head;
            Split(r -> left, l, head -> left, value);
        }
        updCnt(l);
        updCnt(r);
    }
    void Merge(TreapNode * & head, TreapNode * l, TreapNode * r){
        if(!l || !r){
            if(!l){
                head = r;
            }
            else{
                head = l;
            }
        }
        else if (l -> key < r -> key){
            head = r;
            Merge(head -> left, l, head -> left);
        }
        else{
            head = l;
            Merge(head -> right, l -> right, r);
        }
        updCnt(head);
    }
    void Insert(TreapNode * & head, int value){
        TreapNode * l = nullptr;
        TreapNode * r = nullptr;
        Split(head, l, r, value);
        TreapNode * oneValue = new TreapNode(value);
        Merge(l, l, oneValue);
        Merge(l, l, r);
        head = l;
    }
    void Erase(TreapNode * & head, int value){
        TreapNode * l = nullptr;
        TreapNode *mid = nullptr;
        TreapNode * r = nullptr;
        Split(head, l, mid, value - 1);
        Split(head, mid, r, value);
        delete mid;
        Merge(head, l, r);
    }
    int lowerBound(TreapNode * head, int value){
        if(!head){
            return INT_MAX;
        }
        int result = INT_MAX;
        if(head -> value < value){
            result = min(result, lowerBound(head -> right, value));
        }
        else{
            result = min(result, lowerBound(head -> left, value));
            result = min(result, head -> value);
        }
        return result;
    }
    int findNth(TreapNode * head, int number){
        if(!head || head -> cnt < number){
            return INT_MIN;
        }
        assert(cnt(head) == cnt(head -> left) + cnt(head -> right) + 1);
        int leftCnt = cnt(head -> left);
        if (leftCnt + 1 == number){
            return head -> value;
        }
        else if (leftCnt + 1 > number){
            return findNth(head -> left, number);
        }
        else{
            return findNth(head -> right, number - leftCnt - 1);
        }
    }
    void updCnt(TreapNode * head){
        if(head) {
            head->cnt = cnt(head -> left) + cnt(head -> right) + 1;
        }
    }
    int cnt(TreapNode * head){
        return head ? head -> cnt : 0;
    }
    void print(TreapNode * head){
        if(!head){
            return;
        }
        print(head -> left);
        print(head -> right);
        cout << head -> key << " " << head -> value << " " << head -> cnt << endl;
    }
};

void TestLowerBound();
void TestFindNth();

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLowerBound);
    RUN_TEST(tr, TestFindNth);
    return 0;
}

void TestLowerBound(){
    Treap treap;
    vector<int> testVector;
    for(int i = 0; i < 10; ++i){
        treap.insert(i * i);
        testVector.push_back(i * i);
    }
    testVector.push_back(INT_MAX);

    for(int i = 0; i < 100; i += 10){
        ASSERT_EQUAL(treap.lowerBound(i), *lower_bound(testVector.begin(), testVector.end(), i));
    }
}

void TestFindNth(){
    Treap treap;
    vector<int> testVector;
    for(int i = 0; i < 10; ++i){
        treap.insert(i * i);
        testVector.push_back(i * i);
    }
    testVector.push_back(INT_MAX);

    for(int i = 1; i <= 10; ++i){
        ASSERT_EQUAL(treap.findNth(i), testVector[i - 1]);
    }
}
