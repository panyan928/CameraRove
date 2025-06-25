
#ifndef CGEOLAYER_H
#define CGEOLAYER_H


#include "../TMUtil/OMapGlobal.h"
#include "../TMStyle/cstyle.h"
#include "../TMDataBuffer/buffermanager.h"

using namespace TMStyle;



class CGeoLayer
{
public:
    CGeoLayer();
    virtual ~CGeoLayer();
    virtual int draw(Recti bounds, int zoom) = 0;
    virtual int draw(Recti bounds, int zoom, BufferManager* manager) = 0;
    virtual int draw(vector<Vec3i> tiles, int zoom, BufferManager* manager) = 0;
    virtual int drawTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager) = 0;

    virtual int addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager) = 0;
    virtual int addBufferTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager) = 0;
    virtual int drawMultiThreads(void* para) = 0;
    virtual int getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager) = 0;

    int _amount = 0;
    int _time;
    int count = 0;
    int allCount = 0;
    int pierce = 0;

    string layerName() const;
    void setLayerName(string name);

    string geometryType() const;
    void setGeometryType(string type);

    Color color() const;
    void setColor(Color color);

    Vec2i zoom() const;
    void setZoom(Vec2i zoom);

    int maxZoom() const { return _maxZoom; }
    void  setMaxZoom(int zoom) { _maxZoom = zoom; }

    CStyle* getOrCreateStyle();

    bool isVisible() const { return _isVisible; }
    void setVisible(bool visible) { _isVisible = visible; }

private:
    string _layerName;
    string _geometryType;
    Color  _color;
    TMStyle::CStyle* _style;
    Vec2i _zoom;
    int _maxZoom;
    bool _isVisible;
};

struct drawParameter {
    CGeoLayer* layer;
    vector<Vec3i> tiles;
    int zoom;
    BufferManager* manager;
};

#endif // !1
