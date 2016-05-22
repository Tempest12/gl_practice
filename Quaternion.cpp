#include <math.h>

#include "Quaternion.hpp"

Quaternion::Quaternion()
{
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

void Quaternion::normalize(void)
{
    float magnitude = sqrt(this->x * this->x + this->y * this->y +
                           this->z * this->z + this->w * this->w);

    this->x /= magnitude;
    this->y /= magnitude;
    this->z /= magnitude;
    this->w /= magnitude;
}