#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Mandel{
SDL_Window* init();
int update();
void draw();
void destroy();
}
