//=========================================================================
/**
*  @file      CameraRove.h
*															
*	virtual bool Init();														
*		执行所有的初始化工作，如果成功函数返回true							
*																			
*	virtual void Uninit();													
*		执行所有的卸载工作										
*																			
*	virtual void Update(DWORD milliseconds);										
*		执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位
*																			
*	virtual void Draw();															
*		执行所有的绘制操作
*/
//=========================================================================

#ifndef __CAMERA_ROVE_H__
#define __CAMERA_ROVE_H__


#define MANUAL 1
#define AUTOMATIC 2


#include "stdafx.h"
#include <math.h>

//#include "CBMPLoader.h"
#include "GLFrame.h"												/**< 包含基本的框架类 */
//#include "Font.h"
#include "Camera.h"
//#include "Terrain.h"
//#include "ReadTerrainData.h"
//#include "3DSLoader.h"
//#include "Fuser.h"

#define NUM_OF_PLANES 5
/** 从GL_Application派生出一个子类 */
class CameraRove : GLApplication								
{
public:
	bool Init();							/**< 执行所有的初始化工作，如果成功函数返回true */
	void Uninit();						/**< 执行所有的卸载工作 */
	void Update(DWORD milliseconds);		/**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void Draw();							/**< 执行所有的绘制操作 */

    void UpdateCamera();                 /**< 更新摄像机 */
	void CaculateFrameRate();            /**< 计算帧速 */
    void PrintText();
	
	void DrawGrid();                     /**< 绘制网格地面 */

	void DrawPatches(int patchID, int step);
	void calcDist2Step();
	void DrawSphere();                   /**< 绘制球体 */
	void DrawBox();                      /**< 绘制木箱 */

	void DrawAirport();
	void Draw3dsPlane(/*C3DSLoader modle, */float x, float y, float z, float scale);
	void Draw3dsAirport(float x, float y, float z, float scale);
	void updateModelPos();

	void grabBuf(GLubyte * pInnerData, int innerWidth, int innerHeight);
	GLubyte * getInnerWin();
	void getGrabCamera(Camera & refCamera); //从一个参考（前向）得到6个
        void restoreCamera();
        void setTmpCamera(Camera & tCamera);
        Camera & getCamera(int i) ;
private:
	friend class GLApplication;				/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	CameraRove(const char * class_name);	/**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	//CBMPLoader m_Texture;                     /**< 位图载入类 */
	//GLFont     m_Font;                        /**< 字体类 */

	//bool m_visiblePatch[PATCHNUM];
	int currPatchID; // 当前所处的块
	int dist2step[16];

	Camera     m_Camera;                      /**< 摄像机类 */     
  	float      m_Fps;                         /**< 帧速 */
	

	Vector3		planePos;

    Camera tmpCamera;
	Camera     m_grabCamera[6];
	Vector3 samplePlanePos[NUM_OF_PLANES];
};

typedef enum
{
	FRONT_FACE,
	BACK_FACE,
	LEFT_FACE,
	RIGHT_FACE,
	UP_FACE,
	DOWN_FACE,
	NUM_OF_FACES	
}SAMPLE_FACE;

#endif	// __CAMERA_ROVE_H__