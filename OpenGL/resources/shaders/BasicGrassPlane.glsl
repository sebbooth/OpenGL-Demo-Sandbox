#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
};



#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_PlaneColor;
uniform float u_FogDist;

void main()
{
    float fogFactor = (gl_FragCoord.z / gl_FragCoord.w)/ u_FogDist;
    float fogFraction = max(0, (1 - fogFactor));
    vec4 fogColor = vec4(1, 1, 1, 1);

    color = u_PlaneColor;


    color = (1 - fogFraction) * fogColor + fogFraction * color;

    //color = vec4(0, 0, 0, 1);
};