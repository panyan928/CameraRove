#include "OM3DScheduler.h"
#include <sstream>
#include <math.h>

#define STRIDE 1
//10.27更改了zoom等级
OM3DScheduler::OM3DScheduler()
{
	//_eye[2] = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR;
	_eye[0] = 115.89; _eye[1] = 28; _eye[2] = 1 * CGeoUtil::WGS_84_RADIUS_EQUATOR;
	//_eye[0] = 0; _eye[1] = 0; _eye[2] = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR;
	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);
	_up[2] = 1;
	_zoom = 3;
	scale = _zoom - (STRIDE);
	_viewer = 0;
	_isFirstPerson = 0;
	_yaw = 0.0;
	_pitch = 0.0;
}
//10.27更改了zoom等级
OM3DScheduler::OM3DScheduler(Vec2d center)
{
	_eye[0] = center[0]; _eye[1] = center[1];
	_eye[2] = 1 * CGeoUtil::WGS_84_RADIUS_EQUATOR;
	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);
	_up[2] = 1;
	_zoom = 3;
	scale = _zoom - (STRIDE);
	_viewer = 0;
	_isFirstPerson = 0;
	_yaw = 0.0;
	_pitch = 0.0;
}

OM3DScheduler::~OM3DScheduler()
{
}

int OM3DScheduler::zoom() const
{
    return _zoom;
}

void OM3DScheduler::setZoom(int zoom)
{
	_zoom = zoom;
	return;
}

Recti OM3DScheduler::tileBound() const
{
    return _tileBound;
}

Vec3d OM3DScheduler::eyeXYZ() const
{
	return _eyeXYZ;
}

Vec3d OM3DScheduler::center() const
{
	return _center;
}

Vec3d OM3DScheduler::eye() const
{
	return _eye;
}

Vec3d OM3DScheduler::up() const
{
	return _up;
}

Vec2d OM3DScheduler::offset() const
{
	return _offset;
}

//vector<Vec3i> OM3DScheduler::tileSet() const
//{
//	return _tileSet;
//}


int OM3DScheduler::compute() {	
	while (_eye[0] > 180)
		_eye[0] -= 360;
	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);

	return 0;
}

//int OM3DScheduler::compute(int zoom)
//{
//	Vec3d eye = _eye;
//	while (eye[0] > 180)
//		eye[0] -= 360;
//	double height = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale);
//	eye[2] = height;
//
//	OMGeoUtil::lonLatHeight2XYZ(eye, _eyeXYZ);
//
//	//memcpy(&_frustum_tmp, &_frustum, sizeof(OrthoFrustum));
//	//memcpy(&_eye_tmp, &_eye, sizeof(Vec3d));
//	//OMGeoUtil::lonLatHeight2XYZ(_eye_tmp, _eyeXYZ_tmp);
//	updateFrustumForCalculation(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1],
//		CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1], -height * 1.5, height * 1 + 1 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
//
//	getTiles(_tileSet, zoom);
//
//	return 0;
//}

int OM3DScheduler::isSameTiles(){
	_tileSet.clear();
    getTiles(_tileSet, _zoom, 1);
    
    int len=_tileSet.size(),lastLen=_lastTileSet.size();
    if(len != lastLen){
		_lastTileSet = _tileSet;
        return 0;
    }
    else {
        if (len != 0) {
            for (int i=0;i<len;i++) {
                if ((_tileSet[i][0] != _lastTileSet[i][0]) ||\
                    (_tileSet[i][1] != _lastTileSet[i][1]) || \
                    (_tileSet[i][2] != _lastTileSet[i][2])
                    ) {
					_lastTileSet = _tileSet;
                    return 0;
                }
            }
            return 1;
        }
        else {
            return 1;
        }
    }
}

int OM3DScheduler::computeWider(vector<Vec3i>& tiles)
{
	int len = tiles.size();
	int maxX = -99999999999;
	int minX = 99999999999;
	int maxY = -99999999999;
	int minY = 99999999999;
	for (int i = 0; i < len; i++) {
		maxX = (maxX > tiles[i][1] ? maxX : tiles[i][1]);
		minX = (minX < tiles[i][1] ? minX : tiles[i][1]);
		maxY = (maxY > tiles[i][2] ? maxY : tiles[i][2]);
		minY = (minY < tiles[i][2] ? minY : tiles[i][2]);
	}

	int limitMin = 0, limitMax = pow(2, tiles[0][0]);
	if (maxX < limitMax) maxX += 1;
	if (maxY < limitMax) maxY += 1;
	if (minX > limitMin) minX -= 1;
	if (minY > limitMin) minY -= 1;

	for (int i = minX; i <= maxX; i++) {
		tiles.push_back(Vec3i(tiles[0][0], i, maxY));
		tiles.push_back(Vec3i(tiles[0][0], i, minY));
	}
	for (int i = (minY+1); i < maxY; i++) {
		tiles.push_back(Vec3i(tiles[0][0], minX, i));
		tiles.push_back(Vec3i(tiles[0][0], maxX, i));
	}	
	return 0;
}

int OM3DScheduler::computeWider(int zoom, vector<Vec3i>& tiles,set<string> processed)
{	
	double height = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale);
	updateFrustumForCalculation(-5.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], 5.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], -1.5*CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1],
		1.5*CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1], -height * 1.5, height * 1 + 1 * CGeoUtil::WGS_84_RADIUS_EQUATOR);

	getTilesEn(tiles, zoom,processed);

	return 0;
}

