#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 v_Normal;
out vec3 v_LightDir;
out vec3 v_ViewDir;

uniform mat4 u_MVP;
uniform mat4 u_Mod;
uniform vec3 u_ViewPos;
uniform vec3 u_LightPos;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_Normal = normalize(u_Mod*normal);
    vec4 w_Position = u_Mod * position;
    v_LightDir = normalize(u_LightPos - vec3(w_Position));
    v_ViewDir = normalize(vec3(w_Position) - u_ViewPos);
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Normal;
in vec3 v_LightDir;
in vec3 v_ViewDir;

uniform vec4 u_ModelCol;
uniform vec3 u_ViewPos;
uniform sampler2D u_Texture;

uniform float u_AmbientCoeff;
uniform vec3 u_AmbientCol;

uniform float u_DiffuseCoeff;
uniform vec3 u_DiffuseCol;

uniform float u_SpecularCoeff;
uniform vec3 u_SpecularCol;


void main()
{
    //texture color
    vec4 texColor = texture(u_Texture, v_TexCoord);

    //ambients
    vec3 ambient = u_AmbientCoeff * u_AmbientCol;

    //diffuse
    float dotLN = clamp(dot(v_LightDir, vec3(v_Normal)), 0., 1.);
    vec3 diffuse = u_DiffuseCoeff * dotLN * u_DiffuseCol;
       
    //specular
    float dotRV = clamp(dot(reflect(v_LightDir, vec3(v_Normal)), -v_ViewDir), 0., 1.);
    float alpha = 1.;
    vec3 specular = u_SpecularCoeff * pow(dotRV, alpha) * u_SpecularCol;

    vec4 light = vec4(ambient + diffuse + specular, 1);

    color = texColor * light;
};