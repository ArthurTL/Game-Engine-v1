#include "Player.h"
const float Player::RUN_SPEED = 30.;
const float Player::TURN_SPEED = 200.;
const float Player::GRAVITY = -60.;
const float Player::JUMP_POWER = 25.;
const float Player::TERRAIN_HEIGHT = 0.;

void Player::move(Terrain terrain) {
	const float t = WindowManager::getFrameTimeSeconds();
	increaseRotation(0, currentTurnSpeed *t , 0);
	const float distance = currentSpeed * t;
	const float dx = distance * sin(radians(ry));
	const float dz = distance * cos(radians(ry));
	increasePosition(dx, 0, dz);
	upwardsSpeed += GRAVITY * t;
	increasePosition(0, upwardsSpeed * t, 0);
	float terrainHeight = terrain.getHeightOfTerrain(position.x, position.z);
	if (position.y < terrainHeight) {
		isInAir = false;
		upwardsSpeed = 0;
		position.y = terrainHeight;
	}
}

void Player::jump() {
	if (!isInAir) {
		upwardsSpeed = JUMP_POWER;
		isInAir = true;
	}
}

const void Player::checkInputs(int key, int action) {
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		currentSpeed = RUN_SPEED;
	};
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		currentSpeed = 0;
	};
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		currentSpeed = -RUN_SPEED;
	};
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		currentSpeed = 0;
	};
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		currentTurnSpeed = -TURN_SPEED;
	};
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		currentTurnSpeed = 0;
	};
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		currentTurnSpeed = TURN_SPEED;
	};
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		currentTurnSpeed = 0;
	};
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		jump();
	};
}