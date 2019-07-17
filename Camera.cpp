#include "Camera.hpp"
#include "Vector3f.hpp"

Camera::Camera(void)
{
    this->position  = new Vector3f( 0.0f,  0.0f,  0.0f);

    this->forward   = new Vector3f( 0.0f,  0.0f,  1.0f);
    this->backward  = new Vector3f( 0.0f,  0.0f, -1.0f);

    this->right     = new Vector3f(-1.0f,  0.0f,  0.0f);
    this->left      = new Vector3f( 1.0f,  0.0f,  0.0f);

    this->up        = new Vector3f( 0.0f,  1.0f,  0.0f);
    this->down      = new Vector3f( 0.0f, -1.0f,  0.0f);

    //this->transform = new glm::mat4();

    this->panSpeed  = 0.55f;

    this->runSpeed  = 4.5f;
    this->walkSpeed = 0.35f;
}

Camera::~Camera(void)
{
    delete position;

    delete forward;
    delete backward;
    delete right;
    delete left;
    delete up;
    delete down;
}

/*glm::mat4* Camera::getCameraTransform(void)
{
    Vector3f focalPoint = Vector3f(this->position);
    focalPoint.add(this->forward);

    *this->transform = glm::lookAt(this->position->toGLM(),
                                  focalPoint.toGLM(),
                                  this->up->toGLM());

    return this->transform;
}*/

void Camera::moveForward(float time)
{
    Vector3f delta = Vector3f(this->forward);
    delta.scale(time * this->walkSpeed);
    this->position->add(delta);
}

void Camera::moveBackward(float time)
{
    Vector3f delta = Vector3f(this->backward);
    delta.scale(time * this->walkSpeed);
    this->position->add(delta);
}

void Camera::moveLeft(float time)
{
    Vector3f delta = Vector3f(this->left);
    delta.scale(time * this->walkSpeed);
    this->position->add(delta);
}

void Camera::moveRight(float time)
{
    Vector3f delta = Vector3f(this->right);
    delta.scale(time * this->walkSpeed);
    this->position->add(delta);
}

void Camera::moveUp(float time)
{
    Vector3f delta = Vector3f(this->up);
    delta.scale(time * this->walkSpeed);
    this->position->add(delta);
}

void Camera::moveDown(float time)
{
    Vector3f delta = Vector3f(this->down);
    delta.scale(time * this->walkSpeed);
    this->position->add(delta);
}

void Camera::panCameraHorizontally(int magnitude)
{
}

void Camera::panCameraVertically(int magnitude)
{
}
