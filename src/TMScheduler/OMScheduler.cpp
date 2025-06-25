#include "OMScheduler.h"

OMScheduler::OMScheduler()
{
	_zoom = 4;
	Vec2d pt(115, 28);
	OMGeoUtil::Lonlat2WebMecator(pt, _center);
}

OMScheduler::~OMScheduler()
{

}

float OMScheduler::zoom() const
{
	return _zoom;
}

Recti OMScheduler::tileBound() const
{
	return _tileBound;
}

Rectd OMScheduler::windowRect() const
{
	return _windowRect;
}

int OMScheduler::compute()
{
	int zoom_i = static_cast<int> (_zoom);
	Vec2i centerIndex;
	CGeoUtil::webMecator2num(_center[0], _center[1], zoom_i, centerIndex);

	_tileBound[0] = centerIndex[0] - tileBufferSize / 2;
	_tileBound[1] = centerIndex[0] + tileBufferSize / 2;
	_tileBound[2] = centerIndex[1] - tileBufferSize / 2;
	_tileBound[3] = centerIndex[1] + tileBufferSize / 2;

	cout << _tileBound[0] << " " << _tileBound[1] << " " << _tileBound[2] << " " << _tileBound[3] << endl;

	double pixel = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom_i) / 256;

	_windowRect[0] = _center[0] - 1024 / 2 * pixel;
	_windowRect[1] = _center[0] + 1024 / 2 * pixel;
	_windowRect[2] = _center[1] - 768 / 2 * pixel;
	_windowRect[3] = _center[1] + 768 / 2 * pixel;
	return 0;
}

int OMScheduler::updateMapParameter(Vec2d center, float zoom)
{
	memcpy(&_center, &center, sizeof(center));
	_zoom = zoom;
	return 0;
}

int OMScheduler::zoomIn()
{
	_zoom++;
	return 0;
}

int OMScheduler::zoomOut()
{
	_zoom--;
	return 0;
}

int OMScheduler::pan(int direction)
{
	int zoom_i = static_cast<int> (_zoom);
	double pixel = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom_i) / 256;
	switch (direction)
	{
	case 1:
		_center[0] -= 16 * pixel;
		break;
	case 2:
		_center[1] -= 16 * pixel;
		break;
	case 3:
		_center[0] += 16 * pixel;
		break;
	case 4:
		_center[1] += 16 * pixel;
		break;
	default:
		break;
	}
	return 0;
}
