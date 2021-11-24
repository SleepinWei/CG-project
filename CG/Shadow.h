#ifndef SHADOW_H
#define SHADOW_H
#include<glad/glad.h>
#include<iostream>
class Shadow {
public:
	//Shadow Map size
	int shadowWidth;
	int shadowHeight;
	
	unsigned int depthMapFBO;
	unsigned int depthMap;

	Shadow() {
		//set map size
		shadowWidth = 1024;
		shadowHeight = 1024;

		//bind buffers & textures
		glGenFramebuffers(1, &depthMapFBO);

		//create 2D texutre
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 
		//specify texuture's format as DEPTH COMPONENT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
		//attach depth texture as the frambuffer's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE); //no need for color buffer
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void render() {
		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//1. configure shader and matrices 

		//2. render scene 

	}
};

#endif 