int OM3DScheduler::computeWider(int zoom, vector<Vec3i>& tiles)
{
	double height = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale);
	updateFrustumForCalculation(-5.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], 5.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], -1.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1],
		1.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1], -height * 1.5, height * 1 + 1 * CGeoUtil::WGS_84_RADIUS_EQUATOR);

	getTiles(tiles, zoom);

	return 0;
}

int OM3DScheduler::compute(int zoom, vector<Vec3i>& tiles)
{
	Vec3d eye = _eye;
	while (eye[0] > 180)
		eye[0] -= 360;
	double height = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale);
	eye[2] = height;

	//OMGeoUtil::lonLatHeight2XYZ(eye, _eyeXYZ_tmp);

	//memcpy(&_frustum_tmp, &_frustum, sizeof(OrthoFrustum));
	//memcpy(&_eye_tmp, &_eye, sizeof(Vec3d));
	//OMGeoUtil::lonLatHeight2XYZ(_eye_tmp, _eyeXYZ_tmp);
	if (_isFirstPerson) {		
    #if 0
        updateFrustumForCalculation(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1],
			CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1], 0, height * 1 / 3);
    #else
        updateFrustumForCalculation();
    #endif
	}
	else {
		updateFrustumForCalculation(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1],
			CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - scale) + _offset[1], -height * 1.5, height * 1 + 1 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
    }
	

	getTiles(tiles, zoom);

	return 0;
}

//int OM3DScheduler::compute()
//{
//	while (_eye[0] > 180)
//		_eye[0] -= 360;
//
//	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);
//	//_zoom = CGeoUtil::computeTileLevel(_eye[2]);
//
//	/*step1: 计算观察点到与地球相切的切点的长度*/
//	double tangentLength = sqrt(_eyeXYZ[0] * _eyeXYZ[0] + _eyeXYZ[1] * _eyeXYZ[1] + _eyeXYZ[2] * _eyeXYZ[2]
//		- CGeoUtil::WGS_84_RADIUS_EQUATOR * CGeoUtil::WGS_84_RADIUS_EQUATOR);
//
//	/*step2: 计算相机位置对应的地表位置所在的瓦片编号*/
//	int* index = new int[2];
//	int zoom_i = static_cast<int> (_zoom);
//	CGeoUtil::deg2num(_eye[1], _eye[0], zoom_i, index);
//
//	/*step3: 以中心瓦片为基础，向上下左右四方向延伸，直到瓦片被地平线遮挡*/
//	_tileBound[0] = index[0]; _tileBound[1] = index[1];
//	_tileBound[2] = index[0]; _tileBound[3] = index[1];
//
//	while (_tileBound[0] > 0 && _tileBound[1] > 0 && isCovered(_tileBound[0], _tileBound[1], tangentLength) == false) {
//		_tileBound[0]--;
//		_tileBound[1]--;
//	}
//
//	while (_tileBound[2] < pow(2,zoom_i) - 1 && _tileBound[3] < pow(2, zoom_i) - 1 && isCovered(_tileBound[2], _tileBound[3], tangentLength) == false) {
//		_tileBound[2]++;
//		_tileBound[3]++;
//	}
//
//	if (index != 0x00)
//		delete[] index;
//	index = 0x00;
//
//	return 0;
//}

int OM3DScheduler::eyeSet(Vec3d eye)
{
    _eye = eye;
    if(isSameTiles())
        return 1;
    else
        return 0;
}

int OM3DScheduler::zoomIn()
{
	_zoom++;
	//scale++;
    _eye[2] = _eye[2] / 2;
	if (isSameTiles())
		return 1;
	else
		return 0;
}

int OM3DScheduler::zoomOut()
{
	_zoom--;
	//scale--;
    _eye[2] = _eye[2] * 2;
	if (isSameTiles())
		return 1;
	else
		return 0;
}


//10.27增加了参数
int OM3DScheduler::changeViewer(int mode,float pitch)
{
	_viewer = mode;
	this->pitch_ortho = pitch;
	if (mode == 0) {
		memset(&_center, 0, sizeof(Vec3d));
	}
	else if (mode == 1) {
		Vec3d center(_eye[0], _eye[1], 0);
		//center[1] += 2.5 * 45 * pow(2, log((_eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR)) / log(2));
		center[1] +=  pitch* 45 * _eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR;
		//memcpy(&_center_tmp, &_eye, sizeof(Vec3d));
		OMGeoUtil::lonLatHeight2XYZ(center, _center);
	}


	if(isSameTiles())
        return 1;
    else
        return 0;
}

int OM3DScheduler::pan(int direction)
{
	int zoom_i = static_cast<int> (_zoom);
	double unit = 360.0 / pow(2, zoom_i) / 256*5;

	switch (direction)
	{
	case 1:
		_eye[0] -= unit;
		break;
	case 2:
		if ((_eye[1] - unit) >= -55)
			_eye[1] -= unit;
		break;
	case 3:
		_eye[0] += unit;
		break;
	case 4:
		if ((_eye[1] + unit) <= 55)
			_eye[1] += unit;
		break;
	default:
		break;
	}

	if (_viewer == 1) {
		Vec3d center(_eye[0], _eye[1], 0);
		//center[1] += 2.5 * 45 * pow(2, log((_eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR)) / log(2));
		center[1] += pitch_ortho * 45 * _eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR;
		OMGeoUtil::lonLatHeight2XYZ(center, _center);
	}
	if (isSameTiles())
		return 1;
	else
		return 0;
}

