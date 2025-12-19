#include <string>
#include <time.h>
#include <sstream>
#include <stdio.h>

#include "gl\gl.h"	
#include "gl\glu.h"

#include "mapDrv.h"
#include "InputCApi.h"
#include "TMap/JSONLayer.h"
#include "OpenGLEngine/openglengine.h"
#include "TMap/OMap.h"
#include "airlineDraw.h"
#include "airplaneDraw.h"

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

#define M_PI 3.141592653

#ifdef WIN32
OMap* _map = new OMap("D:/pyan/map_wd_20221219/data/map_dat.json");
//OMap* _map = new OMap("C://Users/pyan/Documents/pyan/map_wd_20221219/data/map_zd_vector.json");
//OMap* _map = new OMap("D://pyan/DATA/data/map_win_zd.json");

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
//CFontRender* render = new CFontRender(1920, 1080);
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
#define COUNT 5304
using namespace textRender;
float* _Vertices[COUNT];
unsigned char* _Images[COUNT];
int _indexImage = 0;
float* _textures;
int* _indexes;
int index_num;
bool earth = true;

int isRoma=0;

void* romaFucn(void* args);
void drawRaster_single();

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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint    viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glOrtho(0, viewport[2], 0, viewport[3], -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

 //   memset(a, 0, 30*sizeof(char));
	//sprintf(a,"zoom=%d", _scheduler2d->zoom());
	//oglfDrawString(hzFont[1], x, y-20, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);

	//memset(a, 0, 30 * sizeof(char));
	//sprintf(a,"Version: 1.0");
	//oglfDrawString(hzFont[1], x, y-40, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);
}

void drawRasters(int zoom, int beginRow, int endRow, int beginCol, int endCol) {
    for (int i = beginRow; i <= endRow; i++) {
        for (int j = beginCol; j <= endCol; j++) {
            drawRaster(zoom, i, j);
        }
    }
}

