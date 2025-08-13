#include "gl\gl.h"	
#include "gl\glu.h"

// #include "TMUtil\tmtools.h"
#include "TMUtil\CGeoUtil.h"
#include "OpenGLEngine/openglengine.h"
#include "airlineDraw.h"

/**
 * @brief 根据经纬度坐标点绘制折线
 * @param geoPoints 经纬度坐标点数组，每个点包含[经度, 纬度]
 * @param pointCount 坐标点数量
 * @param color 线条颜色，RGB值范围0-255
 * @param lineWidth 线宽
 * @param isDashed 是否绘制虚线
 */
void drawGeoPolyline(double* geoPoints, int pointCount, Color color, int lineWidth = 2, bool isDashed = false) {
    if (geoPoints == nullptr || pointCount < 2) {
        return;
    }

    // 分配内存用于存储墨卡托坐标
    float* mercatorPoints = new float[pointCount * 3];
    
    // 将经纬度坐标转换为墨卡托坐标
    for (int i = 0; i < pointCount; i++) {
        double lon = geoPoints[i * 2];     // 经度
        double lat = geoPoints[i * 2 + 1]; // 纬度
        double x, y;
        
        // 转换为墨卡托坐标
        CGeoUtil::lonLat2WebMercator(lat, lon, x, y);
        
        // 存储墨卡托坐标，z坐标设为0.1
        mercatorPoints[i * 3] = (float)x;
        mercatorPoints[i * 3 + 1] = (float)y;
        mercatorPoints[i * 3 + 2] = 0.1f;
    }
    // 禁用可能影响颜色的状态
    glDisable(GL_TEXTURE_2D);
    // 设置线条属性
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mercatorPoints);
    glColor3f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f);
    
    // 绘制线条
    if (isDashed) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xF0F0); // 虚线模式
    }
    
    glLineWidth(lineWidth);
    glDrawArrays(GL_LINE_STRIP, 0, pointCount);
    
    if (isDashed) {
        glDisable(GL_LINE_STIPPLE);
    }
    
    // 清理
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_LINE_SMOOTH);
    
    // 释放内存
    delete[] mercatorPoints;
}

// 示例：绘制多条不同样式的路线
void drawSampleRoute() {
    // 示例1：北京-上海-广州
    double routePoints[] = {
        116.3912, 39.9073,  // 北京天安门
        121.4737, 31.2304,  // 上海人民广场
        113.2644, 23.1291  // 广州塔
    };
    
    // 绘制红色实线路线，线宽2像素
    Color redColor(255, 0, 0, 255); // 红色
    drawGeoPolyline(routePoints, 4, redColor, 2, false);
    
    //// 绘制蓝色虚线路线，线宽2像素
    //Color blueColor(0, 0, 255, 255); // 蓝色
    //drawGeoPolyline(routePoints2, 4, blueColor, 2, true);
    //
    //// 绘制绿色粗实线路线，线宽5像素
    //Color greenColor(0, 255, 0, 255); // 绿色
    //drawGeoPolyline(routePoints3, 4, greenColor, 5, false);
}

void drawSamplePoint(double lon, double lat) {
	double x, y;
    CGeoUtil::lonLat2WebMercator(lat, lon, x, y);
    float point[3];
    point[0] = x;
    point[1] = y;
    point[2] = 0.1f;
    
    // 禁用可能影响颜色的状态
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);  // 启用点平滑
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glPointSize(20.0);  // 设置点大小
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, point);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    
    glDrawArrays(GL_POINTS, 0, 1);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);
}