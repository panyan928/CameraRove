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
	Vec3d _angle; // �ɻ���̬��ƫ���ǣ�yaw�������ǣ�pitch���͹����ǣ�roll��
	float _zoom;
	int	  _viewer; // 0 �����ӽǣ�1 ����͸���ӽ�

public:
	CCamera();

	/************ƽ�ƽӿ�****************/
	int pan(int mode);
	int panTo(Vec3d pos);

	/************���Žӿ�****************/
	int zoomIn();
	int zoomOut();
	int zoomTo(float zoom);

	/************��ת�ӿ�****************/
	int rotate(double angle);

	/************�ӽǽӿ�****************/
	void changeViewer(int mode);

	/***********************************/
	bool horizon_culling(Vec3d position); // ��ƽ�߲ü�����

	float zoom() const { return _zoom; }
	Vec3d eye() const { return _eye; }
	Vec3d center() const { return _center; }
	Vec3d up() const { return _up; }
	Vec3d angle() const { return _angle; }
	Vec3d eyeXYZ() { OMGeoUtil::lonLatHeight2XYZ(_eye, _eyeXYZ); return _eyeXYZ; }
};

#endif // !CCAMERA_H
