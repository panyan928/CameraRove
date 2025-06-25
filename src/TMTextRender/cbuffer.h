#ifndef CBUFFER_H
#define CBUFFER_H

/**
  * @file     	cbuffer.h
  * @author   	MYL yunlongma0@163.com
  * @version	V1.0
  * @date    	18-06-2021
  * @brief   	C++ buffer template class
  * @attention
  *     Caching is a common productivity solution for everything from computer \n
  * networks to operating systems and stand-alone and embedded software development. \n
  * This class provides a simple caching template based on C++ Map. It supports the \n
  * storage mode of key-value pairs, and provides basic operations such as queries, \n
  * inserts, and so on.
*/

#include <map>
#include <iostream>
using namespace std;
template <typename BufferIndex, typename BufferT>
class CBuffer
{
private:
    map<BufferIndex, BufferT*> _buffer;
    int _rear;
    int _capacity;
    /**
     * @brief remove some buffer from container
     * @param count The number of caches to remove
     */
    void removeBuffer(int count) {
        typename map<BufferIndex, BufferT*>::iterator iter;
        iter = _buffer.begin();
        while ((iter != _buffer.end()) && count--) {
            if ((*iter).second != 0x00)
                delete (*iter).second;
            (*iter).second = 0x00;
            //iter = _buffer.erase(iter);
            _buffer.erase(iter++);
            _rear--;
        }
    }
public:
    CBuffer(int capacity) {
        _rear = -1;
        _capacity = capacity;
    }
    CBuffer() {
        _rear = -1;
        _capacity = 0;
    }

    virtual ~CBuffer(){
        typename map<BufferIndex, BufferT*>::iterator iter;
        iter = _buffer.begin();
        while (iter != _buffer.end()) {
            delete (*iter).second;
            (*iter).second = 0x00;
            _buffer.erase(iter);
            iter++;
            _rear--;
        }
    }

    bool isEmpty() const {return (_rear == -1);}
    void setCapacity(int capacity) {_capacity = capacity;}

    /**
     * @brief getBufferData
     * @param index the key of the data need to fetch
     * @return the value of the data or 0x00
     */
    BufferT* getBufferData(BufferIndex index) const {
        typename map<BufferIndex, BufferT*>::const_iterator iter = _buffer.find(index);
        if (iter != _buffer.end()) {
            return (*iter).second;
        }
        return 0x00;
    }

    void addBuffer(BufferIndex index, BufferT* buffer) {
        if ((_rear + 1) < _capacity)
            _buffer.insert({index, buffer});
        else {
            removeBuffer(20);
            _buffer.insert({index, buffer});
        }
        _rear++;
    }

    void erase(BufferIndex index) {
        typename map<BufferIndex, BufferT*>::iterator iter = _buffer.find(index);
        if ((*iter).second != 0x00)
            delete (*iter).second;
        (*iter).second = 0x00;
        _buffer.erase(iter);
        _rear--;
    }
};


#endif // CBUFFER_H
