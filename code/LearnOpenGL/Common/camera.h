#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace Camera {
  
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    RISE,
    FALL,
};

// Default camera values
const float YAW           = -90.0f;
const float PITCH         =  0.0f;
const float SPEED         =  2.5f;
const float SENSITIVITY   =  0.05f;
const float FIELD_OF_VIEW =  45.0f;

const vec3 POSITION(0.0f, 0.0f, 0.0f);
const vec3 FRONT(0.0f, 0.0f, -1.0f);
const vec3 UP(0.0f, 1.0f, 0.0f);

class Camera {
public:
  vec3 WorldUp;

  vec3 Position;
  vec3 Front;
  vec3 Up;
  vec3 Right;

  float Pitch;
  float Yaw;

  float MovementSpeed;
  float MouseSensitive;
  float FieldOfView;

  Camera();
  Camera(vec3 pos=POSITION, vec3 up=UP, vec3 front=FRONT, float pitch=PITCH, float yaw=YAW, float movement=SPEED, float mouseSensitive=SENSITIVITY, float fov=FIELD_OF_VIEW);
  Camera(float posX=POSITION.x, float posY=POSITION.y, float posZ=POSITION.z, \
    float upX=UP.x, float upY=UP.y, float upZ=UP.z, \
    float frontX=FRONT.x, float frontY=FRONT.y, float frontZ=FRONT.z, \
    float pitch=PITCH, float yaw=YAW, float movement=SPEED, float mouseSensitive=SENSITIVITY, float fov=FIELD_OF_VIEW);

  // returns the view matrix calculated using Euler Angles and the LookAt Matrix
  mat4 GetViewMatrix();

  // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);

  // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

  // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset);

private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();
};

}