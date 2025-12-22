#include "CVectorTileLayer.h"
//#include "sqlite/sqlite3.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdio>  // 用于 snprintf
#include <iostream>
//#include "memory_monitor.h"
#include <cmath>   // 用于 ceil, sqrt

CVectorTileLayer::CVectorTileLayer()
{
	_path = "";
	_useDatFile = false;
	_vectorDatReader = nullptr;
}

CVectorTileLayer::CVectorTileLayer(string path)
{
	_path = path;
	_useDatFile = false;
	_vectorDatReader = nullptr;
	initDatReader();
}

CVectorTileLayer::~CVectorTileLayer()
{
	if (_vectorDatReader) {
		delete _vectorDatReader;
		_vectorDatReader = nullptr;
	}
}

void CVectorTileLayer::setAnnotation(int anno, string label)
{
	this->_anno = anno;
	this->_label = label;
}

int CVectorTileLayer::getAnnotation()
{
	return _anno;
}

int CVectorTileLayer::draw(Recti bounds, int zoom)
{
#if 0
	float r, g, b;
	CStyle* style = getOrCreateStyle();
	if (bounds[0] > bounds[2])
		bounds[2] += pow(2, zoom);
	//int count = 0;
	for (int i = bounds[0]; i <= bounds[2]; i++) {
		for (int j = bounds[3]; j <= bounds[1]; j++) {
			//count++;

			if (geometryType().compare("polygon") == 0) {
				float* pts;
				int size1;
				string path;
				path = _path + to_string(zoom) + "." + to_string(j) + "." + to_string((i % (int)pow(2, zoom))) + ".dat";
				pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::Proj::WGS84, 2, size1);
				style->fill(0)->getColor()->colorRGB(r, g, b);
				Color color(255 * r, 255 * g, 255 * b, 255);
				int state = openglEngine::OpenGLRenderEngine::drawTriangles<float>(pts, color, size1);
				if (pts) {
					delete[] pts;
					pts = 0x00;
				}
			}
			if (geometryType().compare("polyline") == 0) {
				int size1;
				int ptsCount = 0;
				int stopsCount = 0;
				string stopsPath, ptsPath;
				ptsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".dat";
				stopsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".stops";
				int* stops = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(stopsPath.c_str(), ptsCount, stopsCount);
				float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(ptsPath.c_str(), CGeoUtil::Proj::WGS84, 2, size1);

				int i = 0;
				while (style->stroke(i)) {
					style->stroke(i)->getColor()->colorRGB(r, g, b);
					Color color(255 * r, 255 * g, 255 * b, 255);
					if (style->stroke(i)->dashArray().size())
						openglEngine::OpenGLRenderEngine::drawLines(pts, stops, stopsCount, color, style->stroke(i)->width(), 1);
					//else
					   // openglEngine::OpenGLRenderEngine::drawLines(pts, stops, stopsCount, color, style->stroke(i)->width(), 0);
					i++;
				}


				if (pts != 0x00)
					delete[] pts;
				if (stops != 0x00)
					delete[] stops;

				pts = 0x00; stops = 0x00;
			}
			if (geometryType().compare("point") == 0) {
				int ptsCount = 0;
				int stopsCount = 0;
				string stopsPath, ptsPath, annoPath;
				ptsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".vertice";
				stopsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".stop";
				annoPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".anno";

				float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(ptsPath.c_str(), CGeoUtil::Proj::WGS84, 2, ptsCount);
				int* stops = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(stopsPath.c_str(), ptsCount, stopsCount);
				string buffer = openglEngine::OpenGLFileEngine::getStringFromBinary<int>(annoPath.c_str());

				openglEngine::OpenGLRenderEngine::drawAnnotations(pts, ptsCount, buffer, stops, Color(255, 0, 0, 0), _render);
				openglEngine::OpenGLRenderEngine::drawSymbols<float>(pts, ptsCount, "H:/trainer_map/data/parking.png", _render);
				//openglEngine::OpenGLRenderEngine::drawAnnotations()

				if (pts != 0x00)
					delete[] pts;
				if (stops != 0x00)
					delete[] stops;

				pts = 0x00; stops = 0x00;
			}
		}
	}
#endif
	//cout << "tileCount: " << count << endl;
	return 0;
}

