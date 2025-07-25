#include "mapDrv.h"
#include "InputCApi.h"

#define M_PI 3.141592653
#include "gl\gl.h"	
#include "gl\glu.h"
#include "TMap/JSONLayer.h"
#include "OpenGLEngine/openglengine.h"
#include "TMap/OMap.h"
#include <string>
#include <time.h>
#include <sstream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#ifdef WIN32
#include <pthread.h>
#include <windows.h>
//#include <thread>
//#pragma comment(lib,"libs/pthread/lib/pthreadVC2.lib")
pthread_t bufferTid[10];
pthread_mutex_t g_mutex;
#else
#include "init.h"
#include "taskLib.h"
#endif

#ifdef WIN32
OMap* _map = new OMap("D://pyan//map_wd_20250507//3d//map_jx.json");
//OMap* _map = new OMap("./../data/map_night.json");
//JSONLayer *queryLayer = new JSONLayer("./../data/jiangxi.geojson");
#else//目前只适用于tm3
#include "gljos.h"
#include "GL/gl.h"
#include "glj.h"
OMap* _map = new OMap("D:\\map_tm.json");
//JSONLayer *queryLayer = new JSONLayer("D:\\jiangxi.geojson");
#endif

JTFONT hzFont[2];
CFontRender* render = new CFontRender(1920, 1080);
OM3DScheduler* _scheduler = _map->getOrCreate3DScheduler();
OMScheduler* _scheduler2d = _map->getOrCreateScheduler();   
BufferManager* manager = _map->getOrCreateBufferManager();
CScheduler* _nScheduler = _map->getOrCreateNScheduler();

Vec3d lastEye = Vec3d(0, 0, 0);
int ThreadCount = 0;
float* pts;
float* pts_earth;
int* index_temp;
float* texcoords;
int angle1 = 0;

using namespace textRender;

float* textures;
bool earth = true;

int isRoma=0;

void* romaFucn(void* args);

Vec3d getHeight(double lon, double lat) {
    int zoom = 10;
    int cellNum = 66;
    //sqlite3* heightDB = NULL;
    string path = "D:/pyan/map_wd_20221219/data/jiangxi_height/";
    /*int iRet = sqlite3_open(path.c_str(), &heightDB);
    if (!iRet)
    {
        cout << "open height db failed:" << sqlite3_errmsg(heightDB) << endl;
    }*/
    int tileNum[2];
    CGeoUtil::deg2num(lat, lon, zoom, tileNum);

    int row = tileNum[1];
    int col = tileNum[0];
    Vec2d leftTop;
    CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1], CGeoUtil::formBottom2Top);
    double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom);
    double MecX, MecY;
    CGeoUtil::lonLat2WebMercator(lat, lon, MecX, MecY);
    unsigned int cellX = (MecX - leftTop[0]) / span * cellNum;
    unsigned int cellY = (leftTop[1] - MecY) / span * cellNum;
    int hSize = 0;
    //short int* heights = openglEngine::OpenGLFileEngine::getHeightFromDB<short int>(heightDB, zoom, pow(2, zoom) - row - 1, col, hSize);
    int row1 = pow(2, zoom) - row - 1;
    //path = path + to_string(zoom) + "." + to_string(row1) + "." + to_string(col) + ".height";
    ostringstream ost_temp;//ost_temp.str("");
    ost_temp << path << (zoom) << "." << (row1) << "." << (col) << ".height";
    path = ost_temp.str();
    short int* heights = openglEngine::OpenGLFileEngine::getHeightFromBinary<short int>(path.c_str(), hSize);
    short int height = heights[cellY * 66 + cellX];
    Vec3d pt = Vec3d(lon, lat, height);
    return pt;
}

Vec3d* calSec(double startLon, double startLat, double endLon, double endLat, int num) {
    Vec3d* pts = new Vec3d[10];
    double spanLon = (endLon - startLon) / (num - 1);
    double spanLat = (endLat - startLat) / (num - 1);
    for (int i = 0; i < num; i++)
    {
        pts[i] = getHeight(startLon + i * spanLon, startLat + i * spanLat);
        cout << "第" << i + 1 << "个点的经度为：" << pts[i][0] << " 纬度为：" << pts[i][1] << " 高程为：" << pts[i][2] << endl;
    }
    return pts;
}

double calGreatCircleDis(double startLon, double startLat, double endLon, double endLat) {
    double R = (6378137 + 6356752.31414) / 2;
    double PI = CGeoUtil::PI;
    double dis = R * acos(cos(startLat * PI / 180) * cos(endLat * PI / 180) * cos((startLon - endLon) * PI / 180) + sin(startLat) * sin(endLat)) * PI / 180 / 1000;
    cout << "经纬度："<<startLon<<","<<startLat<<"到"<<"经纬度："<<endLon<<","<<endLat <<"的"<<endl;
    cout << "大圆距离为：" << dis << "千米"<<endl;
    cout <<  endl;
    return dis;
}

//开辟新线程，加载缓存数据
void* getBuffer(void* args) {

    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#ifdef WIN32
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif
    //pthread_cleanup_push(cancelThread,NULL);
    //_map->_bufferLock = true;
    //pthread_mutex_lock(&g_mutex);
    //cout << "加锁" << endl;
    //Sleep(2000);
    
    while (true)
    {
        if (_map->isViewChanged)
        {
            cout<<"********isViewChanged********"<<endl;
            _map->getBuffer();
        }
        /*else{
            #ifdef WIN32
                Sleep(100);
            #else
                taskDelay(200);
            #endif
        }*/
         //cout << "开始缓存" << endl;
    }
    
    //pthread_mutex_unlock(&g_mutex);
    //cout << "解锁" << endl;
    //int q = 0;
    //_map->_bufferLock = false;
    //pthread_cleanup_pop(0);
    //pthread_exit(NULL);
    


    return NULL;
}

void renderFontArray()
{
    renderArray();
}

void drawFontE(float x,float y,char* a) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 0, 768, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    oglfDrawString(hzFont[0], x, y, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);

    memset(a,0,30);
	sprintf(a,"zoom=%d", _scheduler->zoom());
	oglfDrawString(hzFont[1], x, y-20, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);

	memset(a,0,30);
	sprintf(a,"Version: 1.0");
	oglfDrawString(hzFont[1], x, y-40, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);
}

