#include "crastertilelayer.h"
#include "sqlite/sqlite3.h"
#include <sstream>

CRasterTileLayer::CRasterTileLayer(string path)
{
	/************ 构造通用纹理数组 **************/
	int interval = 4;
	int size = 256 / interval + 2;
	_textures = new float[size * size * 2];
	int index_t = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			_textures[index_t++] = 1.0 * (j) / (size - 1);
			_textures[index_t++] = 1.0 * (i) / (size - 1);
		}
	}
	_path = path;
	index_num = (size * size - 2 * size + 1) * 20;
}

CRasterTileLayer::~CRasterTileLayer()
{
	if (_textures != 0x00)
		delete[] _textures;
	_textures = 0x00;
}

int CRasterTileLayer::draw(Recti bounds, int zoom)
{
	if (bounds[0] > bounds[2])
		bounds[2] += pow(2, zoom);
	for (int i = bounds[0]; i <= bounds[2]; i++) {
		for (int j = bounds[3]; j <= bounds[1]; j++) {
			string path_pts, path_index, path_texture;
			int pt_size = 0;

			
            ostringstream ost_temp;//ost_temp.str("");
            ost_temp << _path << (zoom) << "." << ((int)pow(2, zoom) - 1 - j) << "." << ((i % (int)pow(2, zoom))) << ".vertice";
            path_pts = ost_temp.str();
            //path_pts = _path + to_string(zoom) + "." + to_string((int)pow(2, zoom) - 1 - j) + "." + to_string((i % (int)pow(2, zoom))) + ".vertice";
            float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path_pts.c_str(), CGeoUtil::WGS84, 3, pt_size);

            ost_temp.str("");
            ost_temp << _path << (zoom) << "." << ((int)pow(2, zoom) - 1 - j) << "." << ((i % (int)pow(2, zoom))) << ".index";
            path_index = ost_temp.str();
            //path_index = _path + to_string(zoom) + "." + to_string((int)pow(2, zoom) - 1 - j) + "." + to_string((i % (int)pow(2, zoom))) + ".index";
            int* index = openglEngine::OpenGLFileEngine::getIndexFromBinary<int>(path_index.c_str(), pt_size);

            float* texture_coor = openglEngine::OpenGLFileEngine::getTextureCoor<float>(8);
			ost_temp.str("");
            ost_temp << _path << (zoom) << "." << ((int)pow(2, zoom) - 1 - j) << "." << ((i % (int)pow(2, zoom))) << ".jpg";
            path_texture = ost_temp.str();
            //path_texture = _path + to_string(zoom) + "." + to_string((int)pow(2, zoom) - 1 - j) + "." + to_string((i % (int)pow(2, zoom))) + ".jpg";
			openglEngine::OpenGLRenderEngine::drawRasters(pts, texture_coor, index, path_texture.c_str(), pt_size);

			/*float* pts = openglEngine::OpenGLFileEngine::getVerticesFromBinary<float>(path_pts.c_str(), CGeoUtil::WGS84, 3, pt_size);

			path_index = _path + to_string(zoom) + "." + to_string((int)pow(2, zoom) - 1 - j) + "." + to_string((i % (int)pow(2, zoom))) + ".index";
			int* index = openglVertice::getIndexFromBinary<int>(path_index.c_str(), pt_size);

			float* texture_coor = openglVertice::getTextureCoor<float>(8);
			path_texture = _path + to_string(zoom) + "." + to_string((int)pow(2, zoom) - 1 - j) + "." + to_string((i % (int)pow(2, zoom))) + ".jpg";
			openglEngine::OpenGLRenderEngine::drawRasters(pts, texture_coor, index, path_texture.c_str(), pt_size);*/

			if (pts) {
				delete[] pts;
				pts = 0x00;
			}
			if (index) {
				delete[] index;
				index = 0x00;
			}
			if (texture_coor) {
				delete[] texture_coor;
				texture_coor = 0x00;
			}
		}
	}

	return 0;
}

int CRasterTileLayer::draw(Recti bounds, int zoom, BufferManager* manager)
{
	return 0;
}