int OM3DScheduler::translate(int direction)
{
	int zoom_i = static_cast<int> (_zoom);
	double unit = 2 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom_i) / 768;

	switch (direction)
	{
	case 1:
		_offset[0] -= 64 * unit;
		break;
	case 2:
		_offset[1] -= 64 * unit;
		break;
	case 3:
		_offset[0] += 64 * unit;
		break;
	case 4:
		_offset[1] += 64 * unit;
		break;
	default:
		break;
	}
	return 0;
}

void OM3DScheduler::updateFrustum(double left, double right, double bottom, double top, double zNear, double zfar)
{
	_frustum[0] = left;
	_frustum[1] = right; 
	_frustum[2] = bottom;
	_frustum[3] = top;
	_frustum[4] = zNear;
	_frustum[5] = zfar;
}

void OM3DScheduler::updateFrustum(double fovy, double aspect, double zNear, double zFar)
{
	_frustum1[0] = fovy;
	_frustum1[1] = aspect;
	_frustum1[2] = zNear;
	_frustum1[3] = zFar;
}

int OM3DScheduler::changeYaw(double angle)
{
	glm::dvec3 cameraPos = glm::dvec3(_eyeXYZ[0], _eyeXYZ[1], _eyeXYZ[2]);
	glm::dvec3 cameraTarget = glm::dvec3(_center[0], _center[1], _center[2]);
	glm::dvec3 up = glm::dvec3(_up[0], _up[1], _up[2]);
	glm::dvec3 cameraDirection = cameraPos - cameraTarget;
	glm::dvec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::dvec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::dmat4 trans = glm::dmat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle), up);
	glm::dvec3 front = trans * glm::dvec4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);
	glm::dvec3 newCameraTarget = cameraPos - front;

    _center[0] = newCameraTarget.x;
	_center[1] = newCameraTarget.y;
	_center[2] = newCameraTarget.z;
    cameraUp = glm::cross(front, cameraRight);
	_up[0] = cameraUp.x;
	_up[1] = cameraUp.y;
	_up[2] = cameraUp.z;
	
	if(isSameTiles())
        return 1;
    else
        return 0;
}

void OM3DScheduler::updateFrustumForCalculation(double left, double right, double bottom,
	double top, double zNear, double zfar) {
	_frustum_tmp[0] = left;
	_frustum_tmp[1] = right;
	_frustum_tmp[2] = bottom;
	_frustum_tmp[3] = top;
	_frustum_tmp[4] = zNear;
	_frustum_tmp[5] = zfar;
}

void OM3DScheduler::updateFrustumForCalculation()
{
    _frustum_tmp1[0] = _frustum1[0]+5.0;
	_frustum_tmp1[1] = _frustum1[1];
	_frustum_tmp1[2] = _frustum1[2];
	_frustum_tmp1[3] = _frustum1[3];
}   


int OM3DScheduler::changePitch(double angle)
{
	
	glm::dvec3 cameraPos = glm::dvec3(_eyeXYZ[0], _eyeXYZ[1], _eyeXYZ[2]);
	glm::dvec3 cameraTarget = glm::dvec3(_center[0], _center[1], _center[2]);
	glm::dvec3 up = glm::dvec3(_up[0], _up[1], _up[2]);
	glm::dvec3 cameraDirection = cameraPos - cameraTarget;
	glm::dvec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::dvec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::dmat4 trans = glm::dmat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle- _pitch), cameraRight);
	glm::dvec3 front = trans * glm::dvec4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);
	glm::dvec3 newCameraTarget = cameraPos - front;
    
	_center[0] = newCameraTarget.x;
	_center[1] = newCameraTarget.y;
	_center[2] = newCameraTarget.z;    
    cameraUp = glm::cross(front, cameraRight);
	_up[0] = cameraUp.x;
	_up[1] = cameraUp.y;
	_up[2] = cameraUp.z;
    
	_pitch = angle;

	if(isSameTiles())
        return 1;
    else
        return 0;
}

bool OM3DScheduler::isCovered(int row, int col, double tangentLength)
{
	int zoom_i = static_cast<int>(_zoom);
	if (row < 0 || col < 0 || row >= pow(2, zoom_i) || col >= pow(2, zoom_i))
		return false;
	
	/*step1: 计算该瓦片的中心坐标，以此代替瓦片包围球进行计算，效果更好*/
	Vec3d tileCenter;
	CGeoUtil::getTileCenter(zoom_i, col, row, tileCenter[0], tileCenter[1], tileCenter[2]);
	
	/*step2: 计算相机到瓦片中心点的距离*/
	double distance = sqrt((_eyeXYZ[0] - tileCenter[0]) * (_eyeXYZ[0] - tileCenter[0]) + (_eyeXYZ[1] - tileCenter[1] *
		(_eyeXYZ[1] - tileCenter[1]) + (_eyeXYZ[2] - tileCenter[2]) * (_eyeXYZ[2] - tileCenter[2])));

	/*step3: 比较 tangentLength 和 distance*/
	/*
	* tangentLength > distance  说明瓦片被地平线遮挡，不需要被加到渲染队列
	* tangentLength <= distance 说明瓦片在视野范围内，需要被加到渲染队列
	*/
	if (tangentLength > distance)
		return true;
	else
		return false;
}

