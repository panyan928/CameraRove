#include <sstream>
#include <iomanip>
#include "CVectorTileLayer.h"
#include "sqlite/sqlite3.h"
#include "../OpenGLEngine/openglfileengine.h"
#include "../OpenGLEngine/openglrenderengine.h"
#include "BatchRenderer.h"

void CVectorTileLayer::setAnnotation(int anno, string label)
{
	this->_anno = anno;
	this->_label = label;
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

int CVectorTileLayer::draw(vector<Vec3i> tiles, int zoom, BufferManager* manager)
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
	//clock_t end = clock();

	//cout << layerName() << " " << end - begin << endl;
	//cout << allCount << " " << count << endl;
	return 0;
}

// 修改draw方法，使用批处理渲染
int CVectorTileLayer::draw_batch(vector<Vec3i> tiles, int zoom, BufferManager* manager, BatchRenderer* batchRenderer)
{
	//  // 清空批处理渲染器
	//  _batchRenderer->clear();
	 
    Vec2i visibleZoom = this->zoom();
    if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
        return -1;

    _amount = 0;
    count = 0;
    allCount = 0;
    pierce = 0;

    // 加载瓦片数据到批处理渲染器
    int len = tiles.size();
    for (int k = 0; k < len; k++) {
        allCount++;
        int zoom = tiles[k][0];
        int i = tiles[k][1];
        int j = tiles[k][2];

        // 添加到批处理渲染器
        addToBatchRenderer(zoom, i, j, manager, batchRenderer);
    }

    // 使用批处理渲染器渲染
    return 0;
}

// 实现添加到批处理渲染器的方法
void CVectorTileLayer::addToBatchRenderer(int zoom, int col, int row, BufferManager* manager, BatchRenderer* batchRenderer)
{
    // 构建瓦片索引
    ostringstream ost_temp;
    ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
    string tileIndex = ost_temp.str();
    string level2Index = tileIndex + "." + layerName();
    
    // 检查是否已经加载
    TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
    if (!mBuffer) {
        // 如果未加载，则根据几何类型加载数据
        if (geometryType().compare("polygon") == 0) {
            addPolygonBuffer(zoom, col, row, manager, NULL);
        }
        else if (geometryType().compare("polyline") == 0) {
            addPolylineBuffer(zoom, col, row, manager, NULL);
        }
        else if (geometryType().compare("point") == 0) {
            if (_anno) {
                addAnnotationBuffer(zoom, col, row, manager, NULL);
            }
            else {
                addPointBuffer(zoom, col, row, manager, NULL);
            }
        }
        
        // 重新获取加载后的缓冲区
        mBuffer = manager->getFrom2LevelBuffer(level2Index);
    }
    
    // 如果缓冲区存在，创建要素并添加到批处理渲染器
    if (mBuffer) {
        CStyle* style = getOrCreateStyle();
        
        if (geometryType().compare("polygon") == 0) {
            // 处理多边形数据
            Vertices* vertices = mBuffer->vData();
            if (vertices) {
                // 创建要素
                VectorFeature* feature = new VectorFeature();
                feature->type = 2; // 面
                
                // 复制顶点数据
                int size = vertices->size();
                feature->vertices = new float[size * 3];
                memcpy(feature->vertices, vertices->data(), size * 3 * sizeof(float));
                feature->vertexCount = size;

				//创建索引数据
				feature->indices = new int[size];
				for (int i = 0; i < size; i++) {
					feature->indices[i] = i;
				}
				feature->indexCount = size;
                
                // 设置样式
                if (style && style->fill(0)) {
                    float r, g, b;
                    style->fill(0)->getColor()->colorRGB(r, g, b);
                    feature->color = Color(255 * r, 255 * g, 255 * b, 1);
                }
                
                // 添加到批处理渲染器
                batchRenderer->addFeature(feature);
            }
        }
        else if (geometryType().compare("polyline") == 0) {
            // 处理线数据
            Vertices* vertices = mBuffer->vData();
            Stop* stops = mBuffer->sData();
            
            if (vertices && stops) {
                int i = 0;
                while (style->stroke(i)) {
                    // 创建要素
                    VectorFeature* feature = new VectorFeature();
                    feature->type = 1; // 线
                    
                    // 复制顶点数据
                    int size = vertices->size();
                    feature->vertices = new float[size * 3];
                    memcpy(feature->vertices, vertices->data(), size * 3 * sizeof(float));
                    feature->vertexCount = size;
                    
                    // 复制停止点数据
                    int stopCount = stops->size();
                    feature->stops = new int[stopCount];
                    memcpy(feature->stops, stops->data(), stopCount * sizeof(int));
                    feature->stopCount = stopCount;
                    
                    // 设置样式
                    float r, g, b;
                    style->stroke(i)->getColor()->colorRGB(r, g, b);
                    feature->color = Color(255 * r, 255 * g, 255 * b, 1);
                    feature->lineWidth = style->stroke(i)->width();
                    feature->isDashed = isDashArrayValid(style->stroke(i)->dashArray());
                    
                    // 添加到批处理渲染器
                    batchRenderer->addFeature(feature);
                    
                    i++;
                }
            }
        }
        // 点和注释可以类似处理
    }
}

