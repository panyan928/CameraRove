#ifndef LRUCache_H
#define LRUCache_H
#include "ccache.h"

template <class Key, class Value>
class LRUCache :public CCache<Key, Value>{
private:
    //ListNode<Key, Value>* this->vHead;
    //ListNode<Key, Value>* this->vTail;
    //int this->_capacity;
    //int this->_size;
    //unorderedthis->_map<int, ListNode<Key, Value>*> this->_map;

    void insertAtHead(ListNode<Key, Value>* node) {

        node->next = this->vHead->next;
        node->pre = this->vHead;
        this->vHead->next->pre = node;
        this->vHead->next = node;
        this->_size++;
    }

    void removeNode(ListNode<Key, Value>* node) {
        node->pre->next = node->next;
        node->next->pre = node->pre;
        this->_size--;
        //delete node;
    }

    ListNode<Key, Value>* removeTail() {
        ListNode<Key, Value>* node = this->vTail->pre;
        removeNode(node);
        return node;
    }
public:
    LRUCache(int capacity) {
        this->_capacity = capacity;
        this->_size = 0;
        this->vHead = new ListNode<Key, Value>();
        this->vTail = new ListNode<Key, Value>();
        this->vHead->next = this->vTail;
        this->vTail->pre = this->vHead;
    }

    Value get(Key key) {
        typename map<Key, ListNode<Key, Value>*>::iterator iter = this->_map.find(key);
        if (iter != this->_map.end()) {
            // move to head
            removeNode(iter->second);
            insertAtHead(iter->second);
            return iter->second->value;
        }
        else
            return 0x00;
    }

    void erase(Key key) {
        typename map<Key, ListNode<Key, Value>*>::iterator iter = this->_map.find(key);
        if (iter != this->_map.end()) {
            removeNode(iter->second);
            this->_map.erase(iter->second->key);
            if (iter->second != 0x00) {
                delete iter->second;
                iter->second = 0x00;
            }
        }
    }

    void put(Key key, Value value) {
        typename map<Key, ListNode<Key, Value>*>::iterator iter = this->_map.find(key);
        if (iter != this->_map.end()) {
            iter->second->value = value;
            // move to head
            removeNode(iter->second);
            insertAtHead(iter->second);
        }
        else {
            ListNode<Key, Value>* node = new ListNode<Key, Value>(key, value);
            insertAtHead(node);
            this->_map.insert({ key, node });
            if (this->_size > this->_capacity) {
                ListNode<Key, Value>* tmp = removeTail();
                this->_map.erase(tmp->key);
                if (tmp != 0x00) {
                    delete tmp;
                    tmp = 0x00;
                }
            }
        }
    }
};

#endif // !LRUCache_H