#ifndef __MODELTEXTURE_H__
#define __MODELTEXTURE_H__

#include <GLFW/glfw3.h>
#include <glm.hpp>>

class ModelTexture {

private:
	float shineDamper = 1.0;
	float reflectivity = 0.0;
	GLuint normalMap;
	glm::vec3 Ka = glm::vec3(0.5, 0.5, 0.5);
	bool hasTransparency = false;
	bool useFakeLighting = false;

public:
	GLuint textureID;
	int numberOfRows = 1;
	ModelTexture(GLuint id) : textureID(id) {};
	inline int getNormalMap() const { return normalMap; }
	inline void setNormalMap(const int n) { normalMap = n; }
	inline float getReflectivity() const { return reflectivity; }
	inline void setReflectivity(const float r) { reflectivity = r; }
	inline float getShineDamper() const { return shineDamper; }
	inline void setShineDamper(const float f) { shineDamper = f; }
	inline bool getHasTransparency() const { return hasTransparency; }
	inline void setHasTransparency(const bool b) { hasTransparency = b; }
	inline bool getUseFakeLighting() const { return useFakeLighting; }
	inline void setUseFakeLighting(const bool b) { useFakeLighting = b; }
	inline void setNumberOfRows(const int n) { numberOfRows = n; }
	inline glm::vec3 getAmbiantColor() const { return Ka; }
	inline void setAmbiantColor(const glm::vec3 Ka_) { Ka = Ka_; }
};
#endif
