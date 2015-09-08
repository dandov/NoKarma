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

#include "GameSystems/CollisionSystem.h"

void CollisionSystem::update(double time, SystemMessageSender* msgSender)
{
	
	std::vector<c_handle<BoundingSphereComponent>>& spheres = GameObjectFactory::getComponentHandlesOfType<BoundingSphereComponent>();
	std::vector<c_handle<BoundingPlaneComponent>>& planes = GameObjectFactory::getComponentHandlesOfType<BoundingPlaneComponent>();
	
	for(std::vector<c_handle<BoundingSphereComponent>>::iterator it = spheres.begin(); it!=spheres.end();it++)
	{
		BoundingSphereComponent sphere = it->get();
		c_handle<TransformComponent> position_handle = GameObjectFactory::getComponentBrother<TransformComponent>(*it);
		if(position_handle)
		{
			//sphere plane collisions
			sphere.position+=position_handle->position;
			position_handle->position+=spherePlaneCollisionResponse(sphere,planes);

			//sphere sphere collisions
			for(std::vector<c_handle<BoundingSphereComponent>>::iterator it2 = it+1;it2!=spheres.end();it2++)
			{
				BoundingSphereComponent sphere2 = it2->get();
				c_handle<TransformComponent> position_handle2 = GameObjectFactory::getComponentBrother<TransformComponent>(*it2);
				if(position_handle2)
				{
					sphere2.position+=position_handle2->position;
					if(sphereSphereIntersection(sphere,sphere2))
					{
						(*it)->entitiesCollidedTo.push_back((*it2)->getParentEntity());
						(*it2)->entitiesCollidedTo.push_back((*it)->getParentEntity());
					}
				}
			}
		}
	}
}

void CollisionSystem::init(void) 
{
}

void CollisionSystem::send(const Message& msg)
{
}

glm::vec3 CollisionSystem::spherePlaneCollisionResponse(const BoundingSphereComponent& sphere, const std::vector<c_handle<BoundingPlaneComponent>>& planes)
{
	glm::vec3 response(0.0f, 0.0f, 0.0f);
	glm::vec3 totalResponse(0.0f, 0.0f, 0.0f);
	for (std::vector<c_handle<BoundingPlaneComponent>>::const_iterator it = planes.begin(); it != planes.end(); it++) {
		bool collides = spherePlaneIntersection(sphere,it->get(), response);
		if (collides) {
			totalResponse += response;
		}
	}
	return totalResponse;
}

bool CollisionSystem::sphereSphereIntersection(const BoundingSphereComponent& sphere1, const BoundingSphereComponent& sphere2)
{
	glm::vec3 relPos = sphere1.position-sphere2.position;
	float dist = glm::dot(relPos,relPos);
	float minDist = sphere1.radius+sphere2.radius;
	return dist<=(minDist*minDist);
}

bool CollisionSystem::spherePlaneIntersection(const BoundingSphereComponent& sphere, const BoundingPlaneComponent& plane,glm::vec3& outResponse)
{
	glm::vec3 frontInt;
	glm::vec3 backInt;

	float distanceFront = 0.0f;
	float distanceBack = 0.0f;
	bool frontIntersects = rayPlaneIntersection(plane.normal, sphere.position, -plane.normal, plane.v1, plane.v2, plane.v3, plane.v4, frontInt, distanceFront);
	bool backIntersects = rayPlaneIntersection(-plane.normal, sphere.position, plane.normal, plane.v1, plane.v2, plane.v3, plane.v4, backInt, distanceBack);

	if (frontIntersects || backIntersects) {
		if (distanceBack > sphere.radius || distanceFront > sphere.radius) {
			return false;
		}
	
		if (distanceFront > 0) {
			outResponse = - plane.normal*(sphere.radius-distanceFront);
		}
		else {
			outResponse =  plane.normal*(sphere.radius-distanceBack);
		}

		return true;
	}
	return false;
}

