#ifndef CCAMERA_H
#define CCAMERA_H


#include "../TMUtil/OMGeoUtil.h"
#include "../TMUtil/tmtools.h"
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../OpenGLEngine/openglengine.h"


class CCamera
{
private:
	Vec3d _eye; // LLH
	Vec3d _eyeXYZ; // XYZ
	Vec3d _center; // XYZ
	Vec3d _up;
	Vec3d _angle; // 飞机姿态：偏航角（yaw）俯仰角（pitch）和滚动角（roll）
	float _zoom;
	int	  _viewer; // 0 正射视角，1 俯视透视视角

public:
	CCamera();

	/************平移接口****************/
	int pan(int mode);
	int panTo(Vec3d pos);

	/************缩放接口****************/
	int zoomIn();
	int zoomOut();
	int zoomTo(float zoom);

	/************旋转接口****************/
	int rotate(double angle);

	/************视角接口****************/
	void changeViewer(int mode);

	/***********************************/
	bool horizon_culling(Vec3d position); // 地平线裁剪计算

	float zoom() const { return _zoom; }
	Vec3d eye() const { return _eye; }
	Vec3d center() const { return _center; }
	Vec3d up() const { return _up; }
	Vec3d angle() const { return _angle; }
	Vec3d eyeXYZ() { OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ); return _eyeXYZ; }
};

#endif // !CCAMERA_H
