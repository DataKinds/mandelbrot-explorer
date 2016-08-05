#version 330
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform vec2 screenDims;
uniform uint frameNum
void main () {
    vec2 windowFloat = gl_FragCoord.xy / screenDims.xy
    vec3 shade = vec3(windowFloat.x + windowFloat.y);
    frag_color = vec4(shade, 1.0);
}