int CVectorTileLayer::draw(Recti bounds, int zoom, BufferManager* manager)
{
#if 0
	count = 0;

	allCount = 0;

	float r, g, b;
	CStyle* style = getOrCreateStyle();
	if (bounds[0] > bounds[2])
		bounds[2] += pow(2, zoom);
	for (int i = bounds[0]; i <= bounds[2]; i++) {
		for (int j = bounds[3]; j <= bounds[1]; j++) {
			allCount++;
			if (geometryType().compare("polygon") == 0) {
				drawPolygon(zoom, i, j, manager, 1);
			}
			if (geometryType().compare("polyline") == 0) {
				drawPolyline(zoom, i, j, manager, 1);
			}
			if (geometryType().compare("point") == 0) {
				//drawPoint(zoom, i, j, manager);
				if (_anno)
					drawAnnotation(zoom, i, j, manager, 1);
				else
					drawPoint(zoom, i, j, manager, 1);
			}
			//if (geometryType().compare("polyline") == 0) {
			//    int size1;
			//    int ptsCount = 0;
			//    int stopsCount = 0;
			//    string stopsPath, ptsPath;
			//    ptsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".dat";
			//    stopsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".stops";
			//    int* stops = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(stopsPath.c_str(), ptsCount, stopsCount);
			//    float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(ptsPath.c_str(), CGeoUtil::Proj::WGS84, 2, size1);

			//    int i = 0;
			//    while (style->stroke(i)) {
			//        style->stroke(i)->getColor()->colorRGB(r, g, b);
			//        Color color(255 * r, 255 * g, 255 * b, 255);
			//        if (style->stroke(i)->dashArray().size())
			//            openglEngine::OpenGLRenderEngine::drawLines(pts, stops, stopsCount, color, style->stroke(i)->width(), 1);
			//        //else
			//           // openglEngine::OpenGLRenderEngine::drawLines(pts, stops, stopsCount, color, style->stroke(i)->width(), 0);
			//        i++;
			//    }


			//    if (pts != 0x00)
			//        delete[] pts;
			//    if (stops != 0x00)
			//        delete[] stops;

			//    pts = 0x00; stops = 0x00;
			//}

		   /* if (geometryType().compare("point") == 0) {
				int ptsCount = 0;
				int stopsCount = 0;
				string stopsPath, ptsPath, annoPath;
				ptsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".vertice";
				stopsPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".stop";
				annoPath = _path + to_string(zoom) + "." + to_string(j) + "." + to_string(i) + ".anno";

				float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(ptsPath.c_str(), CGeoUtil::Proj::WGS84, 2, ptsCount);
				int* stops = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(stopsPath.c_str(), ptsCount, stopsCount);
				string buffer = openglEngine::OpenGLFileEngine::getStringFromBinary<int>(annoPath.c_str());

				openglEngine::OpenGLRenderEngine::drawAnnotations(pts, ptsCount, buffer, stops, Color(255, 0, 0, 0), _render);
				openglEngine::OpenGLRenderEngine::drawSymbols<float>(pts, ptsCount, "H:/trainer_map/data/mbtiles-jiangxi/symbols/bus.png", _render);
				openglEngine::OpenGLRenderEngine::drawAnnotations()

				if (pts != 0x00)
					delete[] pts;
				if (stops != 0x00)
					delete[] stops;

				pts = 0x00; stops = 0x00;
			}*/
		}
	}
#endif
	//cout << allCount << " " << count << endl;
	return 0;
}

int CVectorTileLayer::draw(vector<Vec3i> tiles, int zoom, BufferManager* manager, int crowdLevel)
{
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;

	//clock_t begin = clock();
	_amount = 0;

	count = 0;

	allCount = 0;

	pierce = 0;

	float r=0.0, g=0.0, b=0.0;
	CStyle* style = getOrCreateStyle();
    if (style->fill()==0x00)
	{
		int q = 0;
	}
	int len = tiles.size();
	for (int k = 0; k < len; k++) {
		allCount++;
		int zoom = tiles[k][0];
		int i = tiles[k][1];
		int j = tiles[k][2];

		if (geometryType().compare("polygon") == 0) {
			drawPolygon(zoom, i, j, manager, 1);
		}
		if (geometryType().compare("polyline") == 0) {
			drawPolyline(zoom, i, j, manager, 1);
		}
		if (geometryType().compare("point") == 0) {
			//drawPoint(zoom, i, j, manager);
			if (_anno)
				drawAnnotation(zoom, i, j, manager, 1);
			else
				drawPoint(zoom, i, j, manager, 1);
		}
	}

	//cout << allCount << " " << count << endl;
	return 0;
}

