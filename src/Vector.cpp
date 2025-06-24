#include "Vector.h"                       /**< ����ͷ�ļ� */

/** ���������ĳ��� */
inline float Vector3::length()
{
	return (float)( x * x + y * y + z * z );
}

/** ����ƽ�������� */
float Vector3::invSqrt(float x){
	float xhalf = 0.5f * x;
	int i = *(int *) &x;
	i = 0x5f3759df - (i >> 1);
	x = *(float *) &i;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

/** ��λ��һ���� */
Vector3 Vector3::normalize()
{
	float len = length();                  /**< ������������ */
	
	if( len == 0 )
		len = 1;

	len = invSqrt(len);
	x = x * len;
	y = y * len;
	z = z * len;
	/*x = x / len;
	y = y / len;
	z = z / len;*/

	return *this;
}

/** ��� */
 float Vector3::dotProduct(const Vector3& v)
{
	return ( x * v.x + y * v.y + z * v.z );
}

/** ��� */
Vector3 Vector3::crossProduct(const Vector3& v)
{
	Vector3 vec;

	vec.x = y * v.z - z * v.y;
	vec.y = z * v.x - x * v.z;
	vec.z = x * v.y - y * v.x;
	
	return vec;
}


/** �н� */
float Vector3::angle(Vector3& v){
	float x1 = length();
	float xv = v.length();

	float cosTheta = dotProduct(v) / invSqrt(x1) / invSqrt(xv);
	return acos(cosTheta);
}

/** ������ + */
 Vector3 Vector3::operator +(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;

	return vec;
}

/** ������ - */
 Vector3 Vector3::operator -(const Vector3& v)
{
	Vector3 vec;
	
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;

	return vec;
}

/** ������ * */
 Vector3 Vector3::operator *(float scale)
{
	x = x * scale;
	y = y * scale;
	z = z * scale;
	
	return *this;
}

/** ������ / */
 Vector3 Vector3::operator /(float scale)
{
	if(scale != 0.0)
	{	
	   x = x / scale;
	   y = y / scale;
	   z = z / scale;
	}
	return *this;
}

/** ���� */
 Vector3 Vector3::operator -()
{
	Vector3 vec( - x,- y, - z);
	return vec;
}













