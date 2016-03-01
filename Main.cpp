#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstring>
#include <iostream>
#include <string>

#include "Main.hpp"

unsigned int windowWidth  = 800;
unsigned int windowHeight = 600;

const glm::mat4 identityMatrix = glm::mat4();

// Shader stuff:
unsigned int vertexShaderId       = 0;
unsigned int fragmentShaderId     = 0;
unsigned int programId            = 0;

// Buffers:
unsigned int vboID                = 0;
unsigned int vaoID                = 0;
unsigned int indexBufferId        = 0;

// Texture stuff:
unsigned int textureId            = 0;

std::string vertexShaderSource = 
"#version 330 core\n"\
""\
"layout (location = 0) in vec3 position;\n"\
"layout (location = 1) in vec2 texCoordIn;\n"\
""\
"out vec2 texCoord;\n"
""\
"uniform mat4 model;\n"\
"uniform mat4 view;\n"\
"uniform mat4 projection;\n"\
""\
"void main()\n"\
"{\n"\
"    gl_Position = projection * view * model * vec4(position.x  , position.y  , position.z, 1.0f);\n"\
"    texCoord    = vec2(texCoordIn.x, texCoordIn.y);\n"\
"}\n";

std::string fragmentShaderSource =
"#version 330 core\n"\
""\
"in vec2 texCoord;\n"\
""\
"uniform sampler2D textureOne;"\
""\
"out vec4 color;\n"\
""\
"void main()\n"\
"{\n"\
"   color = texture(textureOne, texCoord);\n"\
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

    // Now for some GL code:
    glViewport(0, 0, 800, 600);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    checkForErrors("Set the viewport");

    initBuffers();
    initShader();
    initTexture();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void update(void)
{
    glfwPollEvents();
}

void draw(void)
{
    glClearColor(0.5f, 0.5f, 0.75f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programId);

    glm::mat4 model      = glm::rotate(identityMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view       = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    int modelLocation      = glGetUniformLocation(programId, "model");
    int viewLocation       = glGetUniformLocation(programId, "view");
    int projectionLocation = glGetUniformLocation(programId, "projection");

    glUniformMatrix4fv(modelLocation     , 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLocation      , 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(programId, "textureOne"), 0);

    glBindVertexArray(vaoID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    // Vertex Buffer:
    float vertices[] =
    {
         // Positions:          // Tex coords:
         0.5f,  0.5f,  0.0f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f,  0.0f,    0.0f, 1.0f,
        -0.5f,  0.5f,  0.0f,    0.0f, 0.0f
    };

    // Index Buffer for the vertex buffer above.
    unsigned int indexBuffer[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glGenBuffers(1, &indexBufferId);
    glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

void initTexture(void)
{
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width  = 0;
    int height = 0;
    unsigned char* pixels = SOIL_load_image("smiley.png", &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
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