void drawRasters(int zoom, int beginRow, int endRow, int beginCol, int endCol) {
    for (int i = beginRow; i <= endRow; i++) {
        for (int j = beginCol; j <= endCol; j++) {
            drawRaster(zoom, i, j);
        }
    }
}

void ME_ST_TC_014() {
    _scheduler->scale = 3;
    _scheduler->_zoom = 3;
    CRasterTileLayer *layer = (CRasterTileLayer*)_map->getLayer(0);
    layer->displayMode = 2;
    _map->setDislpay(2);
    for (int i = 0; i < 6; i++) {
        _scheduler->zoomIn();
    }
    for (int i = 0; i < 30; i++) {
        _scheduler->pan(1);
        _scheduler->pan(2);
    }
    _map->isViewChanged = true;
}

void ME_ST_TC_018(int mode) {

    switch (mode)
    {
    case 1:
        double worldX;
        double worldY;
        double worldZ;
        openglEngine::openGLCoordinatesEngine::screen2World(700, 600, 1, worldX, worldY, worldZ);
        cout << "x:" << worldX << endl << "y:" << worldY << endl << "z:" << worldZ << endl;
        break;
    case 2:
        double screenX;
        double screenY;
        double screenZ;
        openglEngine::openGLCoordinatesEngine::world2Screen(screenX, screenY, screenZ, 2900000, 140000, 3100000);
        cout << "x:" << screenX << endl << "y:" << screenY << endl;
        break;
    default:
        break;
    }
}
void ME_ST_TC_019() {
    calGreatCircleDis(115, 28, 116, 29);
}

void ME_ST_TC_020() {
    calSec(115, 28, 116, 29, 10);
}

//void ME_ST_TC_Query() {
//    CPolygon* polygon = queryLayer->isinPolygon(Vec3(115.89, 28.67, 0));
//    typename map<string, string>::iterator it;
//    for (it = polygon->properties.begin();it!= polygon->properties.end();it++)
//    {
//        cout << it->first << ":" << it->second << endl;
//    }
//}
//
//
void functionTest() {
//    ME_ST_TC_Query();
    ME_ST_TC_014();
}

void romaCreate()
{
static int isStart=0;
    if(isStart==0)
    {
#ifdef WIN32

#else
    	Test task4;
    	ACoreOs_Task_Param taskCreateParam;
        taskCreateParam.affinity = ACOREOS_TASK_NO_AFFINITY;
    	taskCreateParam.attribute_set =  ACOREOS_PREEMPT | ACOREOS_TIMESLICE;
    	taskCreateParam.domain = (void *)ACOREOS_KERNEL_ID;
    	taskCreateParam.initial_priority = 12;
    	taskCreateParam.stack_size = 4096;
    	task4.taskCreate((ACoreOs_name)"romaFucn", &taskCreateParam);
    	task4.taskStart((ACoreOs_task_entry)romaFucn, 0,(ACoreOs_name)"romaFucn");
#endif
        isStart=1;
    }
return;
}

void Initial(void)
{
	//functionTest();                       //测试函数
    /*pyan 0625 注释，没用到，似乎drawEarth才用到*/
    int interval = 4;
    int size = 256 / interval + 2;
    textures = new float[size * size * 2];
    int index_t = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            textures[index_t++] = 1.0 * (j) / (size - 1);
            textures[index_t++] = 1.0 * (i) / (size - 1);
        }
    }

    pts = new float[(49 * 49) * 3];               int pt_i = 0;
    index_temp = new int[(49 * 49 - 49 * 2 + 1) * 6];    int index_i = 0;
    texcoords = new float[(49 * 49) * 2];         int texture_i = 0;

    for (int i = 0; i <= 48; i++) {
        for (int j = 0; j <= 48; j++) {
            double lon = -M_PI + i * M_PI / 24;
            double lat = -M_PI / 2 + j * M_PI / 48;
            double x, y, z;
            CGeoUtil::lonLatHeight2XYZ(lon, lat, 0, x, y, z);

            pts[pt_i++] = x;
            pts[pt_i++] = y;
            pts[pt_i++] = z;
            
            texcoords[texture_i++] = 1 - i / 48.0;
            texcoords[texture_i++] = 1 - j / 48.0;

            if (i > 0 && j > 0) {
                index_temp[index_i++] = i * 49 + j;
                index_temp[index_i++] = i * 49 + j - 1;
                index_temp[index_i++] = (i - 1) * 49 + j;

                index_temp[index_i++] = i * 49 + j - 1;
                index_temp[index_i++] = (i - 1) * 49 + j - 1;
                index_temp[index_i++] = (i - 1) * 49 + j;
            }
        }
    }

    /*  initialize the fontRnder */
    //_render->initialize("./../data/fonts/");
    #ifdef WIN32
        _render->initialize("D:/pyan/map_wd_20221219/data/fonts/");
        oglfCreateFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", "./../data/fonts/SIMHEI.TTF", 14, &hzFont[0]);
        oglfCreateFont("D:/pyan/map_wd_20221219/data/fonts/青鸟华光简大黑.TTF", "青鸟华光简大黑", 14, &hzFont[1]);
    #else//目前只适用于tm3
        _render->initialize("D:\\fonts/");
        oglfCreateFont("D:\\fonts/SIMHEI.TTF","SIMHEI", 14, &hzFont[0]);
        oglfCreateFont("D:\\fonts/青鸟华光简大黑.TTF", "青鸟华光简大黑", 14, &hzFont[1]);
    #endif
        _render->getFontManager()->getFontIndex("SIMHEI", 14);
        _render->getFontManager()->getFontIndex("青鸟华光简大黑", 14);
    int fontSize = 18;

