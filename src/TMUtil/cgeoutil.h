#ifndef CGeoUtil_H
#define CGeoUtil_H

#include <math.h>
//#include <array>
#include "tmtools.h"
	/**
	 * @author myl 1564496396@qq.com
	 * @class CGeoUtil
	 * @brief 空间工具�?
	 * @details 提供用于空间计算的各种静态接口、常变量
	 */
	class CGeoUtil
	{
	public:
		/**
		 * @brief 瓦片编码
		 */
		enum TileIndex { formTop2Bottom, formBottom2Top };

		enum  Proj { WGS84, CGCS2000, WebMecator, Cartesian, VectorTile };

		//using Vec2 = std::array <double, 2>;
		//using Vec3 = std::array <double, 2>;

		/* 静态常�? */
		static const double PI;
		static const double PI_2;
		static const double PI_4;

		static const double WGS_84_RADIUS_EQUATOR;
		static const double WGS_84_RADIUS_POLAR;
		static const double Web_Mecator_R;
		static const double CAMERA_DISTANCE; // 缩放等级�?20 级时，相机的地表高度，参考《图新地球�?

		/* 静态变�?  */
		static double _radiusEquator;
		static double _radiusPolar;
		static double _eccentricitySquared;

		/* 静态空间接�? */
		static void lonLatHeight2XYZ(double longitude, double latitude, double height, double& X, double& Y, double& Z);
		static void XYZToLatLongHeight(double X, double Y, double Z, double& latitude, double& longitude, double& height);
		static void WebMercator2lonLat(double X, double Y, double& latitude, double& longitude);
		static void WebMercator2XYZ(double x, double y, double& X, double& Y, double& Z);
		static void lonLat2WebMercator(double latitude, double longitude, double& X, double& Y);
		static void getTileLeftTop(int zoom_level, int tile_col, int tile_row, double& mecatorX, double& mecatorY, TileIndex index = formTop2Bottom);
		static void getTileCenter(int zoom_level, int tile_col, int tile_row, double& X, double& Y, double& Z, TileIndex index = formTop2Bottom);
		
		static void num2deg(int xtile, int ytile, int zoom, double* p);
		static void deg2num(double lat_deg, double lon_deg, int zoom, int* p);
		static void webMecator2num(double webMecatorX, double webMecatorY, int zoom, Vec2i& index);

		static void mvt2LonLat(int zoom_level, int tile_col, int tile_row, int x, int y, double& latitude, double& longitude, int size = 4096, TileIndex index = formTop2Bottom);
		
		static double computeTileLevel(double height);
		static bool inFrustum(Frustum frustum, Vec3d pt);

		static void screen2World(Vec3d screen, Vec3d& world);

		static Vec3d lineSegment_WGS84Ellipsoid_intersection(Vec3d p0, Vec3d p1,
			Vec3d center = Vec3d(), double a = CGeoUtil::WGS_84_RADIUS_EQUATOR, double b = CGeoUtil::WGS_84_RADIUS_POLAR);

	};


#endif