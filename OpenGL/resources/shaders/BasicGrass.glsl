#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Position;
out float timeStep;
out float randFloat;
out float randFloat2;

uniform mat4 u_MVP;
uniform float u_Time;
uniform float u_GridNum;
uniform sampler2D u_Texture;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float gridLength = 300.;
    float gridWidth = gridLength / u_GridNum;

    v_TexCoord = texCoord;
    v_Position = position;
    timeStep = mod(u_Time,  1000.)/1000.;
    randFloat = rand(vec2(gl_InstanceID, gl_InstanceID));
    randFloat2 = rand(vec2(randFloat, gl_InstanceID));

    float column = floor(gl_InstanceID / u_GridNum);
    float row = gl_InstanceID - (u_GridNum * column);




    vec3 instancePosition = position;

    instancePosition.x += row * gridWidth - gridLength / 2 + randFloat;
    instancePosition.z += column * gridWidth - gridLength /2 + randFloat2;

    float height = texture(u_Texture, vec2(0.5, 0.5) + (instancePosition.xz/ gridLength)).r;

    instancePosition.y += 5;

    instancePosition.x += instancePosition.y * 0.3 + instancePosition.y * 0.2 * sin((u_Time * randFloat) / 1000.);
    instancePosition.z += instancePosition.y * 0.2 * sin((u_Time * randFloat2) / 1000.);
   
    instancePosition.y += 70 * height;

    gl_Position = u_MVP * vec4(instancePosition, 1);
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Position;
in float timeStep;
in float randFloat;
in float randFloat2;

uniform vec4 u_ModelCol;
uniform float u_FogDist;

void main()
{
    float fogFactor = (gl_FragCoord.z / gl_FragCoord.w) / u_FogDist;
    color = u_ModelCol;
    color.r *= randFloat;
    color.g *= (randFloat2 + 3) / 4;
    color.rgb *= (randFloat2 + 3) / 4;

    vec4 bottomColor = vec4(0, 0, 0, 1);
    vec4 fogColor = vec4(1, 1, 1, 1);

    float fogFraction = max(0, (1 - fogFactor));

    float heightFraction = min(1,max(0,(v_Position.y +5)/ 10));

    color = (1 - heightFraction) * bottomColor + heightFraction * color;

    color = (1 - fogFraction) * fogColor + fogFraction * color;
}