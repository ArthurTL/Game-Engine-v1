#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "TexturedModel.h"
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


using namespace std;
class Entity {

public:
	
	glm::vec3 position;
	float rx, ry, rz;
	float scale;
	vector<TexturedModel*>  meshes;
	const int textureIndex = 0;
	float useTriangleStrips = false;

	Entity(TexturedModel* model_, glm::vec3 position_, float rx_, float ry_, float rz_, float scale_) :
		meshes({ model_ }), position(position_), rx(rx_), ry(ry_), rz(rz_), scale(scale_) {};

	Entity(TexturedModel* model_, int index_, glm::vec3 position_, float rx_, float ry_, float rz_, float scale_) :
		meshes({ model_ }), textureIndex(index_), position(position_), rx(rx_), ry(ry_), rz(rz_), scale(scale_) {};

	Entity(vector<TexturedModel*> meshes_, int index_, glm::vec3 position_, float rx_, float ry_, float rz_, float scale_) :
		meshes(meshes_), textureIndex(index_), position(position_), rx(rx_), ry(ry_), rz(rz_), scale(scale_) {};

	inline void increasePosition(float dx, float dy, float dz) {
		position.x += dx;
		position.y += dy;
		position.z += dz;
	}

	inline void increaseRotation(float dx, float dy, float dz) {
		rx += dx;
		ry += dy;
		rz += dz;
	}

	inline float getTextureXOffset() {
		int column = textureIndex % meshes[0]->texture.numberOfRows;
		return (float)column / (float)meshes[0]->texture.numberOfRows;
	}
	inline float getTextureYOffset() {
		int row = textureIndex / meshes[0]->texture.numberOfRows;
		return (float)row / (float)meshes[0]->texture.numberOfRows;
	}
};

#endif


