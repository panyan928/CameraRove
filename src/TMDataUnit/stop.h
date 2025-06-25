#ifndef Stop_H
#define Stop_H

#include "dataunit.h"
class Stop : public DataUnit
{
private:
	int* _stop;
public:
	Stop(int* stop, int size, std::string index, DataType type = STOP);
	virtual ~Stop();

	void* data();
};

#endif // !Stop_H