#include "copenglcoordinatesengine.h"
#include "../TMUtil/cgeoutil.h"

namespace openglEngine {
    /**
    * @brief screen2World
    * @details 屏幕坐标转世界坐�?
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

    /**
    * @brife world2Screen
    * @details ��������ת��Ļ����
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

        

        gluProject(world_x, world_y, world_z, modelview, projection, viewport, &winX, &winY, &winZ);
        

        screen_x = (winX);
        screen_y = ((GLdouble)viewport[3] - winY);
        screen_z = (winZ);
        
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
