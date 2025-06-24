//=========================================================================
/**
*  @file      CameraRove.h
*															
*	virtual bool Init();														
*		ִ�����еĳ�ʼ������������ɹ���������true							
*																			
*	virtual void Uninit();													
*		ִ�����е�ж�ع���										
*																			
*	virtual void Update(DWORD milliseconds);										
*		ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
*																			
*	virtual void Draw();															
*		ִ�����еĻ��Ʋ���
*/
//=========================================================================

#ifndef __CAMERA_ROVE_H__
#define __CAMERA_ROVE_H__


#define MANUAL 1
#define AUTOMATIC 2


#include "stdafx.h"
#include <math.h>

//#include "CBMPLoader.h"
#include "GLFrame.h"												/**< ���������Ŀ���� */
//#include "Font.h"
#include "Camera.h"
//#include "Terrain.h"
//#include "ReadTerrainData.h"
//#include "3DSLoader.h"
//#include "Fuser.h"

#define NUM_OF_PLANES 5
/** ��GL_Application������һ������ */
class CameraRove : GLApplication								
{
public:
	bool Init();							/**< ִ�����еĳ�ʼ������������ɹ���������true */
	void Uninit();						/**< ִ�����е�ж�ع��� */
	void Update(DWORD milliseconds);		/**< ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ */
	void Draw();							/**< ִ�����еĻ��Ʋ��� */

    void UpdateCamera();                 /**< ��������� */
	void CaculateFrameRate();            /**< ����֡�� */
    void PrintText();
	
	void DrawGrid();                     /**< ����������� */

	void DrawPatches(int patchID, int step);
	void calcDist2Step();
	void DrawSphere();                   /**< �������� */
	void DrawBox();                      /**< ����ľ�� */

	void DrawAirport();
	void Draw3dsPlane(/*C3DSLoader modle, */float x, float y, float z, float scale);
	void Draw3dsAirport(float x, float y, float z, float scale);
	void updateModelPos();

	void grabBuf(GLubyte * pInnerData, int innerWidth, int innerHeight);
	GLubyte * getInnerWin();
	void getGrabCamera(Camera & refCamera); //��һ���ο���ǰ�򣩵õ�6��
        void restoreCamera();
        void setTmpCamera(Camera & tCamera);
        Camera & getCamera(int i) ;
private:
	friend class GLApplication;				/**< ����Ϊ����һ����Ԫ�࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name) */
	CameraRove(const char * class_name);	/**< ���캯�� */

	/** �û��Զ���ĳ������ */ 
	//CBMPLoader m_Texture;                     /**< λͼ������ */
	//GLFont     m_Font;                        /**< ������ */

	//bool m_visiblePatch[PATCHNUM];
	int currPatchID; // ��ǰ�����Ŀ�
	int dist2step[16];

	Camera     m_Camera;                      /**< ������� */     
  	float      m_Fps;                         /**< ֡�� */
	

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