int CVectorTileLayer::drawMultiThreads(void* para)//
{
	drawParameter* data = static_cast<drawParameter*>(para);
	vector<Vec3i> tiles = data->tiles;
	BufferManager* manager = data->manager;

	count = 0;

	allCount = 0;

	float r=0.0, g=0.0, b=0.0;
	CStyle* style = getOrCreateStyle();
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
	/*sqlite3* pDataBase = NULL;
	int iRet = sqlite3_open(this->_path.c_str(), &pDataBase);
	if (iRet)
	{
		cout << "open db failed:" << sqlite3_errmsg(pDataBase) << endl;
	}*/
	int len = tiles.size();
	for (int k = 0; k < len; k++) {
		int zoom = tiles[k][0];
		int col = tiles[k][1];
		int row = tiles[k][2];
        ostringstream ost_temp;//ost_temp.str("");
		ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
		string tileIndex = ost_temp.str();
		//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
		string level2Index = tileIndex + "." + layerName();
		if (geometryType().compare("polygon") == 0) {
			addPolygonBuffer(zoom, col, row, manager,NULL);
		}
		if (geometryType().compare("polyline") == 0) {
			addPolylineBuffer(zoom, col, row, manager, NULL);
		}
		if (geometryType().compare("point") == 0) {
			if (_anno) {
				addAnnotationBuffer(zoom, col, row, manager, NULL);
			}
			else {
				addPointBuffer(zoom, col, row, manager, NULL);
			}

		}
	}
	/*iRet = sqlite3_close(pDataBase);
	if (0 == iRet)
	{
		cout << "close db successfully" << endl;
	}*/
	return 0;
}

int CVectorTileLayer::getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager)
{
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;
	_amount = 0;

	count = 0;

	allCount = 0;

	pierce = 0;
	int len = tiles.size();
	for (int k = 0; k < len; k++) {

		int zoom = tiles[k][0];
		int i = tiles[k][1];
		int j = tiles[k][2];

		if (geometryType().compare("polygon") == 0) {
			drawPolygon(zoom, i, j, manager, 0);
		}
		if (geometryType().compare("polyline") == 0) {
			drawPolyline(zoom, i, j, manager, 0);
		}
		if (geometryType().compare("point") == 0) {
			//drawPoint(zoom, i, j, manager);
			if (_anno)
				drawAnnotation(zoom, i, j, manager, 0);
			else
				drawPoint(zoom, i, j, manager, 0);
		}
	}

	//cout << allCount << " " << count << endl;
	return _amount;
}