void drawRasters2(int zoom, int beginRow, int endRow, int beginCol, int endCol, bool isdraw = 0) {
    for (int i = beginRow; i <= endRow; i++) {
        for (int j = beginCol; j <= endCol; j++) {
            drawRaster_sim(zoom, i, j, isdraw);
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
    // 查询实际支持的纹理单元数量
	GLint maxUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
	cout<<"maxUnits="<<maxUnits<<"\r\n"<<endl;
	//functionTest();                       //测试函数
    /*pyan 0625 注释，没用到，似乎drawEarth才用到*/
    //int interval = 4;
    //int size = 256 / interval + 2;
    int stride = 1;
    int size = 2;
    _textures = new float[size * size * 2];
    _indexes = new int[(size * size - 2 * size + 1) * 6];
    int index_t = 0;
    int index_i = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            _textures[index_t++] = 1.0 * (j) / (size - 1);
            _textures[index_t++] = 1.0 * (i) / (size - 1);
            if (i > 0 && j > 0)
            {
                if (
                    ((i == (size - 1)) && (j % stride == 0)) || \
                    ((j == (size - 1)) && (i % stride == 0)) || \
                    ((i == (size - 1)) && (j == (size - 1))) || \
                    ((i % stride == 0) && (j % stride == 0))
                    ) {
                    _indexes[index_i++] = (i)*size + j;
                    _indexes[index_i++] = (i)*size + j - stride;
                    _indexes[index_i++] = (i - stride) * size + j;
                    _indexes[index_i++] = i * size + j - stride;
                    _indexes[index_i++] = (i - stride) * size + j - stride;
                    _indexes[index_i++] = (i - stride) * size + j;
                }
            }
        }
    }
    /*drawRasters2(12, 1748, 1753, 2675, 2679, 0);*/
    //drawRasters2(12, 1729, 1796, 2629, 2706, 0);
    //读取全部图像后重置索引
    _indexImage = 0;
    index_num = index_i;
#if 0
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
#endif
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
        //_render->getFontManager()->getFontIndex("SIMHEI", 14);
        //_render->getFontManager()->getFontIndex("青鸟华光简大黑", 14);
    int fontSize = 18;

//    /*  load font file */
    //_render->loadFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", 0, 18, 0, 0, 1);
    //_render->loadFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", 0, 18, 1, 0, 1);
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
        //int ret1 = pthread_create(&bufferTid[1], NULL, romaFucn, NULL);
        
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_INT, _render->getData());


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
    //glViewport(0, 0, screenWidth, screenHeight);
    _render->clearScreen();
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
    double left = center[0] - viewportWidth / 2.0;
    double right = center[0] + viewportWidth  / 2.0;
    double bottom = center[1] - viewportHeight / 2.0;
    double top = center[1] + viewportHeight / 2.0;
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
    //飞行计划绘制
    //drawSampleRoute();
    drawAirportIcon(108.94, 34.266, "D:/pyan/map_wd_20221219/data/icons/capital_3.png", center, pixelWidth, pixelHeight, 15, _scheduler2d->getRotation(), zoom); // 西安
    // 绘制飞机图标
    static float air_lng = 55.0f;
    static float air_lat = 25.0f;
    air_lng -= 0.01f;
    air_lat -= 0.01f;
    //Vec2d posAir(55.3, 25.271);
    //Vec2d posMec;
    //OMGeoUtil::WebMecator2Lonlat(center, posAir);
    //drawSamplePoint(116, 28.5);
    //OMGeoUtil::Lonlat2WebMecator(posAir, posMec);
    //drawAirplaneIcon(posMec, center, _scheduler2d->getRotation(), viewportWidth, viewportHeight);
    //drawAirplaneIcon();
    //drawAirportIcon(111.67, 32.38, "D:/pyan/map_wd_20221219/data/milairport_3.png", center, viewportWidth, viewportHeight, 0.05);
    
    //glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //glLoadIdentity();
    //glOrtho(0, screenWidth, 0, screenHeight, -10.0, 10.0);
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glLoadIdentity();

    //

    //// 绘制矩形（使用三角形带）
    ////glBegin(GL_TRIANGLE_STRIP); // 注意：glBegin/glEnd在OpenGL ES 2.0及更高版本中不可用
    ////glVertex3f(screenWidth - 50, 0.0f, 0.0f); // 左下
    ////glVertex3f(screenWidth, 0.0f, 0.0f); // 右下
    ////glVertex3f(screenWidth - 50, 80.0f, 0.0f); // 左上
    ////glVertex3f(screenWidth, 80.0f, 0.0f); // 右上
    ////glEnd();
    //// 设置颜色：蓝色，50%透明度
    //glColor4f(1.0f, 1.0f, 0.0f, 0.7f);
    ///*绘制三角形*/
    //static GLshort vertexArray[] = { screenWidth - 200, 0, 0,
    //                                screenWidth, 0, 0,
    //                                screenWidth - 200, 240, 0,
    //                                screenWidth,  240, 0 };
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_SHORT, 0, vertexArray);

    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //glDisableClientState(GL_VERTEX_ARRAY);
    ////绘制线段
    //glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    ///*static GLshort vertexLine[] = { screenWidth - 200, 0, 0,
    //                                screenWidth - 200, 240, 0,
    //                                screenWidth - 200, 240, 0,
    //                                screenWidth,  240, 0,
    //                                screenWidth,  240, 0,
    //                                screenWidth,  0, 0,
    //                                screenWidth,  0, 0, 
    //                                screenWidth - 200, 0, 0};*/
    //static GLshort vertexLine[] = { screenWidth - 200, 0, 0,
    //                                screenWidth - 200, 240, 0,
    //                                screenWidth, 240, 0,
    //                                screenWidth, 0, 0 };
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_SHORT, 0, vertexLine);

    //glDrawArrays(GL_LINE_LOOP, 0, 4);

    //glDisableClientState(GL_VERTEX_ARRAY);
    ////glViewport(screenWidth - 150, 0, 150, 240);
    ////glOrtho(0, 60, 0, 100, -1, 1);
    //glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    ////oglfSetFontSize(hzFont[1], 24);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 9, (const unsigned char*)"模式", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 8, (const unsigned char*)"地速", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 7, (const unsigned char*)"真航向", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 6, (const unsigned char*)"到点", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 5, (const unsigned char*)"待飞距离", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 4, (const unsigned char*)"应飞航向", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 3, (const unsigned char*)"偏航距", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24 * 2, (const unsigned char*)"标准气压高度", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12 + 24, (const unsigned char*)"待飞时", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //oglfDrawString(hzFont[1], screenWidth - 200 + 5, 12, (const unsigned char*)"时区时间", FONT_JUST_HLEFT, FONT_JUST_VTOP);
    //glPopMatrix();
    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
}

