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

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);

float brightness1 = 0.75f;
float brightness2 = 0.75f;

float clearColors[3] = {0.0f,0.0f,0.0f};
float vertices[] = {
	//pos			 //color				//texcoords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

// for texture
unsigned char* imgData;
unsigned int texture1, texture2;

void changeMixRatio(Shader& shaderInstance) {
	float timeSinceStart = glfwGetTime();
	float mixValue = (sin(timeSinceStart) / 4) + 0.333;
	shaderInstance.Bind();
	shaderInstance.setFloat("mixValue", mixValue);
}

int main() {

	//Initiate GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window and set it as a context to the current thread.
	//Abort if the window can't be created.
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGl Demo", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to initialize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	//Adjust viewport to the resized window.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Abort if glad hasn't loaded all opengl's functions pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("basic_shader.vert", "basic_shader.frag");



	VertexArray va;
	va.Bind();

	VertexBuffer vb(vertices, sizeof(vertices));
	IndexBuffer ib(indices, 6);

	VertexBufferLayout layout;
	layout.AddElement<float>(3);
	layout.AddElement<float>(3);
	layout.AddElement<float>(2);

	va.AddVertexBuffer(vb, layout);

	//Texture
	int width, height, nChannles;
	imgData = stbi_load("ori.jpg", &width, &height, &nChannles, 0);

	if (imgData == NULL) {
		std::cout << "Failed to lead texture" << std::endl;
		return -1;
	}
	glGenTextures(1, &texture1); // generate a 2d texture
	glBindTexture(GL_TEXTURE_2D, texture1); // bind the generated 2d texture to the state

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BACKGROUND_COLOR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData); //specify the 2d texture
	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap for the bound 2d texture

	stbi_image_free(imgData);

	imgData = stbi_load("ori-and-niro.png", &width, &height, &nChannles, 4);

	if (imgData == NULL) {
		std::cout << "Failed to lead texture" << std::endl;
		return -1;
	}
	glGenTextures(1, &texture2); // generate a 2d texture
	glBindTexture(GL_TEXTURE_2D, texture2); // bind the generated 2d texture to the state

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData); //specify the 2d texture
	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap for the bound 2d texture

	stbi_image_free(imgData);

	shader.Bind();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	shader.UnBind();
	va.UnBind();
	vb.UnBind();
	ib.UnBind();

	Renderer renderer;

	/* Render Config */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */
	bool show_demo_window = true;
	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		//processInput(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
			ImGui::ColorEdit3("Clear color", (float*)&clearColors); // Edit 3 floats representing a color
			ImGui::SliderFloat("Brightness", &brightness1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		// Rendering commands start
		renderer.Clear(clearColors);

		changeMixRatio(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		{
			shader.Bind();
			shader.setFloat("brightness", brightness1);
			//rotate -> translate -> scale
			glm::mat4 transformMatrix = glm::mat4(1.0);
			transformMatrix = glm::scale(transformMatrix, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 0.0f));
			transformMatrix = glm::translate(transformMatrix, glm::vec3(0.25f, 0.25f, 0.0f));
			transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int matrixUniformLoc = glGetUniformLocation(shader.ID, "transform");
			glUniformMatrix4fv(matrixUniformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
			renderer.Draw(va, ib, shader);
		}
		{
			shader.Bind();
			shader.setFloat("brightness", brightness2);
			//rotate -> translate -> scale
			glm::mat4 transformMatrix = glm::mat4(1.0);
			transformMatrix = glm::scale(transformMatrix, glm::vec3(cos(glfwGetTime()), sin(glfwGetTime()), 0.0f));
			transformMatrix = glm::translate(transformMatrix, glm::vec3(0.25f, 0.25f, 0.0f));
			transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int matrixUniformLoc = glGetUniformLocation(shader.ID, "transform");
			glUniformMatrix4fv(matrixUniformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
			renderer.Draw(va, ib, shader);
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Rendering commands end
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean resources on loop's end.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
}