int CVectorTileLayer::drawPolygon(int zoom, int col, int row, BufferManager* manager, int draw)
{
	//clock_t t1, t2;
	/***************图锟斤拷锟斤拷锟斤拷锟绞?*********************/
	//t1 = clock();
	CStyle* style = getOrCreateStyle();
	float r=0.0, g=0.0, b=0.0;
    if (style->fill(0)==0x00)
	{
		int q = 0;
	}
	style->fill(0)->getColor()->colorRGB(r, g, b);
	Color color(255 * r, 255 * g, 255 * b, 255);

    // 锟斤拷锟秸�?�拷锟斤拷锟斤拷锟斤�?-->锟斤拷锟斤拷锟斤拷锟斤拷-->锟斤拷锟斤拷顺锟斤拷锟斤拷锟斤拷锟斤拷锟?
    //string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	Vertices* vertices = 0x00;
	if (mBuffer) {
		vertices = mBuffer->vData();
		count++;
	}
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
		int size_render = vertices->size();
		_amount += size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		if (draw){
            int upLimitLen = 65500;	//�޶�����������������
    	    int times=0;//��Ҫ���ƵĴ���
            if(size_render % upLimitLen)
        		times=size_render / upLimitLen + 1;
        	else
        		times = size_render / upLimitLen ;
            int trueLen=0;//ÿ�λ��Ƶ���ʵ����
            for (int i = 0; i < times; i++) {
        		if (i == times - 1) {
        			if(size_render % upLimitLen)
        				trueLen = size_render % upLimitLen;
        			else
        				trueLen = upLimitLen;
        		}
        		else {
        			trueLen = upLimitLen;
        		}
                state = openglEngine::OpenGLRenderEngine::drawTriangles<float>(&pts_render[upLimitLen*i], color, trueLen);
        	}
            
        }
	}
	return state;
}

int CVectorTileLayer::drawPolyline(int zoom, int col, int row, BufferManager* manager, int draw)
{
	CStyle* style = getOrCreateStyle();
	float r=0.0, g=0.0, b=0.0;
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
    ostringstream ost_temp;//ost_temp.str("");
    ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	Vertices* vertices = 0x00;
	Stop* stops = 0x00;
	if (mBuffer) {
		vertices = mBuffer->vData();
		stops = mBuffer->sData();
		count++;
	}
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
		int size_render = stops->size();
		_amount += size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		int* stops_render = static_cast<int*>(stops->data());

		_amount += stops_render[size_render - 1];
		if (draw) {
			int i = 0;
			while (style->stroke(i)) {
				style->stroke(i)->getColor()->colorRGB(r, g, b);
				Color color(255 * r, 255 * g, 255 * b, 255);
				vector<float> dashArray = style->stroke(i)->dashArray();
				if (isDashArrayValid(dashArray))
					state = openglEngine::OpenGLRenderEngine::drawLines(pts_render, stops_render, size_render, color, style->stroke(i)->width(), 1);
				else
					state = openglEngine::OpenGLRenderEngine::drawLines(pts_render, stops_render, size_render, color, style->stroke(i)->width(), 0);
				i++;
			}
		}
	}
	return state;

	/*   if (!mBuffer) {
		   mBuffer = new TMBuffer(BufferType::PolylineBuffer, level2Index);
		   manager->insert(2, mBuffer, level2Index);
	   }
	   else
		   cout << "锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷" << endl;*/

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

	// 锟斤拷锟秸�?�拷锟斤拷锟斤拷锟斤�?-->锟斤拷锟斤拷锟斤拷锟斤拷-->锟斤拷锟斤拷顺锟斤拷锟斤拷锟斤拷锟斤拷锟?
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();

	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);

	Vertices* vertices = 0x00;

	if (mBuffer) {
		vertices = mBuffer->vData();

		count++;
	}
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
		_amount += size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		//TODO("鍔犱竴涓猄ymbol鐨勮矾�?�勫�?閰嶇疆鏂囦�??)
		#ifdef WIN32
            string symbolPath = "./../data/mbtiles-jiangxi/symbols/" + layerName() + ".jpg";
        #else//Ŀǰֻ������tm3
            string symbolPath = "D:\\mbtiles-jiangxi/symbols/" + layerName() + ".jpg";
        #endif
		if (draw)
			openglEngine::OpenGLRenderEngine::drawSymbols<float>(pts_render, size_render, symbolPath.c_str(), _render);
	}
	return state;
}

