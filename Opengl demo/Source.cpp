#include <iostream>
#include <glad/glad.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader_class.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Camera.h"
#include "MaterialPresets.h"
#include "Texture.h"
#include "LightCasters.h"
#include <assimp/config.h>
#include<map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
float clearColors[3] = { 0.5f,0.5f,0.5f };

float points[] = {
	//position		//color
	-0.5f,  0.5f,   1.0f, 0.0f, 0.0f,// top-left / red
	 0.5f,  0.5f,	0.0f, 1.0f, 0.0f,// top-right / green
	 0.5f, -0.5f,	0.0f, 0.0f, 1.0f,// bottom-right / blue
	-0.5f, -0.5f,	1.0f, 0.0f, 1.0f// bottom-left / violet
};

int main() {

	//Initiate GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window and set it as a context to the current thread.
	//Abort if the window can't be created.
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGl Demo", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to initialize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glfwSwapInterval(1);
	//Adjust viewport to the resized window.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Abort if glad hasn't loaded all opengl's functions pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Camera camera(glm::vec3(0.0, 0.0, -4.0), WINDOW_WIDTH, WINDOW_HEIGHT);
	Shader mainShader("basic_shader.vert", "basic_shader.frag");
	Shader linesShader("lines_shader.vert", "lines_shader.frag", "lines_shader.geom");

	Model mainModel("machete2/machete2.obj");

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 3.0f));

	DirectionalLight dirLight;

	Renderer renderer;
	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		renderer.Clear(clearColors);
		camera.SetYaw(90);
		view = camera.GetViewMatrix();
		linesShader.Bind();
		linesShader.setMat4("p", projection);
		linesShader.setMat4("v", view);
		linesShader.setMat4("m", model);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 4);

		mainShader.Bind();

		mainShader.setVec3("viewPos", camera.GetPosition());

		mainShader.setMat4("projection", projection);
		mainShader.setMat4("view", view);
		mainShader.setMat4("model", model);

		mainShader.setVec3("dirLight.direction", dirLight.direction);
		mainShader.setVec3("dirLight.diffuse", dirLight.getDiffuseVector());
		mainShader.setVec3("dirLight.ambient", dirLight.getAmbientVector());
		mainShader.setVec3("dirLight.specular",dirLight.getSpecularVector());

		renderer.Draw(mainModel, mainShader);
		// Rendering commands end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}