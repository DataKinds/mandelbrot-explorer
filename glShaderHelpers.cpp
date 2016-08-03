#include "glShaderHelpers.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <sstream>
#include <fstream>

std::string readFile(std::string filePath) {
    std::ifstream inputFile = std::ifstream(filePath);
    std::stringstream outStringStream;
    outStringStream << inputFile.rdbuf();
    return outStringStream.str();
}
GLuint loadAndCompileShader(std::string programPath, GLenum shaderType) {
    std::string program = readFile(programPath);
    const char* programCStr = program.c_str();
    int shaderId = glCreateShader(shaderType);
    return glShaderSource(shaderId, 1, &programCStr, NULL);
}
