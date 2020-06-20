#include "camera.h"
using namespace glm;

namespace Camera {

Camera::Camera() {
  Position = POSITION;
  WorldUp = UP;
  Front = FRONT;
  Pitch = PITCH;
  Yaw = YAW;
  MovementSpeed = SPEED;
  MouseSensitive = SENSITIVITY;
  FieldOfView = FIELD_OF_VIEW;
  updateCameraVectors();
}

Camera::Camera(vec3 pos, vec3 up, vec3 front, float pitch, float yaw, float movement, float mouseSensitive, float fov) {
  Position = pos;
  WorldUp = up;
  Front = front;
  Pitch = pitch;
  Yaw = yaw;
  MovementSpeed = movement;
  MouseSensitive = mouseSensitive;
  FieldOfView = fov;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, \
    float upX, float upY, float upZ, \
    float frontX, float frontY, float frontZ, \
    float pitch, float yaw, float movement, float mouseSensitive, float fov) {
  Position = vec3(posX, posY, posZ);
  WorldUp = vec3(upX, upY, upZ);
  Front = vec3(frontX, frontY, frontZ);
  Pitch = pitch;
  Yaw = yaw;
  MovementSpeed = movement;
  MouseSensitive = mouseSensitive;
  FieldOfView = fov;
  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  // calculate the new Front vector
  vec3 front;
  front.x = cos(radians(Yaw)) * cos(radians(Pitch));
  front.y = sin(radians(Pitch));
  front.z = sin(radians(Yaw)) * cos(radians(Pitch));

  Front = normalize(front);
  // also re-calculate the Right and Up vector
  Right = normalize(cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  Up    = normalize(cross(Right, Front));
}

mat4 Camera::GetViewMatrix()
{
    return lookAt(Position, Position + Front, Up);
}

void Camera::ProcessMouseScroll(float yoffset) {
    FieldOfView -= yoffset;
    if (FieldOfView < 1.0f)
        FieldOfView = 1.0f;
    if (FieldOfView > 90.0f)
        FieldOfView = 90.0f; 
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitive;
    yoffset *= MouseSensitive;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, float ratio)
{
    float velocity = MovementSpeed * deltaTime * ratio;
    switch(direction) {
      case FORWARD : {
        Position += velocity * Front;
      } break;
      case BACKWARD : {
        Position -= velocity * Front;
      } break;
      case LEFT : {
        Position -= velocity * Right;
      } break;
      case RIGHT : {
        Position += velocity * Right;
      } break;
      case RISE : {
        Position += velocity * Up;
      } break;
      case FALL : {
        Position -= velocity * Up;
      } break;
      default: break;
    }
}

}