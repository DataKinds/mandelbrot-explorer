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
        float a = (z.x * z.x - z.y * z.y) + c.x;
        float b = (2 * z.x * z.y) + c.y;
        if ((a * a + b * b) >= 4.0) {
            break;
        }
        z.xy = vec2(a,b);
    }
    return iterations;
}

void main() {
    //0.0 to 4.0
    vec2 fractalCoords = (gl_FragCoord.xy * (1 / scale)) / screenDims.xy;
    //-2.0 to 2.0
    fractalCoords = fractalCoords - 2;
    vec3 shade = vec3(iterationsToEscape(fractalCoords) / 100.0);
    //ivec2 fragInt = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
    //if ((fragInt.x ^ fragInt.y) == mod(frameNum, 256)) {
    //    shade = vec3(1.0);
    //} else {
    //}
    frag_color = vec4(shade, 1.0);
}
