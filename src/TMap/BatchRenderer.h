#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include "QuadTree.h"
#include "../OpenGLEngine/openglrenderengine.h"
#include "TMUtil/tmtools.h"

const int upLimitLen = 65500; //一次渲染顶点数峰值，indices需要根据此修改

// 批处理渲染键，用于标识相同样式的要素
struct BatchKey {
    int type;           // 要素类型：0-点, 1-线, 2-面
    Color color;       // 颜色
    int lineWidth;      // 线宽
    bool isDashed;      // 是否为虚线
    int textureId;      // 纹理ID（如果有）

    // 用于比较的操作符
    bool operator<(const BatchKey& other) const {
        if (type != other.type) return type < other.type;
        if (color[0] != other.color[0]) return color[0] < other.color[0];
        if (color[1] != other.color[1]) return color[1] < other.color[1];
        if (color[2] != other.color[2]) return color[2] < other.color[2];
        if (color[3] != other.color[3]) return color[3] < other.color[3];
        if (lineWidth != other.lineWidth) return lineWidth < other.lineWidth;
        if (isDashed != other.isDashed) return isDashed < other.isDashed;
        return textureId < other.textureId;
    }
};

// 批处理组，包含相同样式的要素
struct BatchGroup {
    std::vector<float> vertices;    // 合并后的顶点数据
    std::vector<int> indices;       // 合并后的索引数据
    std::vector<int> stops;         // 合并后的线段停止点
    std::vector<int> stopOffsets;   // 每个线段的停止点偏移
    int vertexOffset;               // 当前顶点偏移

    
    BatchGroup() : vertexOffset(0) {}
    
    // 添加一个要素到批处理组
    void addFeature(const VectorFeature* feature) {
        // 记录当前停止点的偏移
        stopOffsets.push_back(stops.size());
        
        // 添加顶点数据
        for (int i = 0; i < feature->vertexCount * 3; i++) {
            vertices.push_back(feature->vertices[i]);
        }
        
        // 添加索引数据（需要调整偏移）
        for (int i = 0; i < feature->indexCount; i++) {
            indices.push_back((feature->indices[i] + vertexOffset)%upLimitLen);
        }
        
        // 添加停止点数据（需要调整偏移）
        // polyline数据不需要调整偏移，stop值代表间隔几个点，不是索引
        for (int i = 0; i < feature->stopCount; i++) {
            stops.push_back(feature->stops[i]);
        }
        
        // 更新顶点偏移
        vertexOffset += feature->vertexCount;
    }
};

// 批处理渲染管理器
class BatchRenderer {
private:
    // 按样式分组的批处理组
    std::map<BatchKey, BatchGroup> batchGroups;
    
    // 四叉树空间索引
    QuadTree spatialIndex;
    std::vector<VectorFeature*> allfeatures;
    
public:
    // 添加要素到批处理渲染器
    void addFeature(VectorFeature* feature) {
        // 添加到空间索引
        // spatialIndex.addFeature(feature);
        allfeatures.push_back(feature);
    }
    //清理空间索引
    void clear() {
        // 清空空间索引
        spatialIndex = QuadTree(); // 重新创建一个空的四叉树
        allfeatures.clear();
    }
    // 准备批处理组
    void prepareBatches(double viewMinX, double viewMinY, double viewMaxX, double viewMaxY) {
        // 清空当前批处理组
        batchGroups.clear();
        
        // 查询可见要素
        // std::vector<VectorFeature*> visibleFeatures = 
        //     spatialIndex.queryVisibleFeatures(viewMinX, viewMinY, viewMaxX, viewMaxY);
        
        // 按样式分组
        for (auto feature : allfeatures) {
            BatchKey key = {
                feature->type,
                feature->color,
                feature->lineWidth,
                feature->isDashed,
                0  // 默认无纹理
            };
            
            // 添加到相应的批处理组
            batchGroups[key].addFeature(feature);
        }
    }
    
    // 渲染所有批处理组
    void render() {
        for (const auto& pair : batchGroups) {
            const BatchKey& key = pair.first;
            const BatchGroup& group = pair.second;
            
            // 跳过空组
            if (group.vertices.empty()) continue;
            
            // 准备渲染数据
            float* vertices = new float[group.vertices.size()];
            std::copy(group.vertices.begin(), group.vertices.end(), vertices);
            
            int* indices = nullptr;
            if (!group.indices.empty()) {
                indices = new int[group.indices.size()];
                std::copy(group.indices.begin(), group.indices.end(), indices);
            }
            /*static int lastVertexCount = 0;
            int currentVertexCount = group.vertices.size()/3;
            if (currentVertexCount != lastVertexCount) {
                printf("批处理类型：%d,颜色：[%d,%d,%d,%d], 定点数变化：%d -> %d (差异: %d)\n", 
                       key.type, key.color[0], key.color[1], key.color[2], key.color[3],
                       lastVertexCount, currentVertexCount, currentVertexCount - lastVertexCount);
            }
            lastVertexCount = currentVertexCount;*/
            
            // 根据要素类型选择渲染方法
            switch (key.type) {
                case 0:  // 点
                    // 点渲染逻辑
                    break;
                    
                case 1:  // 线
                    {
                        // 准备停止点数据
                        int* stops = new int[group.stops.size()];
                        std::copy(group.stops.begin(), group.stops.end(), stops);
                        
                        // 渲染线
                        openglEngine::OpenGLRenderEngine::drawLines(
                            vertices, 
                            stops, 
                            group.stops.size(),
                            key.color, 
                            key.lineWidth, 
                            key.isDashed ? 1 : 0
                        );
                        
                        delete[] stops;
                    }
                    break;
                    
                case 2:  // 面
                    // 渲染面
                    int upLimitLen = 65499; //一次渲染顶点数峰值，indices需要根据此修改	//�޶�����������������
                    int times = 0;//��Ҫ���ƵĴ���
                    int vertices_size = group.vertices.size()/3;
                    /*for (int i = 2; i < vertices_size; i=i+3) {
                        cout << vertices[i] << " ";
                    }*/
                    if (vertices_size % upLimitLen)
                        times = vertices_size / upLimitLen + 1;
                    else
                        times = vertices_size / upLimitLen;
                    int trueLen = 0;//ÿ�λ��Ƶ���ʵ����
                    for (int i = 0; i < times; i++) {
                        if (i == times - 1) {
                            if (vertices_size % upLimitLen)
                                trueLen = vertices_size % upLimitLen;
                            else
                                trueLen = upLimitLen;
                        }
                        else {
                            trueLen = upLimitLen;
                        }
                        //openglEngine::OpenGLRenderEngine::drawTriangles<float>(&vertices[upLimitLen * i * 3], key.color, trueLen);
                        //vertices_size>65500时，indices需要减去偏移量
                        //openglEngine::OpenGLRenderEngine::drawTriangles<float>(&vertices[upLimitLen * i * 3], &indices[upLimitLen * i], key.color, trueLen);
                    }
                    openglEngine::OpenGLRenderEngine::drawTriangles<float>(vertices, key.color, vertices_size);
                    break;
            }
            
            // 清理临时数据
            delete[] vertices;
            if (indices) delete[] indices;
        }
    }
}; 