void DisplayTest() {
    glClearColor(48 / 255.0, 168 / 255.0, 224 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);        //用当前背景色填充窗口

    glEnable(GL_BLEND); //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);                    //指定设定投影参数
    glLoadIdentity();
    Vec2d center{6157802.3286899561, 2915182.7160115866 }; //地图中心点，即视点
    int zoom = 12;

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
    double left = center[0] - viewportWidth / 2.0;
    double right = center[0] + viewportWidth / 2.0;
    double bottom = center[1] - viewportHeight / 2.0;
    double top = center[1] + viewportHeight / 2.0;
    glOrtho(left, right, bottom, top, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //drawRasters(12, 1748, 1753, 2675, 2679);
    //drawRasters2(12, 1729, 1796, 2629, 2706, 1);
    //drawRasters(12, 1729, 1796, 2629, 2706);
    drawRaster_single();
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

void drawRaster_single() {
    //	int width, height, nrChannels;
        // 测试512*512瓦片
    string path1 = "D:/pyan/map_wd_20221219/data/uae/raster/12/2676/1751.jpg";
    string path2 = "D:/pyan/map_wd_20221219/data/uae/raster/12/2677/1751.jpg";
    string path3 = "D:/pyan/map_wd_20221219/data/uae/raster/12/2676/1752.jpg";
    string path4 = "D:/pyan/map_wd_20221219/data/uae/raster/12/2677/1752.jpg";

    int w1, h1, c1, w2, h2, c2, w3, h3, c3, w4, h4, c4;
    unsigned char* data1 = stbi_load(path1.c_str(), &w1, &h1, &c1, 3);
    unsigned char* data2 = stbi_load(path2.c_str(), &w2, &h2, &c2, 3);
    unsigned char* data3 = stbi_load(path3.c_str(), &w3, &h3, &c3, 3);
    unsigned char* data4 = stbi_load(path4.c_str(), &w4, &h4, &c4, 3);
    if (!data1 || !data2 || !data3 || !data4) {
        if (data1) stbi_image_free(data1);
        if (data2) stbi_image_free(data2);
        if (data3) stbi_image_free(data3);
        if (data4) stbi_image_free(data4);
        // 图片加载失败，提前返回或记录日志
        cout << "read image error" << endl;
        return;
    }

    // 要求四张图尺寸一致
    if (!(w1 == w2 && w1 == w3 && w1 == w4 && h1 == h2 && h1 == h3 && h1 == h4)) {
        stbi_image_free(data1);
        stbi_image_free(data2);
        stbi_image_free(data3);
        stbi_image_free(data4);
        cout << "different image" << endl;
        return;
    }
    const int tileW = w1;
    const int tileH = h1;
    const int channels = 3; // 已强制为3通道
    const int outW = tileW * 2;
    const int outH = tileH * 2;

    const int rowBytesTile = tileW * channels;
    const int rowBytesOut = outW * channels;
    unsigned char* image = new unsigned char[outW * outH * channels];

    // 左上（data1）
    for (int y = 0; y < tileH; ++y) {
        unsigned char* dst = image + y * rowBytesOut;
        unsigned char* src = data1 + y * rowBytesTile;
        memcpy(dst, src, rowBytesTile);
    }

    // 右上（data2）
    for (int y = 0; y < tileH; ++y) {
        unsigned char* dst = image + y * rowBytesOut + tileW * channels;
        unsigned char* src = data2 + y * rowBytesTile;
        memcpy(dst, src, rowBytesTile);
    }

    // 左下（data3）
    for (int y = 0; y < tileH; ++y) {
        unsigned char* dst = image + (y + tileH) * rowBytesOut;
        unsigned char* src = data3 + y * rowBytesTile;
        memcpy(dst, src, rowBytesTile);
    }

    // 右下（data4）
    for (int y = 0; y < tileH; ++y) {
        unsigned char* dst = image + (y + tileH) * rowBytesOut + tileW * channels;
        unsigned char* src = data4 + y * rowBytesTile;
        memcpy(dst, src, rowBytesTile);
    }

    //构造顶点坐标，索引坐标
    int size = 2;
    float* vertices = new float[size * size * 3];
    double leftTop[2], pt[2];
    CGeoUtil::getTileLeftTop(12, 2676, 1749, leftTop[0], leftTop[1]);
    //	    double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256 * interval;
    double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, 12) * 2.0;
    int index_v = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {

            pt[0] = leftTop[0] + j * span;
            pt[1] = leftTop[1] - i * span;

            vertices[index_v++] = pt[0];
            vertices[index_v++] = pt[1];
            vertices[index_v++] = 0.1;
        }
    }
    //openglEngine::OpenGLRenderEngine::drawRasters(vertices, _textures, _indexes, image, 512, 512, 3, index_num);
    
    GLuint tex = 0;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glEnable(GL_BLEND); //开混合模式贴图
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 指定混合模式算法
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glDisable(GL_DEPTH_TEST);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY); //启用顶点数组

    glTexCoordPointer(2, GL_FLOAT, 0, _textures);
    glVertexPointer(3, GL_FLOAT, 0, vertices); //设置顶点数组属性
    //		cout<<"pt_size:"<<pt_size<<endl;
    //		ACoreOs_clock_get_timestamp(&time1);
    glDrawElements(GL_TRIANGLES, index_num, GL_UNSIGNED_INT, _indexes);//GL_TRIANGLES
    //		ACoreOs_clock_get_timestamp(&time2);
    //		cout<<"drawelements time: "<<time2- time1 <<endl;
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    //stbi_image_free(data1);
    //stbi_image_free(data2);
    //stbi_image_free(data3);
    //stbi_image_free(data4);
    stbi_image_free(data1);
    stbi_image_free(data2);
    stbi_image_free(data3);
    stbi_image_free(data4);
    delete[] image;
    delete[] vertices;
    return;
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
                    ost_temp << "D://pyan/map_wd_20221219/data/uae/raster/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                #else//目前只适用于tm3
                    ost_temp << "raster/satellite/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                #endif
        	    string path = ost_temp.str();

                unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
                if (data) {
                    pImage = new Image(data, width * height, imIndex);
                    pImage->setInfo(height, width, nrChannels);
                    cout << path << " open success" << endl;
                }

                else {
                    stbi_image_free(data);
                    cout << path << " open failed" << endl;
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
            openglEngine::OpenGLRenderEngine::drawRasters(_vertices, _textures, indexes, image, width, height1, nrChannels,
                (size * size - 2 * size + 1) * 6);
    return ;
}

