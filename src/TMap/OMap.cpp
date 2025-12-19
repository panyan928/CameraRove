#include "OMap.h"
#include <fcntl.h>
#include <sstream>
#ifdef WIN32
#include <windows.h>
#else
#include "taskLib.h"
#endif
#include <set>

//�򿪻���TIN����
#if 0
#define TIN 1
#endif
using namespace std;
//这个暂时没用�?
OMap::OMap(string path)
{
	_scheduler = 0x00;
	_layers.clear();
    _display = 2;  // 0 nothing, 1 vector, 2 raster, 3 all
    lastDisplay=_display;
    isViewChanged = true;
    lastZoom = 0;
    isBreak = 0;
    isBreak2 = 0;
	initialMap(path);
}

OMap::OMap(const char* path)
{
    _crowd = 1;
    _scheduler = 0x00;
    _3dScheduler = 0x00;
    _nScheduler = 0x00;
    _manager = 0x00;
    _manager_2 = 0x00;
    _layers.clear();
    //_display = 3;  // 0 nothing, 1 vector, 2 raster, 3 all
    lastDisplay=_display;
    isViewChanged = true;
    lastZoom = 0;
    isBreak = 0;
    isBreak2 = 0;
    _brightness = 100; // [0, 1]
    string path_s = path;
    initialMap(path_s);
    setDislpay(1); //0-地形 1-矢量 2-卫星 3-卫星+矢量 4-地形+矢量
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
 * @brief 初始化地�?
 * @details 根据map.json中的配置信息初始化地图，主要包括：地图中心�?
 * 缩放等级、地图图层以及其它可变配�?
 * @param path map.json 配置路径
*/
#include "cJSON.h"
int OMap::initialMap(string json_path)
{
//#ifndef WIN32
//        chdir("D:\\");
//#endif
    
    long len=0;//�ļ�����
    char* content;//�ļ�����
    FILE* ifs = fopen(json_path.c_str(), "r");
    if (!ifs)
    {
        cout << "Error opening "<< json_path<<endl;
        //fclose(ifs);
        return -1;
    }
    else{
        cout << "ok opening "<< json_path<<endl;
    }
    fseek(ifs,0,SEEK_END);
    len = ftell(ifs);
    fseek(ifs, 0, SEEK_SET);
    content = (char*)malloc(len+1);
    memset(content, 0, len+1);
    fread(content,1,len, ifs);
    fclose(ifs);

    //CJson调用
#if 0
    cJSON* root = cJSON_Parse(content);
    if (!root) {
        const char* errptr = cJSON_GetErrorPtr();
        fprintf(stderr, "JSON 解析失败: %s\n", errptr ? errptr : "未知错误");
        free(content);
        return -1;
    }
    // 解析基本地图信息
    float zoom, lon, lat;
    //读取zoom
    cJSON* zoom1 = cJSON_GetObjectItem(root, "zoom");
    if (zoom1 != NULL && cJSON_IsNumber(zoom1)) {
        zoom = zoom1->valueint;
        printf("zoom: %d\n", zoom);
    }
    else {
        printf("zoom error\n");
        return -1;
    }
    //读取center
    cJSON* center1 = cJSON_GetObjectItem(root, "center");
    if (center1 != NULL && cJSON_IsArray(center1)) {
        // 获取第一个元素（整数）
        cJSON* item1 = cJSON_GetArrayItem(center1, 0);
        if (item1 != NULL && cJSON_IsNumber(item1)) {
            lon = item1->valuedouble;
            printf("lon: %.2f\n", lon);
        }
        else {
            printf("lon error\n");
            return -1;
        }
        // 获取第二个元素（浮点数，需要转换）
        cJSON* item2 = cJSON_GetArrayItem(center1, 1);
        if (item2 != NULL && cJSON_IsNumber(item2)) {
            lat = item2->valuedouble;
            printf("lat: %.2f\n", lat);
        }
        else {
            printf("lat error\n");
            return -1;
        }
    }
    else {
        printf("center error\n");
        return -1;
    }
    Vec2d center(lon, lat);
    _scheduler = new OMScheduler((int)zoom, center);

    // 解析图层
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    if (layers == NULL || !cJSON_IsArray(layers)) {
        printf("layers error\n");
        return -1;
    }
    int layerCount = cJSON_GetArraySize(layers); // 从JSON获取实际图层数量
    int i;
    for (i = 0; i < layerCount; i++) {
        cJSON* layer = cJSON_GetArrayItem(layers, i);
        // 解析每个图层信息
        char* layerType = cJSON_GetStringValue(cJSON_GetObjectItem(layer, "layer-type")); //从JSON获取图层类型
        if (layerType == NULL) return -1;
        if (strcmp(layerType, "vector-tile") == 0) {
            const char* path = cJSON_GetStringValue(cJSON_GetObjectItem(layer, "path"));
  

            CVectorTileLayer* vtLayer = new CVectorTileLayer(path);
            vtLayer->setLayerName(cJSON_GetStringValue(cJSON_GetObjectItem(layer, "layer-name")));
            string geo_type = cJSON_GetStringValue(cJSON_GetObjectItem(layer, "geometry-type"));
            vtLayer->setGeometryType(geo_type);

            cJSON* zoom2 = cJSON_GetObjectItem(layer, "zoom");
            int minZoom = cJSON_GetArrayItem(zoom2, 0)->valueint;
            int maxZoom = cJSON_GetArrayItem(zoom2, 1)->valueint;

            vtLayer->setZoom(Vec2i(minZoom, maxZoom));

            if (geo_type.compare("point") == 0) {
                int anno = cJSON_GetNumberValue(cJSON_GetObjectItem(layer, "anno"));
                string label = cJSON_GetStringValue(cJSON_GetObjectItem(layer, "anno-field"));
                vtLayer->setAnnotation(anno, label);
            }

            string stylePath = cJSON_GetStringValue(cJSON_GetObjectItem(layer, "style"));
            vtLayer->getOrCreateStyle()->loadStyleCmethod(stylePath.c_str());
            this->_layers.push_back(vtLayer);
        } 
        if (strcmp(layerType, "raster-tile") == 0) {
            const char* path = cJSON_GetStringValue(cJSON_GetObjectItem(layer, "path"));
            CRasterTileLayer* rtLayer = new CRasterTileLayer(path);
            rtLayer->setLayerName(cJSON_GetStringValue(cJSON_GetObjectItem(layer, "layer-name")));
            //        	rtLayer->setHeightPath(cJSON_GetStringValue(cJSON_GetObjectItem(layer, "heightPath")));
            cJSON* zoom2 = cJSON_GetObjectItem(layer, "zoom");
            int minZoom = cJSON_GetArrayItem(zoom2, 0)->valueint;
            int maxZoom = cJSON_GetArrayItem(zoom2, 1)->valueint;
            int height = cJSON_GetObjectItem(layer, "height")->valueint;
            Vec2i _zoom( minZoom, maxZoom);
            rtLayer->setZoom(_zoom);
            //        	rtLayer->setHeightExaggerated(height);

            //        	this->_layers.push_back(rtLayer);
            //        	map->layers[i] = rtLayer;
            this->_layers.push_back(rtLayer);
        }
    }
#endif
#if 1
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
    //调度的初始化在后面坐做了非空检查，这里删了也没事，缺少center参数
    /*_3dScheduler = new OM3DScheduler(center);
    _3dScheduler->setZoom(zoom);*/

    _scheduler = new OMScheduler((int)zoom, center);

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
            if(layer.isMember("anno")) {
                int anno = layer["anno"].asInt();
                if(layer.isMember("anno-field")) {      //如果layer有anno-field标签，则设置注释
                    string label = layer["anno-field"].asString();
                    vtLayer->setAnnotation(anno, label);
                }  
                else {
                    vtLayer->setAnnotation(anno, "");
                }
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
#endif
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
    //BufferManager* manager = getOrCreate2ndBufferManager();
    BufferManager* manager = getOrCreateBufferManager();
    vector<Vec3i> highTiles,lowTiles,nearTiles;
    
    OMScheduler* scheduler2d = getOrCreateScheduler();
    int zoom = scheduler2d->zoom();
    //scheduler->compute(int(zoom), nearTiles);
    int ret = scheduler2d->getTiles(nearTiles, zoom);
    cout << "buffer size: " << manager->size() << endl;
    //if((manager->size() + ret ) >= 1000){
    //    cout << "buffer size is too large, clear buffer" << endl;
    //    //manager->clear();
    //}
    {
        for (int i = 0; i < _layers.size(); i++) {
            if (!_layers[i]->isVisible())
                continue;
            if (isViewChanged)
            {
                isBreak = 1;
                cout << "stage 1 break in i = " << i << endl;
                return 0;
            }
            //Flong 增加的
// 
//栅格图 (raster-tile) - 根据displayMode和layer-name选择不同图层
            if (_layers[i]->geometryType().size() == 0) {
                string layerName = _layers[i]->layerName();
                bool shouldLoad = false;

                if (_display == 0 && layerName == "terrain") {
                    // 地形图模式：只加载terrain图层
                    shouldLoad = true;

                }
                else if (_display == 2 && layerName == "raster") {
                    // 卫星影像模式：只加载raster图层
                    shouldLoad = true;

                }
                else if (_display == 3 && layerName == "raster") {
                    // 矢量+卫星模式：加载raster图层
                    shouldLoad = true;

                }
                else if (_display == 4 && layerName == "terrain") {
                    // 矢量+地形模式：加载terrain图层
                    shouldLoad = true;

                }

                if (shouldLoad) {
                    _layers[i]->addBuffer(nearTiles, zoom, manager);
                }
            }

            //矢量图 (vector-tile)
            if ((_display == 1 || _display == 3 || _display == 4) && _layers[i]->geometryType().size() != 0)
            {
                _layers[i]->addBuffer(nearTiles, zoom, manager);
            }
        }
        isBreak = 0;
        isDoubleBufferLoaded = false;
        cout<<"get buffer stage 1"<<endl;
        return 0;
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
    //scheduler->computeWider(int(zoom), nearTiles);
    //scheduler->computeWider(nearTiles);
    scheduler2d->getTilesBuffer(nearTiles, zoom);
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
        //Flong 增加的
        //栅格图 (raster-tile) - 包括卫星影像和地形图
        if ((_display == 0 || _display == 2 || _display == 3 || _display == 4) && _layers[i]->geometryType().size() == 0) {
            _layers[i]->addBuffer(nearTiles, zoom, manager);
        }
        //矢量图 (vector-tile)
        if ((_display == 1 || _display == 3 || _display == 4) && _layers[i]->geometryType().size() != 0)
        {
            _layers[i]->addBuffer(nearTiles, zoom, manager);
        }
    }
    cout<<"get buffer stage 2-1"<<endl;


    // scheduler->compute(int(zoom + 1), highTiles);
    // scheduler->compute(int(zoom - 1), lowTiles);
    scheduler2d->getTilesBuffer(lowTiles, zoom-1);
    scheduler2d->getTilesBuffer(highTiles, zoom+1);
    for (int i = 0; i < _layers.size(); i++) {
        if (!_layers[i]->isVisible())
            continue;
        if (isViewChanged)
        {
            isBreak2 = 1;
            cout << "stage 2-2 break in i = " << i << endl;
            return 0;
        }

        //Flong新增
//栅格图 (raster-tile) - 根据displayMode和layer-name选择不同图层
        if (_layers[i]->geometryType().size() == 0) {
            string layerName = _layers[i]->layerName();
            bool shouldLoad = false;

            if (_display == 0 && layerName == "terrain") {
                // 地形图模式：只加载terrain图层
                shouldLoad = true;
            }
            else if (_display == 2 && layerName == "raster") {
                // 卫星影像模式：只加载raster图层
                shouldLoad = true;
            }
            else if (_display == 3 && layerName == "raster") {
                // 矢量+卫星模式：加载raster图层
                shouldLoad = true;
            }
            else if (_display == 4 && layerName == "terrain") {
                // 矢量+地形模式：加载terrain图层
                shouldLoad = true;
            }

            if (shouldLoad) {
                _layers[i]->addBuffer(highTiles, zoom + 1, manager);
                _layers[i]->addBuffer(lowTiles, zoom - 1, manager);
            }
        }

        if ((_display == 1 || _display == 3 || _display == 4) && _layers[i]->geometryType().size() != 0)
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
    OMScheduler* scheduler2d = getOrCreateScheduler();
    OM3DScheduler* scheduler = getOrCreate3DScheduler();
    BufferManager* manager = getOrCreateBufferManager();
    BufferManager* managerH = getOrCreate2ndBufferManager();
    vector<Vec3i> tiles;
    //这个zoom可以控制清晰度进而控制渲染效�?
    //int zoom = scheduler->zoom();
    // scheduler->getTiles(tiles, zoom,1);
    int zoom = scheduler2d->zoom();
    scheduler2d->getTiles(tiles, zoom);
    int state = 0;
    int len = _layers.size();
    for (int i = 0; i < len; i++) {
        if (!_layers[i]->isVisible())
            continue;
        //Flong增加的
 //绘制栅格图层 - 根据displayMode和layer-name选择不同图层
        if (_layers[i]->geometryType().size() == 0) {
            string layerName = _layers[i]->layerName();
            bool shouldDraw = false;

            if (_display == 0 && layerName == "terrain") {
                // 地形图模式：只绘制terrain图层
                shouldDraw = true;
            }
            else if (_display == 2 && layerName == "raster") {
                // 卫星影像模式：只绘制raster图层
                shouldDraw = true;
            }
            else if (_display == 3 && layerName == "raster") {
                // 矢量+卫星模式：绘制raster图层
                shouldDraw = true;
            }
            else if (_display == 4 && layerName == "terrain") {
                // 矢量+地形模式：绘制terrain图层
                shouldDraw = true;
            }

            if (shouldDraw) {
                _layers[i]->draw(tiles, zoom, manager, _crowd);
            }
        }
        //绘制矢量图层，矢量图层有几何类型
        if ((_display == 1 || _display == 3 || _display == 4) && _layers[i]->geometryType().size() != 0)
            _layers[i]->draw(tiles, zoom, manager, _crowd);
    }
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
            _layers[i]->draw(tiles, zoom, manager, _crowd);
        if ((_display == 3 || _display == 1) && _layers[i]->geometryType().size() != 0)
            _layers[i]->draw(tiles, zoom, manager, _crowd);
    }


    /******************采用独立坐标系统**************************/
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
    if (_display == 3) {
        turnOffLayer(1);
        turnOffLayer(4);
        turnOffLayer(5);
        turnOffLayer(9);
        turnOffLayer(10);
        turnOffLayer(11);
        //��
        /*turnOffLayer(2);
        turnOffLayer(6);
        turnOffLayer(7);
        turnOffLayer(12);
        turnOffLayer(13);
        turnOffLayer(14);
        turnOffLayer(15);
        turnOffLayer(23);
        turnOffLayer(29);*/
    }
    else if (_display == 1) {
        //��
        turnOnLayer(1);
        turnOnLayer(4);
        turnOnLayer(5);
        turnOnLayer(9);
        turnOnLayer(10);
        turnOnLayer(11);
        //��
        turnOnLayer(2);
        turnOnLayer(6);
        turnOnLayer(7);
        turnOnLayer(12);
        turnOnLayer(13);
        turnOnLayer(14);
        turnOnLayer(15);
        turnOnLayer(23);
        turnOnLayer(29);
    }
    else if (_display == 4) {
        turnOnLayer(1);
        turnOffLayer(4);
        turnOffLayer(5);
        turnOffLayer(9);
        turnOffLayer(10);
        turnOffLayer(11);
    }
}

CGeoLayer* OMap::getLayer(int i)
{
    return _layers[i];
}


int OMap::Dislpay()
{
    return _display;
}

int OMap::getCrowd() {
    return _crowd;
}
void OMap::setCrowd(int level) {
    _crowd = level;
}

void OMap::turnOfforOnLayerbyAnno(int anno)
{
    if(anno == 6){
        for (int i = 2; i < _layers.size(); i++) {
            // 使用 dynamic_cast 安全地将父类指针转换为子类指针
            CVectorTileLayer* vLayer = dynamic_cast<CVectorTileLayer*>(_layers[i]);
            if (vLayer != nullptr) {  // 检查转换是否成功
                int _anno = vLayer->getAnnotation();
                if(_anno == 1 || _anno == 2 || _anno == 3 || _anno == 4 || _anno == 5){
                    vLayer->setVisible(true);
                }
            }
        }
    }
    else{
        bool isVisible  = true;
        for (int i = 2; i < _layers.size(); i++) {
            // 使用 dynamic_cast 安全地将父类指针转换为子类指针
            CVectorTileLayer* vLayer = dynamic_cast<CVectorTileLayer*>(_layers[i]);
            if (vLayer != nullptr) {  // 检查转换是否成功
                if (vLayer->getAnnotation() == anno) {
                    isVisible = vLayer->isVisible();
                    vLayer->setVisible(!isVisible);
                }
            }
        }
    }  
   
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
        _manager = new BufferManager(2000);
    return _manager;
}

BufferManager* OMap::getOrCreate2ndBufferManager()
{
    if (_manager_2 == 0x00)
        _manager_2 = new BufferManager(2000);
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
