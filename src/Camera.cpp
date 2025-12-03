#include "stdafx.h"
#include "Camera.h"                    /**< 包含摄像机头文件 */
#include "Vector.h"                    /**< 包含向量类 */
#include "GLFrame.h"

#define ANG 4.0f

Camera* Camera::m_pCamera = NULL;

/** 构造函数 */
Camera::Camera()
{
	/** 初始化向量值 */
	Vector3 zero = Vector3(0.0, 0.0, 0.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.0);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);		
    
	/** 初始化摄像机 */
	m_Position	= zero;					
	m_View		= view;				
	m_UpVector	= up;	
	m_Speed     = 10.0f;
	
	m_pCamera = this;
}


Camera::~Camera()
{
}

/** 设置摄像机的位置,朝向和向上向量 */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
	/** 构造向量 */
	Vector3 Position	= Vector3(positionX, positionY, positionZ);
	Vector3 View		= Vector3(viewX, viewY, viewZ);
	Vector3 UpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** 设置摄像机 */
	m_Position = Position;	
	m_View     = View;			
	m_UpVector = UpVector;	
}

void Camera::setCamera(Vector3 vPosition, Vector3 vView, Vector3 vUp){
	m_Position = vPosition;
	m_View = vView;
	m_UpVector = vUp;
}

/**  旋转摄像机方向  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	Vector3 newView;

	/** 计算方向向量 */
	Vector3 view = m_View - m_Position;		

	/** 计算 sin 和cos值 */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** 计算旋转向量的x值 */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	/** 计算旋转向量的y值 */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	/** 计算旋转向量的z值 */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	/** 更新摄像机的方向 */
	m_View = m_Position + newView;
}

/** 左右移动摄像机 */
void Camera::yawCamera(float speed)
{
	Vector3 yaw;
	Vector3 cross = m_View - m_Position;
	cross = cross.crossProduct(m_UpVector);

	///归一化向量
	yaw = cross.normalize();
 
	m_Position.x += yaw.x * speed;
	m_Position.y += yaw.y * speed;

	m_View.x += yaw.x * speed;
	m_View.y += yaw.y * speed;


}

/** 前后移动摄像机 */
void Camera::moveCamera(float speed)
{
	/** 计算方向向量 */
	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         /**< 单位化 */
	
	/** 更新摄像机 */
	m_Position.x += vector.x * speed;    /**< 根据速度更新位置 */
	m_Position.z += vector.z * speed;	
	m_Position.y += vector.y * speed;
	
	m_View.x += vector.x * speed;		 /**< 根据速度更新方向 */	
	m_View.y += vector.y * speed;
	m_View.z += vector.z * speed;
	  

}

/** 设置视点 */
void Camera::setLook()
{
	
	/** 设置视口 */
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			  m_View.x,	m_View.y, m_View.z,	
			  m_UpVector.x, m_UpVector.y, m_UpVector.z);
}