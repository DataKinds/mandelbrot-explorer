#version 330
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform ivec2 screenDims;
uniform uint frameNum;
void main () {
    vec3 shade = vec3(0.0);
    ivec2 fragInt = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
    if ((fragInt.x & fragInt.y) == 0) {
        shade = vec3(1.0);
    } else {
    }
    frag_color = vec4(shade, 1.0);
}
