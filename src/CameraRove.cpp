#include "CameraRove.h"	
#include "TMap/OMap.h"
#include "mapDrv.h"
#include "InputCApi.h"
#include <windows.h>
#include <stdio.h>

#define MY_PI 3.14159F
extern OM3DScheduler* _scheduler;
extern OMScheduler* _scheduler2d;
extern OMap* _map;
float angle = 0.0;
//extern textRender::CFontRender* _render;
extern JTFONT hzFont[2];

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	CameraRove * test = new CameraRove(class_name);
	return reinterpret_cast<GLApplication *>(test);
}

/** 构造函数 */
CameraRove::CameraRove(const char * class_name) : GLApplication(class_name)
{
	calcDist2Step();
	//初始化用户自定义的程序变量	
	//m_readTerrain = new ReadTerrainData();
	m_Fps = 0;
	currPatchID = 0;
}

/** 初始化OpenGL */
bool CameraRove::Init()									
{		
	ResizeDraw(true);	   /**< 改变OpenGL窗口大小，直接调用子类的函数 true*/

	m_Timer = 0;	
	//glFlush();
	return true;    /**< 成功返回 */
}

#define TIME 100
/** 用户自定义的卸载函数 */
void CameraRove::Uninit()									
{
	/** 用户自定义的卸载过程 */
	//m_Texture.FreeImage();              /** 释放纹理图像占用的内存 */
	//glDeleteTextures(1, &m_Texture.ID); /**< 删除纹理对象 */

}

