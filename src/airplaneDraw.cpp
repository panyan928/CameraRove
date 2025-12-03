#include <stdio.h>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include "gl\gl.h"
#include "TMUtil/tmtools.h"
#include "TMUtil/OMGeoUtil.h"
#include "stb_image.h"

#define M_PI 3.141592653

// 新增函数：绘制飞机图标
void drawAirplaneIcon(Vec2d  posMec, Vec2d center, float aircraftHeading, float viewportWidth, float viewportHeight) {
    // 保存模型视图矩阵
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    //gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    // 保存当前所有相关状态
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

    // 设置固定的飞机颜色（例如白色）
    glColor3f(1.0f, 1.0f, 1.0f); // 白色

    // 禁用可能影响颜色的状态
    glDisable(GL_LIGHTING);      // 禁用光照
    glDisable(GL_TEXTURE_2D);    // 禁用纹理
    glDisable(GL_BLEND);         // 禁用混合
    glDisable(GL_CULL_FACE);      // 禁用面剔除
    glDisable(GL_DEPTH_TEST);     // 禁用深度测试（确保在最前面）

    double centerX = (center[0] - posMec[0]) * 2 / viewportWidth;
    double centerY = (center[1] - posMec[1]) * 2 / viewportHeight;
    //centerX = centerX * cos(rotationRad) - centerY * sin(rotationRad);
    //centerY = centerX * sin(rotationRad) + centerY * cos(rotationRad);
    // 计算旋转角度（弧度）
    float angleRad = aircraftHeading * M_PI / 180.0f;
    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    // 计算旋转后的偏移量
    float offsetX = centerX * cosA - centerY * sinA;
    float offsetY = centerX * sinA + centerY * cosA;
    // 5. 计算缩放比例
    float aircraftScale = 1.5;

    //glTranslatef(x_ndc, y_ndc, 0.0f);      // 步骤3：移回
    // glRotatef(aircraftHeading, 0.0f, 0.0f, 1.0f); // 旋转
    glTranslatef(-offsetX, -offsetY, 0.0f);    // 步骤1：移到原点
	glRotatef(aircraftHeading, 0.0f, 0.0f, 1.0f); // 飞机旋转
    
    glScalef(aircraftScale, aircraftScale, 1.0f); // 缩放
    // 进一步缩小坐标值
    glScalef(0.0001f, 0.0001f, 1.0f);

    // 替换顶点数据部分为新的飞机数据
    std::vector<GLfloat> vertices = {
        // First set of vertices
           -229.8762207f, 175.4812012f,
           -237.5363770f, 160.5925293f,
           -83.8049316f, 2.2529297f,
           -176.4226074f, 165.3974609f,
           -179.6904297f, 179.2180176f,
           -223.1274414f, 395.2353516f,
           -233.5747070f, 331.5615234f,
           -227.2873535f, -210.7246094f,
           -225.1835938f, -223.1545410f,
           -223.6892090f, -99.1652832f,
           -221.4370117f, -247.0869141f,
           -220.3132324f, -88.3190918f,
           -216.1796875f, -256.2727051f,
           -206.1604004f, -79.2456055f,
           -194.8493652f, -257.4633789f,
           -186.4333496f, -90.3864746f,
           -186.3291016f, -241.3601074f,
           -180.3696289f, -105.4057617f,
           -184.5163574f, -232.6984863f,
           -176.3588867f, -226.6264648f,
           -232.8527832f, -156.7431641f,
           -232.6623535f, -159.5493164f,
           -232.0612793f, -133.5529785f,
           -229.9907227f, -190.0007324f,
           -228.2204590f, -104.9777832f,
           -227.2873535f, -210.7246094f,
           -223.6892090f, -99.1652832f,
           -78.6818848f, 183.4211426f,
           -78.0712891f, 167.1835938f,
           -78.0454102f, 218.9160156f,
           -77.3735352f, 158.3347168f,
           -75.5317383f, 247.7473145f,
           -76.6757813f, 149.4855957f,
           -66.6604004f, -4.0895996f,
           -83.8049316f, 2.2529297f,

           // Second set of vertices
           178.1301270f, 324.0710449f,
           180.5429688f, 311.7988281f,
           180.7573242f, 386.5456543f,
           253.8239746f, 25.7963867f,
           188.1604004f, 397.3041992f,
           230.5800781f, 172.2180176f,
           228.7722168f, 185.0732422f,
           174.5097656f, 367.5109863f,
           173.5646973f, 346.8974609f,
           178.1301270f, 324.0710449f,
           180.7573242f, 386.5456543f,
           233.1030273f, 373.5107422f,
           225.9626465f, 391.0974121f,
           229.8818359f, 321.5881348f,
           232.7788086f, 329.5856934f,
           235.2231445f, 341.0949707f,
           235.5693359f, 356.6062012f,
           228.7722168f, 185.0732422f,
           229.0578613f, 251.1054688f,
           225.9626465f, 391.0974121f,
           212.9418945f, 401.9597168f,
           198.7658691f, 402.2700195f,
           188.1604004f, 397.3041992f,

           // Third set of vertices
           76.6757813f, 149.4855957f,
           78.7253418f, 189.0053711f,
           78.0454102f, 218.9160156f,
           75.0959473f, 251.8095703f,
           71.6879883f, -1.5952148f,
           83.8049316f, 2.2529297f,
           113.7377930f, 151.4187012f,
           596.6018066f, 77.8117676f,
           588.6330566f, 92.3918457f,
           598.2089844f, 72.8640137f,
           578.9230957f, 110.2705078f,
           574.0075684f, 69.5166016f,
           567.9409180f, 125.7082520f,
           556.5568848f, 134.8125000f,
           556.5568848f, 134.8125000f,
           545.3068848f, 139.5625000f,
           574.0075684f, 69.5166016f,
           533.1191406f, 141.8056641f,
           524.7475586f, 63.1584473f,
           517.5834961f, 143.2570801f,
           493.0288086f, 144.6542969f,
           479.7683105f, 57.1484375f,
           524.7475586f, 63.1584473f,
           493.0288086f, 144.6542969f,

           // Fourth set of vertices
           406.7810059f, 148.7500000f,
           335.9465332f, 152.4685059f,
           261.1369629f, 156.3911133f,
           253.8239746f, 25.7963867f,
           249.5437012f, 157.0437012f,
           230.5800781f, 172.2180176f,
           253.8239746f, 25.7963867f,
           261.1369629f, 156.3911133f,
           253.8239746f, 25.7963867f,
           180.5429688f, 311.7988281f,
           180.2363281f, 248.6103516f,
           179.9296875f, 182.6733398f,
           178.2558594f, 169.7165527f,
           164.4267578f, 156.0529785f,
           159.0463867f, 155.0322266f,
           138.2670898f, 153.4187012f,
           113.7377930f, 151.4187012f,
           83.8049316f, 2.2529297f,
           71.6879883f, -1.5952148f,
           75.0959473f, 251.8095703f,
           71.4423828f, 278.0649414f,
           67.1518555f, 299.3334961f,
           62.4082031f, 317.1357422f,
           62.1030273f, -6.7976074f,

           // Fifth set of vertices
           50.2863770f, -19.6965332f,
           52.1308594f, -15.6340332f,
           54.1374512f, 341.8334961f,
           54.5180664f, -12.8903809f,
           62.4082031f, 317.1357422f,
           62.1030273f, -6.7976074f,
           27.5759277f, -153.8125000f,
           27.7233887f, -147.2773438f,
           34.6081543f, 384.8391113f,
           38.7663574f, -84.6137695f,
           44.8830566f, 366.3874512f,
           50.2863770f, -19.6965332f,
           54.1374512f, 341.8334961f,
           20.3447266f, 396.4943848f,
           6.1364746f, 400.3071289f,
           7.7065430f, -336.7617188f,
           9.5646973f, -300.0571289f,
           12.7482910f, -289.8186035f,
           19.6801758f, -280.6442871f,
           26.2619629f, -275.0571289f,
           27.5759277f, -153.8125000f,
           34.6081543f, 384.8391113f,
           6.1364746f, 400.3071289f,
           0.0000000f, 400.7426758f,

           // Sixth set of vertices
           1.8679199f, -412.6647949f,
           5.5783691f, -378.5214844f,
           6.1213379f, -371.4787598f,
           7.7065430f, -336.7617188f,
           0.0000000f, 400.7426758f,
           -34.6081543f, 384.8391113f,
           -27.7233887f, -147.2773438f,
           -22.7609863f, -277.8161621f,
           -10.9279785f, -294.5783691f,
           -7.7065430f, -336.7617188f,
           -6.1213379f, -371.4787598f,
           -2.9990234f, -402.9924316f,
           -1.8679199f, -412.6647949f,
           -0.2868652f, -417.4487305f,
           1.8679199f, -412.6647949f,
           -27.7233887f, -147.2773438f,
           -34.6081543f, 384.8391113f,
           -38.7663574f, -84.6137695f,
           -49.6923828f, 354.5412598f,
           -53.0351563f, -14.3513184f,
           -63.4648438f, 313.4023438f,
           -66.6604004f, -4.0895996f,
           -71.4423828f, 278.0649414f,
           -75.5317383f, 247.7473145f,

           // Seventh set of vertices
           -83.8049316f, 2.2529297f,
           -76.6757813f, 149.4855957f,
           -159.0463867f, 155.0322266f,
           -166.0607910f, 156.5241699f,
           -176.4226074f, 165.3974609f,
           -198.7658691f, 402.2700195f,
           -223.1274414f, 395.2353516f,
           -179.6904297f, 179.2180176f,
           -180.2363281f, 248.6103516f,
           -180.5349121f, 313.3811035f,
           -180.7573242f, 386.5456543f,
           -253.8239746f, 25.7963867f,
           -83.8049316f, 2.2529297f,
           -237.5363770f, 160.5925293f,
           -254.4294434f, 156.7580566f,
           -406.7810059f, 148.7500000f,
           -506.6318359f, 143.8457031f,
           -524.7475586f, 63.1584473f,
           -524.7475586f, 63.1584473f,
           -506.6318359f, 143.8457031f,
           -549.1240234f, 138.2431641f,
           -570.3518066f, 122.8713379f,
           -588.6330566f, 92.3918457f,
           -596.6018066f, 77.8117676f,

           // Eighth set of vertices
           -597.1374512f, 76.7563477f,
           223.7194824f, -98.0266113f,
           223.1416016f, -91.6447754f,
           223.6586914f, -100.3039551f,
           225.1835938f, -223.1545410f,
           231.3923340f, -110.1748047f,
           229.8071289f, -196.6728516f,
           232.5280762f, -155.5031738f,
           206.8740234f, -258.7729492f,
           218.8083496f, -254.3974609f,
           220.3132324f, -88.3190918f,
           221.4370117f, -247.0869141f,
           223.1416016f, -91.6447754f,
           225.1835938f, -223.1545410f,
           202.3745117f, -77.6608887f,
           185.8249512f, -94.4611816f,
           186.3291016f, -241.3601074f,
           192.5529785f, -256.5703125f,
           197.8103027f, -258.3562012f,
           206.8740234f, -258.7729492f,
           220.3132324f, -88.3190918f,
           179.8159180f, -105.6787109f,
           177.2319336f, -112.1340332f,
           179.3801270f, -227.1621094f,

           // Ninth set of vertices
           186.3291016f, -241.3601074f,
           185.8249512f, -94.4611816f,
           177.2319336f, -112.1340332f,
           176.7705078f, -119.4985352f,
           179.3801270f, -227.1621094f,
           175.7092285f, -130.4545898f,
           175.2934570f, -226.5974121f,
           168.8337402f, -140.4560547f,
           156.9604492f, -145.9084473f,
           104.1315918f, -248.2141113f,
           175.2934570f, -226.5974121f,
           156.9604492f, -145.9084473f,
           103.9133301f, -153.2731934f,
           50.8662109f, -160.6379395f,
           40.1054688f, -161.9448242f,
           36.0583496f, -269.6281738f,
           36.0583496f, -269.6281738f,
           40.1054688f, -161.9448242f,
           28.3129883f, -157.6606445f,
           27.5759277f, -153.8125000f,
           26.2619629f, -275.0571289f,
           -28.7553711f, -158.8950195f,
           -44.6752930f, -161.5092773f,
           -36.0583496f, -269.6281738f,

           // Tenth set of vertices
           -22.7609863f, -277.8161621f,
           -27.7233887f, -147.2773438f,
           -103.9133301f, -153.2731934f,
           -156.9604492f, -145.9084473f,
           -104.1315918f, -248.2141113f,
           -36.0583496f, -269.6281738f,
           -44.6752930f, -161.5092773f,
           -170.3103027f, -139.2285156f,
           -176.7705078f, -119.4985352f,
           -176.3588867f, -226.6264648f,
           -104.1315918f, -248.2141113f,
           -156.9604492f, -145.9084473f,
           -177.2319336f, -112.1340332f,
           -180.3696289f, -105.4057617f,
           -176.3588867f, -226.6264648f,
           -176.7705078f, -119.4985352f,
           -173.5646973f, 346.8974609f,
           -180.7573242f, 386.5456543f,
           -180.5349121f, 313.3811035f,
           229.8818359f, 321.5881348f,
           225.9626465f, 391.0974121f,
           229.0578613f, 251.1054688f,
           -186.4333496f, -90.3864746f,
           -180.3696289f, -105.4057617f,

           // Final set of vertices
           -179.8159180f, -105.6787109f,
           -233.5747070f, 331.5615234f,
           -223.1274414f, 395.2353516f,
           -234.5314941f, 368.5380859f
    };
    // 7. 绘制飞机
     // 启用顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());

    // 按照原始分组绘制所有部分
    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 7, 13);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 27, 8);
    glDrawArrays(GL_TRIANGLE_STRIP, 35, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 42, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 46, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 52, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 58, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 65, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 72, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 79, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 86, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 90, 10);
    glDrawArrays(GL_TRIANGLE_FAN, 100, 6);
    glDrawArrays(GL_TRIANGLE_STRIP, 106, 6);
    glDrawArrays(GL_TRIANGLE_STRIP, 112, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 119, 9);
    glDrawArrays(GL_TRIANGLE_FAN, 128, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 134, 11);
    glDrawArrays(GL_TRIANGLE_STRIP, 145, 9);
    glDrawArrays(GL_TRIANGLE_FAN, 154, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 159, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 165, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 172, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 179, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 186, 6);
    glDrawArrays(GL_TRIANGLE_FAN, 192, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 199, 5);
    glDrawArrays(GL_TRIANGLE_STRIP, 204, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 211, 7);
    glDrawArrays(GL_TRIANGLE_FAN, 218, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 223, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 228, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 233, 5);
    glDrawArrays(GL_TRIANGLE_FAN, 238, 4);
    glDrawArrays(GL_TRIANGLES, 242, 12);

    glDisableClientState(GL_VERTEX_ARRAY);

    // 恢复所有属性
    glPopAttrib();
    // 恢复矩阵
    glPopMatrix();
    // 恢复投影矩阵
    glMatrixMode(GL_PROJECTION);  // 切回投影矩阵
    glPopMatrix();                // 恢复投影矩阵

}
#if 0
void drawAirportIcon(double lon, double lat, const char* iconPath, Vec2d center, double viewportWidth, double viewportHeight, double iconSize)
{
    if (!iconPath) {
        std::cout << "错误：机场图标路径为空" << std::endl;
        return;
    }

    // 加载PNG图片
    int width, height, nrChannels;
    unsigned char* imageData = stbi_load(iconPath, &width, &height, &nrChannels, 0);

    if (!imageData) {
        // 图片加载失败，输出错误信息
        const char* reason = stbi_failure_reason();
        std::cout << "错误：无法加载机场图标文件 \"" << iconPath << "\": " << (reason ? reason : "未知错误") << std::endl;
        return;
    }

    std::cout << "成功加载机场图标: " << iconPath << " (" << width << "x" << height << ", " << nrChannels << "通道)" << std::endl;

    // 保存当前OpenGL状态（OpenGL ES 1.1不支持glPushAttrib，需要手动保存）
    GLboolean texture2DEnabled = glIsEnabled(GL_TEXTURE_2D);
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean lightingEnabled = glIsEnabled(GL_LIGHTING);
    GLfloat currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    GLint currentBlendSrc, currentBlendDst;
    glGetIntegerv(GL_BLEND_SRC, &currentBlendSrc);
    glGetIntegerv(GL_BLEND_DST, &currentBlendDst);

    // 设置投影矩阵（使用标准化设备坐标，与飞机图标一致）
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 10.0);

    // 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 将经纬度转换为墨卡托坐标
    Vec2d airportLonLat(lon, lat);
    Vec2d airportMec;
    OMGeoUtil::Lonlat2WebMecator(airportLonLat, airportMec);

    // 计算机场相对于地图中心的屏幕坐标（标准化设备坐标）
    // 使用与飞机图标相同的计算方法
    double centerX = (center[0] - airportMec[0]) * 2.0 / viewportWidth;
    double centerY = (center[1] - airportMec[1]) * 2.0 / viewportHeight;

    // 计算图标大小（标准化设备坐标）
    if (iconSize <= 0.0) {
        // 图标大小设为标准化设备坐标的0.05（可根据需要调整）
        iconSize = 0.05;
    }
    double iconHalfSize = iconSize / 2.0;

    // 平移到机场位置
    glTranslatef(-centerX, -centerY, 0.0f);

    // 创建OpenGL纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 上传纹理数据
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

    // 启用纹理和混合
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // 设置颜色为白色，确保纹理颜色正确显示
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // 使用顶点数组绘制纹理四边形（OpenGL ES 1.1兼容方式）
    // 定义顶点坐标（两个三角形组成一个四边形）
    GLfloat vertices[] = {
        // 第一个三角形：左下、右下、左上
        -iconHalfSize, -iconHalfSize,  // 左下
        iconHalfSize, -iconHalfSize,    // 右下
        -iconHalfSize, iconHalfSize,    // 左上
        // 第二个三角形：右下、右上、左上
        iconHalfSize, -iconHalfSize,    // 右下
        iconHalfSize, iconHalfSize,     // 右上
        -iconHalfSize, iconHalfSize     // 左上
    };

    // 定义纹理坐标（与顶点对应）
    GLfloat texCoords[] = {
        // 第一个三角形
        0.0f, 0.0f,  // 左下
        1.0f, 0.0f,  // 右下
        0.0f, 1.0f,  // 左上
        // 第二个三角形
        1.0f, 0.0f,  // 右下
        1.0f, 1.0f,  // 右上
        0.0f, 1.0f   // 左上
    };

    // 启用顶点数组和纹理坐标数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // 设置顶点数组和纹理坐标数组
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    // 绘制两个三角形（共6个顶点）
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 禁用顶点数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // 清理资源
    glDeleteTextures(1, &texture);
    stbi_image_free(imageData);

    // 恢复OpenGL状态
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // 恢复之前的状态
    if (texture2DEnabled) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
    if (blendEnabled) {
        glEnable(GL_BLEND);
    }
    else {
        glDisable(GL_BLEND);
    }
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }
    glColor4f(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    glBlendFunc(currentBlendSrc, currentBlendDst);

    //    glTranslatef(-centerX, -centerY, 0.0f);
}
#endif
void MecatorToScreen(double mercatorX, double mercatorY, int& screenX, int& screenY, Vec2d now_center, double zoom, float screenWidth, float screenHeight, double aircraftHeading) {
    // 1. 地图中心墨卡托
    //Vec2d now_center = _scheduler2d->center();
    double centerMercatorX = now_center[0];
    double centerMercatorY = now_center[1];

    //2. 计算米/像素
    double worldSize = 20037508.34 * 2;  //全局墨卡托宽度（米）
    /*const int zoom = _scheduler2d->zoom();*/
    double tilesAtZoom = pow(2, zoom);
    double mercatorPerPixel = worldSize / (tilesAtZoom * 256);  //米/像素

    //3. 屏幕中心像素
    double centerScreenX = screenWidth / 2.0;
    double centerScreenY = screenHeight / 2.0;

    //4. 墨卡托相对偏移 -> 像素（与现有实现保持一致，使用 / (..../2.0)）
    double dX_merc = mercatorX - centerMercatorX;
    double dY_merc = mercatorY - centerMercatorY;
    double pxX = dX_merc / mercatorPerPixel;
    double pxY = dY_merc / mercatorPerPixel;

    // 5. 应用正向旋转（screenToMercator 用的是逆旋转）
    double rotationDegrees = aircraftHeading;
    double rotationRad = rotationDegrees * M_PI / 180.0;
    double offsetX = pxX * cos(rotationRad) - pxY * sin(rotationRad);
    double offsetY = pxX * sin(rotationRad) + pxY * cos(rotationRad);

    // 6. 转为屏幕坐标（Y 轴向下）
    screenX = static_cast<int>(std::round(centerScreenX + offsetX));
    screenY = static_cast<int>(std::round(centerScreenY - offsetY));
    return;
}
// 纹理缓存结构
struct TextureCacheEntry {
    GLuint textureID;
    int width;
    int height;
    int channels;
};

