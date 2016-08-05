#version 330
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform vec2 screenDims;
void main () {
    vec2 windowFloat = vec2(gl_FragCoord.x/screenDims.x, gl_FragCoord.y/screenDims.y);
    vec3 shade = vec3(windowFloat.x + windowFloat.y);
    frag_color = vec4(shade, 1.0);
}
