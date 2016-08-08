# mandelbrot-explorer

## Screenshot

![Mandelbrot and Julia fractals](https://raw.githubusercontent.com/Aearnus/mandelbrot-explorer/master/fractalRendererScreenshot.png)

## Controls

- WASD: pan

- Q/E: zoom in and out

- Escape: quit

To move the constant used to create the Julia set, pan the mandelbrot fractal. The center of the mandelbrot viewport is used as the constant.

## Requirements

(Debian package names are used where available)

- OpenGL 3.1 support (mesa, etc).

- libglew-dev

- libsdl2-dev

## Todo

- Include support for OpenGL 4.0, which adds double-precision floating point support. Sadly, I don't own a GPU that supports OpenGL 4.0 (at least one with a sane build environment).
