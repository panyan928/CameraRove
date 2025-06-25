#include "cscheduler.h"
#include <sstream>

bool CScheduler::isPositionVisible(Vec3d position)
{
    //return !(getOrCreatePerspective()->frustum_culling(position) || getOrCreateCamera()->horizon_culling(position));
	CCamera* camera = getOrCreateCamera();
	Vec3d posEye;
	openglEngine::openGLCoordinatesEngine::world2Camera(camera->eyeXYZ(), camera->center(),
		camera->up(), position, posEye);
	return !(getOrCreatePerspective()->frustum_culling(posEye) || getOrCreateCamera()->horizon_culling(position));
	//return true;
	//return getOrCreateCamera()->horizon_culling(position);
}

bool CScheduler::isTileVisible(Vec3i tile)
{
	int zoom = tile[0];
	int row  = tile[1];
	int col  = tile[2];
	
	row = pow(2, zoom) - row - 1;

	double mecatorX = 0;
	double mecatorY = 0;

	double X=0.0, Y=0.0, Z=0.0;
	CGeoUtil::getTileLeftTop(zoom, col, row, mecatorX, mecatorY);
	double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom);

	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	mecatorY -= span / 2;
	Vec3d left(X, Y, Z);

	mecatorX += span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d right(X, Y, Z);

	mecatorX -= span / 2;
	mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d bottom(X, Y, Z);

	mecatorY += span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d top(X, Y, Z);

	//mecatorX += span / 2;
	mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d center(X, Y, Z);

	mecatorX -= span / 2;
	mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d leftBottom(X, Y, Z);

	mecatorX += span;
	//mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d rightBottom(X, Y, Z);

	//mecatorX += span;
	mecatorY += span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d rightTop(X, Y, Z);

	mecatorX -= span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d leftTop(X, Y, Z);


	return (isPositionVisible(center) || isPositionVisible(left) || isPositionVisible(right) ||
		isPositionVisible(top) || isPositionVisible(bottom) || isPositionVisible(leftBottom) ||
		isPositionVisible(rightBottom) || isPositionVisible(rightTop) || isPositionVisible(leftTop));
}

bool CScheduler::isTileVisible(int zoom, int col, int row)
{
	row = pow(2, zoom) - row - 1;

	double mecatorX = 0;
	double mecatorY = 0;

	double X=0.0, Y=0.0, Z=0.0;
	CGeoUtil::getTileLeftTop(zoom, col, row, mecatorX, mecatorY);
	double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom);

	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	mecatorY -= span / 2;
	Vec3d left(X, Y, Z);

	mecatorX += span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d right(X, Y, Z);

	mecatorX -= span / 2;
	mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d bottom(X, Y, Z);

	mecatorY += span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d top(X, Y, Z);

	//mecatorX += span / 2;
	mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d center(X, Y, Z);

	mecatorX -= span / 2;
	mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d leftBottom(X, Y, Z);

	mecatorX += span;
	//mecatorY -= span / 2;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d rightBottom(X, Y, Z);

	//mecatorX += span;
	mecatorY += span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d rightTop(X, Y, Z);

	mecatorX -= span;
	CGeoUtil::WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
	Vec3d leftTop(X, Y, Z);


	return (isPositionVisible(center) || isPositionVisible(left) || isPositionVisible(right) ||
		isPositionVisible(top) || isPositionVisible(bottom) || isPositionVisible(leftBottom) ||
		isPositionVisible(rightBottom) || isPositionVisible(rightTop) || isPositionVisible(leftTop));
}

bool CScheduler::isValid(Vec3i tile, set<string>& processed, int zoom) {
	// index invalid
	if (tile[1] < 0 || tile[1] >= pow(2, zoom) || tile[2] < 0 || tile[2] >= pow(2, zoom))
		return false;
	// processed
	//string index = to_string(tile[0]) + "." + to_string(tile[1]) + "." + to_string(tile[2]);
	ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (tile[0]) << "." << (tile[1]) << "." << (tile[2]);
	string index = ost_temp.str();
	if (processed.find(index) != processed.end())
		return false;

	// invisible
	if (!isTileVisible(tile[0], tile[1], tile[2]))
		return false;

	return true;
}

