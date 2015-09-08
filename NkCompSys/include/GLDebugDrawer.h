/* 
Copyright 2015 Alvaro Herrasti and Daniel Dovali

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include <glm/glm.hpp> // Libreria de Matrices
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

#include <LinearMath/btIDebugDraw.h>



class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:

	GLDebugDrawer(GLuint shaderProgram);
	virtual ~GLDebugDrawer(); 

	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);

	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

	virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color);

	virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
	
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

	virtual void	reportErrorWarning(const char* warningString);

	virtual void	draw3dText(const btVector3& location,const char* textString);

	virtual void	setDebugMode(int debugMode);

	virtual int		getDebugMode() const { return m_debugMode;}

	void setProjMat(const glm::mat4 projMat) {
		this->projMat = projMat;
	}

	void setViewMat(const glm::mat4 viewMat) {
		this->viewMat = viewMat;
	}

private:
	glm::mat4 projMat;
	glm::mat4 viewMat;
	GLuint shaderProgram;

};

#endif//GL_DEBUG_DRAWER_H
