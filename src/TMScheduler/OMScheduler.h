#ifndef OMScheduler_H
#define OMScheduler_H

#include "../TMUtil/tmtools.h"
#include "../TMUtil/OMapGlobal.h"
#include "../TMUtil/cgeoutil.h"
#include "../TMUtil/OMGeoUtil.h"
#include <iostream>
using namespace std;

class OMScheduler
{
public:
	OMScheduler();
	OMScheduler(int zoom, Vec2d center);

	virtual ~OMScheduler();
	Vec2d center()const;
	int zoom() const;
	Recti tileBound() const;
	Rectd windowRect() const;
	double getRotation() const;

	int compute();
	
	int updateMapParameter(Vec2d center, int zoom);

	int zoomIn();
	int zoomOut();
	
	int pan(int direction);
	int rotate(double angle);


	int getTiles(vector<Vec3i>& tiles, int zoom);
	int getTilesBuffer(vector<Vec3i>& tiles, int zoom);
	bool isTileVisible(int zoom, int col, int row);

private:
	Recti _tileBound;
	Rectd _windowRect;
	int _zoom;
	Vec2d _center;
	double _rotationAngle=0.0;

	int isSameTiles();
};

#endif