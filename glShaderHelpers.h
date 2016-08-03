#include <string>
#include <GL/glu.h>
std::string readFile(std::string filePath);
GLuint loadAndCompileShader(std::string program, GLenum shaderType);
