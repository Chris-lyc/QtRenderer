#pragma once

#include "geometry.h"

static const Vec3f CAMERA_POSITION = { 0, 0, 3 };
static const Vec3f CAMERA_TARGET = { 0, 0, 0 };
static const float NEAR = 0.1f;
static const float FAR = 100.f;
static const float FOVY = radians(45.f);
static const Vec3f UP = { 0, 1, 0 };

struct Motion { Vec2f orbit; Vec2f pan; float dolly; };

class Camera
{
public:
	Camera(Vec3f position, Vec3f target, float aspect);
	Camera(float aspect) :position(CAMERA_POSITION), target(CAMERA_TARGET), aspect(aspect) {};
	~Camera() {};
	Vec3f getPosition();
	Vec3f getForward();
	Vec3f getTargetPosition();
	Matrix getViewMatrix();
	Matrix getPerspectiveMatrix();
	Matrix getOrthoMatrix();

	void setPosition(Vec3f pos);
	void rotateAroundTarget(Vec2f motion);
	void moveTarget(Vec2f motion);
	void scale(float ratio);

private:
	Vec3f position;
	Vec3f target;
	float aspect;


};