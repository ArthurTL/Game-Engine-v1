#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "glm.hpp"

using namespace glm;

class Light {
private:
	vec3 position;
	vec3 color;
	vec3 trueColor;
	vec3 attenuation = vec3(1., 0., 0.);
	float time=17000.;
public:
	Light(vec3 position_, vec3 color_) :position(position_), color(color_), trueColor(color_) {};
	Light(vec3 position_, vec3 color_, vec3 attenuation_) :position(position_), color(color_), trueColor(color_), attenuation(attenuation_) {};
	inline vec3 getPosition() const { return position; }
	inline void setPosition(const vec3 p) { position = p; }
	inline vec3 getColor() const { return color; }
	inline void setColor(const vec3 c) { color = c; }
	inline vec3 getAttenuation() const { return attenuation; }
	inline void updateSun(float time){
		
		float intensityFactor;
		if (time >= 0 && time < 5000) {
			intensityFactor = 0.1;

		}
		else if (time >= 5000 && time < 8000) {
			intensityFactor = ((time - 5000) / (8000 - 5000))*0.7+0.1;
		}
		else if (time >= 8000 && time < 20000) {
			intensityFactor = 0.8;
			
		}
		else {
			intensityFactor = 0.8-((time - 20000) / (24000 - 20000))*0.7;
		
		}
		
		color = trueColor * intensityFactor;

	}
	inline void updateLamp(float time) {

		float intensityFactor;
		if (time >= 0 && time < 5000) {
			intensityFactor = 0.;

		}
		else if (time >= 5000 && time < 8000) {
			intensityFactor = ((time - 5000) / (8000 - 5000));
		}
		else if (time >= 8000 && time < 20000) {
			intensityFactor = 1.;

		}
		else {
			intensityFactor = 1. - ((time - 20000) / (24000 - 20000));

		}

		color = trueColor * (1-intensityFactor);

	}
	
};



#endif
