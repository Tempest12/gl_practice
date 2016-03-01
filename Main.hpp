#ifndef _MAIN_HPP
#define _MAIN_HPP

#include <string>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void update(void);
void draw(void);
void init(void);

unsigned int compileShader(std::string* source, unsigned int type);
unsigned int linkProgram(unsigned int vertId, unsigned int fragId);

//Init functions:
void initBuffers(void);
void initShader(void);
void initTexture(void);


void checkForErrors(const std::string& lastCommandName);

#endif