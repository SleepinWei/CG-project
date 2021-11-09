#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos;  
uniform vec3 viewPos;  
uniform vec3 lightColor;  
uniform vec3 objectColor;  


void main()  
{  
    // 环境光照
    float ambientStrength = 0.2;  
    vec3 ambient = ambientStrength * lightColor;  
    // 漫反射光照
    vec3 norm = normalize(Normal);  
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);  
    vec3 diffuse = diff * lightColor;  
    // 镜面光照
    float specularStrength = 0.5;  
    vec3 viewDir = normalize(viewPos - FragPos);  
    vec3 reflectDir = reflect(-lightDir, norm);  
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);  
    vec3 specular = specularStrength * spec * lightColor;  
    // 结果
    vec3 result = (ambient + diffuse + specular) * objectColor;  
    FragColor = vec4(result, 1.0);  
}