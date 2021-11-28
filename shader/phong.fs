#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light[3];
struct TorchLight{
    vec3 position;
    vec3 direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform TorchLight torchlight;
// uniform vec3 lightPos[3];  
uniform vec3 viewPos;  
uniform vec3 lightColor[3];  
uniform vec3 objectColor;  
uniform float specularStrength;

void main()  
{  
    vec3 result = vec3(0.0);
    vec3 torchdirection = normalize(torchlight.position-FragPos);
    float theta = dot(torchdirection,normalize((-torchlight.direction)));
    if(theta > torchlight.cutOff){
        vec3 ambient = torchlight.ambient * objectColor;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, torchdirection), 0.0);
        vec3 diffuse = torchlight.diffuse * diff * objectColor;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-torchdirection, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0),64.0);
        vec3 specular = torchlight.specular * spec * objectColor;  
        
        // attenuation
        float distance    = length(torchlight.position - FragPos);
        float attenuation = 1.0 / (torchlight.constant + torchlight.linear * distance + torchlight.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse   *= attenuation;
        specular *= attenuation;   
            
        result += ambient + diffuse + specular;
    }
    else {
        FragColor = vec4(torchlight.ambient * objectColor,1.0);
    }
    
    
    float distance[3];
    float attenuation[3];
    for(int i = 0;i<3;i++)
    {
        distance[i]  = length(light[i].position-FragPos);
        attenuation[i] = 1.0/(light[i].constant + light[i].linear*distance[i] + light[i].quadratic*distance[i] * distance[i]);
    }
    // 环境光照
    float ambientStrength = 0.2;  
    vec3 ambient;
    for(int i = 0;i<3;i++)
        ambient += ambientStrength * lightColor[i];  
    // 漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir[3];
    for(int i = 0;i<3;i++)  
        lightDir[i] = normalize(light[i].position - FragPos);  
    
    float diff[3];
    for(int i =0;i<3;i++)
        diff[i] = max(dot(norm, lightDir[i]), 0.0);  
    

    for(int i = 0;i<3;i++){
        vec3 diffuse = diff[i] * lightColor[i];  
        // 镜面光照
        // float specularStrength = 0.5;  
        vec3 viewDir = normalize(viewPos - FragPos);  
        // vec3 reflectDir = reflect(-lightDir[i], norm);  
        vec3 halfwayDir = normalize(lightDir[i] + viewDir);  
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);  
        vec3 specular = specularStrength * spec * lightColor[i];  
        result += (ambient + diffuse*attenuation[i] + specular*attenuation[i]) * objectColor;
    }
    // 结果
    // vec3 result = (ambient + diffuse + specular) * objectColor;  
    FragColor = vec4(result, 1.0);  
}