//    /*  load font file */
    _render->loadFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", 0, 18, 0, 0, 1);
    _render->loadFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", 0, 18, 1, 0, 1);
    ///*  render text to the screen texture */
    //_render->render(L"滚滚长江东逝水，浪花淘尽英雄。", Vec2i(0, 0), fontSize, "SIMHEI", Color(255, 0, 0, 255));
    //render->render(L"是非成败转头空。", Vec2i(0, fontSize), fontSize, "SIMHEI", Color(255, 0, 0, 255));
    //render->render(L"123abc青山依旧在，几度夕阳红。", Vec2i(0, fontSize * 2), fontSize, "SIMHEI", Color(255, 0, 0, 255));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);           //设置窗口背景色为白色

    //glMatrixMode(GL_PROJECTION);                    //指定设定投影参数
    //glLoadIdentity();
    //
    ////MyPerspective(60, 4.0 / 3, 6, 20);
    ////gluPerspective(60, 4.0 / 3, 0, 20);
    ////glFrustum(0, 0, 0, 0, 0, 20);
    ////glOrtho(-4.0 / 3, 4.0 / 3, -1, 1, 0, 20);

    //int zoom = _scheduler->zoom();
    //double height = _scheduler->eye()[2];
    //glFrustum(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3), 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3), -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3),
    //    CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3), height, height + 10 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
    //
    //
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    //Vec3d eye = _scheduler->eyeXYZ();
    ////gluLookAt(0, 2 * height, 0, 0, 0, 0, 0, 0, 1);
    //gluLookAt(eye[0], eye[1], eye[2], 0, 0, 0, 0, 0, 1);
    

    //gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0);
    //gluLookAt(0, 0, 1.3 * CGeoUtil::WGS_84_RADIUS_EQUATOR, 0, 0, 0, 0, 1, 0);
   
    //gluLookAt(eye[0], -eye[2], eye[1], 0, 0, 0, 0, 1, 0);

    //glMatrixMode(GL_PROJECTION);        //指定设定投影参数
    //glLoadIdentity();
    //gluOrtho2D(-CGeoUtil::PI * CGeoUtil::Web_Mecator_R * 4 / 3.0, CGeoUtil::PI * CGeoUtil::Web_Mecator_R * 4 / 3.0,
    //    -CGeoUtil::PI * CGeoUtil::Web_Mecator_R, CGeoUtil::PI * CGeoUtil::Web_Mecator_R);            //设置投影参数
    //gluOrtho2D(-180, 180, -90, 90);
	#ifdef WIN32
    	pthread_mutex_init(&g_mutex, NULL);
		int ret = pthread_create(&bufferTid[0], NULL, getBuffer, NULL);
        int ret1 = pthread_create(&bufferTid[1], NULL, romaFucn, NULL);
        
    	typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALFARPROC)(int);
    	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
    	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
    	//wglSwapIntervalEXT(1);//打开垂直分布，限制帧率
    	wglSwapIntervalEXT(0);//关闭垂直分布，充分发挥显卡的渲染能力
	#else
    	Test task3;
    	ACoreOs_Task_Param taskCreateParam;

        taskCreateParam.affinity = ACOREOS_TASK_NO_AFFINITY;
		taskCreateParam.attribute_set =  ACOREOS_PREEMPT | ACOREOS_TIMESLICE;
		taskCreateParam.domain = (void *)ACOREOS_KERNEL_ID;
		taskCreateParam.initial_priority = 12;
		taskCreateParam.stack_size = 4096;
		task3.taskCreate((ACoreOs_name)"getBuffer", &taskCreateParam);
		task3.taskStart((ACoreOs_task_entry)getBuffer, 0,(ACoreOs_name)"getBuffer");

	#endif
    
}

/**
 * @brief 绘制抬头显示要素
 */
void drawText(void) {
	glEnable(GL_BLEND); //开混合模式贴图
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDrawPixels(1024, 768, GL_RGBA, GL_UNSIGNED_BYTE, _render->getData());

	GLuint texture;
	glEnable(GL_TEXTURE_2D);//启用纹理贴图
	glDepthMask(GL_FALSE);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glHint(GL_FOG_HINT, GL_NICEST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/******************采用独立坐标系统**************************/
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	glLoadIdentity();
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	//glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, _render->getData());


	glBegin(GL_POLYGON);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1, 1, 0);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1, 1, 0);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1, -1, 0);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1, -1, 0);
	glEnd();

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);
}

//void pictureShowTest(){
//	int interval = 4;
//	int size = 256 / interval + 2;
//	int* index = new int[(size * size - 2 * size + 1) * 6];
//	int index_t = 0; int index_v = 0; int index_v1 = 0;  int index_i = 0;
//	for (int i = 0; i < size; i++) {
//		for (int j = 0; j < size; j++) {
//			if (i > 0 && j > 0) {
//				index[index_i++] = (i)*size + j;
//				index[index_i++] = (i)*size + j - 1;
//				index[index_i++] = (i - 1) * size + j;
//				index[index_i++] = i * size + j - 1;
//				index[index_i++] = (i - 1) * size + j - 1;
//				index[index_i++] = (i - 1) * size + j;
//			}
//		}
//	}
//
//	int width, height, nrChannels;
//	unsigned char* data = stbi_load("D:\\raster/satellite/11/1685/844.jpg", &width, &height, &nrChannels, 0);
//	/*for (int j = 0; j < 10; j++) {
//		for (int k = 0; k < 3; k++)
//			cout << (int)data[3 * (j) + k] << "#";
//		cout << endl;
//	}*/
//	unsigned char* image = static_cast<unsigned char*>(data);
//
//	GLuint texture;
//	float* texture_coor = new float[size * size * 2];
//	glEnable(GL_TEXTURE_2D);//启用纹理贴图
//	glDepthMask(GL_FALSE);//关掉深度测试
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glEnable(GL_BLEND); //开混合模式贴图
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 指定混合模式算法
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//
//	glEnable(GL_TEXTURE_2D);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glTexCoordPointer(2, GL_FLOAT, 0, texture_coor);
//
//	if (nrChannels == 3)
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	else
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//		//glColor3f(48 / 255.0, 168 / 255.0, 224 / 255.0);
//	glEnableClientState(GL_VERTEX_ARRAY); //启用顶点数组
//	glVertexPointer(3, GL_FLOAT, 0, data11111); //设置顶点数组属性
//	glDrawElements(GL_TRIANGLES, pt_size, GL_UNSIGNED_INT, index);
//
//	glDeleteTextures(1, &texture);
//	glDisableClientState(GL_VERTEX_ARRAY);
//
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_CULL_FACE);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	glDisable(GL_CULL_FACE);
//	//glDisable(GL_BLEND);
//	glDepthMask(GL_TRUE);
//}

