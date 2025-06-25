#ifndef TMBUFFER_H
#define TMBUFFER_H

#include <string>
#include <vector>
#include "../TMDataUnit/tmdataunit.h"
#include "../TMStyle/cstyle.h"
#include "gl\gl.h"	
#include "gl\glu.h"

enum BufferType
{
	PointBuffer,
	PolylineBuffer,
	PolygonBuffer,
	TerrianBuffer,
	RasterBuffer
};

class TMBuffer
{
private:
	BufferType _type;
	std::string _index;
    
    GLuint* _Texture;
	Vertices* _cData;
	Vertices* _vData;
	Heights* _hData;
	Index* _iData;
	Text* _tData;
	Stop* _sData;
	Image* _imData;

	TMStyle::CStyle* _style;

public:
	TMBuffer(BufferType type, std::string index) : _type(type), _index(index),_Texture(0x00),_cData(0x00),_vData(0x00), _hData(0x00),
		_iData(0x00), _tData(0x00), _sData(0x00), _style(0x00) {};

	virtual ~TMBuffer();
    GLuint*				texture()	const;
	Vertices*			cData() const;

	Vertices*			vData() const;
	Heights*			hData() const;
	Index*				iData() const;
	Text*				tData() const;
	Stop*				sData() const;
	Image*				imData() const;
	TMStyle::CStyle*	style() const;

	void setData(void* data, DataType type);
	void setStyle(TMStyle::CStyle* style);
};

#endif // ! TMBUFFER_H
