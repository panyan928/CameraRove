#pragma once
#include <map>
#include <vector>
#include <string>
#include "../TMUtil/tmtools.h"
using namespace std;

class CPolygon
{
public:
	CPolygon();
	vector<Vec3> pts;
	map<string, string> properties;
	bool isInPolygon(Vec3 p);

};

