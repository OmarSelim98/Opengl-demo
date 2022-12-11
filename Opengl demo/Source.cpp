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

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void updateDeltaTime();

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
float brightness1 = 1.0f;
float brightness2 = 0.75f;

float clearColors[3] = { 0.0f,0.0f,0.0f };
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};
//Camera Config
// for texture
unsigned char* imgData;
unsigned int texture1, texture2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(-4.0f, 0.0f, 4.0f), WINDOW_WIDTH, WINDOW_HEIGHT);

std::vector<MaterialPreset> materialPresets;

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGl Demo", NULL, NULL);
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
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Init cam here */

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader shader("basic_shader.vert", "basic_shader.frag");
	Shader lightningShader("lightning_shader.vert", "lightning_shader.frag");

	// va, vb and layout for an object
	VertexArray objectVA;
	objectVA.Bind();

	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout objectLayout;
	objectLayout.AddElement<float>(3); // position vector
	objectLayout.AddElement<float>(3); // normals vector
	objectLayout.AddElement<float>(2); // texture vector

	objectVA.AddVertexBuffer(vb, objectLayout);

	Texture containerTexture("container.jpg");

	objectVA.UnBind();
	vb.UnBind();

	// create a va for light, with the same vb
	VertexArray lightVA;
	lightVA.Bind();

	VertexBufferLayout lightLayout;
	lightLayout.SetStrideManually(true);
	lightLayout.SetStride(8 * sizeof(float));
	lightLayout.AddElement<float>(3);

	lightVA.AddVertexBuffer(vb, lightLayout);

	lightVA.UnBind();
	vb.UnBind();

	Renderer renderer;

	camera.EnableMovement(window);

	int selectedMaterial = 0;
	materialPresets = ImportMaterials();
	Light light;
	/* Render Config */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */
	bool show_demo_window = true;

	//Loop : Swap Buffers and Check events.
	while (!glfwWindowShouldClose(window)) {
		updateDeltaTime();
		processInput(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
			ImGui::ColorEdit3("Clear color", (float*)&clearColors); // Edit 3 floats representing a color
			//ImGui::SliderFloat("Brightness", &brightness1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//ImGui::SliderFloat("ModelRotationZ", &rotation, -180.0f, 180.0);
			ImGui::Text("FOV : %.1f", camera.GetFOV());

			if (ImGui::CollapsingHeader("Light")) {
				ImGui::ColorEdit3("Color", (float*)&light.color);
				//ImGui::SliderFloat("Ambient", );
				ImGui::SliderFloat("Diffuse", &light.diffuse, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular", &light.specular, 0.0f, 1.0f);
			}

			if (ImGui::CollapsingHeader("Material")) {
				if (ImGui::BeginCombo("Material presets", materialPresets[selectedMaterial].name.c_str())) {
					for (int i = 0; i < materialPresets.size(); ++i) {
						const bool isSelected = (selectedMaterial == i);
						if (ImGui::Selectable(materialPresets[i].name.c_str(), isSelected)) {
							selectedMaterial = i;
						}

						// Set the initial focus when opening the combo
						// (scrolling + keyboard navigation focus)
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}
			if (ImGui::Button("Enable Movement"))
				camera.EnableMovement(window);
			ImGui::SameLine();
			if (camera.IsMovementEnabled())
				ImGui::Text("Press F5 to exit camera focus");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}
		// Rendering commands start
		renderer.Clear(clearColors);
		camera.ApplySmoothMovement(deltaTime);


		// Render Object
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection;

		projection = glm::perspective(glm::radians(camera.GetFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();


		light.position = glm::vec3(2.0, (float)cos(glfwGetTime()) * 2, -2.0);
		model = glm::scale(glm::mat4(1.0), glm::vec3(0.5f, 0.5f, 0.5f));
		model = translate(model, light.position);
		lightningShader.Bind();
		lightningShader.setMat4("projection", projection);
		lightningShader.setMat4("view", view);
		lightningShader.setMat4("model", model);
		lightningShader.setVec3("lightColor", light.color[0], light.color[1], light.color[2]);

		renderer.Draw(lightVA, 36);

		model = glm::scale(glm::mat4(1.0), glm::vec3(2.0f, 2.0f, 2.0f));
		model = translate(model, glm::vec3(0.0, 0.0, -4.0));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.25f, 0.0f));

		shader.Bind();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		//shader.setVec3("objectColor", glm::vec3(0.3f, 0.3f, 0.6f));
		shader.setVec3("viewPos", camera.GetPosition());
		shader.setVec3("light.position", light.position);
		shader.setVec3("light.color", light.color[0], light.color[1], light.color[2]);
		shader.setVec3("light.diffuse", light.getDiffuseVector());
		shader.setVec3("light.ambient", light.getAmbientVector());
		shader.setVec3("light.specular", glm::vec3(light.specular));
		//set material
		shader.setVec3("material.ambient", materialPresets[selectedMaterial].ambient);
		shader.setVec3("material.diffuse", materialPresets[selectedMaterial].diffuse);
		shader.setVec3("material.specular", materialPresets[selectedMaterial].specular);
		shader.setFloat("material.shininess", materialPresets[selectedMaterial].shininess);

		renderer.Draw(objectVA, 36);


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
void updateDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.OnCursorPositionChange(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.OnScroll(window, xoffset, yoffset);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.StepForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.StepBackward();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.StepRight();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.StepLeft();
	}
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
		camera.DisableMovement(window);
	}
}