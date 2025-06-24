#include "stdafx.h"
#include "Camera.h"                    /**< ���������ͷ�ļ� */
#include "Vector.h"                    /**< ���������� */
#include "GLFrame.h"

#define ANG 4.0f

Camera* Camera::m_pCamera = NULL;

/** ���캯�� */
Camera::Camera()
{
	/** ��ʼ������ֵ */
	Vector3 zero = Vector3(0.0, 0.0, 0.0);		
	Vector3 view = Vector3(0.0, 1.0, 0.0);		
	Vector3 up   = Vector3(0.0, 0.0, 1.0);		
    
	/** ��ʼ������� */
	m_Position	= zero;					
	m_View		= view;				
	m_UpVector	= up;	
	m_Speed     = 10.0f;
	
	m_pCamera = this;
}


Camera::~Camera()
{
}

/** �����������λ��,������������� */
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
	/** �������� */
	Vector3 Position	= Vector3(positionX, positionY, positionZ);
	Vector3 View		= Vector3(viewX, viewY, viewZ);
	Vector3 UpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	/** ��������� */
	m_Position = Position;	
	m_View     = View;			
	m_UpVector = UpVector;	
}

void Camera::setCamera(Vector3 vPosition, Vector3 vView, Vector3 vUp){
	m_Position = vPosition;
	m_View = vView;
	m_UpVector = vUp;
}

/**  ��ת���������  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	Vector3 newView;

	/** ���㷽������ */
	Vector3 view = m_View - m_Position;		

	/** ���� sin ��cosֵ */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** ������ת������xֵ */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	/** ������ת������yֵ */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	/** ������ת������zֵ */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	/** ����������ķ��� */
	m_View = m_Position + newView;
}

/** �����ƶ������ */
void Camera::yawCamera(float speed)
{
	Vector3 yaw;
	Vector3 cross = m_View - m_Position;
	cross = cross.crossProduct(m_UpVector);

	///��һ������
	yaw = cross.normalize();
 
	m_Position.x += yaw.x * speed;
	m_Position.y += yaw.y * speed;

	m_View.x += yaw.x * speed;
	m_View.y += yaw.y * speed;


}

/** ǰ���ƶ������ */
void Camera::moveCamera(float speed)
{
	/** ���㷽������ */
	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         /**< ��λ�� */
	
	/** ��������� */
	m_Position.x += vector.x * speed;    /**< �����ٶȸ���λ�� */
	m_Position.z += vector.z * speed;	
	m_Position.y += vector.y * speed;
	
	m_View.x += vector.x * speed;		 /**< �����ٶȸ��·��� */	
	m_View.y += vector.y * speed;
	m_View.z += vector.z * speed;
	  

}

/** �����ӵ� */
void Camera::setLook()
{
	
	/** �����ӿ� */
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			  m_View.x,	m_View.y, m_View.z,	
			  m_UpVector.x, m_UpVector.y, m_UpVector.z);
}

