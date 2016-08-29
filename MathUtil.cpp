#include <math.h>

#include "MathUtil.hpp"

//Static stuff:
static const float piOverOneEighty = M_PI  / 180.0f;
static const float oneEightyOverPi = 180.f / M_PI;


float MathUtil::degreeCos(float angle)
{
    return cos(degreesToRadians(angle));
}

float MathUtil::degreeSin(float angle)
{
    return sin(degreesToRadians(angle));
}

float MathUtil::degreeTan(float angle)
{
    return tan(degreesToRadians(angle));
}

float MathUtil::radianCos(float angle)
{
    return cos(angle);
}

float MathUtil::radianSin(float angle)
{
    return sin(angle);
}

float MathUtil::radianTan(float angle)
{
    return tan(angle);
}

float MathUtil::degreesToRadians(float degrees)
{
    return degrees * piOverOneEighty;
}

float MathUtil::radiansToDegrees(float radians)
{
    return radians * oneEightyOverPi;
}