/** 更新摄像机 */
void CameraRove::UpdateCamera()
{
//	m_Camera.setViewByMouse();
	static int isFPV = 0;
	char filename[16];	 
	
	/** 键盘按键响应 */
	if(m_Keys.IsPressed(VK_SHIFT))                      /**< 按下SHIFT键时加速 */
	{
		m_Camera.setSpeed(100.0f);
	}
	else 
	{
		m_Camera.setSpeed(2.0f);
	}
	if (m_Keys.IsPressed(VK_UP) || m_Keys.IsPressed('W'))   /**< 向上方向键或'W'键按下 */
	{
		m_Keys.SetReleased(VK_UP);
		m_Keys.SetReleased('W');
		if(!_scheduler2d->pan(4))
			_map->isViewChanged = true;
		//_scheduler2d->pan(4);
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed(VK_DOWN) || m_Keys.IsPressed('S')) /**< 向下方向键或'S'键按下 */
	{
		m_Keys.SetReleased(VK_DOWN);
		m_Keys.SetReleased('S');
		if (!_scheduler2d->pan(2))
			_map->isViewChanged = true;
		//_scheduler2d->pan(2);
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed(VK_LEFT) || m_Keys.IsPressed('A')) /**< 向左方向键或'A'键按下 */
	{
		m_Keys.SetReleased(VK_LEFT);
		m_Keys.SetReleased('A');
		if (!_scheduler2d->pan(1))
			_map->isViewChanged = true;
		//_scheduler2d->pan(1);
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed(VK_RIGHT) || m_Keys.IsPressed('D')) /**< 向右方向键或'D'键按下 */
	{
		m_Keys.SetReleased(VK_RIGHT);
		m_Keys.SetReleased('D');
		if (!_scheduler2d->pan(3))
			_map->isViewChanged = true;
		//_scheduler2d->pan(3);
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed('N'))
	{
		m_Keys.SetReleased('N');
		_scheduler2d->zoomIn();
			_map->isViewChanged = true;
		//_scheduler2d->zoomIn();
		Sleep(TIME);
	}		
	else if (m_Keys.IsPressed('M'))
	{
		m_Keys.SetReleased('M');
		_scheduler2d->zoomOut();
			_map->isViewChanged = true;
		//_scheduler2d->zoomOut();
		Sleep(TIME);
	}		
	//else if (m_Keys.IsPressed('I'))
	//	_scheduler->translate(2);
	//else if (m_Keys.IsPressed('J'))
	//	_scheduler->translate(3);		
	//else if (m_Keys.IsPressed('K'))
	//	_scheduler->translate(4);		
	//else if (m_Keys.IsPressed('L'))
	//	_scheduler->translate(1);	
	else if (m_Keys.IsPressed('T'))
	{	
		m_Keys.SetReleased('T');
		if (_map->Dislpay() != 1) {
			_map->setDislpay(1);
			_map->isViewChanged = true;
		}	
		//��
		_map->turnOnLayer(1);
		_map->turnOnLayer(4);
		_map->turnOnLayer(5);
		_map->turnOnLayer(9);
		_map->turnOnLayer(10);
		_map->turnOnLayer(11);
		//��
		_map->turnOnLayer(2);
		_map->turnOnLayer(6);
		_map->turnOnLayer(7);
		_map->turnOnLayer(12);
		_map->turnOnLayer(13);
		_map->turnOnLayer(14);
		_map->turnOnLayer(15);
		_map->turnOnLayer(23);
		_map->turnOnLayer(29);
		Sleep(TIME);
	}	
	else if (m_Keys.IsPressed('Y'))
	{
		m_Keys.SetReleased('Y');
		if (_map->Dislpay() != 2) {
			_map->setDislpay(2);
			_map->isViewChanged = true;
		}
		Sleep(TIME);
	}	
	else if (m_Keys.IsPressed('U'))
	{
		m_Keys.SetReleased('U');
		if (_map->Dislpay() != 3) {
			_map->setDislpay(3);
			_map->isViewChanged = true;
		}
		//��
		_map->turnOffLayer(1);
		_map->turnOffLayer(4);
		_map->turnOffLayer(5);
		_map->turnOffLayer(9);
		_map->turnOffLayer(10);
		_map->turnOffLayer(11);
		//��
		_map->turnOffLayer(2);
		_map->turnOffLayer(6);
		_map->turnOffLayer(7);
		_map->turnOffLayer(12);
		_map->turnOffLayer(13);
		_map->turnOffLayer(14);
		_map->turnOffLayer(15);
		_map->turnOffLayer(23);
		_map->turnOffLayer(29);
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed('O'))
	{
		m_Keys.SetReleased('O');
		//angle += 1;
		if(!_scheduler2d->rotate(1))
			_map->isViewChanged = true;
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed('P'))
	{
		m_Keys.SetReleased('P');
		//angle -= 1;
		if(!_scheduler2d->rotate(-1))
			_map->isViewChanged = true;
		Sleep(TIME);
	}	
	/*else if (m_Keys.IsPressed('C')) 
	{
		if(!_scheduler->changeViewer(1, 4))
			_map->isViewChanged = true;
		Sleep(500);
	}
	else if (m_Keys.IsPressed('X'))
	{
		if(!_scheduler->changeViewer(0, 2.5))
			_map->isViewChanged = true;
		Sleep(500);
	}
	else if (m_Keys.IsPressed('Z'))
	{
		if(!_scheduler->changeViewer(1, 2.5))
			_map->isViewChanged = true;
		Sleep(500);
	}
    else if (m_Keys.IsPressed('V'))
	{
		if(!_scheduler->changeViewer(1, 3.25))
			_map->isViewChanged = true;
		Sleep(500);
	}*/
	else if (m_Keys.IsPressed('Q'))
	{
		m_Keys.SetReleased('Q');
		_map->turnUpBrightness();
	}
	else if (m_Keys.IsPressed('E')) 
	{
		m_Keys.SetReleased('E');
		_map->turnDownBrightness();
	}
	else if (m_Keys.IsPressed('B'))
	{
		m_Keys.SetReleased('B');
		isRoma = 1;
	}
	else if (m_Keys.IsPressed('G'))
	{
		m_Keys.SetReleased('G');
		isRoma = 0;
	}
    else if (m_Keys.IsPressed('2'))
	{
		m_Keys.SetReleased('2');
		if (!_scheduler->changePitch(60.0))
		{
			_map->isViewChanged = true;			
		}
		if (isFPV == 0) {
			_scheduler->_isFirstPerson = 1;
			Vec3d eye = _scheduler->eye();
			eye[2] /= 4;
			_scheduler->eyeSet(eye);
			isFPV = 1;
		}
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed('4'))
	{
		m_Keys.SetReleased('4');
			if(!_scheduler->changeYaw(1))
			_map->isViewChanged = true;
		Sleep(TIME);
	}
    else if (m_Keys.IsPressed('5'))
	{
		m_Keys.SetReleased('5');
		if(!_scheduler->changeYaw(-1))
			_map->isViewChanged = true;
		Sleep(TIME);
	}
	else if (m_Keys.IsPressed('3'))
	{
		m_Keys.SetReleased('3');
		if (!_scheduler->changePitch(-60.0)) {
			_map->isViewChanged = true;			
		}
		if (isFPV == 1)
		{
			_scheduler->_isFirstPerson = 0;
			Vec3d eye = _scheduler->eye();
			eye[2] *= 4;
			_scheduler->eyeSet(eye);
			isFPV = 0;
		}
			
		Sleep(TIME);
	}
	/*else if (m_Keys.IsPressed('J'))
	{
		_map->turnOffLayer(0);
	}
	else if (m_Keys.IsPressed('K'))
	{
		_map->turnOffLayer(10);
		_map->turnOffLayer(12);
		_map->turnOffLayer(11);
	}
	else if (m_Keys.IsPressed('L'))
	{
		_map->turnOffLayer(13);
		_map->turnOffLayer(14);
		_map->turnOffLayer(15);
	}
	else if (m_Keys.IsPressed('H'))
	{
		_map->turnOnLayer(10);
		_map->turnOnLayer(12);
		_map->turnOnLayer(11);
		_map->turnOnLayer(13);
		_map->turnOnLayer(14);
		_map->turnOnLayer(15);
	}
	else if (m_Keys.IsPressed('B')) 
	{
		//_scheduler->changeViewer(angle); //oglfSetFontSize(hzFont[1], 24);
	}*/
}

