#ifndef VECTOR_DAT_READER_H
#define VECTOR_DAT_READER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdint>
#include <mutex>

struct VectorTileEntry
{
    uint16_t z;
    uint32_t x;
    uint32_t y;
    uint8_t type;
    uint64_t offset;
    uint32_t length;
};

class VectorDatReader
{
public:
    VectorDatReader();
    ~VectorDatReader();

    bool load(const std::string& path);
    bool isLoaded() const { return _loaded; }

    bool getTileData(int z, int x, int y, uint8_t type, std::vector<unsigned char>& outData);

    // 快速检查瓦片数据是否存在（不读取数据，只检查索引）
    bool hasTileData(int z, int x, int y, uint8_t type) const;

    // 批量检查多个候选位置，返回第一个找到的位置（优化性能，减少锁竞争）
    // 返回找到的索引，如果都没找到返回 -1
    int findFirstAvailable(int z, uint8_t type, const std::vector<std::pair<int, int>>& candidates) const;

private:
    bool parseHeader();
    bool readIndexEntries();
    uint64_t makeKey(uint16_t z, uint32_t x, uint32_t y, uint8_t type) const;

    std::ifstream _file;
    bool _loaded;

    uint32_t _tileCount;
    uint64_t _indexOffset;
    uint64_t _dataOffset;

    std::unordered_map<uint64_t, VectorTileEntry> _indexMap;
#ifdef _WIN32
    mutable std::recursive_mutex _mutex;
#else
    mutable pthread_mutex_t _mutex;
#endif
};

#endif // VECTOR_DAT_READER_H

