#include "OMap.h"
#include <fcntl.h>
#include <sstream>
#ifdef WIN32
#include <windows.h>
#else
#include "taskLib.h"
#endif
#include <set>

//打开绘制TIN网格
#if 0
#define TIN 1
#endif

//杩欎釜鏆傛椂娌＄敤涓?
OMap::OMap(string path)
{
	_scheduler = 0x00;
	_layers.clear();
    _display = 1;  // 0 nothing, 1 vector, 2 raster, 3 all
    lastDisplay=_display;
    isViewChanged = true;
    lastZoom = 0;
    isBreak = 0;
    isBreak2 = 0;
	initialMap(path);
}

OMap::OMap(const char* path)
{
    _scheduler = 0x00;
    _3dScheduler = 0x00;
    _nScheduler = 0x00;
    _manager = 0x00;
    _manager_2 = 0x00;
    _layers.clear();
    _display = 2;  // 0 nothing, 1 vector, 2 raster, 3 all
    lastDisplay=_display;
    isViewChanged = true;
    lastZoom = 0;
    isBreak = 0;
    isBreak2 = 0;
    _brightness = 100; // [0, 1]
    string path_s = path;
    initialMap(path_s);
}

OMap::~OMap()
{
    if (_scheduler != 0x00)
        delete _scheduler;
    _scheduler = 0x00;

    if (_3dScheduler != 0x00)
        delete _3dScheduler;
    _3dScheduler = 0x00;

    if (_nScheduler != 0x00)
        delete _nScheduler;
    _nScheduler = 0x00;

    if (_manager != 0x00)
        delete _manager;
    _manager = 0x00;
    int len = _layers.size();
    for (int i = 0; i < len; i++) {
        if (_layers[i] != 0x00)
            delete _layers[i];
        _layers[i] = 0x00;
    }
    _layers.clear();
}

/**
 * @brief 鍒濆鍖栧湴鍥?
 * @details 鏍规嵁map.json涓殑閰嶇疆淇℃伅鍒濆鍖栧湴鍥撅紝涓昏鍖呮嫭锛氬湴鍥句腑蹇冦�?
 * 缂╂斁绛夌骇銆佸湴鍥惧浘灞備互鍙婂叾瀹冨彲鍙橀厤缃?
 * @param path map.json 閰嶇疆璺緞
*/
int OMap::initialMap(string path)
{
//#ifndef WIN32
//        chdir("D:\\");
//#endif
    
    long len=0;//文件长度
    char* content;//文件内容
    FILE* ifs = fopen(path.c_str(), "r");
    if (!ifs)
    {
        cout << "Error opening "<<path<<endl;
        //fclose(ifs);
        return -1;
    }
    else{
        cout << "ok opening "<<path<<endl;
    }
    fseek(ifs,0,SEEK_END);
    len = ftell(ifs);
    fseek(ifs, 0, SEEK_SET);
    content = (char*)malloc(len+1);
    memset(content, 0, len+1);
    fread(content,1,len, ifs);
    fclose(ifs);

    Json::Value mapRoot;
    Json::Reader jsonReader;
    if (!jsonReader.parse(content, mapRoot))
    {
        cout << "Error reading map.json\n";
        free(content);
        return -1;
    }
    else{
		cout << "ok reading map.json\n";
	}
    
    // base map info
    float zoom = mapRoot["zoom"].asFloat();
   
    double lon=0.0, lat=0.0;
    lon = mapRoot["center"][0].asDouble();
    lat = mapRoot["center"][1].asDouble();
    Vec2d center(lon, lat);
    //璋冨害鐨勫垵濮嬪寲鍦ㄥ悗闈㈠潗鍋氫簡闈炵┖妫�鏌ワ紝杩欓噷鍒犱簡涔熸病浜嬶紝缂哄皯center鍙傛暟
    _3dScheduler = new OM3DScheduler(center);
    _3dScheduler->setZoom(zoom);
    //load layers
    int size = mapRoot["layers"].size();
    for (int i = 0; i < size; i++) {
        Json::Value layer = mapRoot["layers"][i];
        string type = layer["layer-type"].asString();
        if (type.compare("vector-tile") == 0) {
            string path = layer["path"].asString();
            CVectorTileLayer* vtLayer = new CVectorTileLayer(path);
            vtLayer->setLayerName(layer["layer-name"].asString());
            string geo_type = layer["geometry-type"].asString();
            vtLayer->setGeometryType(geo_type);
            
            int minZoom = layer["zoom"][0].asInt();
            int maxZoom = layer["zoom"][1].asInt();

            vtLayer->setZoom(Vec2i(minZoom, maxZoom));

            if (geo_type.compare("point") == 0) {
                int anno = layer["anno"].asInt();
                string label = layer["anno-field"].asString();
                vtLayer->setAnnotation(anno, label);
            }

            string stylePath = layer["style"].asString().c_str();
            vtLayer->getOrCreateStyle()->loadStyleCmethod(stylePath.c_str());
            this->_layers.push_back(vtLayer);        
        }
        if (type.compare("raster-tile") == 0) {
            string path = layer["path"].asString();
            CRasterTileLayer* rtLayer = new CRasterTileLayer(path);
            rtLayer->setLayerName(layer["layer-name"].asString());

            string heightPath = layer["heightPath"].asString();
            rtLayer->setHeightPath(heightPath);

            int minZoom = layer["zoom"][0].asInt();
            int maxZoom = layer["zoom"][1].asInt();
            int height  = layer["height"].asInt();

            rtLayer->setZoom(Vec2i(minZoom, maxZoom));
            rtLayer->setHeightExaggerated(height);

            this->_layers.push_back(rtLayer);
        }
    }
    free(content);
	return 0;
}

