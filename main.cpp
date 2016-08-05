#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "glShaderHelpers.h"
#define SW 1024
#define SH 1024

float rectPoints[] = {
    -1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f,
    -1.0f, 1.0f
};
GLuint vao;
GLuint bigRectVbo;
GLuint shaderProgram;
GLint scaleLoc;
unsigned long long frameNum = 0;

void draw(SDL_Window* win) {
    //clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //load them nice shaders
    glUseProgram(shaderProgram);
    //use the VAO we defined earlier
    //(which is populated with rectPoints[])
    glBindVertexArray(vao);
    //draw the rectPoints
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    SDL_GL_SwapWindow(win);
}

//0 if closing, 1 if not
int update(SDL_Window* win) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return 0;
        } else if (e.type == SDL_KEYDOWN) {
            printf("%i\n", e.key.keysym.sym);
            if (e.key.keysym.sym == SDLK_q) {
                return 0;
            }
            else if (e.key.keysym.sym == SDLK_e) {

            }
        }
    }
    SDL_Delay(10); //delay the amount to keep framerate
    frameNum++;
    return 1;
}

//http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("OpenGL 3.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SW, SH, SDL_WINDOW_OPENGL);
    //oh goodie - let's set the openGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //get an SDL_GLContext for openGL stuff
    SDL_GLContext glc = SDL_GL_CreateContext(win);
    //initialize GLEW
    //use openGL 3.0 and later
    glewExperimental = GL_TRUE;
    glewInit();
    glewExperimental = GL_TRUE;
    //done with glew
    //initialize the VAOs and VBOs
    //put the points in a Vertex Buffer Object
    //pretty much, the VBO just holds info about
    //a shape - position, color, etc.
    bigRectVbo = 0;
    //generate vbo
    glGenBuffers(1, &bigRectVbo);
    //put info in vbo
    glBindBuffer(GL_ARRAY_BUFFER, bigRectVbo);
    //tell gpu what's in buffer
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), rectPoints, GL_STATIC_DRAW);

    //create a VAO
    //the VAO just holds all the VBOs
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    //pass the VAO to argument 0 of the vertex shader
    //2 points at a time
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    //load up the shader
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, loadAndCompileShader("fragShader.glsl", GL_FRAGMENT_SHADER));
    glAttachShader(shaderProgram, loadAndCompileShader("vertShader.glsl", GL_VERTEX_SHADER));
    //link it all together
    glLinkProgram(shaderProgram);
    //use it
    glUseProgram(shaderProgram);
    //pass in the screen height and width
    GLint screenDimsLoc = glGetUniformLocation(shaderProgram, "screenDims");
    if (screenDimsLoc != -1) {
        glUniform2i(screenDimsLoc, SW, SH);
        GLfloat outDims[2];
        glGetUniformfv(shaderProgram, screenDimsLoc, outDims);
        printf("Screen dims set in GLSL as %f, %f\n", outDims[0], outDims[1]);
    } else {
        printf("Please define `screenDims` ivec2 in the fragShader.\n");
        //return 0;
    }
    //bind the framenum uniform
    GLint frameNumLoc = glGetUniformLocation(shaderProgram, "frameNum");
    if (frameNumLoc != -1) {
        //continue with the stuff
    } else {
        printf("Please define `frameNum` uint in the fragShader.\n");
        //return 0;
    }
    scaleLoc = glGetUniformLocation(shaderProgram, "scale");
    if (scaleLoc != -1) {
        //continue with the stuff
    } else {
        printf("Please define `scale` float in the fragShader.\n");
        //return 0;
    }
    //finally, go into the main loop (praise jesus)
    while (update(win)) {
        draw(win);
        //set framenum uniform
        glUniform1ui(frameNumLoc, frameNum);
    }
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
