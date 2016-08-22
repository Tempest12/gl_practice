#include <math.h>
#include <iostream>

#include "Quaternion.hpp"

Quaternion::Quaternion()
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
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

bool Quaternion::equals(Quaternion* that)
{
    return (this->x == that->x) && (this->y == that->y) && (this->z == that->z) && (this->w = that->w);
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

void Quaternion::subtract(Quaternion* that)
{
    this->x -= that->x;
    this->y -= that->y;
    this->z -= that->z;
    this->w -= that->w;
}