#ifndef OM3DSCHEDULER_H
#define OM3DSCHEDULER_H

#include "../TMUtil/tmtools.h"
#include "../TMUtil/cgeoutil.h"
#include "../TMUtil/OMGeoUtil.h"
#include "../OpenGLEngine/openglengine.h"
#include <set>
#include <queue>
#include <sstream>

class OM3DScheduler
{
public:
	OM3DScheduler();
	OM3DScheduler(Vec2d center);
	~OM3DScheduler();

	int zoom() const;
	void setZoom(int zoom);
	Recti tileBound() const;
	Vec3d eyeXYZ() const;
	Vec3d center() const;
	Vec3d eye() const;
	Vec3d up() const;
	Vec2d offset() const;
    vector<Vec3i> tileSet() const;
    int scale;
	int _zoom;
	int _isFirstPerson;
	int compute();
	int compute(int zoom);
	int compute(int zoom, vector<Vec3i>& tiles);
    int computeWider(int zoom, vector<Vec3i>& tiles,set<string> processed);
	int computeWider(vector<Vec3i>& tiles);
	int computeWider(int zoom, vector<Vec3i>& tiles);
    int isSameTiles();
	//int updateMapParameter(Vec3d _eye, Vec3d center, Vec3d up);
    int eyeSet(Vec3d eye);
	int zoomIn();
	int zoomOut();

	//10.27增加了参数
	int changeViewer(int mode,float pitch);

	int pan(int direction);
	int translate(int direction);
	int changeYaw(double angle);
	int changePitch(double angle);
	void updateFrustum(double bottom, double top, double left, double right, double zNear, double zfar);
    void updateFrustum(double fovy, double aspect, double zNear, double zFar);
	void updateFrustumForCalculation(double bottom, double top, double left, double right, double zNear, double zfar);
    void updateFrustumForCalculation();

	bool isVisible(int zoom, int col, int row);
	bool isVisible(int zoom, int col, int row, size_t sample);
	bool isVisible(int zoom, int col, int row, Vec3d eye);
	bool isTileVisible(int zoom, int col, int row, int isdraw=0);
	int rotate(double angle);

	int getTiles(vector<Vec3i>& tiles, int zoom, int isdraw = 0);
    int getTilesEn(vector<Vec3i>& tiles, int zoom,set<string> processed, int isdraw=0);
	void getEightConnectedTiles(queue<Vec3i>& nodes, Vec3i tile, set<string>& processed, int zoom, int isdraw=0);
    
    void getEightConnectedTilesWithoutCull(vector<Vec3i>& tiles, Vec3i tile, set<string>& processed, int zoom);
private:
	Vec3d _eye;				/** (lon, lat, height) of the camera 角度*/
	Vec3d _eyeXYZ;			/** (x, y, z) of the camera */
	Vec3d _eye_tmp;			/** (lon, lat, height) of the camera  */
	Vec3d _eyeXYZ_tmp;			/** (x, y, z) of the camera   */

	Vec3d _center;			
	Vec3d _up;
	vector<Vec3i> _tileSet;
    vector<Vec3i> _lastTileSet;
	Vec3d _earthObservedCenter;

	Recti _tileBound;
	int   _viewer;

	Vec2d _offset;

	//10.27增加了参数
	float pitch_ortho = 2.5;

	double _yaw ;
	double _pitch ;
	
	OrthoFrustum _frustum;
	OrthoFrustum _frustum_tmp;
	double _frustum1[4];
	double _frustum_tmp1[4];

	vector<string> _tiles;

	bool isCovered(int row, int col, double tangentLength);
	Vec3d lineSegment_WGS84Ellipsoid_intersection(Vec3d p0, Vec3d p1,
		Vec3d center = Vec3d(), double a = CGeoUtil::WGS_84_RADIUS_EQUATOR, double b = CGeoUtil::WGS_84_RADIUS_POLAR);

	bool frustum_culling(Vec3d tile);
	bool horizon_culling(Vec3d tile);
	bool isPositionVisible(Vec3d pt);
	bool isPositionVisible(Vec3d pt, Vec3d eye, int zoom);

	

	int traverseSearch(vector<Vec3i>& tiles, int zoom);
	int quadtreeSearch(vector<Vec3i>& tiles, int zoom);
	int centerSearch(vector<Vec3i>& tiles, int zoom, int isdraw=0);
    int centerSearchEn(vector<Vec3i>& tiles, int zoom,set<string> processed, int isdraw=0);

	bool isValid(Vec3i tile, set<string>& processed, int zoom, int isdraw=0);
    bool isValidWithoutCull(Vec3i tile, set<string>& processed, int zoom);

};

#endif // !OM3DSCHEDULER_H

