#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

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
		-50.0f,-50.0f, 0.0f,0.0f,
		50.0f,-50.0f, 1.0f,0.0f,
		50.0f,50.0f, 1.0f,1.0f,
		-50.0f,50.0f, 0.0f,1.0f
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
	glm::mat4 view=glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

	//glm::mat4 mvp = proj*view*model;

	Shader *shader = new Shader("res/shaders/BasicShader.shader");
	shader->Bind();
	//shader->SetUniformMat4f("u_MVP", mvp);
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

	glfwMakeContextCurrent(window);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	ImGui::StyleColorsDark();
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glm::vec3 translationA(200, 200, 0);
	glm::vec3 translationB(400, 200, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();
		ImGui_ImplGlfwGL3_NewFrame();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
			glm::mat4 mvp = proj*view*model;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*va, *ib, *shader);	//material = shader + data(uniforms)
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			glm::mat4 mvp = proj*view*model;
			shader->Bind();
			shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*va, *ib, *shader);	//material = shader + data(uniforms)
		}
		renderer.Draw(*va, *ib, *shader);	//material = shader + data(uniforms)
		{               // Display some text (you can use a format string too)
			ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 640.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 640.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

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
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	return 0;
}