#ifndef Vertices_H
#define Vertices_H

#include "dataunit.h"

class Vertices : public DataUnit
{
private:
	float* _pts;
public:
	Vertices(float* pts, int size, std::string index, DataType type = VERTICE);
	virtual ~Vertices();

	void* data();
};
#endif // !Vertices_H
