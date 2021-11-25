#version 460 core
// Percentage Closer Soft Shadows
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct Plane{
    float zNear;
    vec2 planeSize;
};

vec2 poissonDisk[16] = { 
 vec2( -0.94201624, -0.39906216 ), 
 vec2( 0.94558609, -0.76890725 ), 
 vec2( -0.094184101, -0.92938870 ), 
 vec2( 0.34495938, 0.29387760 ), 
 vec2( -0.91588581, 0.45771432 ), 
 vec2( -0.81544232, -0.87912464 ), 
 vec2( -0.38277543, 0.27676845 ), 
 vec2( 0.97484398, 0.75648379 ), 
 vec2( 0.44323325, -0.97511554 ), 
 vec2( 0.53742981, -0.47373420 ), 
 vec2( -0.26496911, -0.41893023 ), 
 vec2( 0.79197514, 0.19090188 ), 
 vec2( -0.24188840, 0.99706507 ), 
 vec2( -0.81409955, 0.91437590 ), 
 vec2( 0.19984126, 0.78641367 ), 
 vec2( 0.14383161, -0.14100790 ) 
};
#define SAMPLE_SIZE 16

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec2 lightSize; //define light size(rectangle)
uniform mat4 lightView; 
uniform float zNear = 1.0;  //z coordinate of Shadow Map plane 
uniform vec2 planeSize = vec2(10.0,10.0);

float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    //1. blocker search: find average depth in a blocker
    // vec4 lightSpaceQuaterion= lightView * vec4(fs_in.FragPos,1.0);
    vec3 lightSpacePosition = vec3(lightView * vec4(fs_in.FragPos,1.0));
    // lightSpacePosition = 0.5 * lightSpacePosition + 0.5; 

    // vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w;
    
    // if(projCoords.z > 1.0)
    //     return 0.0;

    // vec2 blockerSize = lightSize/lightSpacePosition.z * (lightSpacePosition.z - zNear);    
    // vec2 texelSize = 1.0/textureSize(shadowMap,0);
    // float blockerDepth= 0.0;
    float shadow = 0.0;

    // float blockers = 0.0;
    // for(int i = 0;i<SAMPLE_SIZE;i++){
    //     float closestdistance = texture(shadowMap,projCoords.xy + blockerSize.x/planeSize.x * poissonDisk[i]).r;
    //     //这里不需要texelSize，因为引入zNear和lighSize后，计算出的size是真实的，不是像素数
    //     if(closestdistance < projCoords.z - bias){
    //         blockerDepth+= closestdistance;
    //         blockers += 1; 
    //     }
    // }
    // if(blockers == 0)
    //     return 0.0;
    
    // blockerDepth /= blockers; //average depth

    // vec2 filterSize = (projCoords.z-blockerDepth)/blockerDepth * lightSize/planeSize.x;
    // //2. PCF with filterSize 
    // for(int i = 0;i<SAMPLE_SIZE;i++){
    //     float closestDepth = texture(shadowMap,projCoords.xy+filterSize*poissonDisk[i]).r;
    //     //这里不需要texelSize，因为引入zNear和lighSize后，计算出的size是真实的，不是像素数
    //     shadow += projCoords.z -bias > closestDepth ? 1.0 : 0.0; 
    // }
    // shadow /= SAMPLE_SIZE;
    // debug 
    // shadow = texture(shadowMap,projCoords.xy).r;
    // shadow = blockerSize.x / planeSize.x;
    // shadow = projCoords.z;
    // shadow = blockers/16.0;
    shadow = fs_in.FragPos.z;
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float bias = max(0.05*(1.0-dot(normal,lightDir)),0.005);
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace,bias);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    // FragColor = vec4(lighting, 1.0);
    FragColor = vec4(shadow,shadow,shadow,1.0);
}