#ifndef _PULSE_COUNTERF_HPP
#define _PULSE_COUNTERF_HPP

class PulseCounterf
{

//Variables:
public:

    float counter;
    float delta;

    float max;
    float min;

    bool growing;

protected:
private:

//Functions:
public:

    PulseCounterf(float min, float max, float delta);
    ~PulseCounterf(void);

    void pulse(void);

protected:
private:
    void grow(void);
    void shrink(void);

};

#endif