int CVectorTileLayer::drawAnnotation(int zoom, int col, int row, BufferManager* manager, int draw)
{
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
    ostringstream ost_temp;//ost_temp.str("");
    ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();

	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);

	Vertices* vertices = 0x00;
	Text* annos = 0x00;
	Stop* stops = 0x00;

	if (mBuffer) {
		vertices = mBuffer->vData();
		annos = mBuffer->tData();
		stops = mBuffer->sData();

		count++;
	}
	//else {

	//	string dataIndex = level2Index + ".vertices";
	//	string stopIndex = level2Index + ".stops";
	//	string annoIndex = level2Index + ".text";
	//	if (!vertices) {
	//		//allCount++;

	//		string path = _path + tileIndex + ".vertice";
	//		int size;
	//		float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::Proj::WGS84, 2, size);
	//		if (pts) {
	//			mBuffer = new TMBuffer(BufferType::PolylineBuffer, level2Index);
	//			string dataIndex = level2Index + ".vertices";
	//			vertices = new Vertices(pts, size, dataIndex);
	//			mBuffer->setData(vertices, DataType::Vertice);
	//		}
	//		else {
	//			if (mBuffer != 0x00)
	//				delete mBuffer;
	//			mBuffer = 0x00;
	//			pierce++;
	//			return -1;
	//		}
	//	}

	//	if (!stops) {
	//		string path = _path + tileIndex + ".stop";
	//		int size, size1;
	//		int* pts = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(path.c_str(), size1, size);
	//		if (pts) {
	//			stops = new Stop(pts, size, stopIndex);
	//			mBuffer->setData(stops, DataType::Stop);
	//		}
	//		else {
	//			if (mBuffer != 0x00)
	//				delete mBuffer;
	//			mBuffer = 0x00;
	//			return -1;
	//		}
	//	}

	//	if (!annos) {
	//		string path = _path + tileIndex + ".anno";
	//		string tData = openglEngine::OpenGLFileEngine::getStringFromBinary<char>(path.c_str());
	//		int size = tData.length();
	//		if (size) {
	//			annos = new Text(const_cast<char*>(tData.c_str()), size, annoIndex);
	//			mBuffer->setData(annos, DataType::Text);
	//		}
	//		else {
	//			if (mBuffer != 0x00)
	//				delete mBuffer;
	//			mBuffer = 0x00;
	//			return -1;
	//		}
	//	}
	//	manager->insert(2, mBuffer, level2Index);

	//}
	int state = -1;
	if (vertices && annos && stops) {
		int size_render = vertices->size();
		_amount += 3 * size_render;
		float* pts_render = static_cast<float*>(vertices->data());
		int* stops_render = static_cast<int*>(stops->data());
		string text_render = static_cast<char*>(annos->data());
		if (draw) {
			//if (_anno == 2) {
			//	#ifdef WIN32
   //                 string symbolPath = "./../data/mbtiles-jiangxi/symbols/" + layerName() + ".jpg";
   //             #else//Ŀǰֻ������tm3
   //                 string symbolPath = "D:\\mbtiles-jiangxi/symbols/" + layerName() + ".jpg";
   //             #endif
			//	openglEngine::OpenGLRenderEngine::drawSymbolsAndAnnotations<float>(pts_render, size_render, symbolPath.c_str(),
			//		text_render, stops_render, getOrCreateStyle(), _render);
			//	//openglEngine::OpenGLRenderEngine::drawSymbols<float>(pts_render, size_render, symbolPath.c_str(), _render);
			//}
			///*CColor* color = getOrCreateStyle()->text(0)->getColor();
			//float r, g, b;
			//color->colorRGB(r, g, b);
			//openglEngine::OpenGLRenderEngine::drawAnnotations(pts_render, size_render,
			//	text_render, stops_render, Color(255, 255 * r, 255 * g, 255 * b), _render);*/
			//else
				openglEngine::OpenGLRenderEngine::drawAnnotations(pts_render, size_render,
					text_render, stops_render, getOrCreateStyle(), _render);
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


int CVectorTileLayer::addPolygonBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db) {
    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (mBuffer) {
		return 0;
	}
	Vertices* vertices = 0x00;
	string dataIndex = level2Index + ".vertice";
	string path = _path + tileIndex + ".vertice";
	int size;
	//10.27更改数据�?
	//float* pts = openglEngine::OpenGLFileEngine::getVerticesFromDB<float>(db, zoom, row, col, CGeoUtil::Proj::WGS84, 2, size);
	float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::WGS84, 2, size);
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
	else
		pierce++;
	return 0;
}

