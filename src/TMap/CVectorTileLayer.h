#ifndef CVECTORTILEAYER_H
#define CVECTORTILEAYER_H

#include <unordered_map>
#include <deque>
#include <unordered_set>

#include "CTileLayer.h"
#include "../TMScheduler/OMScheduler.h"
#include "../TMUtil/tmtools.h"
#include "../TMUtil/OMapGlobal.h"
#include "../OpenGLEngine/openglengine.h"
//#include "GL/glut.h"
#include "../TMUtil/OMGeoUtil.h"
//#include <mutex>
#include "VectorDatReader.h"

class CVectorTileLayer : public CTileLayer
{
public:
    CVectorTileLayer();
    CVectorTileLayer(string path);
    virtual ~CVectorTileLayer();
    void setAnnotation(int anno, string label);
    int getAnnotation();
	int draw(Recti bounds, int zoom);
    int draw(Recti bounds, int zoom, BufferManager* manager);
    int draw(vector<Vec3i> tiles, int zoom, BufferManager* manager, int crowdLevel);
    int addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int addBufferTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager){ return 0; }
    int drawTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager) { return 0; }
    virtual int drawMultiThreads(void* para) override { return 0; }
    int getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager) { return 0; }
    


private:
    //Flong新增dat定义
    enum VectorTileDataType : uint8_t {
        VECTOR_TYPE_PBF = 0,
        VECTOR_TYPE_VERTICE = 1,
        VECTOR_TYPE_STOP = 2,
        VECTOR_TYPE_ANNO = 3
    };

    string _path;
    int _anno = 0;
    string _label;
    float _opacity = 1.0f; // 图层透明度 (0.0-1.0)
    //mutex _drawMutex;
    bool _useDatFile = false;
    VectorDatReader* _vectorDatReader = nullptr;
    string _datFilePath;

    int drawPolygon(int zoom, int col, int row, BufferManager* manager, int draw);
    int drawPolyline(int zoom, int col, int row, BufferManager* manager, int draw);
    int drawPoint(int zoom, int col, int row, BufferManager* manager, int draw);
    int drawAnnotation(int zoom, int col, int row, BufferManager* manager, int draw);

    int addPolygonBuffer(int zoom, int col, int row, BufferManager* manager);
    int addPolylineBuffer(int zoom, int col, int row, BufferManager* manager);
    int addPointBuffer(int zoom, int col, int row, BufferManager* manager);
    int addAnnotationBuffer(int zoom, int col, int row, BufferManager* manager);
    bool isDashArrayValid(vector<float>& dash);
    bool loadVerticesFromDat(int zoom, int col, int row, float*& pts, int& size);
    bool loadStopsFromDat(int zoom, int col, int row, int*& stops, int& pointsCount, int& stopsCount);
    bool loadAnnoFromDat(int zoom, int col, int row, std::string& text);
    void initDatReader();


    // 根据瓦片计算dat文件中的坐标（尝试原始/翻转row）
    bool resolveDatCoords(int zoom, int col, int row, uint8_t type, int& datX, int& datY);

    int normalizeColumn(int col, int zoom) const;

    void logDatStatus(const std::string& stage, int zoom, int col, int row, int datX, int datY, bool hasData) const;


    // 记录当前zoom，用于检测zoom变化时清除坐标缓存
    int _currentZoom = -1;

    std::deque<Vec3i> _pendingTiles;
    std::unordered_set<uint64_t> _pendingTileSet;
    int _maxTilesPerFrame = 200;

    uint64_t makeTileKey(int zoom, int col, int row) const;
};

#endif