#ifndef GRASS_H
#define GRASS_H
#include<iostream>
//#include<glm/ext/scalar_constants.hpp>
#include<glm/glm.hpp>
#include<glfw/glfw3.h>
#include<vector>
#include<chrono>
#include<random>
//#include<shader/Shader.h>
#include"ComplexShader.h"
#include"Camera.h"
//typedef ComplexShader Shader;

struct NumBlades {
	std::uint32_t vertexCount = 5;
	std::uint32_t instanceCount = 1;
	std::uint32_t firstVertex = 0;
	std::uint32_t firstInstance = 0;
};

struct Blade {
	glm::vec4 v0; // xyz: Position, w: orientation (in radius)
	glm::vec4 v1; // xyz: Bezier point w: height
	glm::vec4 v2; // xyz: Physical model guide w: width
	glm::vec4 up; // xyz: Up vector w: stiffness coefficient

	Blade(glm::vec4 v0,glm::vec4 v1,glm::vec4 v2,glm::vec4 up):v0(v0),v1(v1),v2(v2),up(up){
		;
	}
};
std::vector<Blade> generate_blades()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> orientation_dis(0, glm::pi<float>());
	std::uniform_real_distribution<float> height_dis(0.6f, 1.2f);
	std::uniform_real_distribution<float> dis(-1, 1);

	std::vector<Blade> blades;

	std::cout << "successful:entering blades" << '\n';

	int size = 10;
	for (int i = -size; i < size; ++i) {
		for (int j = -size; j < size; ++j) {
			const auto x = static_cast<float>(i) / 10 - 1 + dis(gen) * 0.1f;
			const auto y = static_cast<float>(j) / 10 - 1 + dis(gen) * 0.1f;
			const auto blade_height = height_dis(gen);

			blades.emplace_back(
				glm::vec4(x, 0, y, orientation_dis(gen)),
				glm::vec4(x, blade_height, y, blade_height),
				glm::vec4(x, blade_height, y, 0.1f),
				glm::vec4(0, blade_height, 0, 0.7f + dis(gen) * 0.3f));
		}
	}

	std::cout << "successful:quitting blades" << '\n';

	return blades;
}

class Grass
{
public:
	/*variables*/
	GLuint VAO;
	ComplexShaderProgram* grassShader; 
	ComplexShaderProgram* computeShader;
	GLuint bladesCount;

	float wind_magnitude = 1.0f;
	float wind_wave_length = 1.0f;
	float wind_wave_period = 1.0f;

	using DeltaDuration = std::chrono::duration<float, std::milli>;

	/*functions*/

	Grass() {
		init();
	}
	~Grass() {
		delete grassShader;
		delete computeShader;

	}
	void init() {
		const std::vector<Blade> blades = generate_blades();
		bladesCount = static_cast<GLuint>(blades.size());

		std::cout << "successful:entering VAO binding" << '\n';

		
		
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glPatchParameteri(GL_PATCH_VERTICES, 1); //fucking why? This line is causing error but I just fucking don't konw the fucking reason 
		std::cout << "successful:1" << '\n';

		unsigned int grass_input_buffer = 0;
		glGenBuffers(1, &grass_input_buffer);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, grass_input_buffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER,
			static_cast<GLsizei>(blades.size() * sizeof(Blade)),
			blades.data(), GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, grass_input_buffer);

		std::cout << "successful:input buffer" << '\n';

		unsigned int grass_output_buffer = 0;
		glGenBuffers(1, &grass_output_buffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, grass_output_buffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER,
			static_cast<GLsizei>(blades.size() * sizeof(Blade)), nullptr,
			GL_STREAM_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, grass_output_buffer);

		std::cout << "successful:quitting VAO binding" << '\n';

		NumBlades numBlades;
		unsigned int grass_indirect_buffer = 0;
		glGenBuffers(1, &grass_indirect_buffer);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, grass_indirect_buffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(NumBlades), &numBlades,
			GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, grass_output_buffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, grass_indirect_buffer);

		// v0 attribute
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Blade),
			reinterpret_cast<void*>(offsetof(Blade, v0)));
		glEnableVertexAttribArray(0);

		// v1 attribute
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Blade),
			reinterpret_cast<void*>(offsetof(Blade, v1)));
		glEnableVertexAttribArray(1);

		// v2 attribute
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Blade),
			reinterpret_cast<void*>(offsetof(Blade, v2)));
		glEnableVertexAttribArray(2);

		// dir attribute
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Blade),
			reinterpret_cast<void*>(offsetof(Blade, up)));
		glEnableVertexAttribArray(3);

		//test
		std::cout << "successful" << std::endl;
		//
		
		ShaderBuilder builder;
		computeShader = builder.load("grass.comp.glsl", ComplexShader::Type::Compute).build();
		computeShader->use();
		

		ShaderBuilder grassBuilder;
		grassShader = grassBuilder
			.load("grass.vert.glsl", ComplexShader::Type::Vertex)
			.load("grass.tesc.glsl", ComplexShader::Type::TessControl)
			.load("grass.tese.glsl", ComplexShader::Type::TessEval)
			.load("grass.frag.glsl", ComplexShader::Type::Fragment)
			.build();
		//test
		std::cout << "successful" <<"\n";
	}
	void update(float delta_time,Camera& camera) {
		computeShader->use();
		computeShader->setFloat("current_time",
			static_cast<float>(glfwGetTime()));
		computeShader->setFloat("delta_time", delta_time / 1e3f);
		computeShader->setFloat("wind_magnitude", wind_magnitude);
		computeShader->setFloat("wind_wave_length", wind_wave_length);
		computeShader->setFloat("wind_wave_period", wind_wave_period);
		computeShader->setMat4("view", camera.GetViewMatrix());
		computeShader->setMat4("proj", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 1000.0f));

		glDispatchCompute(static_cast<GLuint>(bladesCount), 1, 1);
	}
	void render(Camera& camera) {
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glBindVertexArray(VAO);
		grassShader->use();
		grassShader->setMat4("view", camera.GetViewMatrix());
		grassShader->setMat4("proj", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 1000.0f));
		glDrawArraysIndirect(GL_PATCHES, reinterpret_cast<void*>(0));
	}
};
#endif // !GRASS
