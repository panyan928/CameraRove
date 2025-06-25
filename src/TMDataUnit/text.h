#ifndef Text_H
#define Text_H

#include "dataunit.h"
#include <string>
class Text : public DataUnit
{
private:
	std::string _text;
public:
	Text(std::string text, int size, std::string index, DataType type = TEXT);
	virtual ~Text();

	void* data();
};

#endif // !Text_H