void drawRaster_sim(int zoom, int row, int col, bool isdraw) {
    int interval = 4;
    int size = 256 / interval + 2;
    //size = 2;
    ostringstream ost_temp;//ost_temp.str("");
    ost_temp << (zoom) << "." << (row) << "." << (col);
    string tileIndex = ost_temp.str();

    std::ostringstream ost_temp2;
    ost_temp2 << "D:/pyan/map_wd_20221219/data/uae/raster/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
    int width, height, nrChannels;
    string path = ost_temp2.str();
    //unsigned char* image = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    // 数据读取
    if (!isdraw) {
        unsigned char* image = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!image) {
            stbi_image_free(image);
            cout << path << " open failed" << endl;
            return;
        }
        //cout << path << " open success" << endl;
        _Images[_indexImage] = image;
        //构造顶点坐标，索引坐标
        float* vertices = new float[size * size * 3];
        double leftTop[2], pt[2];
        CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
        double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / (size - 1);

        int index_v = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                pt[0] = leftTop[0] + j * span;
                pt[1] = leftTop[1] - i * span;

                vertices[index_v++] = pt[0];
                vertices[index_v++] = pt[1];
                vertices[index_v++] = 0.1;
            }
        }
		_Vertices[_indexImage] = vertices;
        _indexImage++;
        _indexImage %= COUNT;
    }
    else {
        
        openglEngine::OpenGLRenderEngine::drawRasters(_Vertices[_indexImage], _textures, _indexes, _Images[_indexImage], 256, 256, 3,
            index_num, tileIndex);
        _indexImage++;
		_indexImage %= COUNT;
        //stbi_image_free(image);
        //delete[] vertices;
        //delete[] indexes;
        //delete[] image;
    }
    return;
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

// 添加静态变量防止重复调用
static bool isSwitching = false;
static bool switchRequested = false;
static bool toDayModeRequested = false;
static pthread_mutex_t switchMutex = PTHREAD_MUTEX_INITIALIZER;

