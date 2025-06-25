#ifndef CPERSPECTIVE_H
#define CPERSPECTIVE_H
#include "../TMUtil/tmtools.h"
#include "../OpenGLEngine/openglengine.h"
#include "../TMUtil/OMapGlobal.h"
#include "../TMUtil/OMGeoUtil.h"

class CPerspective
{
private:
	float _zNear;
	float _zFar;
	float _fovy;
	float _aspect;

public:
	void get(float& zNear, float& zFar, float& fovy, float& aspect);
	void set(float zNear, float zFar, float fovy, float aspect);

	bool frustum_culling(Vec3d position);
};

#endif // !1