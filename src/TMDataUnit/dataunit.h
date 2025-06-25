#ifndef DataUnit_H
#define DataUnit_H
#include <string>

enum DataType
{
	TEXTURE,
	COLOR,
	VERTICE,
	INDEX,
	STOP,
	TEXT,
	HEIGHT,
	IMAGE
};

class DataUnit
{
protected:
	int _size;
	DataType _type;
	std::string _index;

public:
	DataUnit();
	virtual ~DataUnit();

	int size() const;
	DataType type() const;
	std::string index() const;

	virtual void* data() = 0;
};

#endif // !DataUnit_H

