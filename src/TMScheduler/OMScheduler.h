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
	Recti _tileBound; //row,col,row,col
	 Rectd _windowRect; //minX,minY,maxX,maxY
	int _zoom;
	Vec2d _center;
	double _rotationAngle=0.0;

	// 新增成员
    Vec2d _rotatedViewportCorners[4]; // 存储旋转后的视口四角点

	int isSameTiles();
	// 辅助函数
	bool doPolygonsIntersect(Vec2d* poly1, int size1, Vec2d* poly2, int size2);
	bool isPointInPolygon(Vec2d& point, Vec2d* polygon, int size);
	bool doLineSegmentsIntersect(Vec2d& p1, Vec2d& p2, Vec2d& q1, Vec2d& q2);
};

#endif