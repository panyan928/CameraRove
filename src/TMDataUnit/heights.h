#ifndef Heights_H
#define Heights_H


#include "dataunit.h"
class Heights : public DataUnit
{
private:
	short int* _heights;
public:
	Heights(short int* heights, int size, std::string index, DataType type = HEIGHT);
	virtual ~Heights();

	void* data();
};

#endif // !Heights_H