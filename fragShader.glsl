#version 330
precision highp float;
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform ivec2 screenDims;
uniform uint frameNum;
uniform vec2 center;
uniform float scale;

int iterationsToEscape(vec2 c) {
    vec2 z = c;
    int iterations = 0;
    //(a + bi)^2
    //(a^2 + 2abi - b^2)
    //real part (a) = (a^2 - b^2)
    //imaginary part (b) = (2abi)
    //full formula:
    //nextIteration (a and b) = lastIteration (a and b)^2 + c (x and y)
    for (; iterations < 100; iterations++) {
        float a = (pow(z.x, 2.0) - pow(z.y, 2.0)) + c.x;
        float b = (2 * z.x * z.y) + c.y;
        if ((pow(a, 2.0) + pow(b, 2.0)) >= 4.0) {
            break;
        }
        z.xy = vec2(a,b);
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