Vec3d OM3DScheduler::lineSegment_WGS84Ellipsoid_intersection(Vec3d p0, Vec3d p1, Vec3d center, double a, double b)
{
	double x0 = p0[0], y0 = p0[1], z0 = p0[2];
	double x1 = p1[0], y1 = p1[1], z1 = p1[2];
	double cx = center[0], cy = center[1], cz = center[2];
	double m = x1 - x0, n = y1 - y0, p = z1 - z0;
	double A = (m * m + n * n) / (a * a) + p * p / (b * b);
	double B = 2 * ((m * (x0 - cx) + n * (y0 - cy)) / (a * a) + p * (z0 - cz) / (b * b));
	double C = ((x0 - cx) * (x0 - cx) + (y0 - cy) * (y0 - cy)) / (a * a) + (z0 - cz) * (z0 - cz) / (b * b) - 1;
	double test = B * B - 4.0 * A * C;
	if (test >= 0.0)
	{
		double t0 = (-B - sqrt(test)) / (2.0 * A);
		double t1 = (-B + sqrt(test)) / (2.0 * A);
		Vec3d lineNormal(m, n, p);
		// 其实有两个解，根据你的需要选择t0还是t1。
		//Vec3d hitp = lineNormal * t0 + p0;
		Vec3d hitp;
		hitp[0] = t0 * m + p0[0];
		hitp[1] = t0 * n + p0[1];
		hitp[2] = t0 * p + p0[2];
		return hitp;
	}
	return Vec3d(0, 0, 0);
}

bool OM3DScheduler::isVisible(int zoom, int col, int row)
{
	Vec2i centerIndex;
	OMGeoUtil::deg2num(_earthObservedCenter, centerIndex, zoom);
	if (col == centerIndex[0] && row == centerIndex[1])
		return true;

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

	// 只要有一个边界点在视域内则该瓦片可见
	return (isPositionVisible(center) || isPositionVisible(left) || isPositionVisible(right) ||
		isPositionVisible(top) || isPositionVisible(bottom) || isPositionVisible(leftBottom) ||
		isPositionVisible(rightBottom) || isPositionVisible(rightTop) || isPositionVisible(leftTop));

	/*return isPositionVisible(center) || isPositionVisible(left) || isPositionVisible(right) ||
		isPositionVisible(top) || isPositionVisible(bottom);*/

	//glm::dvec3 out;
	//openglEngine::openGLCoordinatesEngine::world2Camera(glm::dvec3(_eyeXYZ[0], _eyeXYZ[1], _eyeXYZ[2]),
	//	glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec3(x, y, z), out);
	////return !(frustum_culling(tile) || horizon_culling(tile));
	//Vec3d tile(out.x, out.y, -out.z);
	////return !(frustum_culling(tile));
	//return !(frustum_culling(tile) || horizon_culling(tile));
	//return !(horizon_culling(tile));
}

bool OM3DScheduler::isVisible(int zoom, int col, int row, size_t sample)
{
	//vector<Vec3d> pts;
	//pts.resize(sample * sample);

	Vec2i centerIndex;
	OMGeoUtil::deg2num(_earthObservedCenter, centerIndex, zoom);
	if (col == centerIndex[0] && row == centerIndex[1])
		return true;

	row = pow(2, zoom) - row - 1;

	double mecatorX = 0;
	double mecatorY = 0;
	double X=0.0, Y=0.0, Z=0.0;
	CGeoUtil::getTileLeftTop(zoom, col, row, mecatorX, mecatorY);
	double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom) / (sample - 1);

	double x = mecatorX;
	double y = mecatorY;

	for (int i = 0; i < sample; i++) {
		for (int j = 0; j < sample; j++) {
			x = mecatorX + j * span;
			y = mecatorY - i * span;
			CGeoUtil::WebMercator2XYZ(x, y, X, Y, Z);
			if (isPositionVisible(Vec3d(X, Y, Z)))
				return true;
			//pts.push_back(Vec3d(X, Y, Z));
		}
	}

	return false;
}

bool OM3DScheduler::isVisible(int zoom, int col, int row, Vec3d eye)
{
	Vec2i centerIndex;
	OMGeoUtil::deg2num(_earthObservedCenter, centerIndex, zoom);
	if (col == centerIndex[0] && row == centerIndex[1])
		return true;

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

	// 只要有一个边界点在视域内则该瓦片可见
	/*return (isPositionVisible(center) || isPositionVisible(left) || isPositionVisible(right) ||
		isPositionVisible(top) || isPositionVisible(bottom) || isPositionVisible(leftBottom) ||
		isPositionVisible(rightBottom) || isPositionVisible(rightTop) || isPositionVisible(leftTop));*/

	return isPositionVisible(center, eye, zoom) || isPositionVisible(left, eye, zoom) || 
	isPositionVisible(right, eye, zoom) || isPositionVisible(top, eye, zoom) || isPositionVisible(bottom, eye, zoom);
}

