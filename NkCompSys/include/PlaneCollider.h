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

#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

struct Plane {
public:
	Plane(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, const glm::vec3& normal):
	v1(v1), v2(v2), v3(v3), v4(v4), normal(normal){

	}
glm::vec3 v1;
glm::vec3 v2;
glm::vec3 v3;
glm::vec3 v4;
glm::vec3 normal;
};

class PlaneCollider
{
public:
	PlaneCollider(void);
	~PlaneCollider(void);
	int addPlane(const Plane& plane);
	glm::vec3 spherPlaneCollisionResponse(const glm::vec3& position,const float& sphereRadius);
private:
	std::vector<Plane> collisionPlanes;
	bool spherePlaneIntersection(const glm::vec3& center, const float& radius, glm::vec3& planeNormal, 
							   const glm::vec3& planeV1, const glm::vec3& planeV2, const glm::vec3& planeV3, const glm::vec3& planeV4,
							   glm::vec3& outResponse);
	bool rayPlaneIntersection(const glm::vec3& rayDirection, const glm::vec3& rayStart, const glm::vec3& planeNormal,
						  const glm::vec3& planeV1, const glm::vec3& planeV2, const glm::vec3& planeV3, const glm::vec3& planeV4,
						  glm::vec3& outIntersection, float& outDistance);
	float PlaneCollider::triangleArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
};

