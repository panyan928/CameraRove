/**  
*
*
*   @file   stdafx.h
*
*
*/

#ifndef __STDAFX_H__
#define __STDAFX_H__

/** 包含常用的头文件 */
#include <windows.h>
#include <stdio.h>
#include <math.h>                            
#include <time.h>

/** 包含gl头文件 */
#include "gl\gl.h"	
#include "gl\glu.h"
//#include "gl\glut.h"
//#include <opencv2/opencv.hpp>
//#include <gl\glaux.h>
//#include <gl\glext.h>

/** 包含OpenGL链接库文件 */
						
//#pragma comment(lib, "glaux.lib")	

/** 禁止编译器出现类型转换的警告 */
#pragma warning(disable: 4311)                                 
#pragma warning(disable: 4312)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4267)



#endif