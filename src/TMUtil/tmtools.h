#ifndef TM_TOOLS
#define TM_TOOLS

#include <stdio.h>
//#include <iostream>
//#include <vector>

using namespace std;

class Frustum {
public:
	Frustum(double fovy, double aspectRatio, double zNear, double zFar) :
		fovy(fovy), aspectRatio(aspectRatio), zNear(zNear), zFar(zFar) {};
	Frustum() : fovy(0), aspectRatio(0), zNear(0), zFar(0) {};

	~Frustum() {};
	double& operator[] (int i) {
		switch (i)
		{
		case 0:
			return fovy;
		case 1:
			return aspectRatio;
		case 2:
			return zNear;
		case 3:
			return zFar;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	double fovy;
	double aspectRatio;
	double zNear;
	double zFar;

};

class OrthoFrustum {
public:
	OrthoFrustum(double left, double right, double bottom, double top, double zNear, double zFar) :
		left(left), right(right), bottom(bottom), top(top), zNear(zNear), zFar(zFar) {};
	OrthoFrustum() : left(-1), right(1), bottom(-1), top(1), zNear(0), zFar(10) {};

	~OrthoFrustum() {};
	double& operator[] (int i) {
		switch (i)
		{
		case 0:
			return left;
		case 1:
			return right;
		case 2:
			return bottom;
		case 3:
			return top;
		case 4:
			return zNear;
		case 5:
			return zFar;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	double bottom;
	double top;
	double left;
	double right;
	double zNear;
	double zFar;

};


class Color{
public:
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
		r(r), g(g), b(b), a(a) {};
	Color() : r(255), g(255), b(255), a(255) {};
	~Color() {};
	unsigned char& operator[](int i) {
		switch (i)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		default:
			printf("invalid index!\n");
			break;
		}
	};
	
	// 添加const版本的operator[]，用于const对象
	const unsigned char& operator[](int i) const {
		switch (i)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

class Vec3d {
public:
	Vec3d(double x, double y, double z) : x(x), y(y), z(z) {};
	Vec3d() : x(0), y(0), z(0) {};
	~Vec3d() {};
	double& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			printf("invalid index!\n");
			break;
		}
	};
	/*double distance(Vec3d pt1, Vec3d pt2) { return sqrt((pt1[0] - pt2[0]) * (pt1[0] - pt2[0]) + 
		(pt1[1] - pt2[1]) * (pt1[1] - pt2[1]) + (pt1[2] - pt2[2]) * (pt1[2] - pt2[2])); }*/
private:
	double x;
	double y;
	double z;
};

class Vec3 {
public:
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
	Vec3() : x(0), y(0), z(0) {};
	~Vec3() {};
	float& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	float x;
	float y;
	float z;
};

class Vec3i {
public:
	Vec3i(int x, int y, int z) : x(x), y(y), z(z) {};
	Vec3i() : x(0), y(0), z(0) {};
	~Vec3i() {};
	int& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	int x;
	int y;
	int z;
};

class Vec2d {
public:
	Vec2d(double x, double y) : x(x), y(y) {};
	Vec2d() : x(0), y(0) {};
	~Vec2d() {};
	double& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	double x;
	double y;
};

class Vec2 {
public:
	Vec2(float x, float y) : x(x), y(y) {};
	Vec2() : x(0), y(0) {};
	~Vec2() {};
	float& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	float x;
	float y;
};

class Vec2i {
public:
	Vec2i(int x, int y) : x(x), y(y) {};
	Vec2i() : x(0), y(0) {};
	~Vec2i() {};
	int& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	int x;
	int y;
};

class Vec4d {
public:
	Vec4d(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	Vec4d() : x(0), y(0), z(0), w(0) {};
	~Vec4d() {};
	double& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	double x;
	double y;
	double z;
	double w;
};


class Vec4 {
public:
	Vec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {};
	Vec4() : x(0), y(0), z(0), w(0) {};
	~Vec4() {};
	int& operator[](int i) {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	int x;
	int y;
	int z;
	int w;
};

class Rect {
public:
	Rect(float xMin, float yMin, float xMax, float yMax) 
		: xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax) {};
	Rect() : xMin(0), yMin(0), xMax(0), yMax(0) {};
	~Rect() {};
	float& operator[](int i) {
		switch (i)
		{
		case 0:
			return xMin;
		case 1:
			return yMin;
		case 2:
			return xMax;
		case 3:
			return yMax;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};

class Rectd {
public:
	Rectd(double xMin, double yMin, double xMax, double yMax)
		: xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax) {};
	Rectd() : xMin(0), yMin(0), xMax(0), yMax(0) {};
	~Rectd() {};
	double& operator[](int i) {
		switch (i)
		{
		case 0:
			return xMin;
		case 1:
			return yMin;
		case 2:
			return xMax;
		case 3:
			return yMax;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	double xMin;
	double yMin;
	double xMax;
	double yMax;
};

class Recti {
public:
	Recti(int xMin, int yMin, int xMax, int yMax)
		: xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax) {};
	Recti() : xMin(0), yMin(0), xMax(0), yMax(0) {};
	~Recti() {};
	int& operator[](int i) {
		switch (i)
		{
		case 0:
			return xMin;
		case 1:
			return yMin;
		case 2:
			return xMax;
		case 3:
			return yMax;
		default:
			printf("invalid index!\n");
			break;
		}
	};
private:
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

#endif