// 正射视角
void myDisplay(void) {
    // 清除上一帧的HUD屏幕数组
    _render->clearScreen();

    
    glClearColor(48 / 255.0, 168 / 255.0, 224 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);        //用当前背景色填充窗口
    
    glEnable(GL_BLEND); //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);                    //指定设定投影参数
    glLoadIdentity();

    // 获取实时视点参数
    _scheduler->compute();
    int zoom = _scheduler->zoom();
    //cout<<zoom<<endl;
    double height = _scheduler->eye()[2];
    Vec2d offset = _scheduler->offset();

    int scale = _scheduler->scale ;
    //cout << height << " " << CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, scale) << endl;

    // 根据调度计算的信息实时更新视景体

    if(_scheduler->_isFirstPerson){
        double fovy=100.0,  aspect=4.0 / 3,  zNear= 0,  zFar= height * 4;
        gluPerspective(fovy, aspect, zNear, zFar);
    #if 0
        _scheduler->updateFrustum(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2,zoom - scale) + offset[0], 4.0 / 3 * _ortho_base + offset[0], -_ortho_base + offset[1],
        CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2,zoom - scale) + offset[1],0, height * 1 / 3);
    #else
        _scheduler->updateFrustum(fovy, aspect, zNear, zFar);
    #endif
    }
    else{
        //CGeoUtil::lonLat2WebMercator(_scheduler->eye()[1], _scheduler->eye()[0], offset[0], offset[1]);
        // 优化后：提取公共表达式，减少重复计算
        double ortho_base = _scheduler->orthoBase();
        double left   = -1920.0/1080.0 * 1.5 * ortho_base + offset[0];
        double right  = 1920.0 / 1080.0 * 1.5 * ortho_base + offset[0];
        double bottom = -1.5 * ortho_base + offset[1];
        double top    = 1.5 * ortho_base + offset[1];
        double zNear  = -height * 1.5;
        double zFar   = height * 1 + CGeoUtil::WGS_84_RADIUS_EQUATOR;
        
        glOrtho(left, right, bottom, top, zNear, zFar);
        _scheduler->updateFrustum(left, right, bottom, top, zNear, zFar);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Vec3d eye = _scheduler->eyeXYZ();
    Vec3d center = _scheduler->center();
    Vec3d up = _scheduler->up();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    //functionTest();
    /***************开始绘制*****************/
    //pictureShowTest();

    //if (earth)
        //drawEarth1();
    //drawRasters(9, 200, 205, 395, 400);
    //getBuffer(NULL);
    //_map->getBuffer();
    /*getBuffer(NULL);*/
    if (_map->isDoubleBufferLoaded)
    {
        _map->swapBuffer();
        _map->isDoubleBufferLoaded = false;
    }
    _map->draw();    
    
    //getBuffer(NULL);
    //drawText();
    //glutSwapBuffers();
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 0, 768, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
    //renderArray();
}

