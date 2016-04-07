#version 410 core

layout (location=0) out vec4 fragColour;

uniform vec4 Colour;
uniform vec3 lightPos;
uniform vec4 lightDiffuse;
in vec3 normal;

void main()
{
    fragColour=vec4(0.1, 0.1, 0.1, 0.1);
    vec3 N=normalize(normal);
    vec3 L=normalize(lightPos);
    fragColour+=Colour*lightDiffuse*(dot(L,N));
}