/** 更新飞机模型位置*/
void CameraRove::updateModelPos()
{
	if(m_Timer <= 500){
		planePos.z = 169.0 + (300.0 - m_Timer/500.0 * 300.0);
		planePos.y = 1245603.1 + (m_Timer/500.0 * 5724.0);
		planePos.x = 572449.2;
	}
	else if(m_Timer <= 600){
		planePos.y = 1245603.1 + (m_Timer/500.0 * 5724.0);
	}

	//***更新其他飞机位置
	samplePlanePos[0].x = 572449.2f+1000.0f*cos(2*MY_PI*0.1*m_Timer/30.0);
	samplePlanePos[0].y = 1252000.0f+200.0f*sin(2*MY_PI*0.1*m_Timer/30.0);
	samplePlanePos[0].z = 500.0f;

	samplePlanePos[1].x = 572449.2f+100.0f*cos(2*MY_PI*0.1*m_Timer/30.0);
	samplePlanePos[1].y = 1252000.0f+200.0f*sin(2*MY_PI*0.1*m_Timer/30.0);
	samplePlanePos[1].z = 800.0f;
}

/** 程序更新函数 */
void CameraRove::Update(DWORD milliseconds)						
{

	if (m_Keys.IsPressed(VK_ESCAPE) == true){					/**< ��ESC�˳� */
		TerminateApplication();									
	}
	if (m_Keys.IsPressed(VK_F1) == true){					/**< ��F1�ڴ��ں�ȫ�����л� */
		ToggleFullscreen();									
	}
	UpdateCamera();
}

