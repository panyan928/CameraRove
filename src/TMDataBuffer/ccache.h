#ifndef CCache_H
#define CCache_H
#include <map>
using namespace std;

template <class Key, class Value>
class ListNode {
public:
    ListNode* pre;
    ListNode* next;
    Key key;
    Value value;
    ListNode(Key key, Value val) : pre(0x00), next(0x00), key(key), value(val) {

    }
    //ListNode(Key key, Value* val) : pre(0x00), next(0x00), key(key), value(&val) {
    //    int q = 0;
    //}
    ListNode() : pre(0x00), next(0x00) {}
    ~ListNode()
    {
        Value* t = &(this->value);
        if (*t != 0x00)
        {
            delete *t;
            *t = 0x00;
        }
    }
};

template <class Key, class Value>
class CCache
{
protected:
    ListNode<Key, Value>* vHead;
    ListNode<Key, Value>* vTail;
    int _capacity;
    int _size;
    map<Key, ListNode<Key, Value>*> _map;

public:
    virtual Value get(Key key) = 0;
    virtual void put(Key key, Value value) = 0;
};

#endif // !CCache_H