bool OM3DScheduler::isTileVisible(int zoom, int col, int row, int isdraw)
{
	Vec2i centerIndex;
	OMGeoUtil::deg2num(_earthObservedCenter, centerIndex, zoom);
	if (col == centerIndex[0] && row == centerIndex[1])
		return true;

	row = pow(2, zoom) - row - 1;

	double mecatorX = 0;
	double mecatorY = 0;
	double X, Y, Z;
	CGeoUtil::getTileLeftTop(zoom, col, row, mecatorX, mecatorY);
	double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom);

	double x = mecatorX;
	double y = mecatorY;

	double maxX = -99999999999;
	double minX = 99999999999;
	double maxY = -99999999999;
	double minY = 99999999999;
	double maxZ = -99999999999;
	double minZ = 99999999999;
    int isCull =1;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			x = mecatorX + j * span;
			y = mecatorY - i * span;
			CGeoUtil::WebMercator2XYZ(x, y, X, Y, Z);
			glm::dvec3 out;
			openglEngine::openGLCoordinatesEngine::world2Camera(glm::dvec3(_eyeXYZ[0], _eyeXYZ[1], _eyeXYZ[2]),
				glm::dvec3(_center[0], _center[1], _center[2]), glm::dvec3(_up[0], _up[1], _up[2]), glm::dvec3(X,Y,Z), out);
			if (out.x > maxX)
				maxX = out.x;
			if (out.x < minX)
				minX = out.x;
			if (out.y > maxY)
				maxY = out.y;
			if (out.y < minY)
				minY = out.y;
			if (-out.z > maxZ)
				maxZ = -out.z;
			if (-out.z < minZ)
				minZ = -out.z;

            Vec3d tile(out.x, out.y, -out.z);
            if(!horizon_culling(tile)) isCull=0;
		}
	}
    if(_isFirstPerson){
        if (isdraw) {
            double frustumYmax=tan(_frustum1[0]*3.1415926535/(2*180))*_frustum1[3];
            double frustumXmax=frustumYmax*_frustum1[1];
    		if ((max(_frustum1[2], minZ) <= min(_frustum1[3], maxZ))  && (!isCull)){
                if((max(-frustumXmax, minX) <= min(frustumXmax, maxX)) && (max(-frustumYmax, minY) <= min(frustumYmax, maxY)))
                    return true;
            }            
    		return false;	
    	}
    	else {
    		double frustumYmax=tan(_frustum_tmp1[0]*3.1415926535/(2*180))*_frustum_tmp1[3];
            double frustumXmax=frustumYmax*_frustum_tmp1[1];
    		if ((max(_frustum_tmp1[2], minZ) <= min(_frustum_tmp1[3], maxZ))  && (!isCull)){
                if((max(-frustumXmax, minX) <= min(frustumXmax, maxX)) && (max(-frustumYmax, minY) <= min(frustumYmax, maxY)))
                    return true;
            }            
    		return false;	
    	}
    }
    else{
        if (isdraw) {
    		if ((max(_frustum[0], minX) <= min(_frustum[1], maxX)) && (max(_frustum[2], minY) <= min(_frustum[3], maxY)) && (!isCull))
    			return true;
    	}
    	else {
    		if ((max(_frustum_tmp[0], minX) <= min(_frustum_tmp[1], maxX)) && (max(_frustum_tmp[2], minY) <= min(_frustum_tmp[3], maxY)) && (!isCull))
    			return true;
    	}	
    }
	
	return false;
}

int OM3DScheduler::rotate(double angle)
{
	//1、初始化一个单位矩阵
	glm::dmat4 trans = glm::dmat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle), glm::dvec3(0.0, 1.0, 0.0));
	glm::dvec3 up = trans * glm::dvec4(_up[0], _up[1], _up[2], 1);
	_up[0] = up.x; _up[1] = up.y; _up[2] = up.z;
    if(isSameTiles())
        return 1;
    else
        return 0;
}

bool OM3DScheduler::frustum_culling(Vec3d tile)
{
	if (tile[0] >= _frustum_tmp[0] && tile[0] <= _frustum_tmp[1] && tile[1] >= _frustum_tmp[2] && tile[1] <= _frustum_tmp[3]
		&& tile[2] >= _frustum_tmp[4] && tile[2] <= _frustum_tmp[5])
		return false;
	else
		return true; // 需要裁剪
}

bool OM3DScheduler::horizon_culling(Vec3d tile)
{
	double limitDistance = _eyeXYZ[0] * _eyeXYZ[0] + _eyeXYZ[1] * _eyeXYZ[1] + _eyeXYZ[2] * _eyeXYZ[2]
		- CGeoUtil::WGS_84_RADIUS_POLAR * CGeoUtil::WGS_84_RADIUS_POLAR;
	double distance = tile[0] * tile[0] + tile[1] * tile[1] + tile[2] * tile[2];
	if (distance < limitDistance)
		return false;
	else
		return true; // 需要裁剪
}

bool OM3DScheduler::isPositionVisible(Vec3d pt)
{
	glm::dvec3 out;
	openglEngine::openGLCoordinatesEngine::world2Camera(glm::dvec3(_eyeXYZ_tmp[0], _eyeXYZ_tmp[1], _eyeXYZ_tmp[2]),
		glm::dvec3(_center[0], _center[1], _center[2]), glm::dvec3(_up[0], _up[1], _up[2]), glm::dvec3(pt[0], pt[1], pt[2]), out);
	Vec3d tile(out.x, out.y, -out.z);
	//return !frustum_culling(tile);
	return !(frustum_culling(tile) || horizon_culling(tile));
}

