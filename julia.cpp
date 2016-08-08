#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "constants.h"
#include "glShaderHelpers.h"

namespace Julia {
SDL_Window* win;
unsigned int winId;
SDL_GLContext glc;
GLuint vao;
GLuint vbo;
GLuint shaderProgram;
GLint scaleLoc;
GLint centerLoc;
GLint mandelCenterLoc;
float mandelCenter[2];
float rectPoints[] = {
    -1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f,
    -1.0f, 1.0f
};

void updateMandelCenter(float real, float imaginary) {
    mandelCenter[0] = real;
    mandelCenter[1] = imaginary;
    printf("Setting constant c to %f+%fi\n", mandelCenter[0], mandelCenter[1]);
    char newWindowTitle[256];
    snprintf(newWindowTitle, 256, "Julia Set (c = %f+%fi)", mandelCenter[0], mandelCenter[1]);
    SDL_SetWindowTitle(win, newWindowTitle);
}

void draw() {
    SDL_GL_MakeCurrent(win, glc);
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
int update(SDL_Event e) {
    SDL_GL_MakeCurrent(win, glc);
    glUniform2f(mandelCenterLoc, mandelCenter[0], mandelCenter[1]);
    //make sure events are going to julia window, not mandelbrot
    if (e.window.windowID == winId) {
        if (e.type == SDL_QUIT) {
            return 1;
        }
        //handle keypresses
        else if (e.type == SDL_KEYDOWN) {
            auto eK = e.key.keysym.sym;
            if (eK == SDLK_ESCAPE) {
                return 1;
            }
            //zooming
            else if (eK == SDLK_e) {
                GLfloat currentScale;
                glGetUniformfv(shaderProgram, scaleLoc, &currentScale);
                currentScale *= 1.1f;
                printf("Current scale: %f\n", currentScale);
                glUniform1f(scaleLoc, currentScale);
            }
            else if (eK == SDLK_q) {
                GLfloat currentScale;
                glGetUniformfv(shaderProgram, scaleLoc, &currentScale);
                if (currentScale > 0.1f) {
                    currentScale *= (1.0f / 1.1f);
                    printf("Current scale: %f\n", currentScale);
                }
                glUniform1f(scaleLoc, currentScale);
            }
            //scrolling
            else if ((eK == SDLK_w) || (eK == SDLK_s) || (eK == SDLK_a) || (eK == SDLK_d)) {
                GLfloat currentCenter[2];
                glGetUniformfv(shaderProgram, centerLoc, currentCenter);
                GLfloat currentScale;
                glGetUniformfv(shaderProgram, scaleLoc, &currentScale);
                GLfloat moveFactor = 0.04/currentScale;
                switch (eK) {
                    case SDLK_w: {
                        currentCenter[1] -= moveFactor;
                        break;
                    }
                    case SDLK_s: {
                        currentCenter[1] += moveFactor;
                        break;
                    }
                    case SDLK_a: {
                        currentCenter[0] += moveFactor;
                        break;
                    }
                    case SDLK_d: {
                        currentCenter[0] -= moveFactor;
                        break;
                    }
                }
                glUniform2f(centerLoc, currentCenter[0], currentCenter[1]);
                printf("Current center: %f, %f; moved by %f\n", currentCenter[0], currentCenter[1], moveFactor);
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {

        }
    }
    return 0;
}

SDL_Window* init() {
    win = SDL_CreateWindow("Julia Set", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SW, SH, SDL_WINDOW_OPENGL);
    winId = SDL_GetWindowID(win);
    //get an SDL_GLContext for openGL stuff
    glc = SDL_GL_CreateContext(win);
    SDL_GL_MakeCurrent(win, glc);
    glewExperimental = GL_TRUE;
    glewInit();
    //initialize the VAOs and VBOs
    //put the points in a Vertex Buffer Object
    //pretty much, the VBO just holds info about
    //a shape - position, color, etc.
    vbo = 0;
    //generate vbo
    glGenBuffers(1, &vbo);
    //put info in vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
    glAttachShader(shaderProgram, loadAndCompileShader("juliaFragShader.glsl", GL_FRAGMENT_SHADER));
    glAttachShader(shaderProgram, loadAndCompileShader("vertShader.glsl", GL_VERTEX_SHADER));
    //link it all together
    glLinkProgram(shaderProgram);
    //use it
    glUseProgram(shaderProgram);
    //pass in the screen height and width
    //required
    GLint screenDimsLoc = glGetUniformLocation(shaderProgram, "screenDims");
    if (screenDimsLoc != -1) {
        glUniform2i(screenDimsLoc, SW, SH);
        GLfloat outDims[2];
        glGetUniformfv(shaderProgram, screenDimsLoc, outDims);
        printf("Screen dims set in GLSL as %f, %f\n", outDims[0], outDims[1]);
    } else {
        printf("Please define `screenDims` ivec2 in the fragShader.\n");
        return 0;
    }
    //bind the framenum uniform
    GLint frameNumLoc = glGetUniformLocation(shaderProgram, "frameNum");
    if (frameNumLoc != -1) {
        //continue with the stuff
    } else {
        printf("Please define `frameNum` uint in the fragShader.\n");
        //return 0;
    }
    //required
    scaleLoc = glGetUniformLocation(shaderProgram, "scale");
    if (scaleLoc != -1) {
        //make scale default to 0.25 (-2.0 to 2.0 in coords)
        glUniform1f(scaleLoc, 0.25f);
    } else {
        printf("Please define `scale` float in the fragShader.\n");
    }
    //required
    centerLoc = glGetUniformLocation(shaderProgram, "center");
    if (centerLoc != -1) {
        //make default center 0,0
        glUniform2f(centerLoc, 0.0f, 0.0f);
    } else {
        printf("Please define `center` vec2 in the fragShader.\n");
    }
    //required
    mandelCenterLoc = glGetUniformLocation(shaderProgram, "mandelCenter");
    if (mandelCenterLoc != -1) {
        //make default mandelCenter 0,0
        //this matches mandel.cpp's center
        glUniform2f(mandelCenterLoc, 0.0f, 0.0f);
    } else {
        printf("Please define `mandelCenterLoc` vec2 in the fragShader.\n");
    }
    return win;
}

void destroy() {
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(win);
}
}
