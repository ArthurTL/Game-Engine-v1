#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Player.h"

using namespace glm;

class Camera {
public:

    Camera(Player* player_):player(player_) {};
    inline float getFov() const { return fov; }
    inline void setFoV(const float f) { fov = f; }
    inline float getAspectRatio() const { return aspectRatio; }
    inline void setAspectRatio(const float a) { aspectRatio = a; }
    inline float getNear() const { return near_plane; }
    inline void setNear(const float n) { near_plane = n; }
    inline float getFar() const { return far_plane; }
    inline void setFar(const float n) { far_plane = n; }
    inline float getPitch() const { return pitch; }
    inline void setPitch(const float n) { pitch= n; }
    inline float getYaw() const { return yaw; }
    inline void setYaw(const float n) { yaw = n; }
    inline float getRoll() const { return roll; }
    inline void setRoll(const float n) { roll = n; }
    inline float getAngleAroundPlayer() const { return angleAroundPlayer; }
    inline void setAngleAroundPlayer(const float a) { angleAroundPlayer = a; }
    inline void setMoveForward(const bool b) { moveForward_ = b; }
    inline void setMoveBack(const bool b) { moveBack_ = b; }
    inline void setMoveLeft(const bool b) { moveLeft_ = b; }
    inline void setMoveRight(const bool b) { moveRight_ = b; }
    inline void setMoveUp(const bool b) { moveUp_ = b; }
    inline void setMoveDown(const bool b) { moveDown_ = b; }
    

    inline void setPosition(const glm::vec3& p) { pos = p; }
    inline glm::vec3 getPosition() { return pos; }

    inline void invertPitch() { pitch = -pitch; };
    inline void moveUp() {if(moveUp_) pos.y += 0.5f; }
    inline void moveRight() {if(moveRight_) pos.x += 0.5f; }
    inline void moveLeft() {if(moveLeft_) pos.x -= 0.5f; }
    inline void moveDown() { if (moveDown_) pos.y -= 0.5f; }
    inline void moveForward() { if (moveForward_) pos.z -= 0.5f; }
    inline void moveBack() { if (moveBack_) pos.z += 0.5f; }
    inline void moveFront(double offset) {pos.z -= 3.*offset; }
    //inline void move() { moveBack(); moveRight(); moveLeft(); moveForward(); moveUp(); moveDown(); }
    inline void move() { calculateCameraPosition(distanceFromPlayer); }
    inline void calculateZoom(double offset) { distanceFromPlayer = glm::max(10.,distanceFromPlayer -offset);}


    inline glm::mat4 computeViewMatrix() const {
        mat4 viewMatrix(1.0);
        viewMatrix = glm::rotate(viewMatrix, radians(getPitch()), glm::vec3(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, radians(getYaw()), glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, radians(getRoll()), glm::vec3(0, 0, 1));
        vec3 cameraPos = pos;
        vec3 negativeCameraPos = vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
        viewMatrix = glm::translate(viewMatrix, negativeCameraPos);
        return viewMatrix;
    }

    inline glm::mat4 computeProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, near_plane, far_plane);
    }

private:
    Player* player;
    float distanceFromPlayer = 25;
    float angleAroundPlayer = 0;
    glm::vec3 pos = glm::vec3(80, 7, 0);
    bool moveForward_=false;
    bool moveLeft_=false;
    bool moveRight_ = false;
    bool moveBack_ = false;
    bool moveUp_ = false;
    bool moveDown_ = false;
    float fov = 70.f;        // Field of view, in degrees
    float aspectRatio = 1280.f/720.f; // Ratio between the width and the height of the image
    float near_plane = 0.1f; // Distance before which geometry is excluded fromt he rasterization process
    float far_plane = 1000.f; // Distance after which the geometry is excluded fromt he rasterization process
    float pitch=20;
    float yaw=0;
    float roll=0;

    
    inline void calculateCameraPosition(float distanceFromPlayer) {
        float hdistance = distanceFromPlayer * cos(radians(pitch));
        float vdistance = distanceFromPlayer * sin(radians(pitch));
        float theta = player->ry + angleAroundPlayer;
        float offsetX = hdistance * sin(radians(theta));
        float offsetZ = hdistance * cos(radians(theta));
        pos.x = player->position.x - offsetX;
        pos.y = 5+ player->position.y + vdistance;
        pos.z = player->position.z - offsetZ;
        yaw = 180 - (player->ry + angleAroundPlayer);
    }
};

#endif