bool CScheduler::isSubdivideNeeded(int zoom, int col, int row)
{
	///***************** 1. ¼ÆËãÍßÆ¬ÖÐÐÄ ****************/
	Vec3d tileCenter;
	CGeoUtil::getTileCenter(zoom, col, row, tileCenter[0], tileCenter[1], tileCenter[2]);
	
	/*********** 2. ¼ÆËãÍßÆ¬ÖÐÐÄµÄÊÓ×ø±êÏµ×ø±ê **********/
	Vec3d tileCenterEye;
	CCamera* camera = getOrCreateCamera();
	openglEngine::openGLCoordinatesEngine::world2Camera(camera->eyeXYZ(), camera->center(), camera->up(),
		tileCenter, tileCenterEye);

	/*Vec3d pt1 = camera->eyeXYZ();
	Vec3d pt2 = tileCenter;
	double dis1 = sqrt((pt1[0] - pt2[0]) * (pt1[0] - pt2[0]) +
		(pt1[1] - pt2[1]) * (pt1[1] - pt2[1]) + (pt1[2] - pt2[2]) * (pt1[2] - pt2[2]));
	double dis2 =  sqrt(tileCenterEye[0] * tileCenterEye[0] + tileCenterEye[1] * tileCenterEye[1]
		+ tileCenterEye[2] * tileCenterEye[2]);*/

	double dis1 = camera->eye()[2] + CGeoUtil::WGS_84_RADIUS_EQUATOR;
	/*********** 3. ¼ÆËãÆÁÄ»ÏñËØ¾àÀëºÍÍßÆ¬ÏñËØ¾àÀë **********/
	double distance = abs(tileCenterEye[2]);
	double height   = distance * 2 * tan((double)glm::radians(90.0 / 2));

	double screenUnit = height / 768;
	double tileUnit = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256;
	
	/*********** 4. ±È½ÏÆÁÄ»ÏñËØ¾àÀëºÍÍßÆ¬ÏñËØ¾àÀë **********/
	if (screenUnit > tileUnit)
		return false;
	else
		return true;

	///***************** 1. ¼ÆËãÍßÆ¬ÖÐÐÄ ****************/
	//Vec2d tileLeftTop;
	//CGeoUtil::getTileLeftTop(zoom, col, row, tileLeftTop[0], tileLeftTop[1]);
	//double tileUnit = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256;
	//Vec2d tileCenter(tileLeftTop[0] + 128 * tileUnit, tileLeftTop[1] - 128 * tileUnit);
	//Vec2d tileCenter1(tileLeftTop[0] + 129 * tileUnit, tileLeftTop[1] - 129 * tileUnit);

	//Vec3d ltXYZ, ctXYZ;
	//CGeoUtil::WebMercator2XYZ(tileCenter1[0], tileCenter1[1], ltXYZ[0], ltXYZ[1], ltXYZ[2]);
	//CGeoUtil::WebMercator2XYZ(tileCenter[0], tileCenter[1], ctXYZ[0], ctXYZ[1], ctXYZ[2]);

	//Vec3d ltScreen, ctScreen;
	//openglEngine::openGLCoordinatesEngine::world2Screen(ltScreen[0], ltScreen[1], ltScreen[2], ltXYZ[0], ltXYZ[1], ltXYZ[2]);
	//openglEngine::openGLCoordinatesEngine::world2Screen(ctScreen[0], ctScreen[1], ctScreen[2], ctXYZ[0], ctXYZ[1], ctXYZ[2]);

	//float ratio_x = abs(ltScreen[0] - ctScreen[0]) / 1;
	//float ratio_y = abs(ltScreen[1] - ctScreen[1]) / 1;

	//if (ratio_x > 0.5 || ratio_y > 0.5)
	//	return true;
	//return false;
}

