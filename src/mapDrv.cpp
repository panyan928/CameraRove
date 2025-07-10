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
#else//Ŀǰֻ������tm3
#include "gljos.h"
#include "GL/gl.h"
#include "glj.h"
OMap* _map = new OMap("D:\\map_tm.json");
//JSONLayer *queryLayer = new JSONLayer("D:\\jiangxi.geojson");
#endif

JTFONT hzFont[2];
CFontRender* render = new CFontRender(1920, 1080);
OM3DScheduler* _scheduler = _map->getOrCreate3DScheduler();
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
        cout << "��" << i + 1 << "����ľ���Ϊ��" << pts[i][0] << " γ��Ϊ��" << pts[i][1] << " �߳�Ϊ��" << pts[i][2] << endl;
    }
    return pts;
}

double calGreatCircleDis(double startLon, double startLat, double endLon, double endLat) {
    double R = (6378137 + 6356752.31414) / 2;
    double PI = CGeoUtil::PI;
    double dis = R * acos(cos(startLat * PI / 180) * cos(endLat * PI / 180) * cos((startLon - endLon) * PI / 180) + sin(startLat) * sin(endLat)) * PI / 180 / 1000;
    cout << "��γ�ȣ�"<<startLon<<","<<startLat<<"��"<<"��γ�ȣ�"<<endLon<<","<<endLat <<"��"<<endl;
    cout << "��Բ����Ϊ��" << dis << "ǧ��"<<endl;
    cout <<  endl;
    return dis;
}

//�������̣߳����ػ�������
void* getBuffer(void* args) {

    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#ifdef WIN32
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif
    //pthread_cleanup_push(cancelThread,NULL);
    //_map->_bufferLock = true;
    //pthread_mutex_lock(&g_mutex);
    //cout << "����" << endl;
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
         //cout << "��ʼ����" << endl;
    }
    
    //pthread_mutex_unlock(&g_mutex);
    //cout << "����" << endl;
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
	//functionTest();                       //���Ժ���
    /*pyan 0625 ע�ͣ�û�õ����ƺ�drawEarth���õ�*/
    /*int interval = 4;
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
    }*/
    /*  initialize the fontRnder */
    //_render->initialize("./../data/fonts/");
    #ifdef WIN32
        _render->initialize("D:/pyan/map_wd_20221219/data/fonts/");
        oglfCreateFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", "./../data/fonts/SIMHEI.TTF", 14, &hzFont[0]);
        oglfCreateFont("D:/pyan/map_wd_20221219/data/fonts/���񻪹����.TTF", "���񻪹����", 14, &hzFont[1]);
    #else//Ŀǰֻ������tm3
        _render->initialize("D:\\fonts/");
        oglfCreateFont("D:\\fonts/SIMHEI.TTF","SIMHEI", 14, &hzFont[0]);
        oglfCreateFont("D:\\fonts/���񻪹����.TTF", "���񻪹����", 14, &hzFont[1]);
    #endif
        _render->getFontManager()->getFontIndex("SIMHEI", 14);
        _render->getFontManager()->getFontIndex("���񻪹����", 14);
    int fontSize = 18;

//    /*  load font file */
    _render->loadFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", 0, 18, 0, 0, 1);
    _render->loadFont("D:/pyan/map_wd_20221219/data/fonts/SIMHEI.TTF", 0, 18, 1, 0, 1);
    ///*  render text to the screen texture */
    //_render->render(L"������������ˮ���˻��Ծ�Ӣ�ۡ�", Vec2i(0, 0), fontSize, "SIMHEI", Color(255, 0, 0, 255));
    //render->render(L"�Ƿǳɰ�תͷ�ա�", Vec2i(0, fontSize), fontSize, "SIMHEI", Color(255, 0, 0, 255));
    //render->render(L"123abc��ɽ�����ڣ�����Ϧ���졣", Vec2i(0, fontSize * 2), fontSize, "SIMHEI", Color(255, 0, 0, 255));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);           //���ô��ڱ���ɫΪ��ɫ

    //glMatrixMode(GL_PROJECTION);                    //ָ���趨ͶӰ����
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

    //glMatrixMode(GL_PROJECTION);        //ָ���趨ͶӰ����
    //glLoadIdentity();
    //gluOrtho2D(-CGeoUtil::PI * CGeoUtil::Web_Mecator_R * 4 / 3.0, CGeoUtil::PI * CGeoUtil::Web_Mecator_R * 4 / 3.0,
    //    -CGeoUtil::PI * CGeoUtil::Web_Mecator_R, CGeoUtil::PI * CGeoUtil::Web_Mecator_R);            //����ͶӰ����
    //gluOrtho2D(-180, 180, -90, 90);
	#ifdef WIN32
    	pthread_mutex_init(&g_mutex, NULL);
		int ret = pthread_create(&bufferTid[0], NULL, getBuffer, NULL);
        int ret1 = pthread_create(&bufferTid[1], NULL, romaFucn, NULL);
        
    	typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALFARPROC)(int);
    	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
    	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
    	//wglSwapIntervalEXT(1);//�򿪴�ֱ�ֲ�������֡��
    	wglSwapIntervalEXT(0);//�رմ�ֱ�ֲ�����ַ����Կ�����Ⱦ����
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
 * @brief ����̧ͷ��ʾҪ��
 */
