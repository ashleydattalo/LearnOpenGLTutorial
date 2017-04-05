#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in float aAlp;
layout (location = 3) in float aSca;
uniform mat4 P;
uniform mat4 V;

out vec4 vecCol;
out vec4 vecPos;

void main()
{
    gl_Position = P * V * aPos;
    vecPos = P * V * aPos;
    vecCol = vec4(aCol, aAlp);
    gl_PointSize = aSca;
}