void CScheduler::getEightConnectedTiles(queue<Vec3i>& nodes, Vec3i tile, set<string>& processed, int zoom) {
	Vec3i tile1(tile[0], tile[1] - 1, tile[2] - 1);
	Vec3i tile2(tile[0], tile[1], tile[2] - 1);
	Vec3i tile3(tile[0], tile[1] + 1, tile[2] - 1);
	Vec3i tile4(tile[0], tile[1] - 1, tile[2]);
	Vec3i tile5(tile[0], tile[1] + 1, tile[2]);
	Vec3i tile6(tile[0], tile[1] - 1, tile[2] + 1);
	Vec3i tile7(tile[0], tile[1], tile[2] + 1);
	Vec3i tile8(tile[0], tile[1] + 1, tile[2] + 1);

	if (isValid(tile1, processed, zoom))
		nodes.push(tile1);
	if (isValid(tile2, processed, zoom))
		nodes.push(tile2);
	if (isValid(tile3, processed, zoom))
		nodes.push(tile3);
	if (isValid(tile4, processed, zoom))
		nodes.push(tile4);
	if (isValid(tile5, processed, zoom))
		nodes.push(tile5);
	if (isValid(tile6, processed, zoom))
		nodes.push(tile6);
	if (isValid(tile7, processed, zoom))
		nodes.push(tile7);
	if (isValid(tile8, processed, zoom))
		nodes.push(tile8);

	//string index = to_string(tile1[0]) + "." + to_string(tile1[1]) + "." + to_string(tile1[2]);
	ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (tile1[0]) << "." << (tile1[1]) << "." << (tile1[2]);
	string index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile2[0]) + "." + to_string(tile2[1]) + "." + to_string(tile2[2]);
	ost_temp.str("");
	ost_temp << (tile2[0]) << "." << (tile2[1]) << "." << (tile2[2]);
	index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile3[0]) + "." + to_string(tile3[1]) + "." + to_string(tile3[2]);
	ost_temp.str("");
	ost_temp << (tile3[0]) << "." << (tile3[1]) << "." << (tile3[2]);
	index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile4[0]) + "." + to_string(tile4[1]) + "." + to_string(tile4[2]);
	ost_temp.str("");
	ost_temp << (tile4[0]) << "." << (tile4[1]) << "." << (tile4[2]);
	index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile5[0]) + "." + to_string(tile5[1]) + "." + to_string(tile5[2]);
	ost_temp.str("");
	ost_temp << (tile5[0]) << "." << (tile5[1]) << "." << (tile5[2]);
	index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile6[0]) + "." + to_string(tile6[1]) + "." + to_string(tile6[2]);
	ost_temp.str("");
	ost_temp << (tile6[0]) << "." << (tile6[1]) << "." << (tile6[2]);
	index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile7[0]) + "." + to_string(tile7[1]) + "." + to_string(tile7[2]);
	ost_temp.str("");
	ost_temp << (tile7[0]) << "." << (tile7[1]) << "." << (tile7[2]);
	index = ost_temp.str();
	processed.insert(index);
	//index = to_string(tile8[0]) + "." + to_string(tile8[1]) + "." + to_string(tile8[2]);
	ost_temp.str("");
	ost_temp << (tile8[0]) << "." << (tile8[1]) << "." << (tile8[2]);
	index = ost_temp.str();
	processed.insert(index);
}

int CScheduler::centerSearch(vector<Vec3i>& tiles, int zoom)
{
	Vec3d eyeXYZ, center;
	center = getOrCreateCamera()->center();
	OMGeoUtil::lonLatHeight2XYZ(getOrCreateCamera()->eye(), eyeXYZ);

	Vec3d pt1 = CGeoUtil::lineSegment_WGS84Ellipsoid_intersection(eyeXYZ, center);

	Vec3d lonlat1;
	OMGeoUtil::XYZ2lonLatHeight(lonlat1, pt1);

	Vec2i centerIndex;
	OMGeoUtil::deg2num(lonlat1, centerIndex, zoom);
	Vec3i tile(zoom, centerIndex[0], centerIndex[1]);

	queue<Vec3i> nodes;

	nodes.push(tile);
	set<string> processed;

	while (!nodes.empty()) {
		Vec3i node = nodes.front();
		tiles.push_back(node);
		nodes.pop();

		getEightConnectedTiles(nodes, node, processed, zoom);
	}

	return 0;
}

