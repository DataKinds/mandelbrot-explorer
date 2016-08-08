#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Julia {
SDL_Window* init();
int update();
void draw();
void destroy();
}