void drawText(void) {
	glEnable(GL_BLEND); //�����ģʽ��ͼ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDrawPixels(1024, 768, GL_RGBA, GL_UNSIGNED_BYTE, _render->getData());

	GLuint texture;
	glEnable(GL_TEXTURE_2D);//����������ͼ
	glDepthMask(GL_FALSE);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glHint(GL_FOG_HINT, GL_NICEST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/******************���ö�������ϵͳ**************************/
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
//	glEnable(GL_TEXTURE_2D);//����������ͼ
//	glDepthMask(GL_FALSE);//�ص���Ȳ���
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glEnable(GL_BLEND); //�����ģʽ��ͼ
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// ָ�����ģʽ�㷨
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
//	glEnableClientState(GL_VERTEX_ARRAY); //���ö�������
//	glVertexPointer(3, GL_FLOAT, 0, data11111); //���ö�����������
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

// �����ӽ�
void myDisplay(void) {
    // �����һ֡��HUD��Ļ����
    _render->clearScreen();

    
    glClearColor(48 / 255.0, 168 / 255.0, 224 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);        //�õ�ǰ����ɫ��䴰��
    
    glEnable(GL_BLEND); //�����ģʽ��ͼ
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);                    //ָ���趨ͶӰ����
    glLoadIdentity();

    // ��ȡʵʱ�ӵ����
    _scheduler->compute();
    int zoom = _scheduler->zoom();
    //cout<<zoom<<endl;
    double height = _scheduler->eye()[2];
    Vec2d offset = _scheduler->offset();

    int scale = _scheduler->scale ;
    //cout << height << " " << CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, scale) << endl;

    // ���ݵ��ȼ������Ϣʵʱ�����Ӿ���
#if 1
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
        // �Ż�����ȡ�������ʽ�������ظ�����
        double ortho_base = _scheduler->orthoBase();
        double left   = -1920.0/1080.0 * 2 * ortho_base + offset[0];
        double right  = 1920.0 / 1080.0 * 2 * ortho_base + offset[0];
        double bottom = -2.0 * ortho_base + offset[1];
        double top    =  2.0 * ortho_base + offset[1];
        double zNear  = -height * 1.5;
        double zFar   = height * 1 + CGeoUtil::WGS_84_RADIUS_EQUATOR;

        glOrtho(left, right, bottom, top, zNear, zFar);
        _scheduler->updateFrustum(left, right, bottom, top, zNear, zFar);
    }
    

#else
    
    glOrtho(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[1],
            CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[1], height * 0.95, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
    
    _scheduler->updateFrustum(-4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[0], 4.0 / 3 * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[0], -CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[1],
        CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom - 3) + offset[1], height * 0.95, height * 0.95 + 2.5 * CGeoUtil::WGS_84_RADIUS_EQUATOR);
    
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Vec3d eye = _scheduler->eyeXYZ();
    Vec3d center = _scheduler->center();
    Vec3d up = _scheduler->up();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    //functionTest();
    /***************��ʼ����*****************/
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 0, 768, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //renderArray();
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
    // �����һ֡��HUD��Ļ����
    _render->clearScreen();

    glClear(GL_COLOR_BUFFER_BIT);        //�õ�ǰ����ɫ��䴰��
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND); //�����ģʽ��ͼ
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);                    //ָ���趨ͶӰ����
    glLoadIdentity();

    // ��ȡʵʱ�ӵ����
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
    glEnable(GL_TEXTURE_2D);//����������ͼ

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthMask(GL_FALSE);//�ص���Ȳ���
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND); //�����ģʽ��ͼ
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// ָ�����ģʽ�㷨
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //���ز���������
    int width, height, nrChannels;
    unsigned char* data = stbi_load("H:/trainer_map/data/dataTest/NE1_50M_SR_W1.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        //glColor3f(48 / 255.0, 168 / 255.0, 224 / 255.0);
        glEnableClientState(GL_VERTEX_ARRAY); //���ö�������
        glVertexPointer(3, GL_FLOAT, 0, pts); //���ö�����������
        glDrawElements(GL_TRIANGLES, (49 * 49 - 49 * 2 + 1) * 6, GL_UNSIGNED_INT, index_temp);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
   else
    {
        std::cout << "Failed to load texture" << std::endl;
   }
    stbi_image_free(data);


    //glColor3b(255, 255, 0);


    //glutSolidSphere(CGeoUtil::WGS_84_RADIUS_EQUATOR, 20, 20);
}
void drawEarth1() {
    glCullFace(GL_BACK);
    glDepthMask(GL_FALSE);//�ص���Ȳ���
    glColor3f(48 / 255.0, 168 / 255.0, 224 / 255.0);
    //glEnableClientState(GL_VERTEX_ARRAY); //���ö�������
    //glVertexPointer(3, GL_FLOAT, 0, pts); //���ö�����������
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
                    ost_temp << "./../data/raster/satellite/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                #else//Ŀǰֻ������tm3
                    ost_temp << "raster/satellite/" << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                #endif
        	    string path = ost_temp.str();

                unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
                if (data) {
                    pImage = new Image(data, width * height, imIndex);
                    pImage->setInfo(height, width, nrChannels);
                }

                else {
                    stbi_image_free(data);
                    return;
                }
            }

            if (!pVertices || !pIndex) {
                /******************** �������� *******************/
                float* vertices = new float[size * size * 3];

                int* index = new int[(size * size - 2 * size + 1) * 6];

                //string hPath = "H:/trainer_map/data/jiangxi_height/" + to_string(zoom) + "." + to_string(row) + "." + to_string(col) + ".height";
                ostringstream ost_temp;//ost_temp.str("");
            	
                #ifdef WIN32
                    ost_temp << "./../data/jiangxi_height/" << (zoom) << "." << (row) << "." << (col) << ".height";
                #else//Ŀǰֻ������tm3
                    ost_temp << "jiangxi_height/" << (zoom) << "." << (row) << "." << (col) << ".height";
                #endif
            	string hPath = ost_temp.str();
                int hSize = 0;
                short int* height = openglEngine::OpenGLFileEngine::getHeightFromBinary<short int>(hPath.c_str(), hSize);

                Vec2d leftTop, pt;
                CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
                double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256 * interval;
                double x=0.0, y=0.0, z=0.0;

                int index_t = 0; int index_v = 0; int index_i = 0;
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {

                        pt[0] = leftTop[0] + (j - 1) * span;
                        pt[1] = leftTop[1] - (i - 1) * span;
                        double lat, lon;
                        CGeoUtil::WebMercator2lonLat(pt[0], pt[1], lat, lon);

                        if (height == 0x00)
                            CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, x, y, z);
                        else {
                            int h = 2 * height[i * size + j];
                            CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, h, x, y, z);
                        }
                        vertices[index_v++] = x;
                        vertices[index_v++] = y;
                        vertices[index_v++] = z;



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
                pHeight = new Heights(height, size * size, hIndex);

                buffer->setData(pVertices, VERTICE);
                buffer->setData(pIndex, INDEX);
                buffer->setData(pHeight, HEIGHT);
                buffer->setData(pImage, IMAGE);

                manager->insert(2, buffer, level2Index);

                //manager->insert(3, pVertices, vIndex);
                //manager->insert(3, pIndex, iIndex);
                //manager->insert(3, pHeight, hIndex);
                //manager->insert(3, pImage, imIndex);
            }
        }

        float* vertices = static_cast<float*>(pVertices->data());
        int* indexes = static_cast<int*>(pIndex->data());
        unsigned char* image = static_cast<unsigned char*>(pImage->data());
        int width, height1, nrChannels;
        pImage->getInfo(width, height1, nrChannels);
        if (vertices && indexes)
            openglEngine::OpenGLRenderEngine::drawRasters(vertices, textures, indexes, image, width, height1, nrChannels,
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
        cout << "֡�ʣ�" << framesPerSecond *2 << endl;
        framesPerSecond = 0;
    }
}*/
void cancelThread() {
    //_map->getBuffer();
    //int q = 0;
    //_map->_bufferLock = false;
    //pthread_mutex_unlock(&g_mutex);
    //cout << "����" << endl;
}