int CScheduler::quadtreeSearch(vector<Vec3i>& tiles, int zoom)
{
	CCamera* camera = getOrCreateCamera();

	queue<Vec3i> nodes;
	int level = 0;
	camera->zoomTo(level);
	double height = camera->eye()[2];
	getOrCreatePerspective()->set(height * 0.9, height * 0.9 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR,
		90, 4.0 / 3);
	if (isTileVisible(0, 0, 0))
		nodes.push(Vec3i(0, 0, 0));

	while (!nodes.empty()) {
		int size = nodes.size();
		for (int i = 0; i < size; i++) {
			Vec3i node = nodes.front();
			nodes.pop();

			camera->zoomTo(zoom);
			//if (!isSubdivideNeeded(node[0], node[1], node[2]) || level == zoom)
				//tiles.push_back(node);
			if (level == zoom)
				tiles.push_back(node);
			else {
				camera->zoomTo(level + 1);
				height = camera->eye()[2];
				getOrCreatePerspective()->set(height * 0.9, height * 0.9 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR,
					90, 4.0 / 3);

				Vec3i child1(node[0] + 1, 2 * node[1], 2 * node[2]);
				Vec3i child2(node[0] + 1, 2 * node[1] + 1, 2 * node[2]);
				Vec3i child3(node[0] + 1, 2 * node[1], 2 * node[2] + 1);
				Vec3i child4(node[0] + 1, 2 * node[1] + 1, 2 * node[2] + 1);

				if (isTileVisible(child1[0], child1[1], child1[2]))
					nodes.push(child1);

				if (isTileVisible(child2[0], child2[1], child2[2]))
					nodes.push(child2);

				if (isTileVisible(child3[0], child3[1], child3[2]))
					nodes.push(child3);

				if (isTileVisible(child4[0], child4[1], child4[2]))
					nodes.push(child4);
			}
		}

		level++;

		if (level > zoom)
			break;
	}
	camera->zoomTo(zoom);
	height = camera->eye()[2];
	getOrCreatePerspective()->set(height * 0.9, height * 0.9 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR,
		90, 4.0 / 3);
	return 0;
}

int CScheduler::traverseSearch(vector<Vec3i>& tiles, int zoom)
{
	int len = pow(2, zoom);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (isTileVisible(zoom, i, j)) {
				tiles.push_back(Vec3i(zoom, i, j));
				cout << zoom << " " << i << " " << j << " is visible" << endl;
			}	
		}
	}
	return 0;
}

int CScheduler::getTiles(vector<Vec3i>& tiles, int zoom)
{
	quadtreeSearch(tiles, zoom);
	return 0;
}

CScheduler::~CScheduler()
{
	if (_camera != 0x00)
		delete _camera;
	_camera = 0x00;

	if (_perspective != 0x00)
		delete _perspective;
	_perspective = 0x00;
}

CCamera* CScheduler::getOrCreateCamera()
{
    if (_camera == 0x00)
        _camera = new CCamera();
    return _camera;
}

CPerspective* CScheduler::getOrCreatePerspective()
{
    if (_perspective == 0x00)
        _perspective = new CPerspective();
    return _perspective;
}

int CScheduler::compute(vector<Vec3i>& tiles)
{
	int zoom = getOrCreateCamera()->zoom();
	getTiles(tiles, zoom);
	return 0;
}

int CScheduler::pan(int mode)
{
	getOrCreateCamera()->pan(mode);
	return 0;
}

int CScheduler::zoomIn()
{
	getOrCreateCamera()->zoomIn();
	return 0;
}

int CScheduler::zoomOut()
{
	getOrCreateCamera()->zoomOut();
	return 0;
}
