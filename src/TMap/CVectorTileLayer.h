#ifndef CVECTORTILEAYER_H
#define CVECTORTILEAYER_H

#include "CTileLayer.h"
#include "../TMScheduler/OMScheduler.h"
#include "../TMUtil/tmtools.h"
#include "../TMUtil/OMapGlobal.h"
#include "../OpenGLEngine/openglengine.h"
//#include "GL/glut.h"
#include "../TMUtil/OMGeoUtil.h"
//#include <mutex>

class CVectorTileLayer : public CTileLayer
{
public:
    CVectorTileLayer();
    CVectorTileLayer(string path) : _path(path) {};
    virtual ~CVectorTileLayer();
    void setAnnotation(int anno, string label);
    int getAnnotation();
    int draw(Recti bounds, int zoom);
    int draw(Recti bounds, int zoom, BufferManager* manager);
    int draw(vector<Vec3i> tiles, int zoom, BufferManager* manager, int crowdLevel);
    int addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int addBufferTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager){ return 0; }
    int drawTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager) { return 0; }
    int drawMultiThreads(void* para);
    int getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager);
    


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