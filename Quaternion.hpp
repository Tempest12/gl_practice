#ifndef _QUATERNION_HPP
#define _QUATERNION_HPP

#include "glm/glm.hpp"

//Forward declarations:
class Vector3f;

class Quaternion
{
//Variables:
public:

    float x;// I
    float y;// J
    float z;// K
    float w;// Real number

    glm::mat4* rotationMatrix;

protected:
private:

//Function:
public:

    Quaternion(float x, float y, float z, float w);
    ~Quaternion();

    void       add(Quaternion* that);
    bool       equals(Quaternion* that);
    glm::mat4* getRotationMatrix(void);
    void       multiply(Quaternion* that);
    void       normalize(void);
    void       print(void);
    void       setFromEulerAngles(float xRotation, float yRotation, float zRotation);
    void       subtract(Quaternion* that);

protected:
private:
};

#endif