bool OM3DScheduler::isPositionVisible(Vec3d pt, Vec3d eye, int zoom)
{
	glm::dvec3 out;
	openglEngine::openGLCoordinatesEngine::world2Camera(glm::dvec3(_eyeXYZ_tmp[0], _eyeXYZ_tmp[1], _eyeXYZ_tmp[2]),
		glm::dvec3(_center[0], _center[1], _center[2]), glm::dvec3(_up[0], _up[1], _up[2]), glm::dvec3(pt[0], pt[1], pt[2]), out);
	//openglEngine::openGLCoordinatesEngine::world2Camera(glm::dvec3(_eyeXYZ[0], _eyeXYZ[1], _eyeXYZ[2]),
	//	glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec3(pt[0], pt[1], pt[2]), out);
	Vec3d tile(out.x, out.y, -out.z);

	return !(frustum_culling(tile) || horizon_culling(tile));
}

int OM3DScheduler::getTilesEn(vector<Vec3i>& tiles, int zoom,set<string> processed, int isdraw)
{
	Vec3d pt = lineSegment_WGS84Ellipsoid_intersection(_eyeXYZ, _center);
	///* 有交点继续计算需要加载的瓦片索引，并返回0*/
	Vec3d lonlat1;
	OMGeoUtil::XYZ2lonLatHeight(lonlat1, pt);

	_earthObservedCenter[0] = lonlat1[0];
	_earthObservedCenter[1] = lonlat1[1];
	if (pt[0] != 0 || pt[1] != 0 || pt[2] != 0) {
			centerSearchEn(tiles, zoom,processed, isdraw);
	}
	else {                               //TODO:暂未对quadtreeSearch进行isdraw判断
			quadtreeSearch(tiles, zoom);
	}
	return 0;
}

int OM3DScheduler::getTiles(vector<Vec3i>& tiles, int zoom, int isdraw)
{
	tiles.clear();
	Vec3d pt = lineSegment_WGS84Ellipsoid_intersection(_eyeXYZ, _center);
	///* 有交点继续计算需要加载的瓦片索引，并返回0*/
	Vec3d lonlat1;
	OMGeoUtil::XYZ2lonLatHeight(lonlat1, pt);

	_earthObservedCenter[0] = lonlat1[0];
	_earthObservedCenter[1] = lonlat1[1];
	if (pt[0] != 0 || pt[1] != 0 || pt[2] != 0) {
		//if (displayStyle == 1)
			//centerSearch(tiles, zoom - 1);
		//else
			centerSearch(tiles, zoom, isdraw);
	}
	else {                       //TODO:暂未对quadtreeSearch进行isdraw判断
		//if (displayStyle == 1)
			//quadtreeSearch(tiles, zoom - 1);
		//else
			quadtreeSearch(tiles, zoom);
	}


	//traverseSearch(tiles, zoom);


	//cout << _count << " " << tiles.size() << endl;
	//memcpy(&_eye, &_eye_tmp, sizeof(Vec3d));
	//OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);

	//memcpy(&_frustum, &_frustum_tmp, sizeof(OrthoFrustum));
	return 0;
}

int OM3DScheduler::traverseSearch(vector<Vec3i>& tiles, int zoom)
{
	int len = pow(2, zoom);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			//_count++;
			if (isVisible(zoom, i, j))
				tiles.push_back(Vec3i(zoom, i, j));
		}
	}
	return 0;
}

int OM3DScheduler::quadtreeSearch(vector<Vec3i>& tiles, int zoom) {
	queue<Vec3i> nodes;
	Vec3d pt = lineSegment_WGS84Ellipsoid_intersection(_eyeXYZ_tmp, _center);

	
	
	int level = 0;
	float height = 5 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 3);
	_eye_tmp[2] = height;
	OMGeoUtil::lonLatHeight2XYZ(_eye_tmp, _eyeXYZ_tmp);

	updateFrustumForCalculation(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 3) + _offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 3) + _offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 3) + _offset[1],
		CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 3) + _offset[1], height * 0.95, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
	
	if (isVisible(0, 0, 0))
		nodes.push(Vec3i(0, 0, 0));
	//_count++;

	while (!nodes.empty()) {
		int size = nodes.size();
		//_count += size;

		height = height / 2;
		_eye_tmp[2] = height;
		OMGeoUtil::lonLatHeight2XYZ(_eye_tmp, _eyeXYZ_tmp);

		updateFrustumForCalculation(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 2) + _offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 2) + _offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 2) + _offset[1],
			CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, level - 2) + _offset[1], height * 0.95, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR);

		for (int i = 0; i < size; i++) {
			Vec3i node = nodes.front();
			nodes.pop();
			if (level == zoom)
				tiles.push_back(node);
			else {
				Vec3i child1(node[0] + 1, 2 * node[1], 2 * node[2]);
				Vec3i child2(node[0] + 1, 2 * node[1] + 1, 2 * node[2]);
				Vec3i child3(node[0] + 1, 2 * node[1], 2 * node[2] + 1);
				Vec3i child4(node[0] + 1, 2 * node[1] + 1, 2 * node[2] + 1);
				//_count += 4;
				if (isVisible(child1[0], child1[1], child1[2], 4))
					nodes.push(child1);
				//else
					//cout << child1[0] << " " << child1[1] << " " << child1[2] << " invisible" << endl;

				if (isVisible(child2[0], child2[1], child2[2], 4))
					nodes.push(child2);
				//else
					//cout << child2[0] << " " << child2[1] << " " << child2[2] << " invisible" << endl;

				if (isVisible(child3[0], child3[1], child3[2], 4))
					nodes.push(child3);
				//else
					//cout << child3[0] << " " << child3[1] << " " << child3[2] << " invisible" << endl;

				if (isVisible(child4[0], child4[1], child4[2], 4))
					nodes.push(child4);
				//else
					//cout << child4[0] << " " << child4[1] << " " << child4[2] << " invisible" << endl;
			}
		}
		
		level++;

		if (level > zoom)
			break;
	}
	return 0;
}

