# include "ShadowBox.h"

const float ShadowBox::OFFSET = 25.;
const float ShadowBox::SHADOW_DISTANCE = 170.;
const vec4 ShadowBox::UP = vec4(0.,1.,0.,0.);
const vec4 ShadowBox::FORWARD = vec4(0.,0.,-1.,0.);

void ShadowBox::calculateWidthsAndHeights(Camera *camera){
	farWidth = (float)(SHADOW_DISTANCE * tan(radians(camera->getFov())));
	nearWidth = (float)(camera->getNear()*tan(radians(camera->getFov())));
	farHeight = farWidth / camera->getAspectRatio();
	nearHeight = nearWidth / camera->getAspectRatio();
	
}

vec3 ShadowBox::getCenter(){
	float x = (minX + maxX) / 2.f;
	float y = (minY + maxY) / 2.f;
	float z = (minZ + maxZ) / 2.f;
	vec4 cen = vec4(x, y, z, 1.f);
	mat4 invertedLight = glm::inverse(*lightViewMatrix);
	return vec3(invertedLight*cen);
	
}

void ShadowBox::update(){
	mat4 rotation = calculateCameraRotationMatrix();
	vec3 forwardVector = vec3(rotation * FORWARD);
	vec3 toFar = vec3(forwardVector);
	toFar = toFar * SHADOW_DISTANCE;
	vec3 toNear = vec3(forwardVector);
	toNear = toNear * cam->getNear();
	vec3 centerNear = toNear + cam->getPosition();
	vec3 centerFar = toFar + cam->getPosition();
	std::vector<vec4> points = calculateFrustrumVertices(rotation, forwardVector, centerNear,
		centerFar);
	bool first = true;
	for (vec4 point : points) {
		if (first) {
			minX = point.x;
			maxX = point.x;
			minY = point.y;
			maxY = point.y;
			minZ = point.z;
			maxZ = point.z;
			first = false;
			continue;
		}if (point.x > maxX) {
			maxX = point.x;
		}
		else if (point.x < minX) {
			minX = point.x;
		}
		if (point.y > maxY) {
			maxY = point.y;
		}
		else if (point.y < minY) {
			minY = point.y;
		}
		if (point.z > maxZ) {
			maxZ = point.z;
		}
		else if (point.z < minZ) {
			minZ = point.z;
		}
	}
	maxZ += OFFSET;
}

mat4 ShadowBox::calculateCameraRotationMatrix(){
	mat4 rotation(1.0);
	rotation = glm::rotate(rotation,(float)radians(-cam->getYaw()), vec3(0, 1, 0));
	rotation = glm::rotate(rotation,(float)radians(-cam->getPitch()), vec3(1, 0, 0));
	return rotation;
}

std::vector<vec4> ShadowBox::calculateFrustrumVertices(mat4 rotation, vec3 forwardVector, vec3 centerNear, vec3 centerFar){
	vec3 upVector = vec3(rotation * UP);
	vec3 rightVector = glm::cross(forwardVector, upVector);
	vec3 downVector = -upVector;
	vec3 leftVector = -rightVector;
	vec3 farTop = centerFar + vec3(upVector.x * farHeight, upVector.y * farHeight, upVector.z * farHeight);
	vec3 farBottom = centerFar + vec3(downVector.x * farHeight, downVector.y * farHeight, downVector.z * farHeight);
	vec3 nearTop = centerNear + vec3(upVector.x * nearHeight,upVector.y * nearHeight, upVector.z * nearHeight);
	vec3 nearBottom = centerNear + vec3(downVector.x * nearHeight,downVector.y * nearHeight, downVector.z * nearHeight);
	std::vector<vec4> points(8);
	points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
	points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
	points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
	points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
	points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
	points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
	points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
	points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
	return points;
}

vec4 ShadowBox::calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width){
	vec3 point = startPoint + vec3(direction * width);
	vec4 point4 = vec4(point, 1.f);
	point4 = *lightViewMatrix * point4;
	return point4;
}