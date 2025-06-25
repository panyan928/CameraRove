#include "cperspective.h"

void CPerspective::get(float& zNear, float& zFar, float& fovy, float& aspect)
{
	zNear = _zNear;
	zFar = _zFar;
	fovy = _fovy;
	aspect = _aspect;
}

void CPerspective::set(float zNear, float zFar, float fovy, float aspect)
{
	_zNear = zNear;
	_zFar = zFar;
	_fovy = fovy;
	_aspect = aspect;
}

bool CPerspective::frustum_culling(Vec3d position)
{
	//double screen_x, screen_y, screen_z;
	//openglEngine::openGLCoordinatesEngine::world2Screen(screen_x, screen_y, screen_z, position[0], position[1], position[2]);
	//if (screen_x <= 1024 && screen_x >= 0 && screen_y >= 0 && screen_y <= 768)
	//	return false;
	//else
	//	return true;

	double z = -position[2];

	if (z < _zNear || z > _zFar)
		return true;

	double h = z * 2 * tan((double)glm::radians(_fovy / 2));

	if (position[1] > h / 2 || position[1] < -h / 2)
		return true;

	double w = h * _aspect;

	if (position[0] > w / 2 || position[0] < -w / 2)
		return true;

	return false;
}
