#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace Mandel{
SDL_Window* init();
int update(SDL_Event e);
void draw();
void destroy();
}
