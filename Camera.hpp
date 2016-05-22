#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class Vector3f;

class Camera
{
// Variables:
public:

    Vector3f* position;

    Vector3f* forward;
    Vector3f* backward;
    Vector3f* left;
    Vector3f* right;
    Vector3f* up;
    Vector3f* down;

    glm::fquat* rotation;

    glm::mat4 transform;

    float panSpeed;
    float runSpeed;
    float walkSpeed;

protected:
private:

//Functions:
public:

    Camera(void);
    ~Camera(void);

    glm::mat4 getCameraTransform(void);

    void moveForward(float time);
    void moveBackward(float time);
    void moveLeft(float time);
    void moveRight(float time);
    void moveUp(float time);
    void moveDown(float time);

    void panCameraHorizontally(int magnitude);
    void panCameraVertically  (int magnitude);

protected:
private:
};

#endif