#pragma once

#include <vector>
#include <memory>
#include < unordered_set >
#include "../TMUtil/cgeoutil.h"
#include "TMUtil/tmtools.h"

// 矢量要素的简单封装
struct VectorFeature {
    int type;               // 要素类型：0-点, 1-线, 2-面
    float* vertices;        // 顶点数据
    int vertexCount;        // 顶点数量
    int* indices;           // 索引数据
    int indexCount;         // 索引数量
    int* stops;             // 线段停止点
    int stopCount;          // 停止点数量
    Color color;           // 颜色
    int lineWidth;          // 线宽
    bool isDashed;          // 是否为虚线
    double minX, minY;      // 包围盒最小点
    double maxX, maxY;      // 包围盒最大点

    VectorFeature() : vertices(nullptr), indices(nullptr), stops(nullptr), 
                     vertexCount(0), indexCount(0), stopCount(0), 
                     lineWidth(1), isDashed(false) {}
    
    ~VectorFeature() {
        if (vertices) delete[] vertices;
        if (indices) delete[] indices;
        if (stops) delete[] stops;
    }
};

// 四叉树节点
class QuadTreeNode {
public:
    // 节点范围
    double minX, minY, maxX, maxY;
    
    // 子节点
    std::unique_ptr<QuadTreeNode> children[4];
    
    // 当前节点包含的要素
    std::vector<VectorFeature*> features;
    
    // 最大要素数和最大深度
    static const int MAX_FEATURES = 10;
    static const int MAX_DEPTH = 8;
    
    QuadTreeNode(double minX, double minY, double maxX, double maxY)
        : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }
    
    // 判断要素是否与节点相交
    bool intersects(const VectorFeature* feature) const {
        return !(feature->maxX < minX || feature->minX > maxX ||
                feature->maxY < minY || feature->minY > maxY);
    }
    
    // 判断节点是否完全包含在视口内
    bool isContainedIn(double viewMinX, double viewMinY, double viewMaxX, double viewMaxY) const {
        return minX >= viewMinX && maxX <= viewMaxX && minY >= viewMinY && maxY <= viewMaxY;
    }
    
    // 判断节点是否与视口相交
    bool intersectsWithView(double viewMinX, double viewMinY, double viewMaxX, double viewMaxY) const {
        return !(maxX < viewMinX || minX > viewMaxX || maxY < viewMinY || minY > viewMaxY);
    }
    
    // 细分节点
    void split(int depth) {
        if (depth >= MAX_DEPTH) return;
        
        double midX = (minX + maxX) / 2;
        double midY = (minY + maxY) / 2;
        
        // 创建四个子节点
        children[0] = std::make_unique<QuadTreeNode>(minX, minY, midX, midY);     // 左下
        children[1] = std::make_unique<QuadTreeNode>(midX, minY, maxX, midY);     // 右下
        children[2] = std::make_unique<QuadTreeNode>(minX, midY, midX, maxY);     // 左上
        children[3] = std::make_unique<QuadTreeNode>(midX, midY, maxX, maxY);     // 右上
        
        // 重新分配要素到子节点
        for (auto feature : features) {
            for (int i = 0; i < 4; i++) {
                if (children[i]->intersects(feature)) {
                    children[i]->insert(feature, depth + 1);
                }
            }
        }
        
        // 清空当前节点的要素
        features.clear();
    }
    
    // 插入要素
    void insert(VectorFeature* feature, int depth) {
        // 如果已经有子节点，则插入到子节点
        if (children[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                if (children[i]->intersects(feature)) {
                    children[i]->insert(feature, depth + 1);
                }
            }
            return;
        }
        
        // 添加到当前节点
        features.push_back(feature);
        
        // 如果超过阈值且未达到最大深度，则细分
        if (features.size() > MAX_FEATURES && depth < MAX_DEPTH) {
            split(depth);
        }
    }
    
    // 查询视口内的要素
    void query(double viewMinX, double viewMinY, double viewMaxX, double viewMaxY, 
              std::vector<VectorFeature*>& result) const {
        // 如果节点不与视口相交，直接返回
        if (!intersectsWithView(viewMinX, viewMinY, viewMaxX, viewMaxY)) {
            return;
        }
        
        // 如果有子节点，递归查询子节点
        if (children[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                children[i]->query(viewMinX, viewMinY, viewMaxX, viewMaxY, result);
            }
            return;
        }
        
        // 添加当前节点中与视口相交的要素
        for (auto feature : features) {
            if (!(feature->maxX < viewMinX || feature->minX > viewMaxX ||
                  feature->maxY < viewMinY || feature->minY > viewMaxY)) {
                result.push_back(feature);
            }
        }
    }
};

// 四叉树
class QuadTree {
private:
    std::unique_ptr<QuadTreeNode> root;
    std::vector<std::unique_ptr<VectorFeature>> allFeatures;  // 存储所有要素的所有权
    
public:
    // 创建四叉树，使用墨卡托坐标系的范围
    QuadTree() : root(std::make_unique<QuadTreeNode>(-20037508.34, -20037508.34, 20037508.34, 20037508.34)) {}
    
    // 添加要素
    void addFeature(VectorFeature* feature) {
        // 计算要素的包围盒
        calculateBoundingBox(feature);
        
        // 插入到四叉树
        root->insert(feature, 0);
        
        // 存储要素的所有权
        allFeatures.push_back(std::unique_ptr<VectorFeature>(feature));
    }
    
    // 计算要素的包围盒
    void calculateBoundingBox(VectorFeature* feature) {
        if (feature->vertexCount == 0) return;
        
        feature->minX = feature->maxX = feature->vertices[0];
        feature->minY = feature->maxY = feature->vertices[1];
        
        for (int i = 1; i < feature->vertexCount; i++) {
            double x = feature->vertices[i * 3];
            double y = feature->vertices[i * 3 + 1];
            
            feature->minX = std::min(feature->minX, x);
            feature->minY = std::min(feature->minY, y);
            feature->maxX = std::max(feature->maxX, x);
            feature->maxY = std::max(feature->maxY, y);
        }
    }
    
    // 查询视口内的要素
    std::vector<VectorFeature*> queryVisibleFeatures(double viewMinX, double viewMinY, double viewMaxX, double viewMaxY) {
        std::vector<VectorFeature*> rawResults;
        root->query(viewMinX, viewMinY, viewMaxX, viewMaxY, rawResults);
        // 使用集合去重
        std::unordered_set<VectorFeature*> uniqueFeatures;
        std::vector<VectorFeature*> result;
        
        for (auto feature : rawResults) {
            if (uniqueFeatures.insert(feature).second) {
                result.push_back(feature);
            }
        }
        return result;
    }
}; 