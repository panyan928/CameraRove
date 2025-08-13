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
#include "BatchRenderer.h"
//#include <thread>

/**
 * @author X-Lab MYL
 * @date 2021-10-12
 * @version 1.0
 * @brief ��ͼ������
 * @details �ṩ��ͼ���ݼ��ء�����ת������ͼ���ƺ͵�ͼ�����Ƚӿ�
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
    int draw_batch();
    int getBuffer();
    //int drawMultiThreads();
    
    //int getData();
    int getData(int level);
    void setDislpay(int mode);
    CGeoLayer* getLayer(int i);
    int Dislpay();

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
    //˫����
    BufferManager* _manager_2;
    BatchRenderer* _globalBatchRenderer;

    //vector<thread> dataThreads;
    vector<DataParameter> dataParams;
    vector<Vec3i> lastNearTiles;
    int lastZoom;

    int            _display;
    int            lastDisplay;
    int            _viewer;
    int            _brightness;
    int isSameTiles(vector<Vec3i> nearTiles, vector<Vec3i> lastNearTiles);
};

#endif

