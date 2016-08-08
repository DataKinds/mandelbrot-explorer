#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Julia {
void updateMandelCenter(float real, float imaginary);
SDL_Window* init();
int update(SDL_Event e);
void draw();
void destroy();
}
