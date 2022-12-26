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

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Takes a window's pointer, and the new width & height.
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void updateDeltaTime();

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
float brightness1 = 1.0f;
float brightness2 = 0.75f;

float clearColors[3] = { 0.5f,0.5f,0.5f };
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(-4.0f, 0.0f, 4.0f), WINDOW_WIDTH, WINDOW_HEIGHT);

std::vector<MaterialPreset> materialPresets;

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


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); // z buffer
	glEnable(GL_STENCIL_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // blending mode

	/* Init cam here */

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader shader("basic_shader.vert", "basic_shader.frag");
	Shader stencilShader("stencil_shader.vert", "stencil_shader.frag");

	//Model backpackModel("backpack/backpack.obj");

	//Model carModel("Car/Car.obj");
	//Model bookModel("book/models/Book1.obj");
	//Model bagModel("backpack/backpack.obj");
	Model macheteModel("machete2/machete2.obj");
	//// va, vb and layout for an object

	Renderer renderer;

	camera.EnableMovement(window);

	int selectedMaterial = 0;
	materialPresets = ImportMaterials();
	DirectionalLight dirLight;
	dirLight.direction = glm::vec3(0.0f,-0.8f,0.0f);
	PointLight pointLight;
	pointLight.position = glm::vec3(0.0f,0.0f,-5.0f);

	SpotLight spotLight;
	
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
			if (ImGui::CollapsingHeader("Directional Light")) {
				ImGui::DragFloat3("Point light position", glm::value_ptr(dirLight.direction), 0.1, glm::radians(-180.0), glm::radians(180.0));

				ImGui::ColorEdit3("Color", (float*)&dirLight.color);
				//ImGui::SliderFloat("Ambient", );
				ImGui::SliderFloat("Diffuse", &dirLight.diffuse, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular", &dirLight.specular, 0.0f, 1.0f);
			}
			if (ImGui::CollapsingHeader("Point Light")) {
				ImGui::DragFloat3("Point light position", glm::value_ptr(pointLight.position), 0.5, -500.0, 500.0);
				ImGui::ColorEdit3("Color", (float*)&pointLight.color);
				//ImGui::SliderFloat("Ambient", );
				ImGui::SliderFloat("Diffuse", &pointLight.diffuse, 0.0f,2.0f);
				ImGui::SliderFloat("Specular", &pointLight.specular, 0.0f, 1.0f);
			}
			if (ImGui::CollapsingHeader("FlashLight")) {
				//ImGui::DragFloat3("Point light position", glm::value_ptr(pointLight.position), 0.5, -500.0, 500.0);
				ImGui::ColorEdit3("Color", (float*)&spotLight.color);
				//ImGui::SliderFloat("Ambient", );
				ImGui::SliderFloat("Diffuse", &spotLight.diffuse, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular", &spotLight.specular, 0.0f, 1.0f);
				ImGui::SliderFloat("Radius", &spotLight.innerCutoff, 0.0, 1.0);
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


		/* LIGHT MODELS */
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection;

		projection = glm::perspective(glm::radians(camera.GetFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0));

		shader.Bind();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		//shader.setFloat("wtf",(float) (sin(glfwGetTime() * 2.0 - 1.0)  ));

		shader.setVec3("viewPos", camera.GetPosition());
		//Directional Light : mainly revolves around the direction of the light, without regards to its position.
		shader.setVec3("dirLight.direction", dirLight.direction);
		shader.setVec3("dirLight.diffuse", dirLight.getDiffuseVector());
		shader.setVec3("dirLight.ambient", dirLight.getAmbientVector());
		shader.setVec3("dirLight.specular", dirLight.getSpecularVector());

		shader.setFloat("material.shininess", 32.0);


		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // only update the fragments that passes the stencil test and the depth test
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all to-be drawn fragments should pass the stencil test
		glStencilMask(0xFF); // enable writing to the stencil buffer


		renderer.Draw(macheteModel, shader);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // all to-be drawn fragments should not pass the stencil test
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		// scale the model a little bit
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0));
		//model = glm::scale(glm::mat4(1.0), glm::vec3(1.1f, 1.1f, 1.1f));

		stencilShader.Bind();
		stencilShader.setMat4("projection", projection);
		stencilShader.setMat4("view", view);
		stencilShader.setMat4("model", model);

		renderer.Draw(macheteModel, stencilShader);

		glStencilMask(0x00);

		shader.Bind();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0,1.0,-4.0));
		shader.setMat4("model", model);

		renderer.Draw(macheteModel, shader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);


		/*shader.setMat4("model", model);
		renderer.Draw(macheteModel, shader);*/

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
	//backpackModel.Destroy();
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