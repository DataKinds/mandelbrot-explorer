#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "glShaderHelpers.h"
#include "constants.h"
#include "mandel.h"


SDL_Window* mandelWin;
unsigned long long frameNum = 0;

//http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    //oh goodie - let's set the openGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //initialize GLEW
    //use openGL 3.0 and later
    glewExperimental = GL_TRUE;
    glewInit();
    glewExperimental = GL_TRUE;
    //done with glew

    mandelWin = initMandel();
    //finally, go into the main loop (praise jesus)
    while (updateMandel()) {
        drawMandel();
        //set framenum uniform
        //glUniform1ui(frameNumLoc, frameNum);
    }
    destroyMandel();
    SDL_Quit();
    return 0;
}