/**
 * @brief           栅格图层绘制实现
 * @details         根据调度结果绘制栅格图层
 * @param tiles     待绘制瓦片集合
 * @param zoom      调度等级
 * @param manager   缓存管理对象
 */
int CRasterTileLayer::draw(vector<Vec3i> tiles, int zoom, BufferManager* manager)
{

	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;

	int interval = 4;
	int size = 256 / interval + 2;

	for (int k = 0; k < tiles.size(); k++) {
		int zoom = tiles[k][0];
		int col = tiles[k][1];
		//int row = tiles[k][2];
		int row = pow(2, zoom) - tiles[k][2] - 1;
		//cout << zoom << "." << row << "." << col << endl;
		ostringstream ost_temp;//ost_temp.str("");
		ost_temp << (zoom) << "." << (row) << "." << (col);
		string tileIndex = ost_temp.str();
        //string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string(col);
		string level2Index = tileIndex + "." + layerName();
		cout << level2Index <<"\n";
		TMBuffer* buffer = manager->getFrom2LevelBuffer(level2Index);
		//GLuint *pTexture = 0x00;
		Vertices* pColors = 0x00;
		Vertices* pVertices = 0x00;
		Heights* pHeight = 0x00;
		Index* pIndex = 0x00;
		Image* pImage = 0x00;


		if (buffer) {
			//pTexture = buffer->texture();
			pColors = buffer->cData();
			pVertices = buffer->vData();
			pHeight = buffer->hData();
			pIndex = buffer->iData();
			pImage = buffer->imData();
			//cout << "cache successfully" << endl;
		}
		else {
			continue;
		}
		//else {
		//	buffer = new TMBuffer(BufferType::TerrianBuffer, level2Index);
		//	string vIndex = level2Index + ".vertice";
		//	string iIndex = level2Index + ".index";
		//	string hIndex = tileIndex + ".height";
		//	string imIndex = tileIndex + ".image";

		//	if (!pImage) {
		//		int width, height, nrChannels;
		//		string path = _path + to_string(zoom) + "/" + to_string(col) + "/" + to_string(row) + ".jpg";
		//		int len = 0;
		//		//unsigned char* img = openglEngine::OpenGLFileEngine::getImgFromDB<unsigned char>(pDataBase, zoom, row, col, len);
		//		//unsigned char* data = stbi_load_from_memory(img, len, &width, &height, &nrChannels, 0);
		//		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		//		if (data) {
		//			pImage = new Image(data, width * height, imIndex);
		//			pImage->setInfo(height, width, nrChannels);
		//		}

		//		else {
		//			stbi_image_free(data);
		//			continue;
		//		}
		//	}

		//	if (!pVertices || !pIndex) {
		//		/******************** �������� *******************/
		//		float* vertices = new float[size * size * 3];

		//		int* index = new int[(size * size - 2 * size + 1) * 6];

		//		string hPath = _heightPath + to_string(zoom) + "." + to_string(row) + "." + to_string(col) + ".height";
		//		int hSize = 0;
		//		short int* height = openglEngine::OpenGLFileEngine::getHeightFromDB<short int>(pDataBase, zoom, row, col, hSize);
		//		//short int* height = openglEngine::OpenGLFileEngine::getHeightFromBinary<short int>(hPath.c_str(), hSize);

		//		Vec2d leftTop, pt;
		//		CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
		//		double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256 * interval;
		//		double x, y, z;

		//		int index_t = 0; int index_v = 0; int index_i = 0;
		//		for (int i = 0; i < size; i++) {
		//			for (int j = 0; j < size; j++) {

		//				pt[0] = leftTop[0] + (j - 1) * span;
		//				pt[1] = leftTop[1] - (i - 1) * span;
		//				double lat, lon;
		//				CGeoUtil::WebMercator2lonLat(pt[0], pt[1], lat, lon);

		//				if (height == nullptr)
		//					CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, x, y, z);
		//				else {
		//					int h = 1.5 * height[i * size + j];
		//					//h = 0;
		//					//cout << h << endl;
		//					CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, h, x, y, z);
		//				}
		//				vertices[index_v++] = x;
		//				vertices[index_v++] = y;
		//				vertices[index_v++] = z;



		//				if (i > 0 && j > 0) {
		//					index[index_i++] = (i)*size + j;
		//					index[index_i++] = (i)*size + j - 1;
		//					index[index_i++] = (i - 1) * size + j;

		//					index[index_i++] = i * size + j - 1;
		//					index[index_i++] = (i - 1) * size + j - 1;
		//					index[index_i++] = (i - 1) * size + j;
		//				}
		//			}
		//		}

		//		pVertices = new Vertices(vertices, size * size, vIndex);
		//		pIndex = new Index(index, (size * size - 2 * size + 1) * 6, iIndex);
		//		pHeight = new Heights(height, size * size, hIndex);

		//		buffer->setData(pVertices, DataType::Vertice);
		//		buffer->setData(pIndex, DataType::Index);
		//		buffer->setData(pHeight, DataType::Height);
		//		buffer->setData(pImage, DataType::Image);
		//		manager->insert(2, buffer, level2Index);
		//	}
		//}

		float* vertices = static_cast<float*>(pVertices->data());
		float* colors = static_cast<float*>(pColors->data());
		int* indexes = static_cast<int*>(pIndex->data());
		unsigned char* image = static_cast<unsigned char*>(pImage->data());
		int width=0, height=0, nrChannels=0;
		pImage->getInfo(width, height, nrChannels);
		if (vertices && indexes)
			//openglEngine::OpenGLRenderEngine::drawRasters(vertices, _textures, indexes, image, width, height, nrChannels,(size * size - 2 * size + 1) * 6);
			if (displayMode == 1)
				openglEngine::OpenGLRenderEngine::drawRasters(vertices, _textures, indexes, image, width, height, nrChannels,
				index_num, level2Index);//, level2Index
				//openglEngine::OpenGLRenderEngine::drawRasters(vertices, _textures, indexes, image,pTexture, width, height, nrChannels, (size * size - 2 * size + 1) * 6);
			else if(displayMode == 2)
				openglEngine::OpenGLRenderEngine::drawHeight(vertices, colors,indexes, (size * size - 2 * size + 1) * 6);
	}

	//iRet = sqlite3_close(pDataBase);
	//if (0 == iRet)
	//{
	//	cout << "close db successfully" << endl;
	//}


	return 0;
}

