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

#include "PlaneCollider.h"


PlaneCollider::PlaneCollider(void)
{
}


PlaneCollider::~PlaneCollider(void)
{
}


int PlaneCollider::addPlane(const Plane& plane) {
	collisionPlanes.push_back(plane);
	return collisionPlanes.size()-1;
}
glm::vec3 PlaneCollider::spherPlaneCollisionResponse(const glm::vec3& position, const float& sphereRadius) {
	glm::vec3 response(0.0f, 0.0f, 0.0f);
	glm::vec3 totalResponse(0.0f, 0.0f, 0.0f);
	for (std::vector<Plane>::iterator it = collisionPlanes.begin(); it != collisionPlanes.end(); it++) {
		bool collides = spherePlaneIntersection(position, sphereRadius, it->normal, it->v1, it->v2, it->v3, it->v4, response);
		if (collides) {
			totalResponse += response;
		}
	}
	return totalResponse;
}

bool PlaneCollider::spherePlaneIntersection(const glm::vec3& center, const float& radius, glm::vec3& planeNormal, 
							   const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4,
							   glm::vec3& outResponse) 
{
	glm::vec3 frontInt;
	glm::vec3 backInt;

	float distanceFront = 0.0f;
	float distanceBack = 0.0f;
	bool frontIntersects = rayPlaneIntersection(planeNormal, center, -planeNormal, p1, p2, p3, p4, frontInt, distanceFront);
	bool backIntersects = rayPlaneIntersection(-planeNormal, center, planeNormal, p1, p2, p3, p4, backInt, distanceBack);

	if (frontIntersects || backIntersects) {
		if (distanceBack > radius || distanceFront > radius) {
			return false;
		}
	
		if (distanceFront > 0) {
			//outResponse = center - planeNormal*(radius-distanceFront);
			outResponse = - planeNormal*(radius-distanceFront);
		}
		else {
			//outResponse = center + planeNormal*(radius-distanceBack);
			outResponse =  planeNormal*(radius-distanceBack);
		}

		return true;
	}
	return false;
}

float PlaneCollider::triangleArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
	float a = glm::length(p2-p1);
	float b = glm::length(p3-p2);
	float c = glm::length(p3-p1);
	float s = (a+b+c)/2;

	return sqrt(s*(s-a)*(s-b)*(s-c)); 
}

bool PlaneCollider::rayPlaneIntersection(const glm::vec3& rayDirection, const glm::vec3& rayStart, const glm::vec3& planeNormal,
						  const glm::vec3& planeV1, const glm::vec3& planeV2, const glm::vec3& planeV3, const glm::vec3& planeV4,
						  glm::vec3& outIntersection, float& outDistance) {

	float dotRN = glm::dot(rayDirection, planeNormal);

	if (dotRN == 0.0f) {
		return 0;
	}

	
	float t = (glm::dot(planeV1, planeNormal) - glm::dot(rayStart, planeNormal) )/ dotRN; 

	if (t < 0) {
		return 0;
	}

	glm::vec3 intersection;
		//(*outIntersection).xyz = rayStart.xyz + rayDirection.xyz*t;
	intersection.x = rayStart.x + rayDirection.x*t;
	intersection.y = rayStart.y + rayDirection.y*t;
	intersection.z = rayStart.z + rayDirection.z*t;

	float ab1 = abs(triangleArea(planeV1, planeV2, planeV3) - triangleArea(intersection, planeV2, planeV3) 
		- triangleArea(planeV1, intersection, planeV3) - triangleArea(planeV1, planeV2, intersection));
	float ab2 = abs(triangleArea(planeV1, planeV3, planeV4) - triangleArea(intersection, planeV3, planeV4) 
		- triangleArea(planeV1, intersection, planeV4) - triangleArea(planeV1, planeV3, intersection));

	float epsilon = 0.00055;
	if ( ab1 < epsilon ||

		 ab2 < epsilon) {

			outDistance = t;
			outIntersection = intersection;
			return 1;
	}

	return 0;

	
}
