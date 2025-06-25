#include "JSONLayer.h"
#include <iostream>
//#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <string.h>

JSONLayer::JSONLayer(std::string path)
{
    loadLayer(path);
}

void JSONLayer::loadLayer(std::string path)
{
    /*std::ifstream ifs;
    ifs.open(path);
    if (!ifs.is_open())
    {
        cout << "Error opening map.json\n";
    }*/
	long len=0;//文件长度
	char* content;//文件内容
	FILE* ifs = fopen(path.c_str(), "r");
	if (!ifs)
	{
		cout << "Error opening "<<path<<endl;
		//fclose(ifs);
	}
	else{
		cout << "ok opening "<<path<<endl;
	}
	fseek(ifs,0,SEEK_END);
	len = ftell(ifs);
	fseek(ifs, 0, SEEK_SET);
	content = (char*)malloc(len+1);
	memset(content, 0, len+1);
	fread(content,1,len, ifs);
	fclose(ifs);

    Json::Value mapRoot;
    Json::Reader jsonReader;
    if (!jsonReader.parse(content, mapRoot))
    {
        cout << "Error reading jiangxi.geojson\n";
        free(content);
    }
    else{
		cout << "ok reading jiangxi.geojson\n";
	}
    int featureSize = mapRoot["features"].size();
    for (int i = 0; i < featureSize; i++)
    {
        Json::Value feature = mapRoot["features"][i];
        Json::Value properties = feature["properties"];
        Json::Value geometry = feature["geometry"];

        Json::Value::Members proName = properties.getMemberNames();

        CPolygon* polygon = new CPolygon();
        for (int j = 0; j < proName.size();j++) {
         string name = proName[j];
         string value;
         ostringstream ost_temp;//ost_temp.str("");
            if (properties[name].type() == Json::stringValue)
            {
                value = properties[name].asString();
            }
            else if (properties[name].type() == Json::realValue)
            {
                double tmp = properties[name].asDouble();
                //value = to_string(tmp);
                ost_temp << (tmp);
                value = ost_temp.str();
            }
            else if (properties[name].type() == Json::intValue)
            {
                int tmp = properties[name].asInt();
                //value = to_string(tmp);
                ost_temp << (tmp);
                value = ost_temp.str();
            }
            polygon->properties.insert(pair<string, string>(proName[j], value));
        }
        for (int j = 0; j < geometry["coordinates"].size(); j++)
        {
            this->polygons.push_back(polygon);
            Json::Value pts;
            if (!geometry["type"].compare("MultiPolygon"))
            {
                pts = geometry["coordinates"][0][0];
            }
            else
            {
                pts = geometry["coordinates"][0];
            }
            for (int q = 0; q < pts.size(); q++) {
                double x = pts[q][0].asDouble();
                double y = pts[q][1].asDouble();
                polygon->pts.push_back(Vec3(x, y, 0));
            }
        }
    }
}

CPolygon* JSONLayer::isinPolygon(Vec3 pt)
{
    for (int i = 0; i < this->polygons.size(); i++)
    {
        if (this->polygons[i]->isInPolygon(pt))
        {
            return polygons[i];
        }
    }
    return 0x00;
}