int CRasterTileLayer::drawTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager)
{
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;

	int interval = 4;
	int size = 256 / interval + 2;

	for (int k = 0; k < tiles.size(); k++) {
		int zoom = tiles[k][0];
		int col = tiles[k][1];
		//int row = tiles[k][2];
		int row = pow(2, zoom) - tiles[k][2] - 1;
		//cout << zoom << "." << row << "." << col << endl;
		ostringstream ost_temp;//ost_temp.str("");
		ost_temp << (zoom) << "." << (row) << "." << (col);
		string tileIndex = ost_temp.str();
		//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string(col);
		string level2Index = tileIndex + "." + layerName();

		TMBuffer* buffer = manager->getFrom2LevelBuffer(level2Index);
		Vertices* pVertices = 0x00;
		Heights* pHeight = 0x00;
		Index* pIndex = 0x00;

		if (buffer) {
			pVertices = buffer->vData();
			pHeight = buffer->hData();
			pIndex = buffer->iData();
		}
		else {
			continue;
		}

		float* vertices = static_cast<float*>(pVertices->data());
		int* indexes = static_cast<int*>(pIndex->data());
		int width = 0, height = 0, nrChannels = 0;
		if (vertices && indexes)
			//openglEngine::OpenGLRenderEngine::drawRasters(vertices, _textures, indexes, image, width, height, nrChannels,(size * size - 2 * size + 1) * 6);
			if (displayMode == 1) {
				openglEngine::OpenGLRenderEngine::drawTIN(vertices, _textures, indexes, index_num, level2Index);

				//openglEngine::OpenGLRenderEngine::drawRasters(vertices, _textures, indexes, image,pTexture, width, height, nrChannels, (size * size - 2 * size + 1) * 6);
			}
	}
	return 0;
}

