#ifndef CTILELAYER_H
#define CTILELAYER_H

#include "CGeoLayer.h"
#include "../TMUtil/OMapGlobal.h"



class CTileLayer : public CGeoLayer
{
public:
	CTileLayer();
	virtual ~CTileLayer();
	virtual int draw(Recti bounds, int zoom) = 0;
	virtual int draw(Recti bounds, int zoom, BufferManager* manager) = 0;
	virtual int draw(vector<Vec3i> tiles, int zoom, BufferManager* manager) = 0;
	virtual int drawMultiThreads(void* para) = 0;
	virtual int getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager) = 0;

protected:
	TileType _type;
};

#endif

