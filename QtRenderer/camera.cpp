#include "camera.h"

Camera::Camera(Vec3f position, Vec3f target, float aspect) {
    assert((position - target).norm() > 1e-6 && aspect > 0);
    this->position = position;
    this->target = target;
    this->aspect = aspect;
}

Vec3f Camera::getPosition() {
    return position;
}

Vec3f Camera::getForward() {
    return (target - position).normalize();
}

Vec3f Camera::getTargetPosition() {
    return target;
}

Matrix Camera::getViewMatrix() {
    return lookat(position, target, UP);
}

Matrix Camera::getProjMatrix() {
    return perspective(FOVY, aspect, NEAR, FAR);
    //return projection(-1.f / (position - target).norm());
}

void Camera::rotateAroundTarget(Vec2f motion) {
    Vec3f target2Position = position - target;
    float radius = target2Position.norm();
    float yaw = std::atan2f(target2Position.x, target2Position.z);
    float pitch = std::asinf(target2Position.y / radius);

    float factor = (float)M_PI * 2.f;
    yaw -= motion.x * factor;
    pitch += motion.y * factor;

    if (pitch + M_PI / 2.f < FLT_EPSILON) pitch = -radians(89.9f);
    if (pitch - M_PI / 2.f > FLT_EPSILON) pitch = radians(89.9f);

    Vec3f offset;
    offset.x = radius * std::cosf(pitch) * std::sinf(yaw);
    offset.y = radius * std::sinf(pitch);
    offset.z = radius * std::cosf(pitch) * std::cosf(yaw);

    position = target + offset;
}

void Camera::moveTarget(Vec2f motion) {
    Vec3f position2Target = target - position;
    Vec3f forward = position2Target.normalize();
    Vec3f left = cross(UP, forward).normalize();
    Vec3f up = cross(forward, left).normalize();
    
    float distance = position2Target.norm();
    float factor = distance * tanf(radians(FOVY / 2)) * 200;

    Vec3f deltaX = left * motion.x * factor * aspect;
    Vec3f deltaY = up * motion.y * factor;
    target = target + deltaX + deltaY;
    position = position + deltaX + deltaY;
}

void Camera::scale(float ratio) {
    Vec3f target2Position = position - target;
    float radius = target2Position.norm();
    float yaw = std::atanf(target2Position.x / target2Position.z);
    float pitch = std::asinf(target2Position.y / radius);

    Vec3f offset;
    radius *= (float)std::pow(0.95, ratio);
    offset.x = radius * std::cosf(pitch) * std::sinf(yaw);
    offset.y = radius * std::sinf(pitch);
    offset.z = radius * std::cosf(pitch) * std::cosf(yaw);
    position = target + offset;
}