//10.27更改了数据源，在打开文件的位置
int CRasterTileLayer::addBuffer(vector<Vec3i> tiles, int zoom, BufferManager* manager)
{

	bool isHeight = true;
	if (this->_heightPath == "null")	isHeight = false;
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;

	//height文件是66*66
	int interval = 4;
    int stride = 5; //数据抽取
	int size = 256 / interval + 2;

	for (int k = 0; k < tiles.size(); k++) {
		int zoom = tiles[k][0];
		int col = tiles[k][1];
		//int row = tiles[k][2];
		int row = pow(2, zoom) - tiles[k][2] - 1;
		//cout << zoom << "." << row << "." << col << endl;
		ostringstream ost_temp;//ost_temp.str("");
		ost_temp << (zoom) << "." << (row) << "." << (col);
		string tileIndex = ost_temp.str();
		//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string(col);
		string level2Index = tileIndex + "." + layerName();

		TMBuffer* buffer = manager->getFrom2LevelBuffer(level2Index);
		
		Vertices* pColors = 0x00;
		Vertices* pVertices = 0x00;
		Heights* pHeight = 0x00;
		Index* pIndex = 0x00;
		Image* pImage = 0x00;
		if (buffer) {

			pColors = buffer->cData();
			pVertices = buffer->vData();
			pHeight = buffer->hData();
			pIndex = buffer->iData();
			pImage = buffer->imData();
		}
		else {
			buffer = new TMBuffer(TerrianBuffer, level2Index);
			string cIndex = level2Index + ".color";
			string vIndex = level2Index + ".vertice";
			string iIndex = level2Index + ".index";
			string hIndex = tileIndex + ".height";
			string imIndex = tileIndex + ".image";
			if (!pImage) {
				int width, height, nrChannels;
				int len = 0;
				//10.27数据源更改
				ostringstream ost_temp;//ost_temp.str("");                
                ost_temp << _path << (zoom) << "/" << (col) << "/" << (row) << ".jpg";
                string path = ost_temp.str();
				//string path = _path + to_string(zoom) + "/" + to_string(col) + "/" + to_string(row) + ".jpg";
				//unsigned char* img = openglEngine::OpenGLFileEngine::getImgFromDB<unsigned char>(imgDB, zoom, tiles[k][2], col,len);
				//unsigned char* data = stbi_load_from_memory(img, len, &width, &height, &nrChannels, 0);
				unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
				if (data) {
					pImage = new Image(data, width * height, imIndex);
					pImage->setInfo(height, width, nrChannels);
				}

				else {
					stbi_image_free(data);
					continue;
				}
			}

			if (!pVertices || !pIndex) {
				/******************** �������� *******************/
				float* vertices = new float[size * size * 3];
				float* colors = new float[size * size * 3];

				int* index = new int[(size * size - 2 * size + 1) * 6];

				ostringstream ost_temp;//ost_temp.str("");
				ost_temp << _heightPath << (zoom) << "." << (row) << "." << (col) << ".height";
				string hPath = ost_temp.str();
				//string hPath = _heightPath + to_string(zoom) + "." + to_string(row) + "." + to_string(col) + ".height";
				int hSize = 0;
				//10.27数据源更改
				//short int* height = openglEngine::OpenGLFileEngine::getHeightFromDB<short int>(heightDB,zoom, row,col,hSize);
				short int* height = openglEngine::OpenGLFileEngine::getHeightFromBinary<short int>(hPath.c_str(), hSize);

				Vec2d leftTop, pt;
				CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
				double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / (size - 1);
				double x, y, z;


				int index_t = 0; int index_v = 0; int index_color = 0;  int index_i = 0;
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
							pt[0] = leftTop[0] + (j - 0) * span;
							pt[1] = leftTop[1] - (i - 0) * span;					
						double lat, lon;
						CGeoUtil::WebMercator2lonLat(pt[0], pt[1], lat, lon);
						if (height == 0x00)
							CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, x, y, z);
						else {
							//int h = 0 * height[i * size * 2 + j * 2];
							int h = 2 * height[i * size + j];
							CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, h, x, y, z);
						}
						float h1;
						if (height == 0x00)
							h1 = -999999;
						else
							//h1 = height[i * size * 2 + j * 2];
							h1 = height[i * size + j];
						
						vertices[index_v++] = x;
						vertices[index_v++] = y;
						vertices[index_v++] = z;
						Vec3d color = calColor(h1);
						colors[index_color++] = color[0];
						colors[index_color++] = color[1];
						colors[index_color++] = color[2];

						if (i > 0 && j > 0)
						{
							if (
								((i == (size - 1)) && (j % stride == 0)) || \
								((j == (size - 1)) && (i % stride == 0)) || \
								((i == (size - 1)) && (j == (size - 1))) || \
								((i % stride == 0) && (j % stride == 0))
								) {
								index[index_i++] = (i)*size + j;
								index[index_i++] = (i)*size + j - stride;
								index[index_i++] = (i - stride) * size + j;
								index[index_i++] = i * size + j - stride;
								index[index_i++] = (i - stride) * size + j - stride;
								index[index_i++] = (i - stride) * size + j;
							}
						}
					}
				}
                
				//GLuint* texture = new GLuint();
				//glGenTextures(1, texture);
                index_num = index_i;
				pVertices = new Vertices(vertices, size * size, vIndex);
				pColors = new Vertices(colors, size * size, cIndex);
				pIndex = new Index(index, index_num, iIndex);
				pHeight = new Heights(height, size * size, hIndex);
				buffer->setData(pColors, COLOR);
				buffer->setData(pVertices, VERTICE);
				buffer->setData(pIndex, INDEX);
				buffer->setData(pHeight, HEIGHT);
				buffer->setData(pImage, IMAGE);
				//buffer->setData(texture, TEXTURE);
				manager->insert(2, buffer, level2Index);

			}
		}
	}
	return 0;
}


