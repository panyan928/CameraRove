#ifndef CSCHEDULER_H
#define CSCHEDULER_H

#include "ccamera.h"
#include "cperspective.h"
#include <queue>
#include <set>
#include "../TMUtil/OMapGlobal.h"


class CScheduler
{
private:
	CCamera* _camera;
	CPerspective* _perspective;

	bool isPositionVisible(Vec3d position);
	bool isTileVisible(Vec3i tile);
	
	int centerSearch(vector<Vec3i>& tiles, int zoom);
	int quadtreeSearch(vector<Vec3i>& tiles, int zoom);
	int traverseSearch(vector<Vec3i>& tiles, int zoom);

	int getTiles(vector<Vec3i>& tiles, int zoom);
	void getEightConnectedTiles(queue<Vec3i>& nodes, Vec3i tile, set<string>& processed, int zoom);
	bool isValid(Vec3i tile, set<string>& processed, int zoom);

	bool isSubdivideNeeded(int zoom, int col, int row);

public:
	~CScheduler();

	CCamera* getOrCreateCamera();
	CPerspective* getOrCreatePerspective();

	bool isTileVisible(int zoom, int col, int row);
	int compute(vector<Vec3i>& tiles);

	int pan(int mode);
	int zoomIn();
	int zoomOut();

	int rotate();
};

#endif

