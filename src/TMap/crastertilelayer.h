#ifndef CRasterTileLayer_H
#define CRasterTileLayer_H

#include "CTileLayer.h"
#include "../OpenGLEngine/openglengine.h"
#include "../OpenGLEngine/openglfileengine.h"
#include "../OpenGLEngine/openglrenderengine.h"

//#include "OMFileEngine/openglvertice.h"
//#include "OMRenderEngine/OpenGLRenderEngine.h"

class CRasterTileLayer : public CTileLayer
{
public:
    CRasterTileLayer(string path);
    virtual ~CRasterTileLayer();

    int draw(Recti bounds, int zoom);
    int draw(Recti bounds, int zoom, BufferManager* manager);
    int draw(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int drawTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int addBufferTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager);
    int drawMultiThreads(void* para);
    int getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager);
    int heightExaggerated() const;
    void setHeightExaggerated(int height);

    string heightPath() const;
    void setHeightPath(string path);

    Vec3d calColor(float height);
    int displayMode = 1;
private:
    string _path;
    string _heightPath;
    int _heightExaggerated = 1;
    float* _textures;
    int index_num;
};

#endif // !CRasterTileLayer_H