int CRasterTileLayer::addBufferTIN(vector<Vec3i> tiles, int zoom, BufferManager* manager)
{

	bool isHeight = true;
	if (this->_heightPath == "null")	isHeight = false;
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;

	//height文件是66*66
	int interval = 4;
    int stride = 5; //数据抽取
	int size = 256 / interval + 2;

	for (int k = 0; k < tiles.size(); k++) {
		int zoom = tiles[k][0];
		int col = tiles[k][1];
		//int row = tiles[k][2];
		int row = pow(2, zoom) - tiles[k][2] - 1;
		//cout << zoom << "." << row << "." << col << endl;
		ostringstream ost_temp;//ost_temp.str("");
		ost_temp << (zoom) << "." << (row) << "." << (col);
		string tileIndex = ost_temp.str();
		//string tileIndex = to_string(zoom) + "." + to_string(row) + "." + to_string(col);
		string level2Index = tileIndex + "." + layerName();

		TMBuffer* buffer = manager->getFrom2LevelBuffer(level2Index);
		
		Vertices* pVertices = 0x00;
		Heights* pHeight = 0x00;
		Index* pIndex = 0x00;
		if (buffer) {
			pVertices = buffer->vData();
			pHeight = buffer->hData();
			pIndex = buffer->iData();
		}
		else {
			buffer = new TMBuffer(TerrianBuffer, level2Index);
			string vIndex = level2Index + ".vertice";
			string iIndex = level2Index + ".index";
			string hIndex = tileIndex + ".height";			

			if (!pVertices || !pIndex) {
				/******************** �������� *******************/
				float* vertices = new float[size * size * 3];

				int* index = new int[(size * size - 2 * size + 1) * 20];

				ostringstream ost_temp;//ost_temp.str("");
				ost_temp << _heightPath << (zoom) << "." << (row) << "." << (col) << ".height";
				string hPath = ost_temp.str();
				//string hPath = _heightPath + to_string(zoom) + "." + to_string(row) + "." + to_string(col) + ".height";
				int hSize = 0;
				//short int* height = openglEngine::OpenGLFileEngine::getHeightFromDB<short int>(heightDB,zoom, row,col,hSize);
				short int* height = openglEngine::OpenGLFileEngine::getHeightFromBinary<short int>(hPath.c_str(), hSize);

				Vec2d leftTop, pt;
				CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
				double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / (size - 1);
				double x, y, z;


				int index_t = 0; int index_v = 0; int index_color = 0;  int index_i = 0;
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {

						pt[0] = leftTop[0] + (j - 0) * span;
						pt[1] = leftTop[1] - (i - 0) * span;
						double lat, lon;
						CGeoUtil::WebMercator2lonLat(pt[0], pt[1], lat, lon);
						if (height == 0x00)
							CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, x, y, z);
						else {
							//int h = 0 * height[i * size * 2 + j * 2];
							int h = 2 * height[i * size + j];
							CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, h, x, y, z);
						}
						
						vertices[index_v++] = x;
						vertices[index_v++] = y;
						vertices[index_v++] = z;

						if (i > 0 && j > 0)
						{
							if (
								((i == (size - 1)) && (j % stride == 0)) || \
								((j == (size - 1)) && (i % stride == 0)) || \
								((i == (size - 1)) && (j == (size - 1))) || \
								((i % stride == 0) && (j % stride == 0))
								) {
#if 0
								index[index_i++] = (i - stride) * size + j - stride;//1
								index[index_i++] = (i)*size + j - stride;//2
								index[index_i++] = (i)*size + j - stride;//2
								index[index_i++] = (i)*size + j;//3
								index[index_i++] = (i)*size + j;//3
								index[index_i++] = (i - stride) * size + j - stride;//1
								index[index_i++] = (i - stride) * size + j - stride;//1
								index[index_i++] = (i - stride) * size + j;//4
								index[index_i++] = (i - stride) * size + j;//4
								index[index_i++] = (i)*size + j;//3
#else
								index[index_i++] = (i)*size + j;
								index[index_i++] = (i)*size + j - stride;
								index[index_i++] = (i - stride) * size + j;
								index[index_i++] = i * size + j - stride;
								index[index_i++] = (i - stride) * size + j - stride;
								index[index_i++] = (i - stride) * size + j;
#endif
							}
						}
					}
				}
                
				//GLuint* texture = new GLuint();
				//glGenTextures(1, texture);
                index_num = index_i;
				pVertices = new Vertices(vertices, size * size, vIndex);
				pIndex = new Index(index, index_num, iIndex);
				pHeight = new Heights(height, size * size, hIndex);
				buffer->setData(pVertices, VERTICE);
				buffer->setData(pIndex, INDEX);
				buffer->setData(pHeight, HEIGHT);
				//buffer->setData(texture, TEXTURE);
				manager->insert(2, buffer, level2Index);

			}
		}
	}
	return 0;
}