bool OM3DScheduler::isValid(Vec3i tile, set<string>& processed, int zoom, int isdraw) {
	// index invalid
	if (tile[1] < 0 || tile[1] >= pow(2, zoom) || tile[2] < 0 || tile[2] >= pow(2, zoom)||tile[0] <= 0)
		return false;
	// processed
	//string index = to_string(tile[0]) + "." + to_string(tile[1]) + "." + to_string(tile[2]);
	ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (tile[0]) << "." << (tile[1]) << "." << (tile[2]);
	string index = ost_temp.str();
	if (processed.find(index) != processed.end())
		return false;

	//_count++;
	// invisible
	if (!isTileVisible(tile[0], tile[1], tile[2], isdraw))
		return false;
	//if (!isVisible(tile[0], tile[1], tile[2], 4))
	//	return false;

	return true;
}

bool OM3DScheduler::isValidWithoutCull(Vec3i tile, set<string>& processed, int zoom) {
	// index invalid
	if (tile[1] < 0 || tile[1] >= pow(2, zoom) || tile[2] < 0 || tile[2] >= pow(2, zoom) || tile[0] <= 0)
		return false;
    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (tile[0]) << "." << (tile[1]) << "." << (tile[2]);
	string index = ost_temp.str();
	//string index = to_string(tile[0]) + "." + to_string(tile[1]) + "." + to_string(tile[2]);
	if (processed.find(index) != processed.end())
		return false;
	return true;
}

void OM3DScheduler::getEightConnectedTiles(queue<Vec3i>& nodes, Vec3i tile, set<string>& processed, int zoom, int isdraw) {
	Vec3i tile1(tile[0], tile[1] - 1, tile[2] - 1);
	Vec3i tile2(tile[0], tile[1], tile[2] - 1);
	Vec3i tile3(tile[0], tile[1] + 1, tile[2] - 1);
	Vec3i tile4(tile[0], tile[1] - 1, tile[2]);
	Vec3i tile5(tile[0], tile[1] + 1, tile[2]);
	Vec3i tile6(tile[0], tile[1] - 1, tile[2] + 1);
	Vec3i tile7(tile[0], tile[1], tile[2] + 1);
	Vec3i tile8(tile[0], tile[1] + 1, tile[2] + 1);


	if (isValid(tile1, processed, zoom, isdraw))
		nodes.push(tile1);
	if (isValid(tile2, processed, zoom, isdraw))
		nodes.push(tile2);
	if (isValid(tile3, processed, zoom, isdraw))
		nodes.push(tile3);
	if (isValid(tile4, processed, zoom, isdraw))
		nodes.push(tile4);
	if (isValid(tile5, processed, zoom, isdraw))
		nodes.push(tile5);
	if (isValid(tile6, processed, zoom, isdraw))
		nodes.push(tile6);
	if (isValid(tile7, processed, zoom, isdraw))
		nodes.push(tile7);
	if (isValid(tile8, processed, zoom, isdraw))
		nodes.push(tile8);

#if 1
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
	ost_temp << (tile8[0]) << "." << (tile8[1]) << "." <<(tile8[2]);
	index = ost_temp.str();
	processed.insert(index);
#else
    string index = to_string(tile1[0]) + "." + to_string(tile1[1]) + "." + to_string(tile1[2]);
	processed.insert(index);
	index = to_string(tile2[0]) + "." + to_string(tile2[1]) + "." + to_string(tile2[2]);
	processed.insert(index);
	index = to_string(tile3[0]) + "." + to_string(tile3[1]) + "." + to_string(tile3[2]);
	processed.insert(index);
	index = to_string(tile4[0]) + "." + to_string(tile4[1]) + "." + to_string(tile4[2]);
	processed.insert(index);
	index = to_string(tile5[0]) + "." + to_string(tile5[1]) + "." + to_string(tile5[2]);
	processed.insert(index);
	index = to_string(tile6[0]) + "." + to_string(tile6[1]) + "." + to_string(tile6[2]);
	processed.insert(index);
	index = to_string(tile7[0]) + "." + to_string(tile7[1]) + "." + to_string(tile7[2]);
	processed.insert(index);
	index = to_string(tile8[0]) + "." + to_string(tile8[1]) + "." + to_string(tile8[2]);
	processed.insert(index);
#endif
}

