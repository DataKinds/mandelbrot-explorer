#version 330
precision highp float;
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform ivec2 screenDims;
uniform uint frameNum;

int iterationsToEscape(float x, float y) {
    float a = 0;
    float b = 0;
    int iterations = 0;
    //(a + bi)^2
    //(a^2 + 2abi - b^2)
    //real part (a) = (a^2 - b^2)
    //imaginary part (b) = (2abi)
    //full formula:
    //nextIteration (a and b) = lastIteration (a and b)^2 + c (x and y)
    while (((a * a + b * b) < 4.0) && iterations < 100) {
        a = (a * a - b * b) + x;
        b = (2 * a * b) + y;
        //b = ((mod(float(frameNum)/100.0, 5.0) + 1)*a*b) + y;
        iterations++;
    }
    return iterations;
}

void main() {
    //0.0 to 4.0
    vec2 fractalCoords = (gl_FragCoord.xy * 4.0) / screenDims.xy;
    //-2.0 to 2.0
    fractalCoords = fractalCoords - 2.0;
    vec3 shade = vec3(iterationsToEscape(fractalCoords.x, fractalCoords.y) / 150.0);
    //ivec2 fragInt = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
    //if ((fragInt.x ^ fragInt.y) == mod(frameNum, 256)) {
    //    shade = vec3(1.0);
    //} else {
    //}
    frag_color = vec4(shade, 1.0);
}
