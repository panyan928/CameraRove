#include "TileDatReader.h"
#include <cstring>
#include <iostream>
#include <cmath>
#include <set>
#include <tuple>

static const char MAGIC_NUMBER[4] = { 'T', 'L', 'D', 'T' };

TileDatReader::TileDatReader()
    : _loaded(false),
    _tileCount(0),
    _indexOffset(0),
    _dataOffset(0),
    _windowRadius(16),
    _windowValid(false),
    _windowZ(0),
    _windowCenterX(0),
    _windowCenterY(0)
{
}

TileDatReader::~TileDatReader()
{
    if (_file.is_open())
    {
        _file.close();
    }
}

bool TileDatReader::load(const std::string& path)
{
    _file.close();
    _file.clear();
    _indexMap.clear();
    _loaded = false;
    _windowValid = false;

    _file.open(path, std::ios::binary);
    if (!_file.is_open())
    {
        std::cerr << "TileDatReader: Failed to open file " << path << std::endl;
        return false;
    }

    _path = path;

    if (!parseHeader())
    {
        std::cerr << "TileDatReader: Invalid header " << path << std::endl;
        return false;
    }

    _loaded = true;
    return true;
}

bool TileDatReader::parseHeader()
{
    _file.seekg(0, std::ios::beg);
    char magic[4] = { 0 };
    if (!_file.read(magic, sizeof(magic)))
        return false;

    if (std::memcmp(magic, MAGIC_NUMBER, sizeof(MAGIC_NUMBER)) != 0)
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

uint64_t TileDatReader::makeKey(uint16_t z, uint32_t x, uint32_t y) const
{
    return (static_cast<uint64_t>(z) << 42) |
        (static_cast<uint64_t>(x) << 21) |
        static_cast<uint64_t>(y);
}

bool TileDatReader::getTile(int z, int x, int y, std::vector<unsigned char>& outData)
{
    if (!_loaded)
    {
        return false;
    }

    uint16_t tileZ = static_cast<uint16_t>(z);
    uint32_t tileX = static_cast<uint32_t>(x);
    uint32_t tileY = static_cast<uint32_t>(y);

    // 更新窗口中心（如果变化较大）
    if (!_windowValid || _windowZ != tileZ)
    {
        ensureWindow(tileZ, tileX, tileY);
    }
    else
    {
        int64_t dx = static_cast<int64_t>(tileX) - static_cast<int64_t>(_windowCenterX);
        int64_t dy = static_cast<int64_t>(tileY) - static_cast<int64_t>(_windowCenterY);
        int threshold = _windowRadius;
        if (std::abs(dx) > threshold || std::abs(dy) > threshold)
        {
            ensureWindow(tileZ, tileX, tileY);
        }
    }

    // 先检查缓存
    uint64_t key = makeKey(tileZ, tileX, tileY);
    auto it = _indexMap.find(key);

    // 如果不在缓存中，按需查找索引
    if (it == _indexMap.end())
    {
        if (!findAndCacheIndex(tileZ, tileX, tileY))
        {
            return false;
        }
        it = _indexMap.find(key);
        if (it == _indexMap.end())
        {
            return false;
        }
    }

    // 按需读取数据：只在真正需要时才从文件读取
    const TileIndexEntry& entry = it->second;
    outData.resize(entry.length);
    _file.seekg(entry.offset, std::ios::beg);
    _file.read(reinterpret_cast<char*>(outData.data()), entry.length);
    return _file.good();
}

void TileDatReader::setWindowRadius(int radius)
{
    if (radius < 1)
    {
        radius = 1;
    }
    _windowRadius = radius;
    _windowValid = false;
}

bool TileDatReader::ensureWindow(uint16_t z, uint32_t centerX, uint32_t centerY)
{
    if (!_loaded)
    {
        return false;
    }

    // 如果窗口已有效且中心未变化太多，无需重新加载
    if (_windowValid && _windowZ == z)
    {
        int64_t dx = static_cast<int64_t>(centerX) - static_cast<int64_t>(_windowCenterX);
        int64_t dy = static_cast<int64_t>(centerY) - static_cast<int64_t>(_windowCenterY);
        int threshold = _windowRadius / 2;
        if (std::abs(dx) <= threshold && std::abs(dy) <= threshold)
        {
            return true;
        }
    }

    return loadWindow(z, centerX, centerY);
}

bool TileDatReader::isWithinWindow(uint16_t z, uint32_t x, uint32_t y) const
{
    if (!_windowValid || z != _windowZ)
    {
        return false;
    }

    int64_t dx = static_cast<int64_t>(x) - static_cast<int64_t>(_windowCenterX);
    int64_t dy = static_cast<int64_t>(y) - static_cast<int64_t>(_windowCenterY);
    return std::abs(dx) <= _windowRadius && std::abs(dy) <= _windowRadius;
}

bool TileDatReader::loadWindow(uint16_t z, uint32_t centerX, uint32_t centerY)
{
    // 清理超出窗口的缓存条目（只清理当前 zoom 的）
    if (_windowValid && _windowZ == z)
    {
        std::vector<uint64_t> toRemove;
        for (const auto& pair : _indexMap)
        {
            const TileIndexEntry& entry = pair.second;
            // 只清理当前 zoom 且超出窗口的
            if (entry.z == z)
            {
                int64_t dx = static_cast<int64_t>(entry.x) - static_cast<int64_t>(centerX);
                int64_t dy = static_cast<int64_t>(entry.y) - static_cast<int64_t>(centerY);
                if (std::abs(dx) > _windowRadius || std::abs(dy) > _windowRadius)
                {
                    toRemove.push_back(pair.first);
                }
            }
        }
        for (uint64_t key : toRemove)
        {
            _indexMap.erase(key);
        }
    }
    else if (_windowValid && _windowZ != z)
    {
        // zoom变化时只清空旧 zoom 的缓存，保留其他 zoom 的（如果有）
        std::vector<uint64_t> toRemove;
        for (const auto& pair : _indexMap)
        {
            if (pair.second.z == _windowZ)
            {
                toRemove.push_back(pair.first);
            }
        }
        for (uint64_t key : toRemove)
        {
            _indexMap.erase(key);
        }
    }

    _windowValid = true;
    _windowZ = z;
    _windowCenterX = centerX;
    _windowCenterY = centerY;

    std::cout << "TileDatReader: window z=" << z
        << " center(" << centerX << "," << centerY << ")"
        << " radius=" << _windowRadius
        << " cached_entries=" << _indexMap.size() << std::endl;
    return true;
}

bool TileDatReader::findAndCacheIndex(uint16_t z, uint32_t x, uint32_t y)
{
    if (_tileCount == 0)
    {
        return false;
    }

    // 检查是否在窗口范围内
    if (_windowValid && _windowZ == z)
    {
        int64_t dx = static_cast<int64_t>(x) - static_cast<int64_t>(_windowCenterX);
        int64_t dy = static_cast<int64_t>(y) - static_cast<int64_t>(_windowCenterY);
        if (std::abs(dx) > _windowRadius || std::abs(dy) > _windowRadius)
        {
            return false;
        }
    }

    // 按需扫描索引区，找到目标条目后立即返回
    _file.seekg(_indexOffset, std::ios::beg);

    for (uint32_t i = 0; i < _tileCount; ++i)
    {
        TileIndexEntry entry;
        _file.read(reinterpret_cast<char*>(&entry.z), sizeof(entry.z));
        _file.read(reinterpret_cast<char*>(&entry.x), sizeof(entry.x));
        _file.read(reinterpret_cast<char*>(&entry.y), sizeof(entry.y));
        _file.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        _file.read(reinterpret_cast<char*>(&entry.length), sizeof(entry.length));
        uint16_t reserved;
        _file.read(reinterpret_cast<char*>(&reserved), sizeof(reserved));

        if (!_file)
        {
            return false;
        }

        // 找到目标条目
        if (entry.z == z && entry.x == x && entry.y == y)
        {
            // 检查是否在窗口范围内
            if (_windowValid && _windowZ == z)
            {
                int64_t dx = static_cast<int64_t>(x) - static_cast<int64_t>(_windowCenterX);
                int64_t dy = static_cast<int64_t>(y) - static_cast<int64_t>(_windowCenterY);
                if (std::abs(dx) <= _windowRadius && std::abs(dy) <= _windowRadius)
                {
                    uint64_t key = makeKey(entry.z, entry.x, entry.y);
                    _indexMap[key] = entry;
                    return true;
                }
            }
            else
            {
                // 窗口未设置时也缓存
                uint64_t key = makeKey(entry.z, entry.x, entry.y);
                _indexMap[key] = entry;
                return true;
            }
            return false;
        }
    }

    return false;
}

void TileDatReader::preloadIndices(const std::vector<std::tuple<int, int, int>>& tiles)
{
    if (!_loaded || _tileCount == 0 || tiles.empty())
    {
        return;
    }

    // 计算窗口范围内的最大索引数量（用于限制预加载）
    size_t maxWindowIndices = 0;
    if (_windowValid)
    {
        // 窗口范围内的瓦片数量 = (2 * radius + 1)^2
        size_t windowSize = static_cast<size_t>(2 * _windowRadius + 1);
        maxWindowIndices = windowSize * windowSize;
    }
    else
    {
        // 窗口未设置时，使用默认限制
        maxWindowIndices = MAX_CACHED_INDICES / 2;
    }

    // 内存保护：限制预加载数量为窗口大小
    size_t maxPreload = std::min(tiles.size(), maxWindowIndices);
    if (tiles.size() > maxPreload && _windowValid)
    {
        std::cout << "TileDatReader: tiles list (" << tiles.size()
            << ") exceeds window capacity (" << maxWindowIndices
            << "), limiting preload to window size" << std::endl;
    }

    // 内存保护：如果当前缓存已接近上限，先清理一部分
    if (_indexMap.size() >= MAX_CACHED_INDICES * 0.8)
    {
        // 清理超出窗口范围的旧缓存（更激进的清理）
        if (_windowValid)
        {
            std::vector<uint64_t> toRemove;
            for (const auto& pair : _indexMap)
            {
                const TileIndexEntry& entry = pair.second;
                if (entry.z == _windowZ)
                {
                    int64_t dx = static_cast<int64_t>(entry.x) - static_cast<int64_t>(_windowCenterX);
                    int64_t dy = static_cast<int64_t>(entry.y) - static_cast<int64_t>(_windowCenterY);
                    // 使用更小的半径进行清理（只保留窗口中心附近的）
                    int cleanupRadius = _windowRadius / 2;
                    if (std::abs(dx) > cleanupRadius || std::abs(dy) > cleanupRadius)
                    {
                        toRemove.push_back(pair.first);
                    }
                }
            }
            size_t removed = toRemove.size();
            for (uint64_t key : toRemove)
            {
                _indexMap.erase(key);
            }
            if (removed > 0)
            {
                std::cout << "TileDatReader: cleaned " << removed
                    << " indices to free memory" << std::endl;
            }
        }

        // 如果清理后仍然超过上限，强制限制
        if (_indexMap.size() >= MAX_CACHED_INDICES)
        {
            // 保留最近的 N 个条目，删除最旧的
            size_t toKeep = MAX_CACHED_INDICES * 3 / 4;
            if (_indexMap.size() > toKeep)
            {
                // 简单策略：删除所有非当前 zoom 的条目
                std::vector<uint64_t> toRemove;
                for (const auto& pair : _indexMap)
                {
                    if (_windowValid && pair.second.z != _windowZ)
                    {
                        toRemove.push_back(pair.first);
                    }
                }
                for (uint64_t key : toRemove)
                {
                    _indexMap.erase(key);
                }
                std::cout << "TileDatReader: forced cleanup, removed " << toRemove.size()
                    << " indices from other zoom levels" << std::endl;
            }
        }
    }

    // 构建需要查找的瓦片集合（去重）
    std::set<std::tuple<uint16_t, uint32_t, uint32_t>> targetTiles;
    size_t maxToAdd = MAX_CACHED_INDICES - _indexMap.size(); // 计算还能添加多少
    size_t addedCount = 0;

    for (const auto& tile : tiles)
    {
        // 内存保护：限制预加载数量
        if (addedCount >= maxPreload || targetTiles.size() >= maxToAdd)
        {
            if (addedCount >= maxPreload)
            {
                std::cout << "TileDatReader: WARNING - limiting preload to " << maxPreload
                    << " tiles (requested " << tiles.size() << ") to prevent memory overflow" << std::endl;
            }
            break;
        }

        int z = std::get<0>(tile);
        int x = std::get<1>(tile);
        int y = std::get<2>(tile);

        // 检查是否已在缓存中
        uint64_t key = makeKey(static_cast<uint16_t>(z), static_cast<uint32_t>(x), static_cast<uint32_t>(y));
        if (_indexMap.find(key) != _indexMap.end())
        {
            continue; // 已缓存，跳过
        }

        targetTiles.insert(std::make_tuple(static_cast<uint16_t>(z), static_cast<uint32_t>(x), static_cast<uint32_t>(y)));
        addedCount++;
    }

    if (targetTiles.empty())
    {
        return; // 所有索引都已缓存
    }

    // 一次性扫描索引区，找到所有需要的索引
    _file.seekg(_indexOffset, std::ios::beg);
    uint32_t foundCount = 0;

    for (uint32_t i = 0; i < _tileCount && !targetTiles.empty(); ++i)
    {
        TileIndexEntry entry;
        _file.read(reinterpret_cast<char*>(&entry.z), sizeof(entry.z));
        _file.read(reinterpret_cast<char*>(&entry.x), sizeof(entry.x));
        _file.read(reinterpret_cast<char*>(&entry.y), sizeof(entry.y));
        _file.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
        _file.read(reinterpret_cast<char*>(&entry.length), sizeof(entry.length));
        uint16_t reserved;
        _file.read(reinterpret_cast<char*>(&reserved), sizeof(reserved));

        if (!_file)
        {
            break;
        }

        // 检查是否是目标瓦片
        auto it = targetTiles.find(std::make_tuple(entry.z, entry.x, entry.y));
        if (it != targetTiles.end())
        {
            // 检查是否在窗口范围内（如果窗口已设置）
            bool shouldCache = true;
            if (_windowValid && _windowZ == entry.z)
            {
                int64_t dx = static_cast<int64_t>(entry.x) - static_cast<int64_t>(_windowCenterX);
                int64_t dy = static_cast<int64_t>(entry.y) - static_cast<int64_t>(_windowCenterY);
                shouldCache = (std::abs(dx) <= _windowRadius && std::abs(dy) <= _windowRadius);
            }

            if (shouldCache)
            {
                uint64_t key = makeKey(entry.z, entry.x, entry.y);
                _indexMap[key] = entry;
                foundCount++;
            }

            targetTiles.erase(it); // 从待查找集合中移除（无论是否缓存）
        }
    }

    std::cout << "TileDatReader: preloaded " << foundCount
        << " indices from " << tiles.size() << " requested tiles" << std::endl;
}

