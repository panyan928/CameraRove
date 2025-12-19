#pragma once
#pragma once
#include <iostream>
#include <windows.h>
#include <psapi.h>

// 全局调试标志
extern bool _Debug;
extern bool _MemoryDebug;

// 内存监控函数声明
void printMemoryUsage();
bool isMemoryAllocationSafe(size_t size);

// 获取当前进程内存使用情况
size_t getCurrentMemoryUsage();

// 检查可用内存是否足够
bool checkAvailableMemory(size_t requiredSize);