/** 计算帧速 */
void CameraRove::CaculateFrameRate(){
	static float framesPerSecond    = 0.0f;	     /**< 保存显示帧数 */	
    static float lastTime			= 0.0f;	     /**< 记录上次时间 */						
    float currentTime = GetTickCount() * 0.001f; /**< 获得当前时间 */	 			

	++framesPerSecond;                           /**< 显示帧数递增1 */
    /** 如果时间差大于1.0秒 */
	if( currentTime - lastTime > 1.0f )          
    {
	    lastTime = currentTime;                   /**< 保存当前时间 */
		m_Fps = framesPerSecond;                  /**< 当前帧数传给m_Fps */
        framesPerSecond = 0;                      /**< 将帧数置零 */                    
    }
}

/** 输出文字信息 */
void CameraRove::PrintText()
{	

	//输出帧速
	char a[30];
	CaculateFrameRate();
    sprintf(a,"FPS:%3.0f", m_Fps);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1024, 0, 768, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//oglfSetFontSize(hzFont[1], 24);
	oglfDrawString(hzFont[1], 30.0, 750.0, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);

    memset(a, 0, 30);
    sprintf(a,"zoom = %d", _scheduler2d->zoom());
    oglfDrawString(hzFont[1], 30.0, 730.0, (const unsigned char*)a, FONT_JUST_HLEFT, FONT_JUST_VTOP);
	//renderArray();

#if 0
	//测试文件读取绘制
	ifstream infile;
	infile.open("5.18.26.anno", std::ifstream::binary);
	if (!infile) {
		//cout << path << " open binary file failed" << endl;
		return ;
	}

	infile.seekg(0, std::ios::end);
	int len = infile.tellg();
	infile.seekg(0, std::ios::beg);
	wchar_t buffer[50];
	char temp[50];
	memset(temp, 0, 50);
	memset(buffer, 0, 50);
	// 读取二进制数据 
	infile.read(const_cast<char*>(temp), len);

	int des_len = 0;
	InputVString::GB2312toUnicode2wchart(temp, buffer, &des_len);
	string fontName = "SIMHEI";
	_render->render(buffer, Vec2i(100, 100), 14, fontName.c_str(), Color(255, 0, 0, 255));
#endif
}



/** 绘制网格地面 */
void CameraRove::DrawGrid()
{
    /** 获得场景中一些状态  */
	GLboolean  lp,tp;
	glGetBooleanv(GL_LIGHTING,&lp);  
	glGetBooleanv(GL_TEXTURE_2D,&tp);
	
	/** 关闭纹理和光照 */
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
	
	/** 绘制过程 */
	glPushAttrib(GL_CURRENT_BIT);   /**< 保存当前属性 */
    glPushMatrix();                 /**< 压入堆栈 */
	glTranslatef(0.0f,0.0f,0.0f);  
	glColor3f(0.0f, 0.0f, 1.0f);    /**< 设置颜色 */

	/** 在X,Z平面上绘制网格 */
	for(float i = -5000; i <= 5000; i += 100)
	{
		/** 绘制线 */
		glBegin(GL_LINES);

		    /** X轴方向 */
			glVertex3f(-5000, 0, i);
			glVertex3f(5000, 0, i);

			/** Z轴方向 */
			glVertex3f(i, 0, -5000);
			glVertex3f(i, 0, 5000);

		glEnd();
	}
	glPopMatrix();
	glPopAttrib();
	
	/** 恢复场景状态 */
	if(tp)
	  glEnable(GL_TEXTURE_2D);
	if(lp)
      glEnable(GL_LIGHTING);
}