int CVectorTileLayer::addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager) {
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;
	if (zoom < 0)
		return -1;
	if (!_useDatFile || !_vectorDatReader) {
		//std::cout << "[VectorTileLayer] addBuffer DAT unavailable layer=" << layerName()
		//	<< " path=" << _path << std::endl;
		return -1;
	}

	// 预获取常用值，避免重复调用
	string layerNameStr = layerName();
	string geometryTypeStr = geometryType();

	char tileIndexBuf[64];
	for (auto job : tiles)
	{
		int normalizedCol = normalizeColumn(job[1], job[0]);
		snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", job[0], job[2], normalizedCol);
		string tileIndex(tileIndexBuf);
		string level2Index = tileIndex + "." + layerNameStr;

		if (manager->getFrom2LevelBuffer(level2Index))
		{
			continue;
		}

		if (_useDatFile && _vectorDatReader)
		{
			bool hasData = false;
			int datX = 0, datY = 0;
			if (geometryTypeStr.compare("polygon") == 0 || geometryTypeStr.compare("polyline") == 0 || (!geometryTypeStr.compare("point") && !_anno))
			{
				hasData = resolveDatCoords(job[0], normalizedCol, job[2], VECTOR_TYPE_VERTICE, datX, datY);
			}
			else if (geometryTypeStr.compare("point") == 0 && _anno)
			{
				int datXVert = 0, datYVert = 0;
				int datXAnno = 0, datYAnno = 0;
				bool hasVert = resolveDatCoords(job[0], normalizedCol, job[2], VECTOR_TYPE_VERTICE, datXVert, datYVert);
				bool hasAnno = resolveDatCoords(job[0], normalizedCol, job[2], VECTOR_TYPE_ANNO, datXAnno, datYAnno);
				hasData = hasVert || hasAnno;
			}
			if (!hasData)
			{
				continue;
			}
		}

		if (geometryTypeStr.compare("polygon") == 0) {
			addPolygonBuffer(job[0], job[1], job[2], manager);
		}
		else if (geometryTypeStr.compare("polyline") == 0) {
			addPolylineBuffer(job[0], job[1], job[2], manager);
		}
		else if (geometryTypeStr.compare("point") == 0) {
			if (_anno) {
				addAnnotationBuffer(job[0], job[1], job[2], manager);
			}
			else {
				addPointBuffer(job[0], job[1], job[2], manager);
			}
		}
	}

	return 0;
}

