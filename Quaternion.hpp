#ifndef _QUATERNION_HPP
#define _QUATERNION_HPP

class Vector3f;

class Quaternion
{
//Variables:
public:

    float x;// I
    float y;// J
    float z;// K
    float w;// Real number

protected:
private:

//Function:
public:

    Quaternion();
    Quaternion(float x, float y, float z, float w);
    ~Quaternion();

    void add(Quaternion* that);
    bool equals(Quaternion* that);
    void multiply(Quaternion* that);
    void normalize(void);
    void print(void);
    void subtract(Quaternion* that);

protected:
private:
};

#endif