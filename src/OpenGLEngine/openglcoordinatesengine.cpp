#include "copenglcoordinatesengine.h"
#include "../TMUtil/cgeoutil.h"
#include "TMUtil/OMapGlobal.h"
#include "TMScheduler/OMScheduler.h"

extern OMScheduler* _scheduler2d;
#define M_PI 3.141592653

void MecatorToScreen(double mercatorX, double mercatorY, int& screenX, int& screenY) {
    // 1. 地图中心墨卡托
     Vec2d now_center = _scheduler2d->center();
    double centerMercatorX = now_center[0];
    double centerMercatorY = now_center[1];

     //2. 计算米/像素
    double worldSize = 20037508.34 * 2;  //全局墨卡托宽度（米）
    /*const int zoom = _scheduler2d->zoom();*/
    double tilesAtZoom = pow(2, _scheduler2d->zoom());
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
    double rotationDegrees = _scheduler2d->getRotation();
    double rotationRad = rotationDegrees * M_PI / 180.0;
    double offsetX = pxX * cos(rotationRad) - pxY * sin(rotationRad);
    double offsetY = pxX * sin(rotationRad) + pxY * cos(rotationRad);

    // 6. 转为屏幕坐标（Y 轴向下）
    screenX = static_cast<int>(std::round(centerScreenX + offsetX));
    screenY = static_cast<int>(std::round(centerScreenY - offsetY));
    return;
}

namespace openglEngine {
    /**
    * @brief screen2World
    * @details 灞忓箷鍧愭爣杞?涓栫晫鍧愭??
    */
	void openGLCoordinatesEngine::screen2World(double screen_x, double screen_y, double screen_z, double& world_x, double& world_y, double& world_z)
	{
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLdouble winX, winY, winZ;

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        winX = (GLdouble)screen_x;
        winY = (GLdouble)viewport[3] - (GLdouble)screen_y;
        glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        gluUnProject(winX, winY, screen_z, modelview, projection, viewport, &world_x, &world_y, &world_z);

        return;
	}
    #if 0
    void screenToMercator(int screenX, int screenY, double& mercatorX, double& mercatorY) {
        // 1. ???????????????
        Vec2d now_center = _scheduler2d->center();
        double centerMercatorX = now_center[0];
        double centerMercatorY = now_center[1];
    
        // 2. ???????????????/??
        double worldSize = 20037508.34 * 2; // ??????????
        double tilesAtZoom = pow(2, _scheduler2d->zoom()); // ??????????
        double mercatorPerPixel = worldSize / (tilesAtZoom * 256); // ?/??
    
        // 3. ????????????
        double centerScreenX = screenWidth / 2.0;
        double centerScreenY = screenHeight / 2.0;
    
        // 4. ???????????????Y????
        double offsetX = screenX - centerScreenX;
        double offsetY = centerScreenY - screenY; // ??Y???????Y???
        
        double rotationDegrees = _scheduler2d->get_angle();
        // 5. ??????????????
        double rotationRad = -rotationDegrees * M_PI / 180.0;
        double rotatedOffsetX = offsetX * cos(rotationRad) - offsetY * sin(rotationRad);
        double rotatedOffsetY = offsetX * sin(rotationRad) + offsetY * cos(rotationRad);
    
        // 6. ????????????????2.0?
        mercatorX = centerMercatorX + rotatedOffsetX * mercatorPerPixel/2.0;
        mercatorY = centerMercatorY + rotatedOffsetY * mercatorPerPixel/2.0;
    }
    #endif
    

    /**
    * @brife world2Screen
    * @details 锟斤拷锟斤拷锟斤拷锟斤拷转锟斤拷幕锟斤拷锟斤拷
    */
    void openGLCoordinatesEngine::world2Screen(double& screen_x, double& screen_y, double& screen_z, double world_x, double world_y, double world_z)
    {
        GLint    viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLdouble winX, winY, winZ;

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        
		int screen_x2, screen_y2;
        //gluProject(world_x, world_y, world_z, modelview, projection, viewport, &winX, &winY, &winZ);
        MecatorToScreen(world_x, world_y, screen_x2, screen_y2);

        /*screen_x = (winX);
        screen_y = ((GLdouble)viewport[3] - winY);
        screen_z = (winZ);*/
        screen_x = screen_x2;
        screen_y = screen_y2;
        screen_z = 0.01;
        
        return;
    }

    void openGLCoordinatesEngine::world2Camera(glm::dvec3 eye, glm::dvec3 center, glm::dvec3 up, glm::dvec3 in, glm::dvec3& out)
    {
        glm::dmat4 view;
        view = glm::lookAt(eye, center, up);
        glm::dvec4 world(in, 1);
        glm::dvec4 camera = view * world;
        out.x = camera.x;
        out.y = camera.y;
        out.z = camera.z;
    }

    void openGLCoordinatesEngine::world2Camera(Vec3d eye, Vec3d center, Vec3d up, Vec3d in, Vec3d& out)
    {
        glm::dvec3 output;
        world2Camera(glm::dvec3(eye[0], eye[1], eye[2]), glm::dvec3(center[0], center[1], center[2]),
            glm::dvec3(up[0], up[1], up[2]), glm::dvec3(in[0], in[1], in[2]), output);

        out[0] = output.x;
        out[1] = output.y;
        out[2] = output.z;
    }

    void openGLCoordinatesEngine::camera2World(glm::dvec3 eye, glm::dvec3 center, glm::dvec3 up, glm::dvec3 in, glm::dvec3& out)
    {
        glm::dmat4 inverse_view;
        inverse_view = glm::inverse(glm::lookAt(eye, center, up));
        glm::dvec4 camera(in, 1);
        glm::dvec4 world = inverse_view * camera;
        out.x = world.x;
        out.y = world.y;
        out.z = world.z;
    }

    void openGLCoordinatesEngine::camera2World(Vec3d eye, Vec3d center, Vec3d up, Vec3d in, Vec3d& out)
    {
        glm::dvec3 output;
        camera2World(glm::dvec3(eye[0], eye[1], eye[2]), glm::dvec3(center[0], center[1], center[2]),
            glm::dvec3(up[0], up[1], up[2]), glm::dvec3(in[0], in[1], in[2]), output);

        out[0] = output.x;
        out[1] = output.y;
        out[2] = output.z;
    }

    //void openGLCoordinatesEngine::world2Camera(double& eye_x, double& eye_y, double& eye_z, double world_x, double world_y, double world_z)
    //{
    //    GLint    viewport[4];
    //    GLdouble modelview[16];
    //    GLdouble projection[16];
    //    GLdouble winX, winY, winZ;

    //    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    //    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    //    glGetIntegerv(GL_VIEWPORT, viewport);


    //    glm::dmat4 view;
    //    view = glm::lookAt(glm::dvec3(world_x, world_y, world_z),
    //        glm::dvec3(0.0f, 0.0f, 0.0f),
    //        glm::dvec3(0.0f, 0.0f, 1.0f));
    //    glm::dvec4 eye;
    //    glm::dvec4 world(world_x, world_y, world_z, 1);
    //    eye = view * world;
    //    eye_x = eye.x; eye_y = eye.y; eye_z = eye.z;
    //}
}