void OM3DScheduler::getEightConnectedTilesWithoutCull(vector<Vec3i> &tiles, Vec3i tile, set<string>& processed, int zoom)
{
	Vec3i tile1(tile[0], tile[1] - 1, tile[2] - 1);
	Vec3i tile2(tile[0], tile[1], tile[2] - 1);
	Vec3i tile3(tile[0], tile[1] + 1, tile[2] - 1);
	Vec3i tile4(tile[0], tile[1] - 1, tile[2]);
	Vec3i tile5(tile[0], tile[1] + 1, tile[2]);
	Vec3i tile6(tile[0], tile[1] - 1, tile[2] + 1);
	Vec3i tile7(tile[0], tile[1], tile[2] + 1);
	Vec3i tile8(tile[0], tile[1] + 1, tile[2] + 1);


	if (isValidWithoutCull(tile1, processed, zoom))
		tiles.push_back(tile1);
	if (isValidWithoutCull(tile2, processed, zoom))
		tiles.push_back(tile2);
	if (isValidWithoutCull(tile3, processed, zoom))
		tiles.push_back(tile3);
	if (isValidWithoutCull(tile4, processed, zoom))
		tiles.push_back(tile4);
	if (isValidWithoutCull(tile5, processed, zoom))
		tiles.push_back(tile5);
	if (isValidWithoutCull(tile6, processed, zoom))
		tiles.push_back(tile6);
	if (isValidWithoutCull(tile7, processed, zoom))
		tiles.push_back(tile7);
	if (isValidWithoutCull(tile8, processed, zoom))
		tiles.push_back(tile8);

    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (tile1[0]) << "." << (tile1[1]) << "." << (tile1[2]);
	string index = ost_temp.str();
	//string index = to_string(tile1[0]) + "." + to_string(tile1[1]) + "." + to_string(tile1[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile2[0]) << "." << (tile2[1]) << "." << (tile2[2]);
	index = ost_temp.str();
	//index = to_string(tile2[0]) + "." + to_string(tile2[1]) + "." + to_string(tile2[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile3[0]) << "." << (tile3[1]) << "." << (tile3[2]);
	index = ost_temp.str();
	//index = to_string(tile3[0]) + "." + to_string(tile3[1]) + "." + to_string(tile3[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile4[0]) << "." << (tile4[1]) << "." << (tile4[2]);
	index = ost_temp.str();
	//index = to_string(tile4[0]) + "." + to_string(tile4[1]) + "." + to_string(tile4[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile5[0]) << "." << (tile5[1]) << "." << (tile5[2]);
	index = ost_temp.str();
	//index = to_string(tile5[0]) + "." + to_string(tile5[1]) + "." + to_string(tile5[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile6[0]) << "." << (tile6[1]) << "." << (tile6[2]);
	index = ost_temp.str();
	//index = to_string(tile6[0]) + "." + to_string(tile6[1]) + "." + to_string(tile6[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile7[0]) << "." << (tile7[1]) << "." << (tile7[2]);
	index = ost_temp.str();
	//index = to_string(tile7[0]) + "." + to_string(tile7[1]) + "." + to_string(tile7[2]);
	processed.insert(index);
    ost_temp.str("");
    ost_temp << (tile8[0]) << "." << (tile8[1]) << "." << (tile8[2]);
	index = ost_temp.str();
	//index = to_string(tile8[0]) + "." + to_string(tile8[1]) + "." + to_string(tile8[2]);
	processed.insert(index);
}

int OM3DScheduler::centerSearchEn(vector<Vec3i>& tiles, int zoom,set<string> processed, int isdraw)
{
	
	Vec3d pt1 = lineSegment_WGS84Ellipsoid_intersection(_eyeXYZ, _center);

	///* 有交点继续计算需要加载的瓦片索引，并返回0*/
	Vec3d lonlat1;
	OMGeoUtil::XYZ2lonLatHeight(lonlat1, pt1);

	Vec2i centerIndex;
	OMGeoUtil::deg2num(lonlat1, centerIndex, zoom);
	Vec3i tile(zoom, centerIndex[0], centerIndex[1]);

	queue<Vec3i> nodes;

	nodes.push(tile);

	while (!nodes.empty()) {
		Vec3i node = nodes.front();
		tiles.push_back(node);
		nodes.pop();
		
		//_count++;
		getEightConnectedTiles(nodes, node, processed, zoom, isdraw);
	}
	//OMGeoUtil::deg2num(lonlat2, rightTop, _zoom);

	return 0;
}


int OM3DScheduler::centerSearch(vector<Vec3i>& tiles, int zoom, int isdraw)
{
	
	Vec3d pt1 = lineSegment_WGS84Ellipsoid_intersection(_eyeXYZ, _center);

	///* 有交点继续计算需要加载的瓦片索引，并返回0*/
	Vec3d lonlat1;
	OMGeoUtil::XYZ2lonLatHeight(lonlat1, pt1);
	/*lonlat1[0]:longitude 经度；lonlat1[1]:latitude:纬度 */
	Vec2i centerIndex;
	OMGeoUtil::deg2num(lonlat1, centerIndex, zoom);
	Vec3i tile(zoom, centerIndex[0], centerIndex[1]);

	queue<Vec3i> nodes;

	nodes.push(tile);
	set<string> processed;
	ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (tile[0]) << "." << (tile[1]) << "." << (tile[2]);
	string index = ost_temp.str();
	processed.insert(index);
	while (!nodes.empty()) {
		Vec3i node = nodes.front();
		tiles.push_back(node);
		nodes.pop();
		//_count++;
		getEightConnectedTiles(nodes, node, processed, zoom, isdraw);
	}
	//OMGeoUtil::deg2num(lonlat2, rightTop, _zoom);

	return 0;
}