int CVectorTileLayer::drawPolygon(int zoom, int col, int row, BufferManager* manager, int draw)
{
	//clock_t t1, t2;
/***************?????????????*********************/
//t1 = clock();
	CStyle* style = getOrCreateStyle();
	float r = 0.0, g = 0.0, b = 0.0;
	if (style->fill(0) == 0x00)
	{
		int q = 0;
	}
	style->fill(0)->getColor()->colorRGB(r, g, b);
	// 根据图层透明度和样式透明度计算最终透明度
	float styleOpacity = style->fill(0)->getColor()->opacity();
	float finalOpacity = styleOpacity * _opacity;
	int alpha = (int)(255 * finalOpacity);
	Color color(255 * r, 255 * g, 255 * b, alpha);

	int normalizedCol = normalizeColumn(col, zoom);
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (!mBuffer) {
		return -1;
	}
	Vertices* vertices = 0x00;
	vertices = mBuffer->vData();
	if (!vertices) {
		//std::cout << "[VectorTileLayer] drawPolygon vertices missing layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}
	count++;
	//else {
	//	string dataIndex = level2Index + ".vertice";
	//	string path = _path + tileIndex + ".vertice";
	//	int size;
	//	float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::Proj::WGS84, 2, size);
	//	if (pts) {
	//		mBuffer = new TMBuffer(BufferType::PolygonBuffer, level2Index);
	//		string dataIndex = level2Index + ".vertices";
	//		vertices = new Vertices(pts, size, dataIndex);
	//		mBuffer->setData(vertices, DataType::Vertice);
	//		manager->insert(2, mBuffer, level2Index);
	//	}
	//	else
	//		pierce++;
	//}
	int state = -1;
	if (vertices) {
		int size_render = vertices->size(); // 顶点数量
		if (size_render <= 0) {
			return -1;
		}
		_amount += size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		if (pts_render && draw) {
			const int floatsPerVertex = 3;
			const size_t totalFloatCount = static_cast<size_t>(size_render) * floatsPerVertex;
			int upLimitLen = 65500;	// OpenGL 单次绘制的最大顶点数
			int times = (size_render % upLimitLen) ? (size_render / upLimitLen + 1) : (size_render / upLimitLen);
			for (int i = 0; i < times; i++) {
				int startVertex = upLimitLen * i;
				if (startVertex >= size_render) {
					break;
				}
				int trueLen = (i == times - 1 && (size_render % upLimitLen)) ? (size_render % upLimitLen) : upLimitLen;
				if (startVertex + trueLen > size_render) {
					trueLen = size_render - startVertex;
				}
				if (trueLen <= 0) {
					break;
				}
				size_t startFloatIndex = static_cast<size_t>(startVertex) * floatsPerVertex;
				if (startFloatIndex >= totalFloatCount) {
					break;
				}
				state = openglEngine::OpenGLRenderEngine::drawTriangles<float>(&pts_render[startFloatIndex], color, trueLen);
			}
		}
	}
	return state;
}

int CVectorTileLayer::drawPolyline(int zoom, int col, int row, BufferManager* manager, int draw)
{
	CStyle* style = getOrCreateStyle();
	float r = 0.0, g = 0.0, b = 0.0;
	int normalizedCol = normalizeColumn(col, zoom);
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (!mBuffer) {
		return -1;
	}
	Vertices* vertices = 0x00;
	Stop* stops = 0x00;
	vertices = mBuffer->vData();
	stops = mBuffer->sData();
	
	if (!vertices || !stops) {
		//std::cout << "[VectorTileLayer] drawPolyline still missing data layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}
	count++;
	/*else {
		string dataIndex = level2Index + ".vertice";
		string stopIndex = level2Index + ".stop";
		if (!vertices) {
			string path = _path + tileIndex + ".vertice";
			int size;
			float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::Proj::WGS84, 2, size);
			if (pts) {
				mBuffer = new TMBuffer(BufferType::PolylineBuffer, level2Index);
				string dataIndex = level2Index + ".vertice";
				vertices = new Vertices(pts, size, dataIndex);
				mBuffer->setData(vertices, DataType::Vertice);
			}
			else {
				if (mBuffer != 0x00)
					delete mBuffer;
				mBuffer = 0x00;
				pierce++;
				return -1;
			}
		}

		if (!stops) {
			string path = _path + tileIndex + ".stop";
			int size, size1;
			int* pts = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(path.c_str(), size1, size);
			if (pts) {
				string dataIndex = level2Index + ".stop";
				stops = new Stop(pts, size, dataIndex);
				mBuffer->setData(stops, DataType::Stop);
			}
			else {
				if (mBuffer != 0x00)
					delete mBuffer;
				mBuffer = 0x00;
				return -1;
			}
		}
		manager->insert(2, mBuffer, level2Index);

	}*/
	int state = -1;
	if (vertices && stops) {
		int size_render = stops->size(); // stops 数组长度
		if (size_render <= 0) {
			return -1;
		}
		_amount += size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		int* stops_render = static_cast<int*>(stops->data());

		if (pts_render && stops_render) {
			int totalVertices = stops_render[size_render - 1];
			if (totalVertices <= 0) {
				return -1;
			}
			_amount += totalVertices;
			if (draw) {
				int i = 0;
				while (style->stroke(i)) {
					style->stroke(i)->getColor()->colorRGB(r, g, b);
					// 根据图层透明度和样式透明度计算最终透明度
					float styleOpacity = style->stroke(i)->getColor()->opacity();
					float finalOpacity = styleOpacity * _opacity;
					int alpha = (int)(255 * finalOpacity);
					Color color(255 * r, 255 * g, 255 * b, alpha);
					vector<float> dashArray = style->stroke(i)->dashArray();

					// 使用新的OpenGL ES 1.1兼容函数，传递dashArray向量
					//state = openglEngine::OpenGLRenderEngine::drawLines(pts_render, stops_render, size_render, color, style->stroke(i)->width(), dashArray);
					if (isDashArrayValid(dashArray))
						state = openglEngine::OpenGLRenderEngine::drawLines(pts_render, stops_render, size_render, color, style->stroke(i)->width(), 1);
					else
						state = openglEngine::OpenGLRenderEngine::drawLines(pts_render, stops_render, size_render, color, style->stroke(i)->width(), 0);
					i++;
				}
			}
		}
	}
	return state;

	/*   if (!mBuffer) {
		   mBuffer = new TMBuffer(BufferType::PolylineBuffer, level2Index);
		   manager->insert(2, mBuffer, level2Index);
	   }
	   else
		   cout << "ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½" << endl;*/

		   //Vertices* vertices = mBuffer->vData();
		   //Stop* stops = mBuffer->sData();
		   //if (!vertices) {
		   //    string path = _path + tileIndex + ".dat";
		   //    int size;
		   //    float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::Proj::WGS84, 2, size);

		   //    string dataIndex = level2Index + ".vertices";
		   //    //if (pts) {
		   //    vertices = new Vertices(pts, size, dataIndex);
		   //    manager->insert(3, vertices, dataIndex);
		   //    mBuffer->setData(vertices, DataType::Vertice);
		   //    //}
		   //    //vertices = new Vertices(pts, size, dataIndex);
		   //    //manager->insert(3, vertices, dataIndex);

		   //    
		   //}

		   //if (!stops) {
		   //    string path = _path + tileIndex + ".stops";
		   //    int size;
		   //    int ptSize;
		   //    int* pts = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(path.c_str(), ptSize, size);

		   //    string dataIndex = level2Index + ".stops";
		   //    //if (pts) {
		   //        stops = new Stop(pts, size, dataIndex);
		   //        manager->insert(3, stops, dataIndex);
		   //        mBuffer->setData(stops, DataType::Stop);
		   //    //}   
		   //}

		   //int size_render = vertices->size();
		   //float* pts_render = static_cast<float*>(vertices->data());
		   //int* stops_render = static_cast<int*>(stops->data());

		   //int i = 0;
		   //while (style->stroke(i)) {
		   //     style->stroke(i)->getColor()->colorRGB(r, g, b);
		   //     Color color(255 * r, 255 * g, 255 * b, 255);
		   //     //if (style->stroke(i)->dashArray().size())
		   //         //openglEngine::OpenGLRenderEngine::drawLines(pts_render, stops_render, size_render, color, style->stroke(i)->width(), 1);
		   //     //else
		   //        // openglEngine::OpenGLRenderEngine::drawLines(pts, stops, stopsCount, color, style->stroke(i)->width(), 0);
		   //     i++;
		   // }
		   //int state = openglEngine::OpenGLRenderEngine::drawLines<float>(pts_render,stops_render, )

	return 0;
}

int CVectorTileLayer::drawPoint(int zoom, int col, int row, BufferManager* manager, int draw)
{
	CStyle* style = getOrCreateStyle();

	int normalizedCol = normalizeColumn(col, zoom);
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (!mBuffer) {
		return -1;
	}

	Vertices* vertices = mBuffer->vData();
	if (!vertices) {
		//std::cout << "[VectorTileLayer] drawPoint vertices missing layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	count++;
	/*else {
		mBuffer = new TMBuffer(BufferType::PointBuffer, level2Index);
		string dataIndex = level2Index + ".vertices";
		string path = _path + tileIndex + ".vertice";
		int size;
		float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::Proj::WGS84, 2, size);
		if (pts) {
			vertices = new Vertices(pts, size, dataIndex);
			mBuffer->setData(vertices, DataType::Vertice);
			manager->insert(2, mBuffer, level2Index);
		}
		else {
			if (mBuffer != 0x00)
				delete mBuffer;
			mBuffer = 0x00;
			pierce++;
			return -1;
		}

	}*/
	int state = -1;
	if (vertices) {
		int size_render = vertices->size();
		if (size_render <= 0) {
			return -1;
		}
		_amount += size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		if (pts_render) {
			//TODO("?????ymbol????????????)
#ifdef WIN32
			string symbolPath = "./../data/mbtiles-jiangxi/symbols/" + layerName() + ".png";
#else//?????????tm3
			string symbolPath = "D:\\mbtiles-jiangxi/symbols/" + layerName() + ".png";
#endif
			if (draw)
				openglEngine::OpenGLRenderEngine::drawSymbols<float>(pts_render, size_render, symbolPath.c_str(), _render);
		}
	}
	return state;
}

int CVectorTileLayer::drawAnnotation(int zoom, int col, int row, BufferManager* manager, int draw)
{
	int normalizedCol = normalizeColumn(col, zoom);
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (!mBuffer) {
		return -1;
	}

	Vertices* vertices = mBuffer->vData();
	Text* annos = mBuffer->tData();
	Stop* stops = mBuffer->sData();
	
	if (!vertices || !annos || !stops) {
		//std::cout << "[VectorTileLayer] drawAnnotation still missing data layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	count++;

	int state = -1;
	if (vertices && annos && stops) {
		int size_render = vertices->size();
		if (size_render <= 0) {
			return -1;
		}
		_amount += 3 * size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		int* stops_render = static_cast<int*>(stops->data());
		if (pts_render && stops_render) {
			string text_render = static_cast<char*>(annos->data());
			if (draw) {
				openglEngine::OpenGLRenderEngine::drawAnnotations(pts_render, size_render,
					text_render, stops_render, getOrCreateStyle(), _render, _anno);
			}
		}
	}
	return state;
}

bool CVectorTileLayer::isDashArrayValid(vector<float>& dash)
{
	int len = dash.size();
	for (int i = 0; i < len; i++)
		if (abs(dash[i]) > 0.0001)
			return true;
	return false;
}


int CVectorTileLayer::addPolygonBuffer(int zoom, int col, int row, BufferManager* manager) {
	// DAT-only 入口校验
	if (!_useDatFile || !_vectorDatReader) {
		//std::cout << "[VectorTileLayer] polygon buffer DAT unavailable layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	int normalizedCol = normalizeColumn(col, zoom);

	// 优化字符串生成
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string layerNameStr = layerName();
	string level2Index = tileIndex + "." + layerNameStr;

	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (mBuffer) {
		return 0;
	}

	Vertices* vertices = 0x00;
	string dataIndex = level2Index + ".vertice";
	int size = 0; // 初始化为0，防止未初始化值导致的问题

	float* pts = nullptr;
	// 从.dat文件读取 (注意 DAT 中 x=行, y=列)
	int datX = 0;
	int datY = 0;
	bool hasData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_VERTICE, datX, datY);
	logDatStatus("polygon.vert", zoom, normalizedCol, row, datX, datY, hasData);
	if (hasData) {
		if (!loadVerticesFromDat(zoom, datX, datY, pts, size)) {
			//	std::cout << "[VectorTileLayer] polygon.vert load failed layer=" << layerName()
			//		<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
			pierce++;
			return -1;
		}
	}
	else {
		//std::cout << "[VectorTileLayer] polygon.vert missing layer=" << layerName()
		//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
		pierce++;
		return -1;
	}

	if (pts) {
		/*cout << tileIndex << endl;
		for (int i = 0; i < size / 3; i++) {
			cout << fixed << setprecision(4) << "XYZ: " << pts[i * 3] << " " << pts[i * 3 + 1] << " " << pts[i * 3 + 2] << " " << endl;
			Vec2d LonLat;
			double height, X, Y;
			CGeoUtil::XYZToLatLongHeight(pts[i*3], pts[i*3+1], pts[i*3+2], LonLat[1], LonLat[0], height);
			LonLat[1] = LonLat[1] * 180 / CGeoUtil::PI;
			LonLat[0] = LonLat[0] * 180 / CGeoUtil::PI;
			
			CGeoUtil::lonLat2WebMercator(LonLat[1], LonLat[0], X, Y);
			pts[i*3] = X;
			pts[i*3+1] = Y;
			pts[i*3+2] = 0.1;
			 cout << fixed << setprecision(4) << "Mecator: " << X << " " << Y << " " << endl;
		}*/
		mBuffer = new TMBuffer(PolygonBuffer, level2Index);
		string dataIndex = level2Index + ".vertices";
		vertices = new Vertices(pts, size, dataIndex);
		mBuffer->setData(vertices, VERTICE);
		manager->insert(2, mBuffer, level2Index);
	}
	else {
		//std::cout << "[VectorTileLayer] polygon.vert empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		pierce++;
		return -1;
	}
	return 0;
}

int CVectorTileLayer::addPolylineBuffer(int zoom, int col, int row, BufferManager* manager)
{
	if (!_useDatFile || !_vectorDatReader) {
		//std::cout << "[VectorTileLayer] polyline buffer DAT unavailable layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	int normalizedCol = normalizeColumn(col, zoom);

	// 优化字符串生成
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string layerNameStr = layerName();
	string level2Index = tileIndex + "." + layerNameStr;

	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (mBuffer) {
		return 0;
	}
	Vertices* vertices = 0x00;
	Stop* stops = 0x00;
	string dataIndex = level2Index + ".vertice";
	string stopIndex = level2Index + ".stop";

	// 加载vertices
	float* pts = nullptr;
	int size = 0;
	int datX = 0;
	int datY = 0;
	bool hasVertData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_VERTICE, datX, datY);
	logDatStatus("polyline.vert", zoom, normalizedCol, row, datX, datY, hasVertData);
	if (hasVertData) {
		if (!loadVerticesFromDat(zoom, datX, datY, pts, size)) {
			//std::cout << "[VectorTileLayer] polyline.vert load failed layer=" << layerName()
			//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
			pierce++;
			return -1;
		}
	}
	else {
		//std::cout << "[VectorTileLayer] polyline.vert missing layer=" << layerName()
		//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
		pierce++;
		return -1;
	}

	if (pts) {
		mBuffer = new TMBuffer(PolylineBuffer, level2Index);
		string dataIndex = level2Index + ".vertice";
		vertices = new Vertices(pts, size, dataIndex);
		mBuffer->setData(vertices, VERTICE);
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] polyline.vert empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	// 加载stops
	int* stopData = nullptr;
	int pointsCount = 0;
	int stopsCount = 0;
	int datXStop = 0;
	int datYStop = 0;
	bool hasStopData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_STOP, datXStop, datYStop);
	logDatStatus("polyline.stop", zoom, normalizedCol, row, datXStop, datYStop, hasStopData);
	if (hasStopData) {
		if (!loadStopsFromDat(zoom, datXStop, datYStop, stopData, pointsCount, stopsCount)) {
			if (mBuffer != 0x00)
				delete mBuffer;
			mBuffer = 0x00;
			//std::cout << "[VectorTileLayer] polyline.stop load failed layer=" << layerName()
			//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datXStop << "," << datYStop << ")" << std::endl;
			return -1;
		}
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] polyline.stop missing layer=" << layerName()
		//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datXStop << "," << datYStop << ")" << std::endl;
		return -1;
	}

	if (stopData) {
		string dataIndex = level2Index + ".stop";
		stops = new Stop(stopData, stopsCount, dataIndex);
		mBuffer->setData(stops, STOP);
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] polyline.stop empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		return -1;
	}

	manager->insert(2, mBuffer, level2Index);

	return 0;
}

int CVectorTileLayer::addPointBuffer(int zoom, int col, int row, BufferManager* manager)
{
	if (!_useDatFile || !_vectorDatReader) {
		//std::cout << "[VectorTileLayer] point buffer DAT unavailable layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	int normalizedCol = normalizeColumn(col, zoom);

	// 优化字符串生成
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string layerNameStr = layerName();
	string level2Index = tileIndex + "." + layerNameStr;

	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (mBuffer) {
		return 0;
	}

	Vertices* vertices = 0x00;
	string dataIndex = level2Index + ".vertices";

	float* pts = nullptr;
	int size = 0; // 初始化为0，防止未初始化值导致的问题
	int datX = 0;
	int datY = 0;
	bool hasData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_VERTICE, datX, datY);
	logDatStatus("point.vert", zoom, normalizedCol, row, datX, datY, hasData);
	if (hasData) {
		if (!loadVerticesFromDat(zoom, datX, datY, pts, size)) {
			//std::cout << "[VectorTileLayer] point.vert load failed layer=" << layerName()
			//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
			pierce++;
			return -1;
		}
	}
	else {
		/*	std::cout << "[VectorTileLayer] point.vert missing layer=" << layerName()
				<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;*/
		pierce++;
		return -1;
	}

	if (pts) {
		mBuffer = new TMBuffer(PointBuffer, level2Index);
		vertices = new Vertices(pts, size, dataIndex);
		mBuffer->setData(vertices, VERTICE);
		manager->insert(2, mBuffer, level2Index);
	}
	else {
		//std::cout << "[VectorTileLayer] point.vert empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	return 0;
}

int CVectorTileLayer::addAnnotationBuffer(int zoom, int col, int row, BufferManager* manager)
{
	if (!_useDatFile || !_vectorDatReader) {
		//std::cout << "[VectorTileLayer] annotation buffer DAT unavailable layer=" << layerName()
		//	<< " z=" << zoom << " col=" << col << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	int normalizedCol = normalizeColumn(col, zoom);

	// 优化字符串生成
	char tileIndexBuf[64];
	snprintf(tileIndexBuf, sizeof(tileIndexBuf), "%d.%d.%d", zoom, row, normalizedCol);
	string tileIndex(tileIndexBuf);
	string layerNameStr = layerName();
	string level2Index = tileIndex + "." + layerNameStr;

	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (mBuffer) {
		return 0;
	}
	Vertices* vertices = 0x00;
	Text* annos = 0x00;
	Stop* stops = 0x00;
	string dataIndex = level2Index + ".vertices";
	string stopIndex = level2Index + ".stops";
	string annoIndex = level2Index + ".text";

	// 加载vertices
	float* pts = nullptr;
	int size = 0;
	int datX = 0;
	int datY = 0;
	bool hasVertData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_VERTICE, datX, datY);
	logDatStatus("annotation.vert", zoom, normalizedCol, row, datX, datY, hasVertData);
	if (hasVertData) {
		if (!loadVerticesFromDat(zoom, datX, datY, pts, size)) {
			//std::cout << "[VectorTileLayer] annotation.vert load failed layer=" << layerName()
			//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
			pierce++;
			return -1;
		}
	}
	else {
		//std::cout << "[VectorTileLayer] annotation.vert missing layer=" << layerName()
		//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datX << "," << datY << ")" << std::endl;
		pierce++;
		return -1;
	}

	if (pts) {
		mBuffer = new TMBuffer(PolylineBuffer, level2Index);
		string dataIndex = level2Index + ".vertices";
		vertices = new Vertices(pts, size, dataIndex);
		mBuffer->setData(vertices, VERTICE);
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] annotation.vert empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		pierce++;
		return -1;
	}

	// 加载stops
	int* stopData = nullptr;
	int pointsCount = 0;
	int stopsCount = 0;
	int datXStop = 0;
	int datYStop = 0;
	bool hasStopData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_STOP, datXStop, datYStop);
	logDatStatus("annotation.stop", zoom, normalizedCol, row, datXStop, datYStop, hasStopData);
	if (hasStopData) {
		if (!loadStopsFromDat(zoom, datXStop, datYStop, stopData, pointsCount, stopsCount)) {
			if (mBuffer != 0x00)
				delete mBuffer;
			mBuffer = 0x00;
			//std::cout << "[VectorTileLayer] annotation.stop load failed layer=" << layerName()
			//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datXStop << "," << datYStop << ")" << std::endl;
			return -1;
		}
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] annotation.stop missing layer=" << layerName()
		//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datXStop << "," << datYStop << ")" << std::endl;
		return -1;
	}

	if (stopData) {
		stops = new Stop(stopData, stopsCount, stopIndex);
		mBuffer->setData(stops, STOP);
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] annotation.stop empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		return -1;
	}

	// 加载anno
	string tData;
	int datXAnno = 0;
	int datYAnno = 0;
	bool hasAnnoData = resolveDatCoords(zoom, normalizedCol, row, VECTOR_TYPE_ANNO, datXAnno, datYAnno);
	logDatStatus("annotation.anno", zoom, normalizedCol, row, datXAnno, datYAnno, hasAnnoData);
	if (hasAnnoData) {
		if (!loadAnnoFromDat(zoom, datXAnno, datYAnno, tData)) {
			if (mBuffer != 0x00)
				delete mBuffer;
			mBuffer = 0x00;
			//std::cout << "[VectorTileLayer] annotation.anno load failed layer=" << layerName()
			//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datXAnno << "," << datYAnno << ")" << std::endl;
			return -1;
		}
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] annotation.anno missing layer=" << layerName()
		//	<< " tile(z=" << zoom << ", col=" << normalizedCol << ", row=" << row << ") dat=(" << datXAnno << "," << datYAnno << ")" << std::endl;
		return -1;
	}

	int textSize = tData.length();
	if (textSize) {
		annos = new Text(const_cast<char*>(tData.c_str()), textSize, annoIndex);
		mBuffer->setData(annos, TEXT);
	}
	else {
		if (mBuffer != 0x00)
			delete mBuffer;
		mBuffer = 0x00;
		//std::cout << "[VectorTileLayer] annotation.anno empty buffer layer=" << layerName()
		//	<< " z=" << zoom << " col=" << normalizedCol << " row=" << row << std::endl;
		return -1;
	}

	manager->insert(2, mBuffer, level2Index);
	return 0;

}