void Display2d(){
    //GLint textureNum;
	//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureNum); //获取当前视口尺寸
	//cout << "当前视口支持的纹理单元数量：" << textureNum << endl;

    glClearColor(48 / 255.0, 168 / 255.0, 224 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);        //用当前背景色填充窗口
    
    glEnable(GL_BLEND); //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);                    //指定设定投影参数
    glLoadIdentity();

    // 获取实时视点参数
    _scheduler2d->compute();
    int zoom = _scheduler2d->zoom();
    Vec2d center = _scheduler2d->center(); //地图中心点，即视点
    // 获取旋转角度（以弧度为单位）
    double rotationRad = _scheduler2d->getRotation() * M_PI / 180.0;
    
    // 获取当前视口尺寸
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int pixelWidth = viewport[2];
    int pixelHeight = viewport[3];
    

    // 计算墨卡托单位的视口尺寸
    double mercatorPerPixel = 2 * 20037508.34 / pow(2, zoom) / 256;
    double viewportWidth = pixelWidth * mercatorPerPixel;
    double viewportHeight = pixelHeight * mercatorPerPixel;

    // 优化后：提取公共表达式，减少重复计算
    double left = center[0] - viewportWidth / 2;
    double right = center[0] + viewportWidth  / 2;
    double bottom = center[1] - viewportHeight / 2;
    double top = center[1] + viewportHeight / 2;
    glOrtho(left, right, bottom, top, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Vec3d eye = _scheduler->eyeXYZ();
    // Vec3d center = _scheduler->center();
    // Vec3d up = _scheduler->up();
    //gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    glTranslated(center[0], center[1], 0.0);
    glRotated(_scheduler2d->getRotation(), 0.0, 0.0, 1.0);
    glTranslated(-center[0], -center[1], 0.0);

     //drawRasters(4, 6, 7, 10, 11);
    if (_map->isDoubleBufferLoaded)
    {
        _map->swapBuffer();
        _map->isDoubleBufferLoaded = false;
    }
    _map->draw(); 
    // 绘制飞机图标
    drawAirplaneIcon();

}

// 新增函数：绘制飞机图标
void drawAirplaneIcon() {
    // 保存模型视图矩阵
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, 1.0, -1.0, 0.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);


    // 保存当前所有相关状态
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

    // 设置固定的飞机颜色（例如白色）
    glColor3f(1.0f, 1.0f, 1.0f); // 白色

    // 禁用可能影响颜色的状态
    glDisable(GL_LIGHTING);      // 禁用光照
    glDisable(GL_TEXTURE_2D);    // 禁用纹理
    glDisable(GL_BLEND);         // 禁用混合

    // 设置飞机渲染状态
    glDisable(GL_DEPTH_TEST);    // 确保飞机在最前
    glDisable(GL_CULL_FACE);     // 禁用面剔除

    // 设置飞机渲染状态
    glDisable(GL_DEPTH_TEST);     // 禁用深度测试（确保在最前面）
    glDisable(GL_BLEND);          // 禁用混合（使用不透明色）
    glDisable(GL_CULL_FACE);      // 禁用面剔除

    // 计算动态缩放比例
    float zoomFactor = -pow(0.9f, _scheduler->zoom() - 13); // 当1.1f时，飞机是逐渐变大，0.9f时飞机图标逐渐变小
    float dynamicScale = 0.4 * zoomFactor;

    // 应用缩放
    glScalef(dynamicScale, dynamicScale, 1.0f);

    // 保存当前矩阵状态
    glScalef(0.0005f, 0.0005f, 1.0f);  // 缩小坐标值
    // 替换顶点数据部分为新的飞机数据
    std::vector<GLfloat> vertices = {
        // First set of vertices
           -229.8762207f, 175.4812012f,
           -237.5363770f, 160.5925293f,
           -83.8049316f, 2.2529297f,
           -176.4226074f, 165.3974609f,
           -179.6904297f, 179.2180176f,
           -223.1274414f, 395.2353516f,
           -233.5747070f, 331.5615234f,
           -227.2873535f, -210.7246094f,
           -225.1835938f, -223.1545410f,
           -223.6892090f, -99.1652832f,
           -221.4370117f, -247.0869141f,
           -220.3132324f, -88.3190918f,
           -216.1796875f, -256.2727051f,
           -206.1604004f, -79.2456055f,
           -194.8493652f, -257.4633789f,
           -186.4333496f, -90.3864746f,
           -186.3291016f, -241.3601074f,
           -180.3696289f, -105.4057617f,
           -184.5163574f, -232.6984863f,
           -176.3588867f, -226.6264648f,
           -232.8527832f, -156.7431641f,
           -232.6623535f, -159.5493164f,
           -232.0612793f, -133.5529785f,
           -229.9907227f, -190.0007324f,
           -228.2204590f, -104.9777832f,
           -227.2873535f, -210.7246094f,
           -223.6892090f, -99.1652832f,
           -78.6818848f, 183.4211426f,
           -78.0712891f, 167.1835938f,
           -78.0454102f, 218.9160156f,
           -77.3735352f, 158.3347168f,
           -75.5317383f, 247.7473145f,
           -76.6757813f, 149.4855957f,
           -66.6604004f, -4.0895996f,
           -83.8049316f, 2.2529297f,

           // Second set of vertices
           178.1301270f, 324.0710449f,
           180.5429688f, 311.7988281f,
           180.7573242f, 386.5456543f,
           253.8239746f, 25.7963867f,
           188.1604004f, 397.3041992f,
           230.5800781f, 172.2180176f,
           228.7722168f, 185.0732422f,
           174.5097656f, 367.5109863f,
           173.5646973f, 346.8974609f,
           178.1301270f, 324.0710449f,
           180.7573242f, 386.5456543f,
           233.1030273f, 373.5107422f,
           225.9626465f, 391.0974121f,
           229.8818359f, 321.5881348f,
           232.7788086f, 329.5856934f,
           235.2231445f, 341.0949707f,
           235.5693359f, 356.6062012f,
           228.7722168f, 185.0732422f,
           229.0578613f, 251.1054688f,
           225.9626465f, 391.0974121f,
           212.9418945f, 401.9597168f,
           198.7658691f, 402.2700195f,
           188.1604004f, 397.3041992f,

           // Third set of vertices
           76.6757813f, 149.4855957f,
           78.7253418f, 189.0053711f,
           78.0454102f, 218.9160156f,
           75.0959473f, 251.8095703f,
           71.6879883f, -1.5952148f,
           83.8049316f, 2.2529297f,
           113.7377930f, 151.4187012f,
           596.6018066f, 77.8117676f,
           588.6330566f, 92.3918457f,
           598.2089844f, 72.8640137f,
           578.9230957f, 110.2705078f,
           574.0075684f, 69.5166016f,
           567.9409180f, 125.7082520f,
           556.5568848f, 134.8125000f,
           556.5568848f, 134.8125000f,
           545.3068848f, 139.5625000f,
           574.0075684f, 69.5166016f,
           533.1191406f, 141.8056641f,
           524.7475586f, 63.1584473f,
           517.5834961f, 143.2570801f,
           493.0288086f, 144.6542969f,
           479.7683105f, 57.1484375f,
           524.7475586f, 63.1584473f,
           493.0288086f, 144.6542969f,

           // Fourth set of vertices
           406.7810059f, 148.7500000f,
           335.9465332f, 152.4685059f,
           261.1369629f, 156.3911133f,
           253.8239746f, 25.7963867f,
           249.5437012f, 157.0437012f,
           230.5800781f, 172.2180176f,
           253.8239746f, 25.7963867f,
           261.1369629f, 156.3911133f,
           253.8239746f, 25.7963867f,
           180.5429688f, 311.7988281f,
           180.2363281f, 248.6103516f,
           179.9296875f, 182.6733398f,
           178.2558594f, 169.7165527f,
           164.4267578f, 156.0529785f,
           159.0463867f, 155.0322266f,
           138.2670898f, 153.4187012f,
           113.7377930f, 151.4187012f,
           83.8049316f, 2.2529297f,
           71.6879883f, -1.5952148f,
           75.0959473f, 251.8095703f,
           71.4423828f, 278.0649414f,
           67.1518555f, 299.3334961f,
           62.4082031f, 317.1357422f,
           62.1030273f, -6.7976074f,

           // Fifth set of vertices
           50.2863770f, -19.6965332f,
           52.1308594f, -15.6340332f,
           54.1374512f, 341.8334961f,
           54.5180664f, -12.8903809f,
           62.4082031f, 317.1357422f,
           62.1030273f, -6.7976074f,
           27.5759277f, -153.8125000f,
           27.7233887f, -147.2773438f,
           34.6081543f, 384.8391113f,
           38.7663574f, -84.6137695f,
           44.8830566f, 366.3874512f,
           50.2863770f, -19.6965332f,
           54.1374512f, 341.8334961f,
           20.3447266f, 396.4943848f,
           6.1364746f, 400.3071289f,
           7.7065430f, -336.7617188f,
           9.5646973f, -300.0571289f,
           12.7482910f, -289.8186035f,
           19.6801758f, -280.6442871f,
           26.2619629f, -275.0571289f,
           27.5759277f, -153.8125000f,
           34.6081543f, 384.8391113f,
           6.1364746f, 400.3071289f,
           0.0000000f, 400.7426758f,

           // Sixth set of vertices
           1.8679199f, -412.6647949f,
           5.5783691f, -378.5214844f,
           6.1213379f, -371.4787598f,
           7.7065430f, -336.7617188f,
           0.0000000f, 400.7426758f,
           -34.6081543f, 384.8391113f,
           -27.7233887f, -147.2773438f,
           -22.7609863f, -277.8161621f,
           -10.9279785f, -294.5783691f,
           -7.7065430f, -336.7617188f,
           -6.1213379f, -371.4787598f,
           -2.9990234f, -402.9924316f,
           -1.8679199f, -412.6647949f,
           -0.2868652f, -417.4487305f,
           1.8679199f, -412.6647949f,
           -27.7233887f, -147.2773438f,
           -34.6081543f, 384.8391113f,
           -38.7663574f, -84.6137695f,
           -49.6923828f, 354.5412598f,
           -53.0351563f, -14.3513184f,
           -63.4648438f, 313.4023438f,
           -66.6604004f, -4.0895996f,
           -71.4423828f, 278.0649414f,
           -75.5317383f, 247.7473145f,

           // Seventh set of vertices
           -83.8049316f, 2.2529297f,
           -76.6757813f, 149.4855957f,
           -159.0463867f, 155.0322266f,
           -166.0607910f, 156.5241699f,
           -176.4226074f, 165.3974609f,
           -198.7658691f, 402.2700195f,
           -223.1274414f, 395.2353516f,
           -179.6904297f, 179.2180176f,
           -180.2363281f, 248.6103516f,
           -180.5349121f, 313.3811035f,
           -180.7573242f, 386.5456543f,
           -253.8239746f, 25.7963867f,
           -83.8049316f, 2.2529297f,
           -237.5363770f, 160.5925293f,
           -254.4294434f, 156.7580566f,
           -406.7810059f, 148.7500000f,
           -506.6318359f, 143.8457031f,
           -524.7475586f, 63.1584473f,
           -524.7475586f, 63.1584473f,
           -506.6318359f, 143.8457031f,
           -549.1240234f, 138.2431641f,
           -570.3518066f, 122.8713379f,
           -588.6330566f, 92.3918457f,
           -596.6018066f, 77.8117676f,

           // Eighth set of vertices
           -597.1374512f, 76.7563477f,
           223.7194824f, -98.0266113f,
           223.1416016f, -91.6447754f,
           223.6586914f, -100.3039551f,
           225.1835938f, -223.1545410f,
           231.3923340f, -110.1748047f,
           229.8071289f, -196.6728516f,
           232.5280762f, -155.5031738f,
           206.8740234f, -258.7729492f,
           218.8083496f, -254.3974609f,
           220.3132324f, -88.3190918f,
           221.4370117f, -247.0869141f,
           223.1416016f, -91.6447754f,
           225.1835938f, -223.1545410f,
           202.3745117f, -77.6608887f,
           185.8249512f, -94.4611816f,
           186.3291016f, -241.3601074f,
           192.5529785f, -256.5703125f,
           197.8103027f, -258.3562012f,
           206.8740234f, -258.7729492f,
           220.3132324f, -88.3190918f,
           179.8159180f, -105.6787109f,
           177.2319336f, -112.1340332f,
           179.3801270f, -227.1621094f,

           // Ninth set of vertices
           186.3291016f, -241.3601074f,
           185.8249512f, -94.4611816f,
           177.2319336f, -112.1340332f,
           176.7705078f, -119.4985352f,
           179.3801270f, -227.1621094f,
           175.7092285f, -130.4545898f,
           175.2934570f, -226.5974121f,
           168.8337402f, -140.4560547f,
           156.9604492f, -145.9084473f,
           104.1315918f, -248.2141113f,
           175.2934570f, -226.5974121f,
           156.9604492f, -145.9084473f,
           103.9133301f, -153.2731934f,
           50.8662109f, -160.6379395f,
           40.1054688f, -161.9448242f,
           36.0583496f, -269.6281738f,
           36.0583496f, -269.6281738f,
           40.1054688f, -161.9448242f,
           28.3129883f, -157.6606445f,
           27.5759277f, -153.8125000f,
           26.2619629f, -275.0571289f,
           -28.7553711f, -158.8950195f,
           -44.6752930f, -161.5092773f,
           -36.0583496f, -269.6281738f,

           // Tenth set of vertices
           -22.7609863f, -277.8161621f,
           -27.7233887f, -147.2773438f,
           -103.9133301f, -153.2731934f,
           -156.9604492f, -145.9084473f,
           -104.1315918f, -248.2141113f,
           -36.0583496f, -269.6281738f,
           -44.6752930f, -161.5092773f,
           -170.3103027f, -139.2285156f,
           -176.7705078f, -119.4985352f,
           -176.3588867f, -226.6264648f,
           -104.1315918f, -248.2141113f,
           -156.9604492f, -145.9084473f,
           -177.2319336f, -112.1340332f,
           -180.3696289f, -105.4057617f,
           -176.3588867f, -226.6264648f,
           -176.7705078f, -119.4985352f,
           -173.5646973f, 346.8974609f,
           -180.7573242f, 386.5456543f,
           -180.5349121f, 313.3811035f,
           229.8818359f, 321.5881348f,
           225.9626465f, 391.0974121f,
           229.0578613f, 251.1054688f,
           -186.4333496f, -90.3864746f,
           -180.3696289f, -105.4057617f,

           // Final set of vertices
           -179.8159180f, -105.6787109f,
           -233.5747070f, 331.5615234f,
           -223.1274414f, 395.2353516f,
           -234.5314941f, 368.5380859f
    };
    // 7. 绘制飞机
     // 启用顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());

    // 按照原始分组绘制所有部分
    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 7, 13);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 27, 8);
    glDrawArrays(GL_TRIANGLE_STRIP, 35, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 42, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 46, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 52, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 58, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 65, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 72, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 79, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 86, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 90, 10);
    glDrawArrays(GL_TRIANGLE_FAN, 100, 6);
    glDrawArrays(GL_TRIANGLE_STRIP, 106, 6);
    glDrawArrays(GL_TRIANGLE_STRIP, 112, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 119, 9);
    glDrawArrays(GL_TRIANGLE_FAN, 128, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 134, 11);
    glDrawArrays(GL_TRIANGLE_STRIP, 145, 9);
    glDrawArrays(GL_TRIANGLE_FAN, 154, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 159, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 165, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 172, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 179, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 186, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 192, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 199, 5);
    glDrawArrays(GL_TRIANGLE_STRIP, 204, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 211, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 218, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 223, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 228, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 233, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 238, 4);
    glDrawArrays(GL_TRIANGLES, 242, 12);

    glDisableClientState(GL_VERTEX_ARRAY);

    // 恢复矩阵
    glPopMatrix();
    // 恢复投影矩阵
    glMatrixMode(GL_PROJECTION);  // 切回投影矩阵
    glPopMatrix();                // 恢复投影矩阵

    // 恢复所有属性
    glPopAttrib();
}

