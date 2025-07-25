#include "cgeoutil.h"
/*
    * 静态常变量 定义初始�?
    * 不能放在.h中，否则会造成重定�?
    */
const double CGeoUtil::WGS_84_RADIUS_EQUATOR = 6378137.0;
const double CGeoUtil::WGS_84_RADIUS_POLAR = 6356752.3142;
const double CGeoUtil::Web_Mecator_R = 6378137.0;
const double CGeoUtil::CAMERA_DISTANCE = 270.0;

double CGeoUtil::_radiusEquator = WGS_84_RADIUS_EQUATOR;
double CGeoUtil::_radiusPolar = WGS_84_RADIUS_POLAR;
double flattening = (CGeoUtil::_radiusEquator - CGeoUtil::_radiusPolar) / CGeoUtil::_radiusEquator;
double CGeoUtil::_eccentricitySquared = 2 * flattening - flattening * flattening;


const double CGeoUtil::PI = 3.14159265358979323846;
const double CGeoUtil::PI_2 = 1.57079632679489661923;
const double CGeoUtil::PI_4 = 0.78539816339744830962;


/************* 静态接�?******************/

/**
 * @brief 经纬高转空间坐标�?xyz
 * @details 不同的椭球参数计算会得到不同的结�?默认�?WGS84 椭球参数
 * @param longitude 经度
 * @param latitude 纬度
 * @param height 高度
 * @param X
 * @param Y
 * @param Z 输出的空间坐�?
 */
void CGeoUtil::lonLatHeight2XYZ(double longitude, double latitude, double height, double& X, double& Y, double& Z)
{
    // for details on maths see http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif
    double sin_latitude = sin(latitude);
    double cos_latitude = cos(latitude);
    double N = _radiusEquator / sqrt(1.0 - _eccentricitySquared * sin_latitude * sin_latitude);
    X = (N + height) * cos_latitude * cos(longitude);
    Y = (N + height) * cos_latitude * sin(longitude);
    Z = (N * (1 - _eccentricitySquared) + height) * sin_latitude;
}

/**
 * @brief 空间XYZ坐标系转经纬�?
 * @details 不同的椭球参数计算会得到不同的结�?默认�?WGS84 椭球参数
 * @param X
 * @param Y
 * @param Z 输入的空间坐�?
 * @param longitude 经度
 * @param latitude 纬度，弧度制
 * @param height 高度
 */
void CGeoUtil::XYZToLatLongHeight(double X, double Y, double Z, double& latitude, double& longitude, double& height)
{
    // handle polar and center-of-earth cases directly.
    if (X != 0.0)
        longitude = atan2(Y, X);
    else
    {
        if (Y > 0.0)
            longitude = PI_2;
        else if (Y < 0.0)
            longitude = -PI_2;
        else
        {
            // at pole or at center of the earth
            longitude = 0.0;
            if (Z > 0.0)
            { // north pole.
                latitude = PI_2;
                height = Z - _radiusPolar;
            }
            else if (Z < 0.0)
            { // south pole.
                latitude = -PI_2;
                height = -Z - _radiusPolar;
            }
            else
            { // center of earth.
                latitude = PI_2;
                height = -_radiusPolar;
            }
            return;
        }
    }

    // http://www.colorado.edu/geography/gcraft/notes/datum/gif/xyzllh.gif
    double p = sqrt(X * X + Y * Y);
    double theta = atan2(Z * _radiusEquator, (p * _radiusPolar));
    double eDashSquared = (_radiusEquator * _radiusEquator - _radiusPolar * _radiusPolar) /
        (_radiusPolar * _radiusPolar);

    double sin_theta = sin(theta);
    double cos_theta = cos(theta);

    latitude = atan((Z + eDashSquared * _radiusPolar * sin_theta * sin_theta * sin_theta) /
        (p - _eccentricitySquared * _radiusEquator * cos_theta * cos_theta * cos_theta));

    double sin_latitude = sin(latitude);
    double N = _radiusEquator / sqrt(1.0 - _eccentricitySquared * sin_latitude * sin_latitude);

    height = p / cos(latitude) - N;
}


/**
 * @brief 经纬度转web墨卡�?
 * @param longitude 经度
 * @param latitude 纬度
 * @param X
 * @param Y 输出的平面坐�?
 */
