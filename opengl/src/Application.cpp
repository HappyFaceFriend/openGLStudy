#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	glfwSwapInterval(1);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	float positions[] = {
		100.0f,100.0f, 0.0f,0.0f,
		200.0f,100.0f, 1.0f,0.0f,
		200.0f,200.0f, 1.0f,1.0f,
		100.0f,200.0f, 0.0f,1.0f
	};

	unsigned int indicies[] = {
		0,1,2,
		2,3,0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	VertexArray *va = new VertexArray();
	VertexBuffer *vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va->AddBuffer(*vb, layout);

	IndexBuffer *ib = new IndexBuffer(indicies, 6);

	glm::mat4 proj = glm::ortho(0.0f,640.0f,0.0f,480.0f, -1.0f, 1.0f);	//orthographic : 2d / prerspective : 3d
	glm::mat4 view=glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

	glm::mat4 mvp = proj*view*model;

	Shader *shader = new Shader("res/shaders/BasicShader.shader");
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", mvp);
	//shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	float r = 0.0f;
	float increment = 0.05f;

	Renderer renderer;

	Texture *texture = new Texture("res/textures/player.png");
	texture->Bind();

	shader->SetUniform1i("u_Texture", 0);

	va->Bind();
	shader->Bind();
	vb->Bind();
	ib->Bind();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		shader->Bind();
		r += increment;
		if (r > 1.0f)
			increment = -0.05f;
		else if (r <0.0f)
			increment = 0.05f;
		//shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		renderer.Draw(*va, *ib, *shader);	//material = shader + data(uniforms)

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete shader;
	delete va;
	delete vb;
	delete ib;

	glfwTerminate();
	return 0;
}