int isRotate = 0,rotateconut=0;
void rotate90() {    
    rotateconut += 1;
    //rotateAngle %= 360;
    cout<<"rotate90"<<endl;
    if(!_scheduler->rotate(-0.5)){        
        _map->isViewChanged = true;
    }
        

#ifdef WIN32
    Sleep(100);
#else
    taskDelay(200);
#endif

    
    if (rotateconut == (180)){
        isRotate = 0;
        rotateconut = 0;
    }
}

int isAdd = 1, isAddy = 1;
Vec3d eye = _scheduler->eye();
#define BU 0.001//0.0001
void* romaFucn(void* args)
{   
    while(1){    
        if(isRoma){
            static int start = 1;
            if(start){
                eye = _scheduler->eye();
                eye[0] = 116.01;
                eye[1] = 28.51;//29.56
                start=0;
            }
            if (isRotate) {
                rotate90();
            }
            else {
                if (isAdd && isAddy) {
                    eye[1] += BU;
                    if (eye[1] > 29.6) {
                        isAddy = 0;
                        isRotate = 1;
                    }
                }
                else if ((isAddy == 0) && isAdd) {
                    eye[0] += BU;
                    if (eye[0] > 118.1) {
                        isAdd = 0;
                        isRotate = 1;
                    }
                }
                else if ((isAdd == 0) && (isAddy == 0)) {
                    eye[1] -= BU;
                    if (eye[1] < 28.4) {
                        isAddy = 1;
                        isRotate = 1;
                    }
                }
                else {
                    eye[0] -= BU;
                    if (eye[0] < 115.9) {
                        isAdd = 1;
                        isRotate = 1;
                    }
                }
                cout<<"romaFucn"<<endl;
                if(!_scheduler->eyeSet(eye)){                    
                    _map->isViewChanged = true;
                }
                    
            }
            
            #ifdef WIN32
                Sleep(100);
            #else
                taskDelay(200);
            #endif   
        }
        else{
            #ifdef WIN32
                Sleep(100);
            #else
                taskDelay(2000);
            #endif   
        }
        
     
    }
    return NULL;
}

