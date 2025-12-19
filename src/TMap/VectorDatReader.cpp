#include "VectorDatReader.h"
#include <cstring>
#include <cmath>   // 用于 abs
#include <iostream>

static const char VECTOR_MAGIC[4] = { 'T', 'L', 'D', 'T' };

VectorDatReader::VectorDatReader()
	: _loaded(false),
	_tileCount(0),
	_indexOffset(0),
	_dataOffset(0)
{
}

VectorDatReader::~VectorDatReader()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

bool VectorDatReader::load(const std::string& path)
{
	std::lock_guard<std::recursive_mutex> guard(_mutex);
	std::cout << "[VectorDatReader] 开始加载dat文件: " << path << std::endl;

	_file.close();
	_file.clear();
	_indexMap.clear();
	_loaded = false;

	_file.open(path, std::ios::binary);
	if (!_file.is_open())
	{
		std::cerr << "[VectorDatReader] 错误: 无法打开文件 " << path << std::endl;
		return false;
	}
	std::cout << "[VectorDatReader] 文件打开成功: " << path << std::endl;

	if (!parseHeader())
	{
		std::cerr << "[VectorDatReader] 错误: 无效的文件头 " << path << std::endl;
		return false;
	}
	std::cout << "[VectorDatReader] 文件头解析成功, 瓦片数量: " << _tileCount
		<< ", 索引偏移: " << _indexOffset << ", 数据偏移: " << _dataOffset << std::endl;

	// 直接加载所有索引，无需窗口机制
	if (!readIndexEntries())
	{
		std::cerr << "[VectorDatReader] 错误: 读取索引失败 " << path << std::endl;
		return false;
	}
	std::cout << "[VectorDatReader] 索引读取成功, 索引项数量: " << _indexMap.size() << std::endl;

	_loaded = true;
	std::cout << "[VectorDatReader] dat文件加载完成: " << path << std::endl;
	return true;
}

bool VectorDatReader::parseHeader()
{
	_file.seekg(0, std::ios::beg);
	char magic[4] = { 0 };
	if (!_file.read(magic, sizeof(magic)))
		return false;

	if (std::memcmp(magic, VECTOR_MAGIC, sizeof(VECTOR_MAGIC)) != 0)
		return false;

	uint16_t version = 0;
	if (!_file.read(reinterpret_cast<char*>(&version), sizeof(version)))
		return false;

	if (!_file.read(reinterpret_cast<char*>(&_tileCount), sizeof(_tileCount)))
		return false;

	if (!_file.read(reinterpret_cast<char*>(&_indexOffset), sizeof(_indexOffset)))
		return false;

	if (!_file.read(reinterpret_cast<char*>(&_dataOffset), sizeof(_dataOffset)))
		return false;

	char reserved[6];
	if (!_file.read(reserved, sizeof(reserved)))
		return false;

	return _file.good();
}

bool VectorDatReader::readIndexEntries()
{
	std::lock_guard<std::recursive_mutex> guard(_mutex);
	if (_tileCount == 0)
	{
		return true;
	}

	_file.seekg(_indexOffset, std::ios::beg);
	for (uint32_t i = 0; i < _tileCount; ++i)
	{
		VectorTileEntry entry;
		_file.read(reinterpret_cast<char*>(&entry.z), sizeof(entry.z));
		_file.read(reinterpret_cast<char*>(&entry.x), sizeof(entry.x));
		_file.read(reinterpret_cast<char*>(&entry.y), sizeof(entry.y));
		_file.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
		_file.read(reinterpret_cast<char*>(&entry.length), sizeof(entry.length));
		uint8_t type = 0;
		uint8_t reserved = 0;
		_file.read(reinterpret_cast<char*>(&type), sizeof(type));
		_file.read(reinterpret_cast<char*>(&reserved), sizeof(reserved));
		entry.type = type;

		if (!_file)
		{
			return false;
		}

		uint64_t key = makeKey(entry.z, entry.x, entry.y, entry.type);
		_indexMap[key] = entry;
	}

	return true;
}

uint64_t VectorDatReader::makeKey(uint16_t z, uint32_t x, uint32_t y, uint8_t type) const
{
	return (static_cast<uint64_t>(z) << 48) |
		(static_cast<uint64_t>(x) << 24) |
		(static_cast<uint64_t>(y) << 8) |
		static_cast<uint64_t>(type);
}

bool VectorDatReader::hasTileData(int z, int x, int y, uint8_t type) const
{
	std::lock_guard<std::recursive_mutex> guard(_mutex);
	if (!_loaded)
	{
		std::cout << "[VectorDatReader] hasTileData: dat文件未加载" << std::endl;
		return false;
	}

	// 直接从 _indexMap 查找，无需窗口检查
	uint64_t key = makeKey(static_cast<uint16_t>(z), static_cast<uint32_t>(x), static_cast<uint32_t>(y), type);
	bool found = _indexMap.find(key) != _indexMap.end();

	if (found) {
		//std::cout << "[VectorDatReader] 找到瓦片数据: z=" << z << ", x=" << x << ", y=" << y << ", type=" << (int)type << std::endl;
	}
	else {
		//std::cout << "[VectorDatReader] 未找到瓦片数据: z=" << z << ", x=" << x << ", y=" << y << ", type=" << (int)type << std::endl;
	}
	return found;
}

bool VectorDatReader::getTileData(int z, int x, int y, uint8_t type, std::vector<unsigned char>& outData)
{
	std::lock_guard<std::recursive_mutex> guard(_mutex);
	if (!_loaded)
	{
		std::cerr << "[VectorDatReader] 错误: dat文件未加载" << std::endl;
		return false;
	}

	uint64_t key = makeKey(static_cast<uint16_t>(z), static_cast<uint32_t>(x), static_cast<uint32_t>(y), type);
	auto it = _indexMap.find(key);
	if (it == _indexMap.end())
	{
		//std::cout << "[VectorDatReader] 未找到瓦片: z=" << z << ", x=" << x << ", y=" << y << ", type=" << (int)type << std::endl;
		return false;
	}

	const VectorTileEntry& entry = it->second;
	//std::cout << "[VectorDatReader] 读取瓦片数据: z=" << z << ", x=" << x << ", y=" << y
	//	<< ", type=" << (int)type << ", offset=" << entry.offset << ", length=" << entry.length << std::endl;

	outData.resize(entry.length);
	//_file.seekg(entry.offset, std::ios::beg);
	//_file.read(reinterpret_cast<char*>(outData.data()), entry.length);

	//bool success = _file.good();
	//if (success) {
	//	std::cout << "[VectorDatReader] 成功读取 " << entry.length << " 字节数据" << std::endl;
	//}
	//else {
	//	std::cerr << "[VectorDatReader] 错误: 读取数据失败, z=" << z << ", x=" << x << ", y=" << y << ", type=" << (int)type << std::endl;
	//}
	if (entry.length > 0)
	{
		std::cout << "[VectorDatReader] 读取瓦片数据: z=" << z << ", x=" << x << ", y=" << y
		<< ", type=" << (int)type << ", offset=" << entry.offset << ", length=" << entry.length << std::endl;
		_file.seekg(entry.offset, std::ios::beg);
		_file.read(reinterpret_cast<char*>(outData.data()), entry.length);

		if (!_file.good())
		{
			std::cerr << "[VectorDatReader] 错误: 读取数据失败, z=" << z << ", x=" << x << ", y=" << y << ", type=" << (int)type << std::endl;
			return false;
		}
	}

	// 成功读取（包括空瓦片），不再打印成功日志，避免重复输出（与TileDatReader保持一致）
	return true;
}

