#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;
layout(location = 3) in vec4 vertexColor;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec4 v_VertexColor;

uniform mat4 u_MVP;

void main()
{   
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_VertexColor = vertexColor;
    v_TexIndex = texIndex;
};



#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec4 v_VertexColor;

uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TexIndex);
    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    color = v_VertexColor + texColor;
};