/** 绘制球体 */
void CameraRove::DrawSphere()
{
	//设置材质属性
	GLfloat mat_ambient[] = { 0.9f, 0.5f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.9f, 0.5f, 0.8f, 1.0f };
	GLfloat mat_shininess[] = { 100.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
	//获得纹理启用状态
	GLboolean tp;
	glGetBooleanv(GL_TEXTURE_2D,&tp);
    glDisable(GL_TEXTURE_2D);                   /**< 关闭纹理 */
    
	//绘制过程
	glPushMatrix();
    glTranslatef(-5.0f,2.0f,-10.0f);
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricNormals(sphere,GLU_SMOOTH);
	gluSphere(sphere,2.0,50,50);
	gluDeleteQuadric(sphere);
    glPopMatrix();
    
	//恢复状态
	if(tp)
	   glEnable(GL_TEXTURE_2D);
	
}

/** 绘制木箱 */
void CameraRove::DrawBox()
{
	// 设置材质属性
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
	glTranslatef(5.0f,2.0f,-10.0f);
	glScalef(2.0f,2.0f,2.0f);
	
	// 选择纹理 
	//glBindTexture(GL_TEXTURE_2D, m_Texture.ID);
	
	// 开始绘制四边形 
	glBegin(GL_QUADS);												
		
	    /// 前侧面
		glNormal3f( 0.0f, 0.0f, 1.0f);								/**< 指定法线指向观察者 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		
		/// 后侧面
		glNormal3f( 0.0f, 0.0f,-1.0f);								/**< 指定法线背向观察者 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		
		/// 顶面
		glNormal3f( 0.0f, 1.0f, 0.0f);								/**< 指定法线向上 */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		
		/// 底面
		glNormal3f( 0.0f,-1.0f, 0.0f);								/**< 指定法线朝下 */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		
		/// 右侧面
		glNormal3f( 1.0f, 0.0f, 0.0f);								/**< 指定法线朝右 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		
		/// 左侧面
		glNormal3f(-1.0f, 0.0f, 0.0f);								/**< 指定法线朝左 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();
	glPopMatrix();
}

void CameraRove::DrawAirport(){
	// 设置材质属性
	glPushMatrix();
	
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glTranslatef(572460.0f - 1548.1f, 1251150.0f, 40.0f);
	//glScalef(2.0f,2.0f,2.0f);

	// 选择纹理 
	//glBindTexture(GL_TEXTURE_2D, m_Texture.ID);

	// 开始绘制四边形 
	glBegin(GL_QUADS);

	/// 前侧面
	glNormal3f( 0.0f, 1.0f, 0.0f);								/**< 指定法线指向观察者 */
	glVertex3f(0.0f, 2000.0f, 120.0f);	
	glVertex3f(2000.0f, 2000.0f, 120.0f);	
	glVertex3f(2000.0f, 2000.0f, 0.0f);	
	glVertex3f(0.0f, 2000.0f, 0.0f);	

	/// 后侧面
	glNormal3f(0.0f, -1.0f, 0.0f);								/**< 指定法线背向观察者 */
	glVertex3f(0.0f, 0.0f, 120.0f);	
	glVertex3f(2000.0f, 0.0f, 120.0f);	
	glVertex3f(2000.0f, 0.0f, 0.0f);	
	glVertex3f(0.0f, 0.0f, 0.0f);		

	glNormal3f( 0.0f, 0.0f, 1.0f);								/**< 指定法线向上 */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 2000.0f, 120.0f);	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 120.0f);	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2000.0f, 1.0f, 120.0f);	
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2000.0f, 2000.0f, 120.0f);	
	
	/// 底面
	//glNormal3f( 0.0f, 0.0f, -1.0f);								/**< 指定法线朝下 */
	//glVertex3f(0.0f, 2000.0f, 120.0f);	
	//glVertex3f(0.0f, 0.0f, 120.0f);	
	//glVertex3f(2000.0f, 1.0f, 120.0f);	
	//glVertex3f(2000.0f, 2000.0f, 120.0f);	

	/// 右侧面
	glNormal3f( 1.0f, 0.0f, 0.0f);								/**< 指定法线朝右 */
	glVertex3f( 0.0f, 0.0f, 120.0f);	
	glVertex3f( 0.0f, 2000.0f, 120.0f);	
	glVertex3f( 0.0f, 2000.0f, 0.0f);	
	glVertex3f( 0.0f, 0.0f,  0.0f);	

	/// 左侧面
	glNormal3f(-1.0f, 0.0f, 0.0f);								/**< 指定法线朝左 */
	glVertex3f( 2000.0f, 0.0f, 120.0f);	
	glVertex3f( 2000.0f, 2000.0f, 120.0f);	
	glVertex3f( 2000.0f, 2000.0f, 0.0f);	
	glVertex3f( 2000.0f, 0.0f, 0.0f);	

	glEnd();
	glPopMatrix();
}

