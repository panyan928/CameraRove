#include "ccamera.h"

CCamera::CCamera()
{
	_eye[0] = 116.31; _eye[1] = 28.45; _eye[2] = 0.2 * CGeoUtil::WGS_84_RADIUS_EQUATOR;
	//_eye[0] = 103.62; _eye[1] = 31.07; _eye[2] = 0.2 * CGeoUtil::WGS_84_RADIUS_EQUATOR;
	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);
	_up[2] = 1;
	_zoom = 5;

	_viewer = 0;
}

int CCamera::pan(int mode)
{
	int zoom_i = static_cast<int> (_zoom);
	double unit = 360.0 / pow(2, zoom_i) / 256;

	switch (mode)
	{
	case 1:
		_eye[0] -= 16 * unit;
		break;
	case 2:
		if ((_eye[1] - 16 * unit) >= -55)
			_eye[1] -= 16 * unit;
		break;
	case 3:
		_eye[0] += 16 * unit;
		break;
	case 4:
		if ((_eye[1] + 16 * unit) <= 55)
			_eye[1] += 16 * unit;
		break;
	default:
		break;
	}
	if (_viewer == 1) {
		Vec3d center(_eye[0], _eye[1], 0);
		//center[1] += 2.5 * 45 * pow(2, log((_eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR)) / log(2));
		center[1] += 5 * 45 * _eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR;
		OMGeoUtil::lonLatHeight2XYZ(center, _center);
	}
	

	return 0;
}

int CCamera::panTo(Vec3d pos)
{
	memcpy(&_eye, &pos, sizeof(pos));
	return 0;
}

int CCamera::zoomIn()
{
	_zoom++;
	_eye[2] /= 2;
	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);
	return 0;
}

int CCamera::zoomOut()
{
	_zoom--;
	_eye[2] *= 2;
	return 0;
}

int CCamera::zoomTo(float zoom)
{
	double dif = static_cast<double> (zoom - _zoom);
	_eye[2] = _eye[2] * pow(2, -dif);
	_zoom = zoom;
	OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ);
	return 0;
}

int CCamera::rotate(double angle)
{
	_angle[0] = angle;

	glm::dmat4 trans = glm::dmat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle), glm::dvec3(0.0, 1.0, 0.0));
	glm::dvec3 up = trans * glm::dvec4(0, 0, 1, 1);
	_up[0] = up.x; _up[1] = up.y; _up[2] = up.z;

	return 0;
}


bool CCamera::horizon_culling(Vec3d position)
{
	Vec3d eyeXYZ;
	OMGeoUtil::lonLatHeight2XYZ(_eye, eyeXYZ);

	double limitDistance = eyeXYZ[0] * eyeXYZ[0] + eyeXYZ[1] * eyeXYZ[1] + eyeXYZ[2] * eyeXYZ[2]
		- CGeoUtil::WGS_84_RADIUS_POLAR * CGeoUtil::WGS_84_RADIUS_POLAR;

	//double distance = position[0] * position[0] + position[1] * position[1] + position[2] * position[2];
	double distance = (eyeXYZ[0] - position[0]) * (eyeXYZ[0] - position[0]) + (eyeXYZ[1] - position[1]) *
		(eyeXYZ[1] - position[1]) + (eyeXYZ[2] - position[2]) * (eyeXYZ[2] - position[2]);

	if (distance < limitDistance)
		return false;
	else
		return true; // ÐèÒª²Ã¼ô
}

void CCamera::changeViewer(int mode)
{
	_viewer = mode;

	if (mode == 0) {
		memset(&_center, 0, sizeof(Vec3d));
	}
	else if (mode == 1){
		Vec3d center(_eye[0], _eye[1], 0);
		//center[1] += 2.5 * 45 * pow(2, log((_eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR)) / log(2));
		center[1] += 5 * 45 * _eye[2] / CGeoUtil::WGS_84_RADIUS_EQUATOR;
		OMGeoUtil::lonLatHeight2XYZ(center, _center);
	}
}