void myDisplay1(void) {
    // 清除上一帧的HUD屏幕数组
    _render->clearScreen();

    glClear(GL_COLOR_BUFFER_BIT);        //用当前背景色填充窗口
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND); //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);                    //指定设定投影参数
    glLoadIdentity();

    // 获取实时视点参数
    //_nScheduler->compute();
    int zoom = _nScheduler->getOrCreateCamera()->zoom();
    double height = _nScheduler->getOrCreateCamera()->eye()[2];
    //Vec2d offset = _nScheduler->offset();

    gluPerspective(90, 4.0 / 3, height * 0.5, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
    //gluPerspective(120, 4.0 / 3, height * 0.95, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
    _nScheduler->getOrCreatePerspective()->set(height * 0.95, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR,
        90, 4.0 / 3);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vec3d eye = _nScheduler->getOrCreateCamera()->eye();
    Vec3d eyeXYZ;
    OMGeoUtil::lonLatHeight2XYZ(eye, eyeXYZ);

    Vec3d center = _nScheduler->getOrCreateCamera()->center();
    Vec3d up = _nScheduler->getOrCreateCamera()->up();

    //gluLookAt(eyeXYZ[0], eyeXYZ[1], eyeXYZ[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    gluLookAt(eyeXYZ[0], eyeXYZ[1], eyeXYZ[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    //gluLookAt(eyeXYZ[0], eyeXYZ[1], eyeXYZ[2], eyeXYZ[0] + 1, eyeXYZ[1], eyeXYZ[2], 0, 1, 0);
    //_nScheduler->isTileVisible(7, 64, 64);
    //check(_nScheduler->getOrCreateCamera()->zoom());
    //double screen_x, screen_y, screen_z;
    //openglEngine::openGLCoordinatesEngine::world2Screen(screen_x, screen_y, screen_z, 0, 0, 0);

    if (earth)
        drawEarth1();
    //drawRasters(14, 6824, 6834, 13483, 13493);
    //drawRasters(13, 3412, 3417, 6742, 6747);
    _map->draw1();
    //drawRasters(11, 818, 831, 1639, 1650);
    //drawRasters(12, 1694, 1701, 3363, 3373);
    //drawRasters(13, 3390, 3400, 6731, 6741);
    //drawText();

    //glutSwapBuffers();
}


