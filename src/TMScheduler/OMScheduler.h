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
	OMScheduler(float zoom, Vec2d center) : _zoom(zoom), _center(center) {};

	virtual ~OMScheduler();

	float zoom() const;
	Recti tileBound() const;
	Rectd windowRect() const;

	int compute();
	
	int updateMapParameter(Vec2d center, float zoom);

	int zoomIn();
	int zoomOut();

	int pan(int direction);

private:
	Recti _tileBound;
	Rectd _windowRect;
	float _zoom;
	Vec2d _center;
};

#endif