#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


#include "Camera.hpp"
#include "Vector3f.hpp"

Camera::Camera(void)
{
    this->position  = new Vector3f(0.0f);

    this->forward    = new Vector3f( 0.0f,  0.0f,  1.0f);
    this->backward  = new Vector3f( 0.0f,  0.0f, -1.0f);

    this->right     = new Vector3f( 1.0f,  0.0f,  0.0f);
    this->left      = new Vector3f(-1.0f,  0.0f,  0.0f);

    this->up        = new Vector3f( 0.0f,  1.0f,  0.0f);
    this->down      = new Vector3f( 0.0f, -1.0f,  0.0f);

    this->transform = glm::mat4();
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

glm::mat4 Camera::getCameraTransform(void)
{
    this->transform = glm::lookAt(this->position->toGLM(),
                                  this->forward->toGLM(),
                                  this->up->toGLM());

    return this->transform;
}