float CollisionSystem::triangleArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) 
{
	float a = glm::length(p2-p1);
	float b = glm::length(p3-p2);
	float c = glm::length(p3-p1);
	float s = (a+b+c)/2;

	return sqrt(s*(s-a)*(s-b)*(s-c)); 
}

//http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
void barycentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& intersect, glm::vec3& out)
{
	glm::vec3 v0 = p2 - p1, v1 = p3 - p1, v2 = intersect - p1;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    out.x = (d11 * d20 - d01 * d21) / denom;
    out.y = (d00 * d21 - d01 * d20) / denom;
    out.z = 1.0f - out.x - out.y;
}

//http://gamedeveloperjourney.blogspot.mx/2009/04/point-plane-collision-detection.html
/*
bool intersectsTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& intersect) {
	
	v1 = intersect - vP1;
    v2 = intersect - vP2;
    v3 = intersect - vP3;
    D3DXVec3Normalize(&v1, &v1);
    D3DXVec3Normalize(&v2, &v2);
    D3DXVec3Normalize(&v3, &v3);
    
    // Angles around intersection should total 360 degrees (2 PI)
    thetaSum = acos(D3DXVec3Dot(&v1, &v2)) 
             + acos(D3DXVec3Dot(&v2, &v3)) 
             + acos(D3DXVec3Dot(&v3, &v1));
    
    if (fabs(thetaSum - (2 * D3DX_PI)) < 0.1)
        return true;
    else
        return false;
}
*/


//http://blogs.msdn.com/b/rezanour/archive/2011/08/07/barycentric-coordinates-and-point-in-triangle-tests.aspx
bool PointInTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& intersect)
{
    // Prepare our barycentric variables
	glm::vec3 u = p2 - p1;
    glm::vec3 v = p3 - p1;
    glm::vec3 w = intersect - p1;
    glm::vec3 vCrossW = glm::cross(v, w);
	glm::vec3 vCrossU = glm::cross(v, u);

    // Test sign of r
    if (glm::dot(vCrossW, vCrossU) < 0) {
        return false;
	}
 
    glm::vec3 uCrossW = glm::cross(u, w);
	glm::vec3 uCrossV = glm::cross(u, v);
 
    // Test sign of t
    if (glm::dot(uCrossW, uCrossV) < 0)
        return false;
 
    // At this piont, we know that r and t and both > 0
	float denom = glm::length(uCrossV);
	float r = glm::length(vCrossW) / denom;
	float t = glm::length(uCrossW) / denom;
 
    return (r <= 1 && t <= 1 && r + t <= 1);
}

bool CollisionSystem::rayPlaneIntersection(const glm::vec3& rayDirection, const glm::vec3& rayStart, const glm::vec3& planeNormal,
						  const glm::vec3& planeV1, const glm::vec3& planeV2, const glm::vec3& planeV3, const glm::vec3& planeV4,
						  glm::vec3& outIntersection, float& outDistance)
{

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

	glm::vec3 baryT1;
	barycentric(planeV1, planeV2, planeV3, intersection, baryT1);

	glm::vec3 baryT2;
	barycentric(planeV1, planeV3, planeV4, intersection, baryT2);


	if ( (baryT1.x >=0 && baryT1.y >= 0 && baryT1.x+baryT1.y < 1)
		|| (baryT2.x >= 0 && baryT2.y >= 0 && baryT2.x+baryT2.y < 1) ) {
		outDistance = t;
		outIntersection = intersection;
		return 1;
	}
	return 0;


	/*if (PointInTriangle(planeV1, planeV2, planeV3, intersection) || PointInTriangle(planeV1, planeV3, planeV4, intersection)) {
		outDistance = t;
		outIntersection = intersection;
		return 1;
	}

	return 0;
	*/
	//Impreciso
	/*float ab1 = abs(triangleArea(planeV1, planeV2, planeV3) - triangleArea(intersection, planeV2, planeV3) 
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

	return 0;*/

	
}
