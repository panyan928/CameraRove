#ifndef OMap_H
#define OMap_H

#include "../TMUtil/OMapGlobal.h"
#include "../TMScheduler/OMScheduler.h"
#include "../TMScheduler/OM3DScheduler.h"
#include "../TMScheduler/cscheduler.h"
#include "CGeoLayer.h"
#include "CVectorTileLayer.h"
#include "crastertilelayer.h"
#include "json/json.h"
#include "../TMDataBuffer/buffermanager.h"

//#include <thread>

/**
 * @author X-Lab MYL
 * @date 2021-10-12
 * @version 1.0
 * @brief 地图管理类
 * @details 提供地图数据加载、数据转换、地图绘制和地图操作等接口
*/

struct DataParameter {
    CGeoLayer* layer;
    vector<Vec3i> tiles;
    int zoom;
    BufferManager* manager;
};

class OMap
{
public:
    OMap(string path);
    OMap(const char* path);
    ~OMap();

    int initialMap(string path);
    int draw();
    int draw1();
    int getBuffer();
    //int drawMultiThreads();
    
    //int getData();
    int getData(int level);
    void setDislpay(int mode);
    CGeoLayer* getLayer(int i);
    int Dislpay();
    int getCrowd();
    void setCrowd(int level);
    void turnOfforOnLayerbyAnno(int anno);
    void turnOffLayer(int index);
    void turnOnLayer(int index);
    //int multiGetData(DataParameter* para);

    OMScheduler*    getOrCreateScheduler();
    OM3DScheduler*  getOrCreate3DScheduler();
    CScheduler*     getOrCreateNScheduler();
    BufferManager*  getOrCreateBufferManager();
    BufferManager*  getOrCreate2ndBufferManager();
    

    int             changeViewer(int mode);
    int             changeBrightness(int value);
    void            turnUpBrightness() { _brightness += 5; if (_brightness > 100) _brightness = 100; }
    void            turnDownBrightness() { _brightness -= 5; if (_brightness < 0) _brightness = 0; }
    int             getBrightness() const { return _brightness; }
    void            swapBuffer();
    bool           _bufferLock = false;
    bool            isViewChanged = false;
    bool            isDoubleBufferLoaded = false;
    vector<Vec3i>   tiles;
    int isBreak , isBreak2 ;
private:
    OMScheduler* _scheduler;
    OM3DScheduler* _3dScheduler;
    CScheduler* _nScheduler;

    vector<CGeoLayer*> _layers;
    BufferManager* _manager;
    //双缓存
    BufferManager* _manager_2;

    //vector<thread> dataThreads;
    vector<DataParameter> dataParams;
    vector<Vec3i> lastNearTiles;
    int lastZoom;

    int            _display;
    int            lastDisplay;
    int            _viewer;
    int            _brightness;
    int            _crowd = 6; //防拥级别1-6 1 - 水系 2 - 县以下 3 - 道路 4 - 大城市 5 - 机场 6 - 全显示
    int isSameTiles(vector<Vec3i> nearTiles, vector<Vec3i> lastNearTiles);
};

#endif