// 静态纹理缓存
static std::map<std::string, TextureCacheEntry> textureCache;

// 获取或创建纹理（带缓存）
static GLuint getOrCreateTexture(const char* iconPath, int& width, int& height, int& nrChannels) {
    if (!iconPath) {
        return 0;
    }

    std::string pathStr(iconPath);

    // 检查缓存
    auto it = textureCache.find(pathStr);
    if (it != textureCache.end()) {
        // 使用缓存的纹理
        width = it->second.width;
        height = it->second.height;
        nrChannels = it->second.channels;
        return it->second.textureID;
    }

    // 缓存未命中，加载图片
    unsigned char* imageData = stbi_load(iconPath, &width, &height, &nrChannels, 0);
    if (!imageData) {
        const char* reason = stbi_failure_reason();
        std::cout << "错误：无法加载机场图标文件 \"" << iconPath << "\": " << (reason ? reason : "未知错误") << std::endl;
        return 0;
    }

    // 创建纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 上传纹理数据
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

    // 释放图片数据（纹理已上传到GPU）
    stbi_image_free(imageData);

    // 存入缓存
    TextureCacheEntry entry;
    entry.textureID = texture;
    entry.width = width;
    entry.height = height;
    entry.channels = nrChannels;
    textureCache[pathStr] = entry;

    std::cout << "成功加载并缓存机场图标: " << iconPath << " (" << width << "x" << height << ", " << nrChannels << "通道)" << std::endl;

    return texture;
}

