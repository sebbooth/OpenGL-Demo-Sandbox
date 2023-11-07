#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_LightDir;
out vec3 v_ViewDir;
out float v_LightDist;

uniform mat4 u_MVP;
uniform mat4 u_Mod;
uniform vec3 u_ViewPos;
uniform vec3 u_LightPos;
uniform float u_InstancesSqrt;

void main()
{
    float objectSize = 40.;
    float lightStrength = 500.;
    float row = floor(gl_InstanceID / u_InstancesSqrt);
    float column = gl_InstanceID - (u_InstancesSqrt * row);

    vec3 instancePosition = position;
    instancePosition.x += (row - u_InstancesSqrt/2) * objectSize;
    instancePosition.z += (column - u_InstancesSqrt / 2) * objectSize;
    gl_Position = u_MVP * vec4(instancePosition, 1);

    v_TexCoord = texCoord;
    v_Normal = normalize(u_Mod * vec4(normal, 0)).xyz;
    vec3 w_Position = (u_Mod * vec4(instancePosition, 0)).xyz;
    vec3 lightToVert = w_Position - u_LightPos;
    v_LightDir = normalize(lightToVert);
    v_ViewDir = normalize(w_Position - u_ViewPos);

    v_LightDist = (lightStrength - length(lightToVert))/ lightStrength;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_LightDir;
in vec3 v_ViewDir;
in float v_LightDist;

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
    float dotLN = clamp(dot(v_LightDir, -v_Normal), 0., 1.);
    vec3 diffuse = v_LightDist * u_DiffuseCoeff * dotLN * u_DiffuseCol;
       
    //specular
    float dotRV = clamp(dot(reflect(v_LightDir, v_Normal), -v_ViewDir), 0., 1.);
    float alpha = 1.;
    vec3 specular = v_LightDist * u_SpecularCoeff * pow(dotRV, alpha) * u_SpecularCol;

    vec4 light = vec4(ambient + diffuse + specular, 1);

    color = texColor * light;
};