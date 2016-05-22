#ifndef _QUATERNION_HPP
#define _QUATERNION_HPP

class Vector3f;

class Quaternion
{
//Variables:
public:

    float x;
    float y;
    float z;
    float w;

protected:
private:

//Function:
public:

    Quaternion();
    ~Quaternion();

    void add(Quaternion* that);
    void normalize(void);

protected:
private:
};

#endif