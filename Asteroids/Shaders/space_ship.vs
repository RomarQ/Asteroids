#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 color;
out vec2 texCoord1;
out vec2 texCoord2;

uniform float xOffset;
uniform float yOffset;

void main()
{
    gl_Position = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0f);
    color = aColor;
    texCoord1 = vec2(inTexCoord.x, inTexCoord.y);
    texCoord2 = vec2(inTexCoord.x, inTexCoord.y+0.1);
}
