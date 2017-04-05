#version 330 core
out vec4 color;
uniform vec4 ourColor;
uniform float time;

in vec4 vecCol;
in vec4 vecPos;

void main()
{
    vec2 cord = 2.0 * gl_PointCoord - 1.0;
    if (dot(cord, cord) <= 1.0f) {
        color = vec4(vecPos.xyz, vecCol.w);
    }
    else {
        color = vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }
}