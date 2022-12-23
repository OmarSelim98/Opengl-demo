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
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
// for texture
unsigned char* imgData;
unsigned int texture1, texture2;

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
	glEnable(GL_DEPTH_TEST); // z buffer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // blending mode

	/* Init cam here */

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader shader("basic_shader.vert", "basic_shader.frag");
	Shader lightningShader("lightning_shader.vert", "lightning_shader.frag");

	//Model backpackModel("backpack/backpack.obj");

	//Model carModel("Car/Car.obj");
	//Model bookModel("book/models/Book1.obj");
	Model bagModel("backpack/backpack.obj");
	//// va, vb and layout for an object
	VertexArray objectVA;
	objectVA.Create();
	objectVA.Bind();

	VertexBuffer vb;
	vb.Create(vertices, sizeof(vertices));

	VertexBufferLayout objectLayout;
	objectLayout.AddElement<float>(3); // position vector
	objectLayout.AddElement<float>(3); // normals vector
	objectLayout.AddElement<float>(2); // texture vector

	objectVA.AddVertexBuffer(vb, objectLayout);

	Texture containerTexture("container2.png");
	Texture containerSpecularTexture("container2_specular.png");
	Texture containerEmissionTexture("container2_emission.png");
	Texture cookieTexture("cookie.png");

	objectVA.UnBind();
	vb.UnBind();

	// create a va for light, with the same vb
	VertexArray lightVA;
	lightVA.Create();
	lightVA.Bind();
	vb.Bind();
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
	DirectionalLight dirLight;
	dirLight.direction = glm::vec3(0.0f,-0.8f,0.0f);
	PointLight pointLight;
	pointLight.position = glm::vec3(0.0f,0.0f,-5.0f);

	SpotLight spotLight;
	
	/* Render Config */
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe polygons
	/* ========== */
	bool show_demo_window = true;

	int w, h = 0;
	glfwGetWindowSize(window,&w,&h);
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

		/* BOX MODELS */
		model = glm::scale(glm::mat4(1.0), glm::vec3(3.0f, 3.0f, 3.0f));
		model = translate(model, glm::vec3(0.0, 0.0, -4.0));

		shader.Bind();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		//shader.setMat4("model", model);
		//shader.setVec3("objectColor", glm::vec3(0.3f, 0.3f, 0.6f));
		shader.setVec3("viewPos", camera.GetPosition());
		//Directional Light : mainly revolves around the direction of the light, without regards to its position.
		shader.setVec3("dirLight.direction", dirLight.direction);
		shader.setVec3("dirLight.diffuse", dirLight.getDiffuseVector());
		shader.setVec3("dirLight.ambient", dirLight.getAmbientVector());
		shader.setVec3("dirLight.specular", dirLight.getSpecularVector());

		//Point Light : depends on the light's position and direction, also the effect of the light on a fragment is calculated through a quadratic equation
		shader.setVec3("pointLight.position", pointLight.position);
		shader.setVec3("pointLight.diffuse", pointLight.getDiffuseVector());
		shader.setVec3("pointLight.ambient", pointLight.getAmbientVector());
		shader.setVec3("pointLight.specular", pointLight.getSpecularVector());
		shader.setFloat("pointLight.constant", pointLight.constant);
		shader.setFloat("pointLight.linear", pointLight.linear);
		shader.setFloat("pointLight.quadratic", pointLight.quadratic);

		//SpotLight (in flashlight mode)
		spotLight.position = camera.GetPosition();
		spotLight.direction = camera.GetFront();

		shader.setVec3("spotLight.position", spotLight.position);
		shader.setVec3("spotLight.direction", spotLight.direction);
		shader.setFloat("spotLight.innerCutoff", spotLight.innerCutoff);
		shader.setFloat("spotLight.outerCutoff", spotLight.outerCutoff);
		shader.setVec3("spotLight.diffuse", spotLight.getDiffuseVector());
		shader.setVec3("spotLight.ambient", spotLight.getAmbientVector());
		shader.setVec3("spotLight.specular", spotLight.getSpecularVector());
		shader.setFloat("spotLight.constant", spotLight.constant);
		shader.setFloat("spotLight.linear", spotLight.linear);
		shader.setFloat("spotLight.quadratic", spotLight.quadratic);

		shader.setInt("spotLight.cookie", cookieTexture.getIndex());
		shader.setFloat("material.shininess", 32.0);
		shader.setFloat("material.diffuse1", containerTexture.getIndex());
		shader.setFloat("material.specular1", containerSpecularTexture.getIndex());

		containerTexture.Bind();
		containerSpecularTexture.Bind();
		cookieTexture.Bind();

		//renderer.Draw(backpackModel, shader);

		/*for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime()* glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			renderer.Draw(objectVA, 36);
		}*/

		shader.setMat4("model", model);

		//renderer.Draw(carModel, shader);
		//renderer.Draw(bookModel, shader);
		renderer.Draw(bagModel, shader);

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