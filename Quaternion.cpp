#include <math.h>
#include <iostream>

#include "MathUtil.hpp"
#include "Quaternion.hpp"

Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;

    this->rotationMatrix = new glm::mat4();
}

Quaternion::~Quaternion()
{
}

void Quaternion::add(Quaternion* that)
{
    this->x += that->x;
    this->y += that->y;
    this->z += that->z;
    this->w += that->w;
}

void Quaternion::addAndNormalize(Quaternion* that)
{
    this->add(that);
    this->normalize();
}

void Quaternion::conjugate(void)
{
    // Conjugation just means we invert(multiply by -1.0f) the imanginary coefficents: (The scalar value is unchagned.)

    this->x *= -1.0f;
    this->y *= -1.0f;
    this->z *= -1.0f;
}

bool Quaternion::equals(Quaternion* that)
{
    return (this->x == that->x) && (this->y == that->y) && (this->z == that->z) && (this->w = that->w);
}

glm::mat4* Quaternion::getRotationMatrix(void)
{
    float x1 = this->x;
    float y1 = this->y;
    float z1 = this->z;
    float w1 = this->w;

    float x2 = this->x * this->x;
    float y2 = this->y * this->y;
    float z2 = this->z * this->z;
    float w2 = this->w * this->w;

    (*this->rotationMatrix)[0].x = w2 + x2 - y2 - z2;
    (*this->rotationMatrix)[0].y = 2.0f * (x1 * y1 - w1 * z1);
    (*this->rotationMatrix)[0].z = 2.0f * (w1 * y1 + x1 * z1);
    (*this->rotationMatrix)[0].w = 0.0f;

    (*this->rotationMatrix)[1].x = 2.0f * (x1 * y1 + w1 * z1);
    (*this->rotationMatrix)[1].y = w2 - x2 + y2 - z2;
    (*this->rotationMatrix)[1].z = 2.0f * (y1 * z1 - w1 * x1);
    (*this->rotationMatrix)[1].w = 0.0f;

    (*this->rotationMatrix)[2].x = 2.0f * (x1 * z1 - w1 * y1);
    (*this->rotationMatrix)[2].y = 2.0f * (w1 * x1 + y1 * z1);
    (*this->rotationMatrix)[2].z = w2 - x2 - y2 + z2;
    (*this->rotationMatrix)[2].w = 0.0f;

    (*this->rotationMatrix)[3].x = 0.0f;
    (*this->rotationMatrix)[3].y = 0.0f;
    (*this->rotationMatrix)[3].z = 0.0f;
    (*this->rotationMatrix)[3].w = 1.0f;

    return this->rotationMatrix;
}

void Quaternion::multiply(Quaternion* that)
{
    float newX = this->w * that->x + this->x * that->w + this->y * that->z - this->z * that->y;
    float newY = this->w * that->y - this->x * that->z + this->y * that->w + this->z * that->x;
    float newZ = this->w * that->z + this->x * that->y - this->y * that->x + this->z * that->w;
    float newW = this->w * that->w - this->x * that->x - this->y * that->y - this->z * that->z;

    this->x = newX;
    this->y = newY;
    this->z = newZ;
    this->w = newW;
}

void Quaternion::normalize(void)
{
    float magnitude = sqrt(this->x * this->x + this->y * this->y +
                           this->z * this->z + this->w * this->w);

    this->x /= magnitude;
    this->y /= magnitude;
    this->z /= magnitude;
    this->w /= magnitude;
}

void Quaternion::print(void)
{
    std::cout << "X: " << this->x << " ";
    std::cout << "Y: " << this->y << " ";
    std::cout << "Z: " << this->z << " ";
    std::cout << "W: " << this->w << std::endl;
}

void Quaternion::scale(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    this->w *= scalar;
}

void Quaternion::setFromEulerAngles(float xRotation, float yRotation, float zRotation)
{
    float cx = MathUtil::degreeCos(xRotation / 2.0f);
    float cy = MathUtil::degreeCos(yRotation / 2.0f);
    float cz = MathUtil::degreeCos(zRotation / 2.0f);

    float sx = MathUtil::degreeSin(xRotation / 2.0f);
    float sy = MathUtil::degreeSin(yRotation / 2.0f);
    float sz = MathUtil::degreeSin(zRotation / 2.0f);

    this->w = (cx * cy * cz) + (sx * sy * sz);
    this->x = (sx * cy * cz) - (cx * sy * sz);
    this->y = (cx * sy * cz) + (sx * cy * sz);
    this->z = (cx * cy * sz) - (sx * sy * cz);
}

void Quaternion::subtract(Quaternion* that)
{
    this->x -= that->x;
    this->y -= that->y;
    this->z -= that->z;
    this->w -= that->w;
}

////////////////////////////////////////////////////////////////////////////////
// Static Stuff:
////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::getConjugateOf(Quaternion* that)
{
    return Quaternion(that->x * -1.0f, that->y * -1.0f, that->z * -1.0f, that->w);
}