void CGeoUtil::lonLat2WebMercator(double latitude, double longitude, double& X, double& Y)
{
    double x = longitude * 20037508.34 / 180;
    double y = log(tan((90 + latitude) * PI / 360)) / (PI / 180);
    y = y * 20037508.34 / 180;
    X = x;
    Y = y;
}

/**
 * @brief 获取瓦片左上角坐�?
 * @details 通过输入的瓦片编号参�? 计算瓦片左上角的web墨卡托坐�?
 * @param zoom_level
 * @param tile_col
 * @param tile_row
 * @param mecatorX
 * @param mecatorY 输出坐标
 * @param index 瓦片编号方法
 */
void CGeoUtil::getTileLeftTop(int zoom_level, int tile_col, int tile_row, double& mecatorX, double& mecatorY, TileIndex index)
{
    double span = 2 * PI * Web_Mecator_R / pow(2, zoom_level);
    mecatorX = -PI * Web_Mecator_R + tile_col * span;
    if (index == formTop2Bottom)
        mecatorY = PI * Web_Mecator_R - tile_row * span;
    else if (index == formBottom2Top) {
        mecatorY = -PI * Web_Mecator_R + tile_row * span + span;
    }
    /* test */
    /*
    double lon, lat;
    WebMercator2lonLat(mecatorX, mecatorY, lon, lat);

    double lonlat[2];
    num2deg(tile_col, tile_row, zoom_level, lonlat);

    int a = 0;*/
}

/**
 * @brief 获取瓦片中心坐标，以 XYZ 形式返回
 * @param zoom_level
 * @param tile_col
 * @param tile_row
 * @param X
 * @param Y
 * @param Z
 * @param index
 */
void CGeoUtil::getTileCenter(int zoom_level, int tile_col, int tile_row, double& X, double& Y, double& Z, TileIndex index)
{
    double mecatorX = 0;
    double mecatorY = 0;
    getTileLeftTop(zoom_level, tile_col, tile_row, mecatorX, mecatorY, index);
    double span = 2 * PI * Web_Mecator_R / pow(2, zoom_level);
    mecatorX += 0.5 * span;
    mecatorY -= 0.5 * span;
    WebMercator2XYZ(mecatorX, mecatorY, X, Y, Z);
}



/**
 * @brief web墨卡托坐标转经纬�?
 * @param X
 * @param Y web墨卡托坐�?
 * @param latitude
 * @param longitude 输出的经纬度坐标，角度制
 */
void CGeoUtil::WebMercator2lonLat(double X, double Y, double& latitude, double& longitude)
{
    double x = X / 20037508.34 * 180;
    double y = Y / 20037508.34 * 180;
    y = 180 / PI * (2 * atan(exp(y * PI / 180)) - PI / 2);
    longitude = x;
    latitude = y;
}

/**
 * @brief web墨卡托坐标转空间直角坐标�?
 * @param x
 * @param y web墨卡托坐�?
 * @param X
 * @param Y
 * @param Z 输出的空间直角坐标系
 */
void CGeoUtil::WebMercator2XYZ(double x, double y, double& X, double& Y, double& Z)
{
    double lon = 0.0, lat = 0.0;
    WebMercator2lonLat(x, y, lat, lon);
    lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, X, Y, Z);
}

/**
 * @brief 瓦片索引转经纬度
 * @param xtile
 * @param ytile
 * @param zoom  瓦片索引
 * @param p     输出经纬度坐标(经度，纬度)，角度制
 */
void CGeoUtil::num2deg(int xtile, int ytile, int zoom, double* p) {
    int n = pow(2, zoom);
    double lon_deg = xtile * 1.0 / n * 360.0 - 180.0;
    double lat_rad = atan(sinh(PI * (1 - 2.0 * ytile / n)));
    double lat_deg = lat_rad * 180 / PI;
    p[0] = lon_deg;
    p[1] = lat_deg;
}

/**
* @brief 经纬度转瓦片索引
* @param lat_deg 纬度
* @param lon_deg 经度
* @param zoom  缩放等级
* @param p     输出瓦片索引 (列号，行�?
*/
void CGeoUtil::deg2num(double lat_deg, double lon_deg, int zoom, int* p) {
    double lat_rad = lat_deg * PI / 180;
    int n = pow(2, zoom);
    int xtile = int((lon_deg + 180.0) / 360.0 * n);
    int ytile = int((1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / PI) / 2.0 * n);
    p[0] = xtile;
    p[1] = ytile;
    p[1] = n - ytile - 1;
}

