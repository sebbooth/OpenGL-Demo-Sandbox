#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Position;
out float timeStep;
out float randFloat;

uniform mat4 u_MVP;
uniform mat4 u_Mod;
uniform vec3 u_ViewPos;
uniform float u_Time;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    v_TexCoord = texCoord;
    v_Position = position;
    timeStep = mod(u_Time,  1000.)/1000.;
    randFloat = rand(vec2(gl_InstanceID, gl_InstanceID));

    float column = floor(gl_InstanceID / 300.);
    float row = gl_InstanceID - (300. * column);

    vec3 instancePosition = position;
    instancePosition.x += row - 150;
    instancePosition.x += instancePosition.y * 0.2 * sin((u_Time * randFloat) / 1000.);
    instancePosition.z += instancePosition.y * 0.2 * sin(u_Time * rand(vec2(randFloat, gl_InstanceID)) / 1000.);
    instancePosition.z -= column - 50;

    gl_Position = u_MVP * vec4(instancePosition, 1);
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Position;
in float timeStep;
in float randFloat;

uniform vec4 u_ModelCol;
uniform vec3 u_ViewPos;
uniform sampler2D u_Texture;
uniform float u_FogDist;

void main()
{
    float fogFactor = gl_FragCoord.z / gl_FragCoord.w / u_FogDist;
    color = u_ModelCol * (1 + v_Position.y);
    color *= (1 - fogFactor) * randFloat;

}