#ifndef TILE_DAT_READER_H
#define TILE_DAT_READER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdint>

struct TileIndexEntry
{
    uint16_t z;
    uint32_t x;
    uint32_t y;
    uint64_t offset;
    uint32_t length;
};

class TileDatReader
{
public:
    TileDatReader();
    ~TileDatReader();

    bool load(const std::string& path);
    bool isLoaded() const { return _loaded; }

    bool getTile(int z, int x, int y, std::vector<unsigned char>& outData);
    void setWindowRadius(int radius);
    bool ensureWindow(uint16_t z, uint32_t centerX, uint32_t centerY);

    // 批量预加载指定瓦片的索引（不加载数据）
    void preloadIndices(const std::vector<std::tuple<int, int, int>>& tiles);

private:
    bool parseHeader();
    bool loadWindow(uint16_t z, uint32_t centerX, uint32_t centerY);
    bool isWithinWindow(uint16_t z, uint32_t x, uint32_t y) const;
    uint64_t makeKey(uint16_t z, uint32_t x, uint32_t y) const;
    bool findAndCacheIndex(uint16_t z, uint32_t x, uint32_t y);
    std::ifstream _file;
    std::string _path;
    bool _loaded;

    uint32_t _tileCount;
    uint64_t _indexOffset;
    uint64_t _dataOffset;

    std::unordered_map<uint64_t, TileIndexEntry> _indexMap;
    int _windowRadius;
    bool _windowValid;
    uint16_t _windowZ;
    uint32_t _windowCenterX;
    uint32_t _windowCenterY;


    // 内存保护：最大缓存索引数量（防止高 zoom 级别时内存爆满）
    static constexpr size_t MAX_CACHED_INDICES = 10000; // 约 240KB 内存
};

#endif // TILE_DAT_READER_H