int CVectorTileLayer::addPolylineBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db)
{
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
	ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	if (mBuffer) {
		return 0;
	}
	Vertices* vertices = 0x00;
	Stop* stops = 0x00;
	string dataIndex = level2Index + ".vertice";
	string stopIndex = level2Index + ".stop";
		if (!vertices) {
			string path = _path + tileIndex + ".vertice";
			int size;
			//10.27更改数据�?
			//float* pts = openglEngine::OpenGLFileEngine::getVerticesFromDB<float>(db, zoom, row, col, CGeoUtil::Proj::WGS84, 2, size);
			float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::WGS84, 2, size);
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
				pierce++;
				return -1;
			}
		}

		if (!stops) {
			string path = _path + tileIndex + ".stop";
			int size, size1;
			//10.27更改数据�?
			//int* pts = openglEngine::OpenGLFileEngine::getStopsFromDB<int>(db,zoom,row,col,size1,size);
			int* pts = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(path.c_str(), size1, size);
			if (pts) {
				string dataIndex = level2Index + ".stop";
				stops = new Stop(pts, size, dataIndex);
				mBuffer->setData(stops, STOP);
			}
			else {
				if (mBuffer != 0x00)
					delete mBuffer;
				mBuffer = 0x00;
				return -1;
			}
		}
		manager->insert(2, mBuffer, level2Index);
	

	return 0;
}

int CVectorTileLayer::addPointBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db)
{
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
    ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();
	string level2Index = tileIndex + "." + layerName();
	TMBuffer* mBuffer = manager->getFrom2LevelBuffer(level2Index);
	Vertices* vertices = 0x00;
	mBuffer = new TMBuffer(PointBuffer, level2Index);
	if (mBuffer) {
		return 0;
	}
	string dataIndex = level2Index + ".vertices";
		string path = _path + tileIndex + ".vertice";
		int size;
		//10.27更改数据�?
		//float* pts = openglEngine::OpenGLFileEngine::getVerticesFromDB<float>(db, zoom, row, col, CGeoUtil::Proj::WGS84, 2, size);
		float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::WGS84, 2, size);
		if (pts) {
			vertices = new Vertices(pts, size, dataIndex);
			mBuffer->setData(vertices, VERTICE);
			manager->insert(2, mBuffer, level2Index);
		}
		else {
			if (mBuffer != 0x00)
				delete mBuffer;
			mBuffer = 0x00;
			pierce++;
			return -1;
		}
	

	return 0;
}

int CVectorTileLayer::addAnnotationBuffer(int zoom, int col, int row, BufferManager* manager,sqlite3* db)
{
	//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string((col % (int)pow(2, zoom)));
	ostringstream ost_temp;//ost_temp.str("");
	ost_temp << (zoom) << "." << (row) << "." << ((col % (int)pow(2, zoom)));
	string tileIndex = ost_temp.str();
	string level2Index = tileIndex + "." + layerName();
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

		if (!vertices) {
			string path = _path + tileIndex + ".vertice";
			int size;
			//10.27更改数据�?
			//float* pts = openglEngine::OpenGLFileEngine::getVerticesFromDB<float>(db, zoom, row, col, CGeoUtil::Proj::WGS84, 2, size);
			float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path.c_str(), CGeoUtil::WGS84, 2, size);
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
				pierce++;
				return -1;
			}
		}
		if (!stops) {
			string path = _path + tileIndex + ".stop";
			int size, size1;
			//10.27更改数据�?
			//int* pts = openglEngine::OpenGLFileEngine::getStopsFromDB<int>(db, zoom, row, col, size1, size);
			int* pts = openglEngine::OpenGLFileEngine::getStopsFromBinary<int>(path.c_str(), size1, size);
			if (pts) {
				stops = new Stop(pts, size, stopIndex);

				mBuffer->setData(stops, STOP);
			}
			else {
				if (mBuffer != 0x00)
					delete mBuffer;
				mBuffer = 0x00;
				return -1;
			}
		}
		if (!annos) {
			string path = _path + tileIndex + ".anno";
			//string tData = openglEngine::OpenGLFileEngine::getStringFromDB<char>(db,zoom,row,col);
			string tData = openglEngine::OpenGLFileEngine::getStringFromBinary<char>(path.c_str());
			int size = tData.length();
			if (size) {
				annos = new Text(const_cast<char*>(tData.c_str()), size, annoIndex);

				mBuffer->setData(annos, TEXT);

			}
			else {
				if (mBuffer != 0x00)
					delete mBuffer;
				mBuffer = 0x00;
				return -1;
			}
		}
		manager->insert(2, mBuffer, level2Index);
	
	return 0;

}


