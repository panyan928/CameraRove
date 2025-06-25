#ifndef IMAGE_H
#define IMAGE_H

#include "dataunit.h"

class Image : public DataUnit
{
public:
	Image(unsigned char* data, int size, std::string index, DataType type = IMAGE);
	~Image();
	void* data();
	void setInfo(int height, int width, int nrChannels);
	void getInfo(int& height, int& width, int& nrChannels) const;

private:
	unsigned char* _data;

	int _height;
	int _width;
	int _nrChannels;
};

#endif // !IMAGE_H