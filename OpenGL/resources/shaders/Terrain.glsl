#shader vertex
#version 330 core
        
layout(location = 0) in vec3 position;

out vec3 heightPos;
out float heightFloat;

uniform mat4 u_MVP;
uniform float u_GridSize;
uniform float u_GridW;
uniform float u_GridL;
uniform float u_MaxHeight;
uniform sampler2D u_Texture;

void main()
{
    vec2 standardPos = vec2(position.x/u_GridSize/u_GridW, position.z/u_GridSize/u_GridL);
    standardPos += vec2(0.5, 0.5);
    vec4 texColor = texture(u_Texture, standardPos);
    heightFloat = texColor.r;

    heightPos = position;
    heightPos.y += u_MaxHeight * heightFloat;

    gl_Position = u_MVP * vec4(heightPos,1);
};



#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec3 heightPos;
in float heightFloat;

uniform vec4 u_Color;
uniform float u_FogDist;

void main()
{
   color = vec4(u_Color.rgb * heightFloat, u_Color.a);
};

