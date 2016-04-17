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

protected:
private:

//Functions:
public:

    Camera(void);
    ~Camera(void);

    glm::mat4 getCameraTransform(void);

protected:
private:
};

#endif