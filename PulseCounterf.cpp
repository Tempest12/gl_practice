#include <math.h>

#include "PulseCounterf.hpp"

PulseCounterf::PulseCounterf(float min, float max, float delta)
{
    this->counter = min;
    this->delta   = delta;

    this->min = min;
    this->max = max;

    this->growing = true;
}

PulseCounterf::~PulseCounterf(void)
{
}

// Public:
void PulseCounterf::pulse(void)
{
    if(this->growing == true)
    {
        this->grow();
    }
    else
    {
        this->shrink();
    }
}

//Private:
void PulseCounterf::grow(void)
{
    this->counter += delta;

    if(counter >= max)
    {
        this->counter = max;
        this->growing = false;
    }
}

void PulseCounterf::shrink(void)
{
    this->counter -= delta;

    if(counter <= min)
    {
        this->counter = min;
        this->growing = true;
    }
}