// 异步切换函数
void* asyncSwitchMapStyle(void* args) {
    pthread_mutex_lock(&switchMutex);

    bool toDayMode = *(bool*)args;

    printf("AsyncSwitchMapStyle: 开始异步切换样式...\n");

    // 1) 保存状态
    Vec2d center =  _scheduler2d->center();
    int displayMode = _map->Dislpay();
    int brightness = _map->getBrightness();
    int zoom = _scheduler2d ? _scheduler2d->zoom() : _map->getOrCreateScheduler()->zoom();

    double angle = 0.0;
    if (_scheduler2d) {
        angle = _scheduler2d->getRotation();
        printf("AsyncSwitchMapStyle: 保存角度 = %f\n", angle);
    }

    printf("AsyncSwitchMapStyle: 保存状态 - displayMode=%d, brightness=%d, zoom=%d\n",
        displayMode, brightness, zoom);

    // 2) 停止后台线程
#ifdef WIN32
    printf("AsyncSwitchMapStyle: 停止后台线程...\n");
    for (int i = 0; i < 10; i++) {
        pthread_cancel(bufferTid[i]);
    }
    Sleep(5); // 最小等待时间
#endif

    // 3) 重建 OMap
    printf("AsyncSwitchMapStyle: 重建地图对象...\n");
    delete _map;
    _map = nullptr;
    const char* cfg = toDayMode ? "E:/2C/data/map.json" : "E:/2C/data/map2.json";
    _map = new OMap(cfg);
    _map->turnOffLayer(6);
    _map->turnOffLayer(2);
    _map->turnOffLayer(7);
    // 4) 重绑全局调度器
    _scheduler2d = _map->getOrCreateScheduler();

    // 5) 恢复状态
    printf("AsyncSwitchMapStyle: 恢复状态...\n");
    _map->setDislpay(displayMode);
    _map->changeBrightness(brightness);
    _scheduler2d->updateMapParameter(center, zoom);
    _scheduler2d->rotate(angle);
    printf("AsyncSwitchMapStyle: 恢复角度 = %f, 验证角度 = %f\n", angle, _scheduler2d->getRotation());
    printf("AsyncSwitchMapStyle: 恢复状态 - displayMode=%d, brightness=%d, zoom=%d\n",
        displayMode,  brightness, zoom);
    printf("AsyncSwitchMapStyle: 验证恢复后 - 实际displayMode=%d\n", _map->Dislpay());

    _map->isViewChanged = true;

    // 6) 重新启动后台线程
#ifdef WIN32
    printf("AsyncSwitchMapStyle: 重新启动后台线程...\n");
    for (int i = 0; i < 10; i++) {
        pthread_create(&bufferTid[i], NULL, getBuffer, NULL);
    }
#endif

    // 7) 最终验证状态
    printf("AsyncSwitchMapStyle: 最终验证 - displayMode=%d, angle=%f\n",
        _map->Dislpay(), _scheduler2d->getRotation());

    printf("AsyncSwitchMapStyle: 异步切换完成\n");
    isSwitching = false;
    switchRequested = false;

    pthread_mutex_unlock(&switchMutex);
    return NULL;
}

void SwitchMapStyle(bool toDayMode)
{
    // 防止重复调用
    if (isSwitching || switchRequested) {
        printf("SwitchMapStyle: 正在切换中，忽略重复调用\n");
        return;
    }

    // 检查地图对象是否有效
    if (!_map) {
        printf("SwitchMapStyle: 地图对象无效，无法切换\n");
        return;
    }

    switchRequested = true;
    toDayModeRequested = toDayMode;

    printf("SwitchMapStyle: 启动异步切换到 %s 模式...\n", toDayMode ? "白天" : "夜间");

    // 启动异步切换线程
#ifdef WIN32
    pthread_t switchThread;
    int result = pthread_create(&switchThread, NULL, asyncSwitchMapStyle, &toDayModeRequested);
    if (result != 0) {
        printf("SwitchMapStyle: 创建切换线程失败，错误码: %d\n", result);
        switchRequested = false;
        return;
    }
    pthread_detach(switchThread); // 分离线程，让它自动清理
    printf("SwitchMapStyle: 异步切换线程已启动\n");
#endif
}