/**
* @brief 瓦片索引转经纬度
* @param xtile
* @param ytile
* @param zoom  瓦片索引
* @param p     输出经纬度坐标(经度，纬度)，角度制
*/
void CGeoUtil::webMecator2num(double webMecatorX, double webMecatorY, int zoom, Vec2i& index)
{
    double span = 2 * CGeoUtil::PI * CGeoUtil::Web_Mecator_R / pow(2, zoom);
    double begin = -CGeoUtil::PI * CGeoUtil::Web_Mecator_R;
    index[0] = (webMecatorX - begin) / span;
    index[1] = (webMecatorY - begin) / span;
    //index[1] = pow(2, zoom) - (webMecatorY - begin) / span - 1;
}

/**
 * @brief mvt瓦片坐标转经纬度地理坐标
 * @details mvt 矢量瓦片坐标系属于屏幕坐标，从左下角起算，右上角�?(size-1,size-1), size 默认�?4096
 * @param zoom_level
 * @param tile_col
 * @param tile_row 瓦片参数
 * @param x
 * @param y mvt坐标
 * @param latitude
 * @param longitude 输出的经纬度坐标，角度制
 * @param size mvt坐标大小, 默认最大值为4096
 * @param index 瓦片编号方法
 */
void CGeoUtil::mvt2LonLat(int zoom_level, int tile_col, int tile_row, int x, int y, double& latitude, double& longitude, int size, TileIndex index)
{
    double leftTopX = 0.0; double leftTopY = 0.0;
    getTileLeftTop(zoom_level, tile_col, tile_row, leftTopX, leftTopY, index);
    double span = 2 * PI * Web_Mecator_R / pow(2, zoom_level);
    double out_x = 0; double out_y = 0;
    out_x = leftTopX + x * 1.0 / size * span;
    //if(index == formTop2Bottom)
    out_y = leftTopY - span + y * 1.0 / size * span;
    //else if(index == formBottom2Top)
    //out_y = leftTopY - span + y * 1.0 / size * span;
    WebMercator2lonLat(out_x, out_y, latitude, longitude);
}

double CGeoUtil::computeTileLevel(double height)
{
    double level = 0.0;
    if (height > 0) {
        level = log(260.0 / height) / log(2) + 19;
    }
    else {
        level = 2;
    }
    return level;
}

bool CGeoUtil::inFrustum(Frustum frustum, Vec3d pt)
{

    return false;
}

void CGeoUtil::screen2World(Vec3d screen, Vec3d& world)
{

}

/**
* @brief 直线和椭球求�?
* @details 计算给定输入�?p0和p1 的连线与位于center位置
* 的椭球交点，默认椭球参数为WGS84椭球
* @param p0
* @param p1       确定直线的两�?
* @param center   椭球中心
* @param a
* @param b 椭球参数，长半轴和短半轴
* @return Vec3d 返回近交点坐标，没有交点返回(0,0,0)
*/
Vec3d CGeoUtil::lineSegment_WGS84Ellipsoid_intersection(Vec3d p0, Vec3d p1, Vec3d center, double a, double b)
{
    double x0 = p0[0], y0 = p0[1], z0 = p0[2];
    double x1 = p1[0], y1 = p1[1], z1 = p1[2];
    double cx = center[0], cy = center[1], cz = center[2];
    double m = x1 - x0, n = y1 - y0, p = z1 - z0;
    double A = (m * m + n * n) / (a * a) + p * p / (b * b);
    double B = 2 * ((m * (x0 - cx) + n * (y0 - cy)) / (a * a) + p * (z0 - cz) / (b * b));
    double C = ((x0 - cx) * (x0 - cx) + (y0 - cy) * (y0 - cy)) / (a * a) + (z0 - cz) * (z0 - cz) / (b * b) - 1;
    double test = B * B - 4.0 * A * C;
    if (test >= 0.0)
    {
        double t0 = (-B - sqrt(test)) / (2.0 * A);
        double t1 = (-B + sqrt(test)) / (2.0 * A);
        Vec3d lineNormal(m, n, p);

        //Vec3d hitp = lineNormal * t0 + p0;
        // 返回近交�?
        Vec3d hitp;
        hitp[0] = t0 * m + p0[0];
        hitp[1] = t0 * n + p0[1];
        hitp[2] = t0 * p + p0[2];
        return hitp;
    }
    // 没有交点
    return Vec3d(0, 0, 0);
}
