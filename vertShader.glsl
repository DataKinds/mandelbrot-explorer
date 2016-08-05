#version 330
in vec2 vp;
uniform uint frameNum;
void main () {
    gl_Position = vec4(vp, 1.0, 1.0);
}
