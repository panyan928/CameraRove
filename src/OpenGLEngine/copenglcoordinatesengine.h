#ifndef CopenGLCoordinatesEngine_H
#define CopenGLCoordinatesEngine_H

//#include "GL/freeglut.h"
#include "gl\gl.h"	
#include "gl\glu.h"
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../TMUtil/tmtools.h"

namespace openglEngine {
	class openGLCoordinatesEngine
	{
	public:
		static void screen2World(double screen_x, double screen_y, double screen_z,
			double& world_x, double& world_y, double& world_z);
	
		static void world2Screen(double& screen_x, double& screen_y, double& screen_z,
			double world_x, double world_y, double world_z);

		static void world2Camera(glm::dvec3 eye, glm::dvec3 center, glm::dvec3 up, 
			glm::dvec3 in, glm::dvec3& out);
		
		static void camera2World(glm::dvec3 eye, glm::dvec3 center, glm::dvec3 up,
			glm::dvec3 in, glm::dvec3& out);

		static void camera2World(Vec3d eye, Vec3d center, Vec3d up,
			Vec3d in, Vec3d& out);

		static void world2Camera(Vec3d eye, Vec3d center, Vec3d up,
			Vec3d in, Vec3d& out);
	};
}

#endif // !1