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
        if (!node) return;
        node->next = this->vHead->next;
        node->pre = this->vHead;
        this->vHead->next->pre = node;
        this->vHead->next = node;
        this->_size++;
    }

    void removeNode(ListNode<Key, Value>* node) {
        if (!node || !node->pre || !node->next) return;
        node->pre->next = node->next;
        node->next->pre = node->pre;
        this->_size--;
        //delete node;
    }

    ListNode<Key, Value>* removeTail() {
		if (this->vTail->pre == this->vHead) return nullptr;
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
        this->vHead->pre = nullptr;
        this->vTail->pre = this->vHead;
        this->vTail->next = nullptr;
    }

    void clear(){  
        while (this->vHead->next != this->vTail) {
            ListNode<Key, Value>* node = this->vHead->next;
            this->vHead->next = node->next;
            node->next->pre = this->vHead;
            this->_map.erase(node->key);
            delete node;
        }
        this->_size = 0;
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
            //int size = this->_map.size();
            //cout << "node size: " << this->_size << " map size: " << size << endl;
            //if (size < this->_size) {
            //    int index = 0;
            //    for (node = this->vHead->next; node->next != 0x00; node = node->next) {
            //            //cout << "key:" << node->key << endl;
            //        iter = this->_map.find(node->key);
            //        if (iter == this->_map.end()) cout << "node: " << node->key << endl;
            //        index++;
            //    }
            //    cout << "real node size: " << index << endl;
            //}
            //if (size > this->_size) { 
            //    
            //    for (iter = this->_map.begin(); iter != this->_map.end(); iter++) {
            //        int index = 0;
            //        for (node = this->vHead->next; node->next != 0x00; node = node->next) {
            //            index++;
            //            if (node->key == iter->first) {
            //                cout << node->key << " find index: " << index << endl;
            //                break;
            //            }
            //        }
            //        if (node->next == 0x00)//到最后一个都没找到
            //        {
            //            cout << "map: " << iter->first << endl;
            //        }
            //        if(node->next == this->vTail) cout << "real node size:" << index << endl;
            //    }
            //    
            //}
            if (this->_size > this->_capacity) {
                ListNode<Key, Value>* tmp = removeTail();
                if(tmp!= nullptr){
                    this->_map.erase(tmp->key);
                    delete tmp;
                    tmp = 0x00;
                }
                
            }
        }
    }
};

#endif // !LRUCache_H