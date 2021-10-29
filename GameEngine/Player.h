#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "WindowManager.h"
#include "Terrain.h"

using namespace glm;
class Player : public Entity {

private:

	static const float RUN_SPEED;
	static const float TURN_SPEED;
	static const float GRAVITY;
	static const float JUMP_POWER;
	static const float TERRAIN_HEIGHT;
	float currentSpeed = 0.;
	float currentTurnSpeed = 0.;
	float upwardsSpeed = 0;
	bool isInAir = false;


public:
	Player(TexturedModel *model_, glm::vec3 position_, float rx_, float ry_, float rz_, float scale_) :
		Entity({model_}, position_, rx_, ry_, rz_, scale_) {};
	void const checkInputs(int key, int action);
	void move(Terrain terrain);
	void jump();

};



#endif
