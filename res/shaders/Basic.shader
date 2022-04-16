#shader fragment
#version 330 core

layout (location=0) out vec4 Color;
uniform vec4 uColor;
void main()
{
   Color = uColor;
};

#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.5); // MAGNIFICATION 
};