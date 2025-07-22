#include "buffermanager.h"

int BufferManager::insert_2_level_Buffer(TMBuffer* data, string index)
{
	//_2_level_buffer->addBuffer(index, data);
	_2_level_buffer->put(index, data);
	return 0;
}

int BufferManager::insert_3_level_Buffer(DataUnit* data, string index)
{
	//_3_level_buffer->put(index, data);
	return 0;
}

BufferManager::BufferManager()
{
	/*_2_level_buffer = new CBuffer<string, TMBuffer>;
	_3_level_buffer = new CBuffer<string, DataUnit>;*/

	_2_level_buffer = new LRUCache< string, TMBuffer*>(500);
	//_3_level_buffer = new LRUCache< string, DataUnit*>(1000);
}

BufferManager::BufferManager(int _2_level_capacity)
{
	_2_level_buffer = new LRUCache< string, TMBuffer*>(_2_level_capacity);
	//_3_level_buffer = new LRUCache< string, DataUnit*>(_3_level_capacity);
	//_2_level_buffer->setCapacity(_2_level_capacity);
	//_3_level_buffer->setCapacity(_3_level_capacity);
}

BufferManager::~BufferManager()
{
	//if (_2_level_buffer != 0x00)
	//	delete _2_level_buffer;
	//_2_level_buffer = 0x00;

	//if (_3_level_buffer != 0x00)
	//	delete _3_level_buffer;
	//_3_level_buffer = 0x00;
}

int BufferManager::size(){
	return _2_level_buffer->_size;
}

void BufferManager::clear(){
	_2_level_buffer->clear();
}

void BufferManager::setCapacity(int level2, int level3)
{
	//_2_level_buffer->setCapacity(level2);
	//_3_level_buffer->setCapacity(level3);
}

int BufferManager::insert(int level, void* data, string index)
{
	switch (level)
	{
	case 2:
		insert_2_level_Buffer(static_cast<TMBuffer*>(data), index);
		break;
	case 3:
		//insert_3_level_Buffer(static_cast<DataUnit*>(data), index);
		break;
	default:
		break;
	}
	return 0;
}

void BufferManager::remove(int level, string index)
{
	switch (level)
	{
	case 2:
		//_2_level_buffer->erase(index);
		break;
	case 3:
		//_3_level_buffer->erase(index);
		break;
	default:
		break;
	}
}

TMBuffer* BufferManager::getFrom2LevelBuffer(string index)
{
	//return _2_level_buffer->getBufferData(index);
	return _2_level_buffer->get(index);
}

DataUnit* BufferManager::getFrom3LevelBuffer(string index)
{
	//return _3_level_buffer->getBufferData(index);;
	//return _3_level_buffer->get(index);
	return NULL;
}
