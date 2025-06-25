#include "CPolygon.h"
#include "../TMUtil/cgeoutil.h"
CPolygon::CPolygon()
{
}

bool CPolygon::isInPolygon(Vec3 lonlat) {
    {
        int nCross = 0;
        double x, y;
        CGeoUtil::lonLat2WebMercator(lonlat[1], lonlat[0], x, y);
        for (int i = 0; i < pts.size(); i++) {
            Vec3 p1 = pts.at(i);
            Vec3 p2 = pts.at((i + 1) % pts.size());

            double x1, y1, x2, y2;
            CGeoUtil::lonLat2WebMercator(p1[1], p1[0], x1, y1);
            CGeoUtil::lonLat2WebMercator(p2[1], p2[0], x2, y2);
            double xmin = x1 < x2 ? x1 : x2;
            double xmax = x1 > x2 ? x1 : x2;

            if (x1 == x2)
                continue;
            if (x < xmin)
                continue;
            if (x >= xmax)
                continue;
            double tmpY = (x - x1) * (y2 - y1)
                / (x2 - x1) + y1;
            if (tmpY > y)
                nCross++;
        }
        return (nCross % 2 == 1);
    }
}