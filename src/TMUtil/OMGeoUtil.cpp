#include "OMGeoUtil.h"

void OMGeoUtil::WebMecator2Lonlat(Vec2d proj, Vec2d& lonlat)
{
	CGeoUtil::WebMercator2lonLat(proj[0], proj[1], lonlat[1], lonlat[2]);
}

void OMGeoUtil::Lonlat2WebMecator(Vec2d lonlat, Vec2d& proj)
{
	CGeoUtil::lonLat2WebMercator(lonlat[1], lonlat[0], proj[0], proj[1]);
}

void OMGeoUtil::lonLatHeight2XYZ(Vec3d lonlat, Vec3d& proj)
{
	CGeoUtil::lonLatHeight2XYZ(lonlat[0] * CGeoUtil::PI / 180, lonlat[1] * CGeoUtil::PI / 180, lonlat[2], proj[0], proj[1], proj[2]);
}

void OMGeoUtil::XYZ2lonLatHeight(Vec3d& lonlat, Vec3d proj)
{
	CGeoUtil::XYZToLatLongHeight(proj[0], proj[1], proj[2], lonlat[1], lonlat[0], lonlat[2]);
	lonlat[1] = lonlat[1] * 180 / CGeoUtil::PI;
	lonlat[0] = lonlat[0] * 180 / CGeoUtil::PI;
}

void OMGeoUtil::deg2num(Vec3d lonlat, Vec2i& index, int zoom)
{
	int p[2]={0};
	CGeoUtil::deg2num(lonlat[1], lonlat[0], zoom, p);
	index[0] = p[0];
	index[1] = p[1];
}

