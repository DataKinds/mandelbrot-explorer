#include <GL/glew.h>
#include <string>

std::string readFile(std::string filePath);
GLuint loadAndCompileShader(std::string program, GLenum shaderType);
