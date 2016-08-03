#include "glShaderHelpers.h"
#include <GL/glew.h>
#include <sstream>
#include <fstream>

//http://headerphile.com/sdl2/opengl-part-3-shaders-and-the-rendering-pipeline/
std::string readFile(std::string filePath) {
    std::ifstream t(filePath);

    // Read file into buffer
    std::stringstream buffer;
    buffer << t.rdbuf();

    // Make a std::string and fill it with the contents of buffer
    std::string fileContent = buffer.str();

    return fileContent;
}
GLuint loadAndCompileShader(std::string programPath, GLenum shaderType) {
    std::string program = readFile(programPath);
    const char* programCStr = program.c_str();
    int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &programCStr, NULL);
    glCompileShader(shaderId);
    return shaderId;
}
