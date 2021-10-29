#ifndef __SHADOWBOX_H__
#define __SHADOWBOX_H__
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"
class ShadowBox {

private:

	static const float OFFSET;
	static const float SHADOW_DISTANCE;
	static const vec4 UP;
	static const vec4 FORWARD;

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	mat4 *lightViewMatrix;
	Camera* cam;
	float farHeight, farWidth, nearHeight, nearWidth;
	
	void calculateWidthsAndHeights(Camera* camera);
	std::vector<vec4> calculateFrustrumVertices(mat4 rotation, vec3 forwardVector,
		vec3 centerNear, vec3 centerFar);
	vec4 calculateLightSpaceFrustumCorner(vec3 startPoint, vec3 direction, float width);
	mat4 calculateCameraRotationMatrix();

public:
	ShadowBox(mat4* lightViewMatrix_, Camera* camera) :lightViewMatrix(lightViewMatrix_), cam(camera){
		calculateWidthsAndHeights(camera);
	}
	inline float getWidth() const { return maxX - minX; }
	inline float getHeight() const { return maxY - minY; }
	inline float getLength() const { return maxZ - minZ; }
	inline float getWidth2() const { return maxX + minX; }
	inline float getHeight2() const { return maxY + minY; }
	inline float getLength2() const { return maxZ + minZ; }
	vec3 getCenter();
	void update();
	







};


#endif