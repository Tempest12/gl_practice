#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstring>
#include <iostream>
#include <string>

#include "Camera.hpp"
#include "Main.hpp"
#include "PulseCounterf.hpp"

unsigned int windowWidth  = 800;
unsigned int windowHeight = 600;

const glm::mat4 identityMatrix = glm::mat4();
Camera* camera;
PulseCounterf* pulse;

// Camera Control Keys:
bool upKeyDown       = false;
bool downKeyDown     = false;
bool leftKeyDown     = false;
bool rightKeyDown    = false;
bool forwardKeyDown  = false;
bool backwardKeyDown = false;

// Shader stuff:
unsigned int vertexShaderId       = 0;
unsigned int fragmentShaderId     = 0;
unsigned int programId            = 0;

// Buffers:
unsigned int vboID                = 0;
unsigned int vaoID                = 0;

// Texture stuff:

std::string vertexShaderSource = 
"#version 330 core\n"\
""\
"layout (location = 0) in  vec3 position;\n"\
"layout (location = 1) in  vec2 texCoordIn;\n"\
""\
"out vec2 texCoord;\n"
""\
"uniform mat4 vs_model;\n"\
"uniform mat4 vs_view;\n"\
"uniform mat4 vs_projection;\n"\
""\
"void main()\n"\
"{\n"\
"    gl_Position = vs_projection * vs_view * vs_model * vec4(position.x  , position.y  , position.z, 1.0f);\n"\
"    texCoord    = vec2(texCoordIn.x, texCoordIn.y);\n"\
"}\n";

std::string fragmentShaderSource =
"#version 330 core\n"\
""\
"in vec2 texCoord;\n"\
""\
"out vec4 color;\n"\
""\
"uniform float fs_scalar;\n"\
""\
"void main()\n"\
"{\n"\
"   float threshold = 1.0f;\n"\
"   float distance = sqrt((texCoord.x * texCoord.x) + (texCoord.y * texCoord.y));\n"\
"   float result = distance - threshold;\n"\
"   result *= fs_scalar;\n"\
"   color = vec4(result, result, result, result);\n"\
"}\n";

int main(int argc, char** argv)
{
    // GLFW initailizations. (Sort of replaces glut...)

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "gl", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Couldn't create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // GLEW initializations:
    glewExperimental = true;
    GLenum error = glewInit();
    checkForErrors("Glew sucks");

    if(error != GLEW_OK)
    {
        std::cout << "Couldn't initialize glew." << std::endl;
        glfwTerminate();
        return -1;
    }

    camera = new Camera();
    pulse = new PulseCounterf(1.5f, 2.65f, 0.015f);

    // Now for some GL code:
    glViewport(0, 0, 800, 600);
    checkForErrors("Set the viewport");

    initBuffers();
    initShader();

    glPolygonMode(GL_FRONT, GL_FILL);

    //Depth:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Alpha:
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.75f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(glfwWindowShouldClose(window) == false)
    {
        update();
        draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            if(action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }
            break;

        case GLFW_KEY_E:
            upKeyDown = (action == GLFW_PRESS);
            break;

        case GLFW_KEY_Q:
            downKeyDown = (action == GLFW_PRESS);
            break;

        case GLFW_KEY_A:
            leftKeyDown = (action == GLFW_PRESS);
            break;

        case GLFW_KEY_D:
            rightKeyDown = (action == GLFW_PRESS);
            break;

        case GLFW_KEY_W:
            forwardKeyDown = (action == GLFW_PRESS);
            break;

        case GLFW_KEY_S:
            backwardKeyDown = (action == GLFW_PRESS);
            break;
    }

}

void update(void)
{
    glfwPollEvents();

    if(upKeyDown == true)
    {
        camera->moveUp(1.0f);
    }
    if(downKeyDown == true)
    {
        camera->moveDown(1.0f);
    }

    if(leftKeyDown == true)
    {
        camera->moveLeft(1.0f);
    }
    if(rightKeyDown == true)
    {
        camera->moveRight(1.0f);
    }

    if(forwardKeyDown == true)
    {
        camera->moveForward(1.0f);
    }
    if(backwardKeyDown == true)
    {
        camera->moveBackward(1.0f);
    }

    pulse->pulse();
}