bool CVectorTileLayer::resolveDatCoords(int zoom, int col, int row, uint8_t type, int& datX, int& datY)
{
	if (!_useDatFile || !_vectorDatReader) {
		datX = col;
		datY = row;
		return true;
	}
	// DAT 中 x 对应文件名第二段（原 row），y 对应第三段（归一化列）
	int normalizedCol = normalizeColumn(col, zoom);
	int candidateX = row;
	int candidateY = normalizedCol;
	if (_vectorDatReader->hasTileData(zoom, candidateX, candidateY, type)) {
		datX = candidateX;
		datY = candidateY;
		return true;
	}
	// 尝试翻转 row (TMS)
	int maxRow = (1 << zoom) - 1;
	int flippedRow = maxRow - row;
	candidateX = flippedRow;
	if (_vectorDatReader->hasTileData(zoom, candidateX, candidateY, type)) {
		datX = candidateX;
		datY = candidateY;
		return true;
	}
	// 未找到
	datX = row;
	datY = normalizedCol;
	return false;
}

int CVectorTileLayer::normalizeColumn(int col, int zoom) const
{
	if (zoom < 0)
	{
		return col;
	}
	int maxCol = (1 << zoom);
	if (maxCol <= 0)
	{
		return col;
	}
	int normalized = col % maxCol;
	if (normalized < 0)
	{
		normalized += maxCol;
	}
	return normalized;
}