void drawEarth() {

    GLuint texture;
    //glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);//启用纹理贴图

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthMask(GL_FALSE);//关掉深度测试
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND); //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 指定混合模式算法
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //加载并生成纹理
    int width, height, nrChannels;
    unsigned char* data = stbi_load("H:/trainer_map/data/dataTest/NE1_50M_SR_W1.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        //glColor3f(48 / 255.0, 168 / 255.0, 224 / 255.0);
        glEnableClientState(GL_VERTEX_ARRAY); //启用顶点数组
        glVertexPointer(3, GL_FLOAT, 0, pts); //设置顶点数组属性
        glDrawElements(GL_TRIANGLES, (49 * 49 - 49 * 2 + 1) * 6, GL_UNSIGNED_INT, index_temp);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
   else
    {
        cout << "Failed to load texture" << std::endl;
   }
    stbi_image_free(data);


    //glColor3b(255, 255, 0);


    //glutSolidSphere(CGeoUtil::WGS_84_RADIUS_EQUATOR, 20, 20);
}
void drawEarth1() {
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);//关掉深度测试
    glColor3f(48 / 255.0, 168 / 255.0, 224 / 255.0);
    //glEnableClientState(GL_VERTEX_ARRAY); //启用顶点数组
    //glVertexPointer(3, GL_FLOAT, 0, pts); //设置顶点数组属性
    //glDrawElements(GL_TRIANGLES, (49 * 49 - 49 * 2 + 1) * 6, GL_UNSIGNED_INT, index_temp);
    //glDisableClientState(GL_VERTEX_ARRAY);
}



void drawRaster(int zoom, int row, int col) {
    int interval = 4;
    int size = 256 / interval + 2;

    //cout << zoom << "." << row << "." << col << endl;
    //string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string(col);
    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << (col);
	string tileIndex = ost_temp.str();
    string level2Index = tileIndex + "." + "raster";

    TMBuffer* buffer = manager->getFrom2LevelBuffer(level2Index);

    Vertices* pVertices = 0x00;
    Heights* pHeight = 0x00;
    Index* pIndex = 0x00;
    Image* pImage = 0x00;


        if (buffer) {
            pVertices = buffer->vData();
            pHeight = buffer->hData();
            pIndex = buffer->iData();
            pImage = buffer->imData();
            //cout << "cache successfully" << endl;
        }
        else {
            buffer = new TMBuffer(TerrianBuffer, level2Index);
            string vIndex = level2Index + ".vertice";
            string iIndex = level2Index + ".index";
            string hIndex = tileIndex + ".height";
            string imIndex = tileIndex + ".image";

            //pVertices = static_cast<Vertices*>(manager->getFrom3LevelBuffer(vIndex));
            //pIndex = static_cast<Index*>(manager->getFrom3LevelBuffer(iIndex));
            //pImage = static_cast<Image*>(manager->getFrom3LevelBuffer(imIndex));

            if (!pImage) {
                int width=0, height=0, nrChannels=0;
                //string path = "./../data/raster/satellite/" + to_string(zoom) + "/" + to_string(col) + "/" + to_string(row) + ".jpg";
                ostringstream ost_temp;//ost_temp.str("");
                
                #ifdef WIN32
                    ost_temp << "D://pyan/map_wd_20221219/data/uae/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                #else//目前只适用于tm3
                    ost_temp << "raster/satellite/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                #endif
        	    string path = ost_temp.str();

                unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
                if (data) {
                    pImage = new Image(data, width * height, imIndex);
                    pImage->setInfo(height, width, nrChannels);
                    cout << path << "open success" << endl;
                }

                else {
                    stbi_image_free(data);
                    cout << path << "open failed" << endl;
                    return;
                }
            }

            if (!pVertices || !pIndex) {
                /******************** 计算坐标 *******************/
                float* vertices = new float[size * size * 3];

                int* index = new int[(size * size - 2 * size + 1) * 6];

                Vec2d leftTop, pt;
                CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
                double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256 * interval;
                double x=0.0, y=0.0, z=0.0;

                int index_t = 0; int index_v = 0; int index_i = 0;
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {

                        pt[0] = leftTop[0] + j * span;
                        pt[1] = leftTop[1] - i * span;
                        double lat, lon;
                        CGeoUtil::WebMercator2lonLat(pt[0], pt[1], lat, lon);
                        CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, x, y, z);

                        vertices[index_v++] = pt[0];
                        vertices[index_v++] = pt[1];
                        vertices[index_v++] = 0.1;

                        if (i > 0 && j > 0) {
                            index[index_i++] = (i)*size + j;
                            index[index_i++] = (i)*size + j - 1;
                            index[index_i++] = (i - 1) * size + j;

                            index[index_i++] = i * size + j - 1;
                            index[index_i++] = (i - 1) * size + j - 1;
                            index[index_i++] = (i - 1) * size + j;
                        }
                    }
                }

                pVertices = new Vertices(vertices, size * size, vIndex);
                pIndex = new Index(index, (size * size - 2 * size + 1) * 6, iIndex);
                //pHeight = new Heights(height, size * size, hIndex);

                buffer->setData(pVertices, VERTICE);
                buffer->setData(pIndex, INDEX);
                //buffer->setData(pHeight, HEIGHT);
                buffer->setData(pImage, IMAGE);

                manager->insert(2, buffer, level2Index);

                //manager->insert(3, pVertices, vIndex);
                //manager->insert(3, pIndex, iIndex);
                //manager->insert(3, pHeight, hIndex);
                //manager->insert(3, pImage, imIndex);
            }
        }

        float* _vertices = static_cast<float*>(pVertices->data());
        int* indexes = static_cast<int*>(pIndex->data());
        unsigned char* image = static_cast<unsigned char*>(pImage->data());
        int width, height1, nrChannels;
        pImage->getInfo(width, height1, nrChannels);
        if (_vertices && indexes)
            openglEngine::OpenGLRenderEngine::drawRasters(_vertices, textures, indexes, image, width, height1, nrChannels,
                (size * size - 2 * size + 1) * 6);
    return ;
}

void check(int zoom) {
    int len = pow(2, zoom);
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (_nScheduler->isTileVisible(zoom, i, j)) {
                //tiles.push_back(Vec3i(zoom, i, j));
                cout << zoom << " " << i << " " << j << " is visible" << endl;
            }

        }
    }
}

/*void CalculateFrameRate()
{
    static float framesPerSecond = 0.0f;       // This will store our fps
    static float lastTime = 0.0f;       // This will hold the time from the last frame
    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    if (currentTime - lastTime > 0.5f)
    {
        lastTime = currentTime;
        cout << "帧率：" << framesPerSecond *2 << endl;
        framesPerSecond = 0;
    }
}*/
void cancelThread() {
    //_map->getBuffer();
    //int q = 0;
    //_map->_bufferLock = false;
    //pthread_mutex_unlock(&g_mutex);
    //cout << "解锁" << endl;
}
