#include "OMScheduler.h"
#include "gl/gl.h"

OMScheduler::OMScheduler()
{
	_zoom = 4;
	Vec2d pt(115, 28);
	OMGeoUtil::Lonlat2WebMecator(pt, _center);
}

OMScheduler::OMScheduler(int zoom, Vec2d center)
{
	_zoom = zoom;
	OMGeoUtil::Lonlat2WebMecator(center, _center);
}

OMScheduler::~OMScheduler()
{

}

Vec2d OMScheduler::center() const
{
	return _center;
}

int OMScheduler::zoom() const
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
    // 获取整数缩放级别
    int zoom_i = static_cast<int>(_zoom);
    
    // 计算一个像素在墨卡托坐标系下的大小
    double pixel = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom_i) / 256;
    
    // 获取视口大小
	GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewportWidth = 1920;
    int viewportHeight = 1080;
    
    // 计算窗口的墨卡托坐标范围
    _windowRect[0] = _center[0] - viewportWidth / 2.0 * pixel; // 左边界
    _windowRect[2] = _center[0] + viewportWidth / 2.0 * pixel; // 右边界
    _windowRect[1] = _center[1] - viewportHeight / 2.0 * pixel; // 下边界
    _windowRect[3] = _center[1] + viewportHeight / 2.0 * pixel; // 上边界
    
    // 计算墨卡托坐标范围对应的瓦片索引范围
    double begin = -CGeoUtil::PI * CGeoUtil::Web_Mecator_R;
    double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom_i);
    
    // 计算视口边界对应的瓦片索引
    _tileBound[0] = floor((_windowRect[0] - begin) / span); // 左边界瓦片
    _tileBound[2] = ceil((_windowRect[2] - begin) / span);  // 右边界瓦片
    _tileBound[1] = floor((_windowRect[1] - begin) / span); // 下边界瓦片
    _tileBound[3] = ceil((_windowRect[3] - begin) / span);  // 上边界瓦片

	return 0;
}

int OMScheduler::isSameTiles() {
	// 保存当前的瓦片边界
    Recti oldTileBound = _tileBound;
    
    // 重新计算新的瓦片边界
    compute();
    
    // 比较新旧瓦片边界是否相同
    return (oldTileBound[0] == _tileBound[0] &&
            oldTileBound[1] == _tileBound[1] &&
            oldTileBound[2] == _tileBound[2] &&
            oldTileBound[3] == _tileBound[3]);
}

int OMScheduler::updateMapParameter(Vec2d center, int zoom)
{
	memcpy(&_center, &center, sizeof(center));
	_zoom = zoom;
	return 0;
}

int OMScheduler::zoomIn()
{
	_zoom++;
	return 1;
}

int OMScheduler::zoomOut()
{
	_zoom--;
	return 1;
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
	if (isSameTiles())
		return 1;
	else
		return 0;
}

int OMScheduler::rotate(double angle){
	_rotationAngle = angle;
	if(_rotationAngle<0) _rotationAngle = _rotationAngle + 360;
	if(_rotationAngle>360) _rotationAngle = _rotationAngle - 360;
	return 0;
}

double OMScheduler::getRotation() const
{
	return _rotationAngle;
}

int OMScheduler::getTilesBuffer(vector<Vec3i>& tiles, int zoom)
{
	// 清空现有瓦片列表
	tiles.clear();
    
    // 计算一个像素在墨卡托坐标系下的大小
    double pixel = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom) / 256;
    
    // 获取视口大小
	GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int viewportWidth = 1920;
    int viewportHeight = 1080;
    
	Recti tileBound2;
	Rectd windowRect2;

    // 计算窗口的墨卡托坐标范围
    windowRect2[0] = _center[0] - viewportWidth / 2.0 * pixel; // 左边界
    windowRect2[2] = _center[0] + viewportWidth / 2.0 * pixel; // 右边界
    windowRect2[1] = _center[1] - viewportHeight / 2.0 * pixel; // 下边界
    windowRect2[3] = _center[1] + viewportHeight / 2.0 * pixel; // 上边界
    
    // 计算墨卡托坐标范围对应的瓦片索引范围
    double begin = -CGeoUtil::PI * CGeoUtil::Web_Mecator_R;
    double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom);
    
	// 添加额外的瓦片缓冲（可选）
	
	int tileBuffer = 1; // 额外加载视口外1个瓦片的范围
    // 计算视口边界对应的瓦片索引
    tileBound2[0] = floor((windowRect2[0] - begin) / span) - tileBuffer;  // 左边界瓦片
    tileBound2[2] = ceil((windowRect2[2] - begin) / span) + tileBuffer;  // 右边界瓦片
    tileBound2[1] = floor((windowRect2[1] - begin) / span) - tileBuffer; // 下边界瓦片
    tileBound2[3] = ceil((windowRect2[3] - begin) / span) + tileBuffer;  // 上边界瓦片

	
	//tileBound2[0] = _tileBound[0] - tileBuffer;  // 左边界瓦片
	//tileBound2[2] = _tileBound[2] + tileBuffer;  // 右边界瓦片
	//tileBound2[1] = _tileBound[1] - tileBuffer; // 下边界瓦片
	//tileBound2[3] = _tileBound[3] + tileBuffer;  // 上边界瓦片

	
	// 确保边界不超出有效范围
	int maxTileIndex = pow(2, zoom) - 1;
	int minCol = max(0, tileBound2[0]);
	int maxCol = min(maxTileIndex, tileBound2[2]);
	int minRow = max(0, tileBound2[1]);
	int maxRow = min(maxTileIndex, tileBound2[3]);
	
	// 遍历并添加所有可见瓦片
	for (int col = minCol; col <= maxCol; col++) {
		for (int row = minRow; row <= maxRow; row++) {
                tiles.push_back(Vec3i(zoom, col, row));
		}
	}
	
	return tiles.size(); // 返回瓦片数量
}

int OMScheduler::getTiles(vector<Vec3i>& tiles, int zoom)
{
	// 清空现有瓦片列表
	tiles.clear();
	
	// 计算当前视图状态（确保瓦片边界是最新的）
	compute();
	
	// 确保边界不超出有效范围
	int maxTileIndex = pow(2, zoom) - 1;
	int minCol = max(0, _tileBound[0]);
	int maxCol = min(maxTileIndex, _tileBound[2]);
	int minRow = max(0, _tileBound[1]);
	int maxRow = min(maxTileIndex, _tileBound[3]);
	
	// 遍历并添加所有可见瓦片
	for (int col = minCol; col <= maxCol; col++) {
		for (int row = minRow; row <= maxRow; row++) {
			//if (isTileVisible(zoom, col, row)) {
                tiles.push_back(Vec3i(zoom, col, row));
            //}
		}
	}
	
	return tiles.size(); // 返回瓦片数量
}

bool OMScheduler::isTileVisible(int zoom, int col, int row)
{
    // 计算瓦片的墨卡托坐标范围
    double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom);
    double begin = -CGeoUtil::PI * CGeoUtil::Web_Mecator_R;
    
    double tileLeft = begin + col * span;
    double tileRight = tileLeft + span;
    double tileBottom = begin + row * span;
    double tileTop = tileBottom + span;
    
    // 检查瓦片是否与视口相交
    return !(tileRight < _windowRect[0] || 
             tileLeft > _windowRect[1] || 
             tileTop < _windowRect[2] || 
             tileBottom > _windowRect[3]);
}