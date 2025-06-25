#pragma once
#include <map>
#include <vector>
#include "CPolygon.h"
#include "../TMUtil/tmtools.h"
#include "json/json.h"

using namespace std;
class JSONLayer
{
public:
	JSONLayer(std::string path);
	void loadLayer(std::string path);
	CPolygon* isinPolygon(Vec3 pt);
	vector<CPolygon*> polygons;
	map<string,string> properties;
};