void draw(void)
{
    glClearColor(0.0f, 0.2f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programId);

    glm::mat4 model      = glm::mat4(camera->getCameraTransform());
    glm::mat4 view       = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    int vs_modelLocation      = glGetUniformLocation(programId, "vs_model");
    int vs_viewLocation       = glGetUniformLocation(programId, "vs_view");
    int vs_projectionLocation = glGetUniformLocation(programId, "vs_projection");
    int fs_scalarLocation     = glGetUniformLocation(programId, "fs_scalar");

    glUniformMatrix4fv(vs_modelLocation     , 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(vs_viewLocation      , 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(vs_projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(fs_scalarLocation, pulse->counter);

    glBindVertexArray(vaoID);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);

    glUseProgram(0);
}

unsigned int linkProgram(unsigned int vertId, unsigned int fragId)
{
    unsigned int progId = glCreateProgram();
    glAttachShader(progId, vertId);
    glAttachShader(progId, fragId);
    glLinkProgram( progId);

    //Did the program link?
    int errorCode = 0;

    glGetProgramiv(progId, GL_LINK_STATUS, &errorCode);

    if(errorCode == GL_FALSE)
    {
        int logSize = 0;
        char* errorLog;

        glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &logSize);
        errorLog = new char[logSize];
        memset(errorLog, 0, logSize);
        glGetProgramInfoLog(progId, logSize, NULL, errorLog);

        std::cout << "Shader Program didn't link... Error log: \"" << errorLog << "\"." << std::endl;
        exit(1);
    }

    return progId;
}

unsigned int compileShader(std::string* source, unsigned int type)
{
    int errorCode = 0;

    int shaderLength = source->size();
    const char* sourcePointer = source->c_str();
    unsigned int id = glCreateShader(type);
    
    glShaderSource(id, 1, &sourcePointer, &shaderLength);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &errorCode);

    if(errorCode == GL_FALSE)
    {
        //Compilation has failed.  We're going to quit but we want to see the error message.
        int logSize = 0;
        char* errorLog;    

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logSize);   
        errorLog = new char[logSize];
        glGetShaderInfoLog(id, logSize, NULL, errorLog);

        std::cout << "Shader: " << type << " failed to compile.  Compilation error message \"" <<  errorLog << "\"." << std::endl;

        exit(1);
    }

    return id;
}

void initBuffers(void)
{
    float maxx = 1.0f;
    float star = 1.0f;
    float size = 1.5f;

    // Vertex Buffer:
    float vertices[] =
    {
         // Positions:          // Tex coords:
         0.0f,  size,  0.0f,  0.0f, star,
         0.0f,  0.0f,  0.0f,  maxx, maxx,
        -size,  0.0f,  0.0f,  star, 0.0f,

         0.0f,  0.0f,  0.0f,  maxx, maxx,
        -size,  0.0f,  0.0f,  star, 0.0f,
         0.0f, -size,  0.0f,  0.0f, star,

         size,  0.0f,  0.0f,  star, 0.0f,
         0.0f,  0.0f,  0.0f,  maxx, maxx,
         0.0f, -size,  0.0f,  0.0f, star,

         size,  0.0f,  0.0f,  star, 0.0f,
         0.0f,  size,  0.0f,  0.0f, star,
         0.0f,  0.0f,  0.0f,  maxx, maxx,
    };

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void initShader(void)
{
    vertexShaderId   = compileShader(&vertexShaderSource  , GL_VERTEX_SHADER);
    fragmentShaderId = compileShader(&fragmentShaderSource, GL_FRAGMENT_SHADER);
    programId        = linkProgram(vertexShaderId, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void checkForErrors(const std::string& lastCommandName)
{
    int errorCode = glGetError();

    if(errorCode != GL_NO_ERROR)
    {
        std::cout << "Okay we did something wrong when we did this: " <<  lastCommandName << std::endl;
        std::cout << "Error code is: " << errorCode << std::endl;
    }
}