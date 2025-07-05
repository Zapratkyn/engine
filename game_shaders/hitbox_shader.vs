#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec4 Color;

uniform mat4 position;

void main()
{
   gl_Position = position * vec4(aPos, 0.0f, 1.0f);
   Color = vec4(aColor, 0.3f);
}