void CameraRove::Draw3dsPlane(/*C3DSLoader modle, */float x, float y, float z, float scale){
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	//m_airplane.Draw();
	glPopMatrix();
}

void CameraRove::Draw3dsAirport(float x, float y, float z, float scale){
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	//m_airport.Draw();
	glPopMatrix();
}

void CameraRove::restoreCamera()
{
	m_Camera.m_Position = tmpCamera.m_Position;
	m_Camera.m_View = tmpCamera.m_View;
	m_Camera.m_UpVector = tmpCamera.m_UpVector;

	return;
}

void CameraRove::setTmpCamera(Camera & tCamera)
{
	tmpCamera.m_Position= m_Camera.m_Position;
	tmpCamera.m_View = m_Camera.m_View;
	tmpCamera.m_UpVector = m_Camera.m_UpVector;
	m_Camera.m_Position = tCamera.m_Position;
	m_Camera.m_View = tCamera.m_View;
	m_Camera.m_UpVector = tCamera.m_UpVector;

	

	return;
}

Camera & CameraRove::getCamera(int i) 
{
	return m_grabCamera[i];
}
/** 绘制函数 */
void CameraRove::Draw()											
{
    PrintText();				    
}




GLfloat ground_mat_ambient2[]  = {0.74f, 0.72f, 0.55f, 1.0f};
GLfloat ground_mat_diffuse0[]  = {0.90f, 0.90f, 0.90f, 1.0f};
GLfloat ground_mat_diffuse1[]  = {0.49f, 0.60f, 0.44f, 1.0f};

GLfloat ground_mat_diffuse2[]  = {0.74f, 0.72f, 0.55f, 1.0f};
GLfloat ground_mat_diffuse3[]  = {0.66f, 0.56f, 0.33f, 1.0f};
GLfloat ground_mat_diffuse4[]  = {0.91f, 0.25f, 0.28f, 1.0f};


double Height = 3000;

void CameraRove::DrawPatches(int patchID, int step){
#if 0
	glBindTexture(GL_TEXTURE_2D, m_readTerrain->patchTexID[patchID]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_mat_diffuse0);
	for(int i = 0; i < m_readTerrain->stepx - step; i+=step){
		glBegin(GL_TRIANGLE_STRIP);
		for(int j = 0; j < m_readTerrain->stepy; j+=step){

			glTexCoord2f((float)i/m_readTerrain->stepx, (float)j/m_readTerrain->stepy);
			glNormal3f(
				m_readTerrain->patchNorm[patchID].terrainNorm[i][j].x,
				m_readTerrain->patchNorm[patchID].terrainNorm[i][j].y, 
				m_readTerrain->patchNorm[patchID].terrainNorm[i][j].z
			);
			glVertex3f(
				m_readTerrain->patchData[patchID].terrainPoints[i][j].x,
				m_readTerrain->patchData[patchID].terrainPoints[i][j].y,
				m_readTerrain->patchData[patchID].terrainPoints[i][j].z
			);


			glTexCoord2f((float)(i+step)/m_readTerrain->stepx, (float)j/m_readTerrain->stepy);
			glNormal3f(
				m_readTerrain->patchNorm[patchID].terrainNorm[i+step][j].x,
				m_readTerrain->patchNorm[patchID].terrainNorm[i+step][j].y, 
				m_readTerrain->patchNorm[patchID].terrainNorm[i+step][j].z
			);
			glVertex3d(
				m_readTerrain->patchData[patchID].terrainPoints[i+step][j].x,
				m_readTerrain->patchData[patchID].terrainPoints[i+step][j].y,
				m_readTerrain->patchData[patchID].terrainPoints[i+step][j].z
			);
			if(m_readTerrain->stepy < j + 1 + step && j + 1 + step < m_readTerrain->stepy + step){
				j = m_readTerrain->stepy - 1 - step;
			}	
		}
		glEnd();
		if(m_readTerrain->stepx - step < i + 1 + step && i + 1 + step < m_readTerrain->stepx){
			i = m_readTerrain->stepx - 1 - step - step;
		}
	}
#endif
}