int OMap::isSameTiles(vector<Vec3i> nearTiles,vector<Vec3i> lastNearTiles){
    int len=nearTiles.size(),lastLen=lastNearTiles.size();
    if(len != lastLen){
        return 0;
    }
    else {
        if (len != 0) {
            for (int i=0;i<len;i++) {
                if ((nearTiles[i][0] != lastNearTiles[i][0]) ||\
                    (nearTiles[i][1] != lastNearTiles[i][1]) || \
                    (nearTiles[i][2] != lastNearTiles[i][2])
                    ) {
                    return 0;
                }
            }
            return 1;
        }
        else {
            return 1;
        }
    }
}

int OMap::getBuffer() {
    isViewChanged = false;
    OM3DScheduler* scheduler = getOrCreate3DScheduler();
    BufferManager* manager = getOrCreate2ndBufferManager();
    BufferManager* managerF = getOrCreateBufferManager();
    vector<Vec3i> highTiles,lowTiles,nearTiles;
    int zoom = scheduler->zoom();
    
    scheduler->compute(int(zoom), nearTiles);
    /*if( (isBreak) || (lastDisplay != _display) || (lastZoom!=zoom)||( !isSameTiles(nearTiles,lastNearTiles) )){
        lastNearTiles=nearTiles;
        lastZoom=zoom;
        isBreak = 0;
        isBreak2=0;//高优先级情况，消除掉isBreak2=1的状态
        lastDisplay=_display;
    }
    else if(isBreak2){
        ;//因为下面要用到isBreak2，这里不能isBreak2=0;
    }
    else{
        cout << "nearTiles==lastNearTiles" << endl;
        #ifdef WIN32
            Sleep(100);
        #else
            taskDelay(500);
        #endif
        
        return 0;
    } */

    //if(!isBreak2)
    {
        //如果是isBreak2导致的,不进行stage 1，不交换内存
        for (int i = 0; i < _layers.size(); i++) {
            if (!_layers[i]->isVisible())
                continue;
            if (isViewChanged)
            {
                isBreak = 1;
                cout << "stage 1 break in i = " << i << endl;
                return 0;
            }
            if ((_display == 2 || _display == 3) && _layers[i]->geometryType().size() == 0) {
#ifdef TIN
                _layers[i]->addBufferTIN(nearTiles, zoom, manager);
#else
                _layers[i]->addBuffer(nearTiles, zoom, manager);
#endif
            }
                
            if ((_display == 1 || _display == 3) && _layers[i]->geometryType().size() != 0)
            {
                _layers[i]->addBuffer(nearTiles, zoom, manager);
            }
        }
        isBreak = 0;
        isDoubleBufferLoaded = true;
        cout<<"get buffer stage 1"<<endl;
        while (isDoubleBufferLoaded); 

        
    } 
#if 0
    int len = nearTiles.size();
    set<string> processed;
    ostringstream ost_temp;
    for (int i = 0; i < len; i++) {
        ost_temp.str("");
        ost_temp << (nearTiles[i][0]) << "." << (nearTiles[i][1]) << "." << (nearTiles[i][2]);
        string index = ost_temp.str();
        processed.insert(index);
    }
    scheduler->computeWider(int(zoom), nearTiles,processed);
#else
    scheduler->computeWider(int(zoom), nearTiles);
    //scheduler->computeWider(nearTiles);
#endif
    for (int i = 0; i < _layers.size(); i++) {
        if (!_layers[i]->isVisible())
            continue;
        if (isViewChanged)
        {
            isBreak2 = 1;
            cout << "stage 2-1 break in i = " << i << endl;
            return 0;
        }
        if ((_display == 2 || _display == 3) && _layers[i]->geometryType().size() == 0) {
#ifdef TIN
            _layers[i]->addBufferTIN(nearTiles, zoom, manager);
#else
            _layers[i]->addBuffer(nearTiles, zoom, manager);
#endif
        }
            
        if ((_display == 1 || _display == 3) && _layers[i]->geometryType().size() != 0)
        {
            _layers[i]->addBuffer(nearTiles, zoom, manager);
        }
    }
    cout<<"get buffer stage 2-1"<<endl;


    scheduler->compute(int(zoom + 1), highTiles);
    scheduler->compute(int(zoom - 1), lowTiles);
    for (int i = 0; i < _layers.size(); i++) {
        if (!_layers[i]->isVisible())
            continue;
        if (isViewChanged)
        {
            isBreak2 = 1;
            cout << "stage 2-2 break in i = " << i << endl;
            return 0;
        }
        if ((_display == 2 || _display == 3) && _layers[i]->geometryType().size() == 0) {
#ifdef TIN
            _layers[i]->addBufferTIN(highTiles, zoom + 1, manager);
            _layers[i]->addBufferTIN(lowTiles, zoom - 1, manager);
            //_layers[i]->addBufferTIN(nearTiles, zoom, manager);
#else
            _layers[i]->addBuffer(highTiles, zoom + 1, manager);
            _layers[i]->addBuffer(lowTiles, zoom - 1, manager);
            //_layers[i]->addBuffer(nearTiles, zoom, manager);
#endif
            
        }

        if ((_display == 1 || _display == 3) && _layers[i]->geometryType().size() != 0)
        {
            _layers[i]->addBuffer(highTiles, zoom + 1, manager);
            _layers[i]->addBuffer(lowTiles, zoom - 1, manager);
            //_layers[i]->addBuffer(nearTiles, zoom, manager);
        }
    }
    isBreak2 = 0;
    cout<<"get buffer stage 2-2"<<endl;    
    
    //isDoubleBufferLoaded = true;
    return 0;
}
int OMap::draw()
{

    OM3DScheduler* scheduler = getOrCreate3DScheduler();
    BufferManager* manager = getOrCreateBufferManager();
    BufferManager* managerH = getOrCreate2ndBufferManager();
    vector<Vec3i> tiles;
    //杩欎釜zoom鍙互鎺у埗娓呮櫚搴﹁繘鑰屾帶鍒舵覆鏌撴晥鐜?
    int zoom = scheduler->zoom();
    scheduler->getTiles(tiles, zoom,1);

    int state = 0;
    int len = _layers.size();
    for (int i = 0; i < len; i++) {
        if (!_layers[i]->isVisible())
            continue;
        
        //if (_display == 3) {
        //    
        //    if (_layers[i]->geometryType().size() == 0) {
        //        state = _layers[i]->draw(tiles, zoom, manager);
        //    }
        //    else {
        //        if (state == 0) {
        //            if (_layers[i]->geometryType().compare("polygon") != 0)
        //                _layers[i]->draw(tiles, zoom, manager);
        //        }
        //        else
        //            _layers[i]->draw(tiles, zoom, manager);
        //    }

        //}
        if ((_display == 2 || _display == 3) && _layers[i]->geometryType().size() == 0)
        {
#ifdef TIN
            _layers[i]->drawTIN(tiles, zoom, manager);
#else
            _layers[i]->draw(tiles, zoom, manager);
#endif
        }
            
        if ((_display == 1 || _display == 3) && _layers[i]->geometryType().size() != 0)
            _layers[i]->draw(tiles, zoom, manager);
    }
  

    
    /******************閲囩敤鐙珛鍧愭爣绯荤粺**************************/
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    glEnable(GL_BLEND);
    glColor4f(0.0, 0.0, 0.0, 1 - _brightness / 100.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1, 1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(-1, -1, 0);
    glEnd();

    glPopMatrix();
    glDisable(GL_BLEND);

    return 0;
}

int OMap::draw1()
{
    CScheduler* scheduler = getOrCreateNScheduler();
    BufferManager* manager = getOrCreateBufferManager();

    vector<Vec3i> tiles;
    int zoom = scheduler->getOrCreateCamera()->zoom();
    scheduler->compute(tiles);
    int len = _layers.size();
    for (int i = 0; i < len; i++) {
       /* if (_layers[i]->layerName().compare("china_region") == 0)
            _layers[i]->draw(tiles, zoom, manager);*/
        if ((_display == 3 || _display == 2) && _layers[i]->geometryType().size() == 0)
            _layers[i]->draw(tiles, zoom, manager);
        if ((_display == 3 || _display == 1) && _layers[i]->geometryType().size() != 0)
            _layers[i]->draw(tiles, zoom, manager);
    }


    /******************閲囩敤鐙珛鍧愭爣绯荤粺**************************/
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    glLoadIdentity();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
    
    glEnable(GL_BLEND);
    glColor4f(0.0, 0.0, 0.0, 1 - _brightness / 100.0);
    glBegin(GL_POLYGON);
        glVertex3f(-1, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(-1, -1, 0);
    glEnd();
    
    glPopMatrix();
    glDisable(GL_BLEND);
    return 0;
}

//void drawUnit(void* para) {
//    drawParameter* draw = static_cast<drawParameter*>(para);
//
//    cout << draw->layer->layerName() << " " << draw->tiles.size() << endl;
//
//    if (draw && draw->layer)
//        draw->layer->draw(draw->tiles, draw->zoom, draw->manager);
//}
//
//int OMap::drawMultiThreads()
//{
//    //OMScheduler* scheduler = getOrCreateScheduler();
//    OM3DScheduler* scheduler = getOrCreate3DScheduler();
//
//    BufferManager* manager = getOrCreateBufferManager();
//    manager->setCapacity(500, 100);
//    clock_t begin, end;
//    begin = clock();
//    scheduler->compute();
//    end = clock();
//    //cout << "schedule tile:" << end - begin << " ms" << endl;
//
//    vector<Vec3i> tiles = scheduler->tileSet();
//    //Recti bounds = scheduler->tileBound();
//    float zoom = scheduler->zoom();
//    
//    thread layerThreads[100];
//    drawParameter draws[100];
//
//    begin = clock();
//    for (int i = 0; i < _layers.size(); i++) {
//        draws[i].layer = _layers[i];
//        draws[i].manager = manager;
//        draws[i].tiles = vector<Vec3i>(tiles.begin(), tiles.end());
//        draws[i].zoom = zoom;
//
//        layerThreads[i] = thread(drawUnit, &draws[i]);
//        //_layers[i]->drawMultiThreads(draw);
//    }
//    
//    for (int i = 0; i < _layers.size(); i++) {
//        //if (layerThreads[i].joinable())
//        layerThreads[i].join();
//    }
//
//    //if (layerThreads != 0x00)
//        //delete[] layerThreads;
//
//    end = clock();
//    //cout << "render data:" << end - begin << " ms" << endl;
//    return 0;
//}
//
//int multiGetData(CGeoLayer* layer, vector<Vec3i> tiles, int zoom, BufferManager* manager) 
//{
//    layer->getData(tiles, zoom, manager);
//    return 0;
//}

int OMap::getData(int level)
{
    OM3DScheduler* scheduler = getOrCreate3DScheduler();

    BufferManager* manager = getOrCreateBufferManager();

    int zoom = scheduler->zoom();
    vector<Vec3i> tiles;
    scheduler->compute(zoom + level, tiles);
    
    int all = 0;
    int len = _layers.size();
    for (int i = 0; i < len; i++) {
        all += _layers[i]->getData(tiles, zoom + level, manager);
    }

    return all;
}

void OMap::setDislpay(int mode)
{
    _display = mode;
}

CGeoLayer* OMap::getLayer(int i)
{
    return _layers[i];
}


int OMap::Dislpay()
{
    return _display;
}

void OMap::turnOffLayer(int index)
{
    if (index < 0 || index >= _layers.size())
        return;
    _layers[index]->setVisible(false);
}

void OMap::turnOnLayer(int index)
{
    if (index < 0 || index >= _layers.size())
        return;
    _layers[index]->setVisible(true);
}

OMScheduler* OMap::getOrCreateScheduler()
{
	if (_scheduler == 0x00)
		_scheduler = new OMScheduler();
	return _scheduler;
}

OM3DScheduler* OMap::getOrCreate3DScheduler()
{
    if (_3dScheduler == 0x00)
        _3dScheduler = new OM3DScheduler();
    return _3dScheduler;
}

CScheduler* OMap::getOrCreateNScheduler()
{
    if (_nScheduler == 0x00)
        _nScheduler = new CScheduler();
    return _nScheduler;
}

BufferManager* OMap::getOrCreateBufferManager()
{
    if (_manager == 0x00)
        _manager = new BufferManager(1000);
    return _manager;
}

BufferManager* OMap::getOrCreate2ndBufferManager()
{
    if (_manager_2 == 0x00)
        _manager_2 = new BufferManager(1000);
    return _manager_2;
}

int OMap::changeBrightness(int value)
{
    _brightness = value;
    return 0;
}

void OMap::swapBuffer()
{
#if 0
    BufferManager tmp = *_manager;
    *_manager = *_manager_2;
    *_manager_2 = tmp;
#else
    BufferManager *tmp = _manager;
    _manager = _manager_2;
    _manager_2 = tmp;
#endif
    cout << "swap buffer" << endl;
}
