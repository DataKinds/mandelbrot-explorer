#version 330
out vec4 frag_color;
in vec4 gl_FragCoord;
uniform vec2 screenDims;
uniform uint frameNum;
void main () {
    vec2 windowFloat = gl_FragCoord.xy / screenDims.xy;
    vec3 shade = vec3(windowFloat.x + windowFloat.y);
    //float frameNumFloat = float(frameNum);
    //vec3 shade = vec3(mod(frameNum, 256.0) / 256.0);
    frag_color = vec4(shade, 1.0);
}
