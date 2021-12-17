#ifndef SKYBOX_H
#define SKYBOX_H
#include<glad/glad.h>
#include<glfw/glfw3.h>
#include<vector>
#include<string>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include<shader/Shader.h>
extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;
extern Camera camera;

class SkyBox {
public:
    /*attributes*/
	unsigned int textureID;
	int width, height, nrChannels;
    std::vector<std::string> faces;
    GLuint VAO, VBO;
    std::vector<float> vertices;

    /*functions*/
    SkyBox() 
    {
        this->faces = {
        "right.jpg","left.jpg","top.jpg",
        "bottom.jpg","front.jpg","back.jpg"
        };
        this->vertices = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
        };
        setUp();
    }

    unsigned int loadCubemap()
    {
        /*load cubeMap*/
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        std::string dirPath = "../resources/textures/skybox/";
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load((dirPath+faces[i]).c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        return textureID;
    }
    void setUp() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        
        loadCubemap();
        //glBindVertexArray(VAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }
    void render(Shader& shader,Camera& camera) {
        //something in the main function
        //glm::mat4 view, projection;

        //glDepthFunc(GL_LESS);
        //skyShader.use();
        //view = glm::mat4(glm::mat3(camera.GetViewMatrix()));//remove translation 
        //projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //skyShader.setMat4("model", model);
        //skyShader.setMat4("view", view);
        //skyShader.setMat4("projection", projection);
        //glBindVertexArray(skybox->VAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LEQUAL);

        shader.use();
        glm::mat4 view,projection;
        //view = glm::mat4(glm::mat3(camera.GetViewMatrix()));//remove translation 
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }
};
unsigned int blankTexture(int w, int h, int format)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, (GLenum)format, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    return texture;
}
unsigned int makeFramebuffer(unsigned int* renderTexture, unsigned int* depthTexture, int w, int h)
{
    *renderTexture = blankTexture(w, h, GL_RGBA);
    *depthTexture = blankTexture(w, h, GL_DEPTH_COMPONENT);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *renderTexture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *depthTexture, 0);
    GLuint array[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
    glDrawBuffers(2, array);
    return framebuffer;
}
glm::mat4 getViewMatrix(float x, float y, float z, float a, float p)
{
    float cosy = cosf(a), siny = sinf(a), cosp = cosf(p), sinp = sinf(p);
    glm::mat4 result;
    result[0][0] = cosy,
    result[0][1] = siny * sinp;
    result[0][2] = siny * cosp;
    result[1][1] = cosp;
    result[1][2] = -sinp;
    result[2][0] = -siny;
    result[2][1] = cosy * sinp;
    result[2][2] = cosp * cosy;
    result[3][0] = -(cosy * x - siny * z);
    result[3][1] = -(siny * sinp * x + cosp * y + cosy * sinp * z);
    result[3][2] = -(siny * cosp * x - sinp * y + cosp * cosy * z);
    result[3][3] = 1.0f;
    return result;
}
glm::mat4 getProjectionMatrix(int w, int h)
{
    float fov = camera.Zoom;
    float aspect = (float)w / (float)h;
    float near = 1.0f;
    float far = 1000.0f;

    glm::mat4 result;
    result[0][0] = 1.0f / (aspect * tanf(fov * 3.14f / 180.0f / 2.0f));
    result[1][1] = 1.0f / tanf(fov * 3.14f / 180.0f / 2.0f);
    result[2][2] = -(far + near) / (far - near);
    result[2][3]= -1.0f;
    result[3][2] = -(2.0f * far * near) / (far - near);
    return result;
}


class DynamicSky {
public:
    GLuint VAO1, VBO1;
    GLuint VAO2, VBO2,FBO;
    GLuint textures[16];

    GLuint vertices;
    Shader* skyShader;
    Shader* postShader;

    DynamicSky() {
        vertices = 4;
        //VAO1
        glGenVertexArrays(1, &VAO1);
        glBindVertexArray(VAO1);
        glGenBuffers(1, &VBO1);
        glBindBuffer(GL_ARRAY_BUFFER,VBO1);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

        //VAO2
        glGenVertexArrays(1, &VAO2);
        glBindVertexArray(VAO2);
        glGenBuffers(1, &VBO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
        //FBO
        FBO = makeFramebuffer(&textures[0], &textures[1], SCR_WIDTH, SCR_HEIGHT);

        skyShader = new Shader("../shader/sky/sky.vs", "../shader/sky/sky.fs");
        postShader = new Shader("../shader/sky/post.vs", "../shader/sky/post.fs");
    }
    ~DynamicSky() {
        //delete shaders 
        delete skyShader;
        delete postShader;
        glDeleteBuffers(1, &VBO1);
        glDeleteBuffers(1, &VBO2);
        glDeleteFramebuffers(1, &FBO);
        glDeleteVertexArrays(1, &VAO1);
        glDeleteVertexArrays(1, &VAO2);
    }

    void renderSky(Camera& camera){

        glDepthFunc(GL_LEQUAL); 
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float rate = 20.f;
        float time = (float)glfwGetTime() * rate - 0.0f;

        const float near = 1.0f;
        const float far = 1000.f;
        //projection = glm::inverse(projection);
        //glm::mat4 projection = getProjectionMatrix(SCR_WIDTH, SCR_HEIGHT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, near,far);
        //projection = glm::inverse(projection);
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = getViewMatrix(camera.Position.x, camera.Position.y, camera.Position.z, 180.0f-camera.Yaw, camera.Pitch);
        glm::mat4 view = camera.GetViewMatrix();

        //render to framebuffer
        skyShader->use();
        skyShader->setMat4("P", projection);
        skyShader->setMat4("V", view);
        skyShader->setFloat("time", time);
        glDisable(GL_DEPTH_TEST);
       
        glBindVertexArray(VAO1);
//#define TEST
#ifdef TEST
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices);
        
        //post effect
        postShader->use();
        for (int i = 0; i < 2; i++) {
            glActiveTexture(GL_TEXTURE0 + (unsigned int)i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glUniform1i(glGetUniformLocation(postShader->ID,"tex")+i, i);
        }
        postShader->setFloat("time", 0.0f);
        // is frambuffer, vertices=4,srcwidth,srcheight,not depth-test
        postShader->setMat4("P", projection);
        postShader->setMat4("V", view);
        glBindVertexArray(VAO2);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices);

        glDepthFunc(GL_LESS);
    }
};
#endif // !SKYBOX_H
