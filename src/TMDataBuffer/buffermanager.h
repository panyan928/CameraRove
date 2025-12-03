#ifndef BufferManager_H
#define BufferManager_H
#include "../TMTextRender/cbuffer.h"

#include "tmbuffer.h"
#include "LRUCache.h"
#include <string>

using namespace std;

class BufferManager
{
private:
	//CBuffer<string, TMBuffer>* _2_level_buffer;
	//CBuffer<string, DataUnit>* _3_level_buffer;

	LRUCache< string, TMBuffer*>* _2_level_buffer;
	//LRUCache< string, DataUnit*>* _3_level_buffer;

	int insert_2_level_Buffer(TMBuffer* data, string index);
	int insert_3_level_Buffer(DataUnit* data, string index);
	
	void* _mutexHandle;

public:
	BufferManager();
	BufferManager(int _2_level_capacity);
	virtual ~BufferManager();
	void clear();

	int size();
	void setCapacity(int level2, int level3);
	int insert(int level, void* data, string index);
	void remove(int level, string index);

	
	TMBuffer* getFrom2LevelBuffer(string index);
	DataUnit* getFrom3LevelBuffer(string index);
};

#endif // !BufferManager_H
  