void drawAirportIcon(double lon, double lat, const char* iconPath, Vec2d center, float viewportWidth, float viewportHeight, double iconSize, double aircraftHeading, double zoom)
{
    if (!iconPath) {
        std::cout << "错误：机场图标路径为空" << std::endl;
        return;
    }

    // 使用纹理缓存获取或创建纹理
    int width, height, nrChannels;
    GLuint texture = getOrCreateTexture(iconPath, width, height, nrChannels);
    if (texture == 0) {
        return; // 加载失败，已在getOrCreateTexture中输出错误信息
    }

    // 保存当前OpenGL状态（OpenGL ES 1.1不支持glPushAttrib，需要手动保存）
    GLboolean texture2DEnabled = glIsEnabled(GL_TEXTURE_2D);
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean lightingEnabled = glIsEnabled(GL_LIGHTING);
    GLfloat currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    GLint currentBlendSrc, currentBlendDst;
    glGetIntegerv(GL_BLEND_SRC, &currentBlendSrc);
    glGetIntegerv(GL_BLEND_DST, &currentBlendDst);

    //// 设置投影矩阵（使用标准化设备坐标，与飞机图标一致）
    //glMatrixMode(GL_PROJECTION);
    ////glPushMatrix();
    //glLoadIdentity();
    ////glOrtho(left, right, bottom, top, -1.0, 1.0);
    ////glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 10.0);
    //glOrtho(0, viewportWidth, 0, viewportHeight, 0.0, 10.0);

    ////// 设置模型视图矩阵
    //glMatrixMode(GL_MODELVIEW);
    ////glPushMatrix();
    //glLoadIdentity();


    // 将经纬度转换为墨卡托坐标
    Vec2d airportLonLat(lon, lat);
    Vec2d airportMec;
    OMGeoUtil::Lonlat2WebMecator(airportLonLat, airportMec);
    int centerX;
    int centerY;
    MecatorToScreen(airportMec[0], airportMec[1], centerX, centerY, center, zoom, viewportWidth, viewportHeight, aircraftHeading);
    // 计算机场相对于地图中心的屏幕坐标（标准化设备坐标）
    // 使用与飞机图标相同的计算方法
    //double centerX = (center[0] - airportMec[0]) * 2.0 / viewportWidth;
    //double centerY = (center[1] - airportMec[1]) * 2.0 / viewportHeight;

    // 计算图标大小（标准化设备坐标）
    if (iconSize <= 0.0) {
        // 图标大小设为标准化设备坐标的0.05（可根据需要调整）
        iconSize = 0.05;
    }
    double iconHalfSize = iconSize / 2.0;

    // 绑定缓存的纹理
    glBindTexture(GL_TEXTURE_2D, texture);

    // 启用纹理和混合
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // 设置颜色为白色，确保纹理颜色正确显示
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

    // 使用顶点数组绘制纹理四边形（OpenGL ES 1.1兼容方式）
    // 定义顶点坐标（两个三角形组成一个四边形）
    GLfloat vertices[] = {
        // 第一个三角形：左下、右下、左上
        -iconHalfSize + centerX, -iconHalfSize + viewportHeight - centerY,  // 左下
        iconHalfSize + centerX, -iconHalfSize + viewportHeight - centerY,    // 右下
        -iconHalfSize + centerX, iconHalfSize + viewportHeight - centerY,    // 左上
        // 第二个三角形：右下、右上、左上
        iconHalfSize + centerX, -iconHalfSize + viewportHeight - centerY,   // 右下
        iconHalfSize + centerX, iconHalfSize + viewportHeight - centerY,    // 右上
        -iconHalfSize + centerX, iconHalfSize + viewportHeight - centerY    // 左上
    };
    glTranslatef(centerX, viewportHeight - centerY, 0.0f);
    glRotatef(aircraftHeading, 0.0, 0.0, 1.0);
    glTranslatef(-centerX, -viewportHeight + centerY, 0.0f);

    // 定义纹理坐标（与顶点对应）
    GLfloat texCoords[] = {
        // 第一个三角形
        0.0f, 0.0f,  // 左下
        1.0f, 0.0f,  // 右下
        0.0f, 1.0f,  // 左上
        // 第二个三角形
        1.0f, 0.0f,  // 右下
        1.0f, 1.0f,  // 右上
        0.0f, 1.0f   // 左上
    };

    // 启用顶点数组和纹理坐标数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // 设置顶点数组和纹理坐标数组
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);



    // 绘制两个三角形（共6个顶点）
    glDrawArrays(GL_TRIANGLES, 0, 6);



    // 禁用顶点数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // 注意：不再删除纹理，因为纹理已缓存供后续使用

    // 恢复OpenGL状态
    //glPopMatrix();
    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);

    // 恢复之前的状态
    if (texture2DEnabled) {
        glEnable(GL_TEXTURE_2D);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
    if (blendEnabled) {
        glEnable(GL_BLEND);
    }
    else {
        glDisable(GL_BLEND);
    }
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    }
    else {
        glDisable(GL_LIGHTING);
    }
    glColor4f(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    glBlendFunc(currentBlendSrc, currentBlendDst);
}