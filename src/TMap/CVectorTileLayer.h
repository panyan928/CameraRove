#ifndef CVECTORTILEAYER_H
#define CVECTORTILEAYER_H

#include "CTileLayer.h"
#include "../TMScheduler/OMScheduler.h"
#include "../TMUtil/tmtools.h"
#include "../TMUtil/OMapGlobal.h"
#include "../OpenGLEngine/openglengine.h"
//#include "GL/glut.h"
#include "../TMUtil/OMGeoUtil.h"
#include "BatchRenderer.h"
//#include <mutex>

// 前向声明
class BatchRenderer;

class CVectorTileLayer : public CTileLayer
{
public:
    CVectorTileLayer();
    CVectorTileLayer(string path) : _path(path) {};
    virtual ~CVectorTileLayer() = default;
    void setAnnotation(int anno, string label);

    int draw(Recti bounds, int zoom);
    int draw(Recti bounds, int zoom, BufferManager* manager);
    int draw(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int addBufferTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager){ return 0; }
    int drawTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager) { return 0; }
    int drawMultiThreads(void* para);
    int getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager);
    
    // 新增：使用批处理渲染器渲染
    int draw_batch(vector<Vec3i> tiles, int zoom, BufferManager* manager, BatchRenderer* batchRenderer);
    // 新增：将数据添加到批处理渲染器
    void addToBatchRenderer(int zoom, int col, int row, BufferManager* manager, BatchRenderer* batchRenderer);

private:
    string _path;
    int _anno = 0;
    string _label;
    //mutex _drawMutex;

    int drawPolygon(int zoom, int col, int row, BufferManager* manager, int draw);
    int drawPolyline(int zoom, int col, int row, BufferManager* manager, int draw);
    int drawPoint(int zoom, int col, int row, BufferManager* manager, int draw);
    int drawAnnotation(int zoom, int col, int row, BufferManager* manager, int draw);

    int addPolygonBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db);
    int addPolylineBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db);
    int addPointBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db);
    int addAnnotationBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db);
    bool isDashArrayValid(vector<float>& dash);
    

    DataUnit* getDataUnit(TMBuffer* buffer, DataType type); 
};

#endif