int CRasterTileLayer::drawMultiThreads(void* para)
{
	return 0;
}

int CRasterTileLayer::getData(vector<Vec3i>& tiles, int zoom, BufferManager* manager)
{
	Vec2i visibleZoom = this->zoom();
	if (zoom < visibleZoom[0] || zoom > visibleZoom[1])
		return -1;
	int len = tiles.size();
	for (int k = 0; k < len; k++) {
		int zoom = tiles[k][0];
		int row = tiles[k][1];
		int col = tiles[k][2];


	}

	return 0;
}

int CRasterTileLayer::heightExaggerated() const
{
	return _heightExaggerated;
}

void CRasterTileLayer::setHeightExaggerated(int height)
{
	_heightExaggerated = height;
}

string CRasterTileLayer::heightPath() const
{
	return _heightPath;
}

void CRasterTileLayer::setHeightPath(string path)
{
	_heightPath = path;
}

Vec3d CRasterTileLayer::calColor(float height)
{
	Vec3d color;
	if (height > 48)
		color = Vec3d(0.76, 0.07, 0.05);
	else if (height > 24)
		color = Vec3d(0.81, 0.41, 0.01);
	else if (height > 12)
		color = Vec3d(0.84, 0.88, 0.0);
	else if (height > 6)
		color = Vec3d(0.02, 0.60, 0.04);
	else if (height > 3)
		color = Vec3d(0.01, 0.88, 0.89);
	else if (height > -20000)
		color = Vec3d(0.00, 0.02, 0.87);
	else
		color = Vec3d(48 / 255.0, 168 / 255.0, 224 / 255.0);
	return color;
}

