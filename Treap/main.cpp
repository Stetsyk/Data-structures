#include <iostream>
#include <random>
#include <limits>
#include <vector>
#include "test_runner.h"
using namespace std;

struct Treap{
    int key; // Keys are ordered as in the heap
    int value; // Values are ordered as in the binary search tree
    Treap * left;
    Treap * right;
    Treap(int value, int key = rand()){
        this -> value = value;
        this -> key = key;
        left = right = nullptr;
    }
    ~Treap(){
        deleteTreap(this);
    }
private:
    void deleteTreap(Treap * head){
        if(head -> left){
            deleteTreap(head -> left);
        }
        if(head -> right){
            deleteTreap(head -> right);
        }
        delete head;
    }

};
void Split(Treap * head, Treap * & l, Treap * & r, int value){
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
}
void Merge(Treap * & head, Treap * l, Treap * r){
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
}
void Insert(Treap * & head, int value){
    Treap * l = nullptr;
    Treap * r = nullptr;
    Split(head, l, r, value);
    Treap * oneValue = new Treap(value);
    Merge(l, l, oneValue);
    Merge(l, l, r);
    head = l;
}
void Erase(Treap * & head, int value){
    Treap * l = nullptr;
    Treap *mid = nullptr;
    Treap * r = nullptr;
    Split(head, l, mid, value - 1);
    Split(head, mid, r, value);
    delete mid;
    Merge(head, l, r);
}
int lowerBound(Treap * head, int value){
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
void TestTreap(){
    unique_ptr<Treap> headTreap = nullptr;
    Treap * head = headTreap.get();
    vector<int> testVector;
    for(int i = 0; i < 10; ++i){
        Insert(head, i * i);
        testVector.push_back(i * i);
    }
    testVector.push_back(INT_MAX);

    for(int i = 0; i < 100; i += 10){
        ASSERT_EQUAL(lowerBound(head, i),
                *lower_bound(testVector.begin(), testVector.end(), i));
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTreap);
    return 0;
}
