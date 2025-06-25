#ifndef Index_h
#define Index_h

#include "dataunit.h"
class Index : public DataUnit
{
private:
	int* _indexData;
public:
	Index(int* indexData, int size, std::string index, DataType type = STOP);
	virtual ~Index();

	void* data();
};

#endif // !Index_h