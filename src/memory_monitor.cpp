#include "memory_monitor.h"
#include <iostream>
#include <iomanip>

// 全局调试标志定义
bool _Debug = false;
bool _MemoryDebug = false;

// 获取当前进程内存使用情况（字节）
size_t getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
}

// 打印内存使用情况
void printMemoryUsage() {
    size_t currentMemory = getCurrentMemoryUsage();
    size_t memoryMB = currentMemory / (1024 * 1024);

    std::cout << "Current Memory Usage: " << memoryMB << " MB ("
        << currentMemory << " bytes)" << std::endl;

    // 获取系统内存信息
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        size_t totalMB = memInfo.ullTotalPhys / (1024 * 1024);
        size_t availableMB = memInfo.ullAvailPhys / (1024 * 1024);
        size_t usedMB = totalMB - availableMB;

        std::cout << "System Memory: " << usedMB << " MB used / "
            << totalMB << " MB total ("
            << std::fixed << std::setprecision(1)
            << (double)usedMB / totalMB * 100 << "% used)" << std::endl;
    }
}

// 检查可用内存是否足够
bool checkAvailableMemory(size_t requiredSize) {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        size_t availableBytes = memInfo.ullAvailPhys;
        return availableBytes > requiredSize;
    }
    return false;
}

// 检查内存分配是否安全
bool isMemoryAllocationSafe(size_t size) {
    // 检查分配大小是否合理（小于1GB）
    if (size > 1024 * 1024 * 1024) {
        if (_Debug) {
            std::cout << "Allocation size too large: " << size / (1024 * 1024) << " MB" << std::endl;
        }
        return false;
    }

    // 检查系统可用内存是否足够
    if (!checkAvailableMemory(size)) {
        if (_Debug) {
            std::cout << "Insufficient system memory for allocation: " << size / (1024 * 1024) << " MB" << std::endl;
        }
        return false;
    }

    // 检查当前进程内存使用情况
    size_t currentMemory = getCurrentMemoryUsage();
    size_t projectedMemory = currentMemory + size;

    // 如果分配后内存使用超过2GB，认为不安全
    if (projectedMemory > 2ULL * 1024 * 1024 * 1024) {
        if (_Debug) {
            std::cout << "Projected memory usage too high: "
                << projectedMemory / (1024 * 1024) << " MB" << std::endl;
        }
        return false;
    }

    return true;
}