uint64_t CVectorTileLayer::makeTileKey(int zoom, int col, int row) const
{
	uint64_t key = (static_cast<uint64_t>(zoom) << 42) |
		((static_cast<uint64_t>(col) & ((1ull << 21) - 1ull)) << 21) |
		(static_cast<uint64_t>(row) & ((1ull << 21) - 1ull));
	return key;
}

void CVectorTileLayer::initDatReader()
{
	if (_path.empty()) {
		return;
	}

	// 检查路径是否以.dat结尾
	auto hasDatExtension = [](const string& filePath)->bool {
		if (filePath.size() < 4) return false;
		string ext = filePath.substr(filePath.size() - 4);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		return ext == ".dat";
		};

	if (hasDatExtension(_path)) {
		_vectorDatReader = new VectorDatReader();
		if (_vectorDatReader->load(_path)) {
			_useDatFile = true;
			_datFilePath = _path;
		}
		else {
			delete _vectorDatReader;
			_vectorDatReader = nullptr;
			_useDatFile = false;
		}
	}
	else {
	}
}

bool CVectorTileLayer::loadVerticesFromDat(int zoom, int col, int row, float*& pts, int& size)
{
	if (!_useDatFile || !_vectorDatReader) {
		return false;
	}

	std::vector<unsigned char> tileData;
	if (!_vectorDatReader->getTileData(zoom, col, row, VECTOR_TYPE_VERTICE, tileData)) {
		return false;
	}

	if (tileData.empty()) {
		return false;
	}

	pts = openglEngine::OpenGLFileEngine::getVerticesFromMemory<float>(
		tileData.data(), tileData.size(), CGeoUtil::WGS84, 2, size);

	bool success = (pts != nullptr && size > 0);
	return success;
}

