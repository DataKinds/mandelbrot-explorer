#version 330
precision highp float;
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform ivec2 screenDims;
uniform uint frameNum;
uniform vec2 center;
uniform vec2 mandelCenter;
uniform float scale;

int iterationsToEscape(vec2 c) {
    float i = mod(frameNum / 1000.0, 1.0);
    int iterations = 0;
    //same as mandel formula, except for that c doesnt change
    //pixel-per-pixel
    vec2 z = c;
    for (; iterations < 100; iterations++) {
        float a = (pow(z.x, 2.0) - pow(z.y, 2.0)) + mandelCenter.x;
        float b = (2 * z.x * z.y) + mandelCenter.y;
        if ((pow(a, 2.0) + pow(b, 2.0)) >= 4.0) {
            break;
        }
        z.xy = vec2(a, b);
    }
    return iterations;
}

void main() {
    //0.0 to 1.0
    vec2 fractalCoords = gl_FragCoord.xy / (screenDims.xy);
    //-0.5 to 0.5
    fractalCoords = fractalCoords - 0.5;
    //-0.5scale to 0.5scale
    fractalCoords = fractalCoords / scale;
    //adjust for center
    fractalCoords = fractalCoords - center;
    vec3 shade = vec3(iterationsToEscape(fractalCoords) / 100.0);
    frag_color = vec4(shade, 1.0);
}
