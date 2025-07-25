#pragma once
#include "cgeoutil.h"
#include "tmtools.h"

class OMGeoUtil
{
public:
	static void WebMecator2Lonlat(Vec2d proj, Vec2d& lonlat);
	static void Lonlat2WebMecator(Vec2d lonlat, Vec2d& proj);

	static void lonLatHeight2XYZ(Vec3d lonlat, Vec3d& proj);
	static void XYZ2lonLatHeight(Vec3d& lonlat, Vec3d proj);

	static void deg2num(Vec3d lonlat, Vec2i& index, int zoom);

	template <typename T>
	static T* projectLonlat2XYZ(T* input, int size);

	template <typename T>
	static T* projectLonlatHeight2XYZ(T* input, int size);

	template<typename T>
	static T* projectLonlat2Mecator(T* input, int size);
};

template<typename T>
inline T* OMGeoUtil::projectLonlat2XYZ(T* input, int size)
{
	if (input == 0x00 || size <= 0)
		return 0x00;

	T* output = new T[size * 3];
	double x, y, z;
	for (int i = 0; i < size; i++) {
		CGeoUtil::lonLatHeight2XYZ(input[2 * i] * CGeoUtil::PI / 180, input[2 * i + 1] * CGeoUtil::PI / 180, 0, x, y, z);
		output[3 * i] = x;
		output[3 * i + 1] = y;
		output[3 * i + 2] = z;
	}

	delete[] input;
	input = 0x00;

	return output;
}

template<typename T>
inline T* OMGeoUtil::projectLonlatHeight2XYZ(T* input, int size)
{
	if (input == 0x00 || size <= 0)
		return 0x00;

	T* output = new T[size * 3];
	double x, y, z;
	for (int i = 0; i < size; i++) {
		CGeoUtil::lonLatHeight2XYZ(input[3 * i] * CGeoUtil::PI / 180, input[3 * i + 1] * CGeoUtil::PI / 180, 2 * input[3 * i + 2], x, y, z);
		output[3 * i] = x;
		output[3 * i + 1] = y;
		output[3 * i + 2] = z;
	}

	delete[] input;
	input = 0x00;

	return output;
}

template<typename T>
inline T* OMGeoUtil::projectLonlat2Mecator(T* input, int size)
{
	if (input == 0x00 || size <= 0)
		return 0x00;

	T* output = new T[size * 3];
	double x, y;
	for (int i = 0; i < size; i++) {
		CGeoUtil::lonLat2WebMercator(input[2 * i + 1], input[2 * i], x, y);
		//CGeoUtil::lonLatHeight2XYZ(input[2 * i] * CGeoUtil::PI / 180, input[2 * i + 1] * CGeoUtil::PI / 180, 0, x, y, z);
		output[3 * i] = x;
		output[3 * i + 1] = y;
		output[3 * i + 2] = 0.1;
	}

	delete[] input;
	input = 0x00;

	return output;
}