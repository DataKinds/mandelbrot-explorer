#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "glShaderHelpers.h"
#include <SDL2/SDL_opengl.h>
#include "algs.h"
#define SW 512
#define SH 512

void pix(SDL_Renderer* ren, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    SDL_SetRenderDrawColor(ren, r, g, b, 255);
    SDL_RenderDrawPoint(ren, x, y);
}

//what it used to be is in comments
//void draw(SDL_Window* win, SDL_Renderer* ren) {
void draw(SDL_Window* win, SDL_GLContext glc) {
    //draw a rect to cover the screen
    float rectPoints[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };
    //create a VAO
    //the VAO just holds all the VBOs
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    //the VAO is set up, define it for the GPU
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //put the points in a Vertex Buffer Object
    //pretty much, the VBO just holds info about
    //a shape
    GLuint bigRectVbo = 0;
    //populate vbo
    glGenBuffers(1, &bigRectVbo);
    //put buffer on graphics card
    glBindBuffer(GL_ARRAY_BUFFER, bigRectVbo);
    //tell gpu what's in buffer
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), rectPoints, GL_STATIC_DRAW);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    /*
    SDL_RenderClear(ren);
    for (int x = 0; x++ < SW;){
        for (int y = 0; y++ < SH;){
            pix(ren, x, y, red(x,y), green(x,y), blue(x,y));
            //printf("drawing at (%i, %i) with color 0x%02x%02x%02x\n", x, y, c, c, c);
        }
    }
    SDL_RenderPresent(ren);
    */
    //draw to the screen
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
        }
    }
    SDL_Delay(10); //delay the amount to keep framerate
    return 1;
}

//http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("OpenGL 3.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SW, SH, SDL_WINDOW_OPENGL);
    //oh goodie - let's set the openGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //get an SDL_GLContext for openGL stuff
    SDL_GLContext glc = SDL_GL_CreateContext(win);
    //initialize GLEW
    //use openGL 3.0 and later
    glewExperimental = GL_TRUE;
    glewInit();
    //done with glew

    while (update(win)) {
        draw(win, glc);
    }
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