bool CVectorTileLayer::loadStopsFromDat(int zoom, int col, int row, int*& stops, int& pointsCount, int& stopsCount)
{
	if (!_useDatFile || !_vectorDatReader) {
		return false;
	}

	std::vector<unsigned char> tileData;
	if (!_vectorDatReader->getTileData(zoom, col, row, VECTOR_TYPE_STOP, tileData)) {
		return false;
	}

	if (tileData.empty()) {
		return false;
	}

	stops = openglEngine::OpenGLFileEngine::getStopsFromMemory<int>(
		tileData.data(), tileData.size(), pointsCount, stopsCount);

	bool success = (stops != nullptr && stopsCount > 0);
	return success;
}

bool CVectorTileLayer::loadAnnoFromDat(int zoom, int col, int row, std::string& text)
{
	if (!_useDatFile || !_vectorDatReader) {
		return false;
	}

	std::vector<unsigned char> tileData;
	if (!_vectorDatReader->getTileData(zoom, col, row, VECTOR_TYPE_ANNO, tileData)) {
		return false;
	}

	if (tileData.empty()) {
		return false;
	}

	text = openglEngine::OpenGLFileEngine::getStringFromMemory<char>(
		tileData.data(), tileData.size());

	bool success = !text.empty();
	return success;
}

void CVectorTileLayer::logDatStatus(const std::string& stage, int zoom, int col, int row, int datX, int datY, bool hasData) const
{
	std::cout << "[VectorTileLayer][" << stage << "] layer=" << layerName()
		<< " tile(z=" << zoom << ", col=" << col << ", row=" << row << ")"
		<< " datCoord=(" << datX << "," << datY << ") hasData=" << (hasData ? "true" : "false")
		<< std::endl;
}