void CameraRove::calcDist2Step(){
	dist2step[0] = 1;
	for(int i = 1; i < 16; i++){
		dist2step[i] = dist2step[i-1] << 1;
	}
}

GLubyte * CameraRove::getInnerWin(){
	FILE* innerFile = fopen("innerbox2.bmp", "rb");
	if(NULL == innerFile){
		exit(1);
	}
	GLubyte *pInnerData;
	GLint innerWidth = 480;
	GLint innerHeight = 320;
	GLint i = innerWidth * 3;
	while(i % 4 != 0) ++i;
	GLint innerDataLength = innerHeight * i;
	pInnerData = (GLubyte *)malloc(innerDataLength);
	fseek(innerFile, 54, SEEK_SET);
	fread(pInnerData, innerDataLength, 1, innerFile);
	fclose(innerFile);
	return NULL;
}

void CameraRove::grabBuf(GLubyte * pInnerData, int innerWidth, int innerHeight)
{
	FILE*    pDummyFile;
	FILE*    pWritingFile;
	GLubyte* pPixelData;
	GLubyte  BMP_Header[54];
	GLint    PixelDataLength;

	// 计算像素数据的实际长度
	GLint windowWidth = m_Window.GetWidth();
	GLint windowHeight = m_Window.GetHeight();
	GLint lineLen = windowWidth * 3;
	while( lineLen%4 != 0 )
		++lineLen;
	
	PixelDataLength = lineLen * windowHeight;

	// 分配内存和打开文件
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if( pPixelData == 0 )
		exit(0);

	pDummyFile = fopen("dummy.bmp", "rb");
	if( pDummyFile == 0 )
		exit(0);

	pWritingFile = fopen("grab.bmp", "wb");
	if( pWritingFile == 0 )
		exit(0);

	// 读取像素
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, windowWidth, windowHeight,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	int bottomBand = (windowHeight - innerHeight) / 2;
	int leftBand = (windowWidth - innerWidth) / 2;
	for(int p = bottomBand; p < bottomBand + innerHeight; p++){
		int i = p - bottomBand;
		for(int q = leftBand; q < leftBand + innerWidth; q++){
			int j = q - leftBand;
			(pPixelData[p * windowWidth * 3 + q * 3] += pInnerData[i * innerWidth * 3 + j * 3]) /= 2;
			(pPixelData[p * windowWidth * 3 + q * 3 + 1] += pInnerData[i * innerWidth * 3 + j * 3 + 1]) /= 2;
			(pPixelData[p * windowWidth * 3 + q * 3 + 2] += pInnerData[i * innerWidth * 3 + j * 3 + 2]) /= 2;
		}
	}

	GLdouble modelMatrix[16], projMatrix[16];
	GLint viewport[4];
	GLdouble objx, objy, objz;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluUnProject(0, 0, 0, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

	glRasterPos3d(objx, objy, objz);

	glDrawPixels(windowWidth, windowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
	
	// 把dummy.bmp的文件头复制为新文件的文件头
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	int i = windowWidth;
	int j = windowHeight;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}

void CameraRove :: getGrabCamera(Camera & refCamera)
{
	//Vector3 eye( 571856.6f, 1253468.6f, 1086.4f);
	//Vector3 ref(569551.2f, -113389.5f, 166874.9f);
	Vector3 eye(refCamera.getPosition());
	Vector3 ref(refCamera.getView());

	Vector3 n = ref - eye;

	//后向
	m_grabCamera[BACK_FACE].m_Position = eye;
	m_grabCamera[BACK_FACE].m_View = eye*2.0f - ref;
	m_grabCamera[BACK_FACE].m_UpVector = refCamera.m_UpVector;
	return;
}
#ifdef __BIG_ENDIAN__
///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_WORD(void *pWord)
	{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;
    
    temp = pBytes[0];
    pBytes[0] = pBytes[1];
    pBytes[1] = temp;
	}

///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_DWORD(void *pWord)
	{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;
    
    // Swap outer bytes
    temp = pBytes[3];
    pBytes[3] = pBytes[0];
    pBytes[0] = temp;
    
    // Swap inner bytes
    temp = pBytes[1];
    pBytes[1] = pBytes[2];
    pBytes[2] = temp;
	}
#else

// Define them away on little endian systems
#define LITTLE_ENDIAN_WORD 
#define LITTLE_ENDIAN_DWORD 
#endif
// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)


////////////////////////////////////////////////////////////////////
// Capture the current viewport and save it as a targa file.
// Be sure and call SwapBuffers for double buffered contexts or
// glFinish for single buffered contexts before calling this function.
// Returns 0 if an error occurs, or 1 on success.
GLint gltWriteTGA(const char *szFileName)
	{
    FILE *pFile;                // File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;   // Size in bytes of image
    GLbyte	*pBits = NULL;      // Pointer to bits
    GLint iViewport[4];         // Viewport in pixels
    GLenum lastBuffer;          // Storage for the current read buffer setting
    
	// Get the viewport dimensions
	glGetIntegerv(GL_VIEWPORT, iViewport);
	
    // How big is the image going to be (targas are tightly packed)
	lImageSize = iViewport[2] * 3 * iViewport[3];	
	
    // Allocate block. If this doesn't work, go home
    pBits = (GLbyte *)malloc(lImageSize);
    if(pBits == NULL)
        return 0;
	
    // Read bits from color buffer
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    
    // Get the current read buffer setting and save it. Switch to
    // the front buffer and do the read operation. Finally, restore
    // the read buffer state
    glGetIntegerv(GL_READ_BUFFER, (GLint *)&lastBuffer);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, iViewport[2], iViewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);
    glReadBuffer(lastBuffer);
    
    // Initialize the Targa header
    tgaHeader.identsize = 0;
    tgaHeader.colorMapType = 0;
    tgaHeader.imageType = 2;
    tgaHeader.colorMapStart = 0;
    tgaHeader.colorMapLength = 0;
    tgaHeader.colorMapBits = 0;
    tgaHeader.xstart = 0;
    tgaHeader.ystart = 0;
    tgaHeader.width = iViewport[2];
    tgaHeader.height = iViewport[3];
    tgaHeader.bits = 24;
    tgaHeader.descriptor = 0;
    
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif
    
    // Attempt to open the file
    pFile = fopen(szFileName, "wb");
    if(pFile == NULL)
		{
        free(pBits);    // Free buffer and return error
        return 0;
		}
	
    // Write the header
    fwrite(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
    
    // Write the image data
    fwrite(pBits, lImageSize, 1, pFile);
	
    // Free temporary buffer and close the file
    free(pBits);    
    fclose(pFile);
    
    // Success!
    return 1;
	}


////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
	{
    FILE *pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte	*pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;
    
    // Attempt to open the fil
    pFile = fopen(szFileName, "rb");
    if(pFile == NULL)
        return NULL;
	
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
    
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif
	
	
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
	
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if(pBits == NULL)
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1)
		{
        free(pBits);
        return NULL;
		}
    
    // Set OpenGL format expected
    switch(sDepth)
		{
        case 3:     // Most likely case
            *eFormat = GL_BGR_EXT;
            *iComponents = GL_RGB8;
            break;
        case 4:
            *eFormat = GL_BGRA_EXT;
            *iComponents = GL_RGBA8;
            break;
        case 1:
            *eFormat = GL_LUMINANCE;
            *iComponents = GL_LUMINANCE8;
            break;
		};
	
    
    // Done with File
    fclose(pFile);
	
    // Return pointer to image data
    return pBits;
	}
