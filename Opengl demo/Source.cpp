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
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void prepareGrass(VertexArray& va, VertexBuffer& vb, VertexBufferLayout& layout, unsigned int& texId);
void drawGrass(VertexArray& va, unsigned int& texId, Shader& shader);
std::map<float, glm::vec3> sortPositions(std::vector<glm::vec3> positions, Camera& camera);
void updateDeltaTime();


unsigned int CreateTexture2D(const char* fileName);
unsigned int CreateFrameBuffer(unsigned int texColorBuffer, unsigned int renderBuffer);
unsigned int CreateFrameBufferTexture(int width, int height);
unsigned int CreateRenderBuffer(unsigned int format, int width, int height);
unsigned int CreateCubeMap(std::vector<std::string> sides);

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
float brightness1 = 1.0f;
float brightness2 = 0.75f;

float clearColors[3] = { 0.5f,0.5f,0.5f };
float mirrorClearColors[3] = { 0.4f,0.4f,0.5f };
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
Camera mirrorCamera(glm::vec3(0.0f, 0.0f, -10.0f), WINDOW_WIDTH, WINDOW_HEIGHT);

std::vector<MaterialPreset> materialPresets;

unsigned int postProcessingFrameBuffer;
unsigned int postProcessingRenderBuffer;
unsigned int postProcessingColorBuffer;

unsigned int mirrorFrameBuffer;
unsigned int mirrorRenderBuffer;
unsigned int mirrorColorBuffer;

std::vector<glm::vec3> grassPos;
float grassVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};
float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};
float mirrorVertices[] = {
	//pos				//tex coords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};
std::vector<std::string> cubeFaces { "skybox/right.jpg",
	"skybox/left.jpg",
	"skybox/top.jpg",
	"skybox/bottom.jpg",
	"skybox/front.jpg",
	"skybox/back.jpg" };
float skyboxVertices[] = {
	// positions          
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
std::map<float, glm::vec3> sortedGrass;

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
	//glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // blending mode
	//glCullFace(GL_BACK);
	/* Init cam here */

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader shader("basic_shader.vert", "basic_shader.frag");
	Shader quadShader("quad_shader.vert", "quad_shader.frag");
	Shader mirrorShader("mirror_shader.vert", "mirror_shader.frag");
	Shader skyboxShader("skybox_shader.vert", "skybox_shader.frag");

	//Model backpackModel("backpack/backpack.obj");

	//Model carModel("Car/Car.obj");
	//Model bookModel("book/models/Book1.obj");
	//Model bagModel("backpack/backpack.obj");
	Model macheteModel("machete2/machete2.obj");
	Model poolTable("pool-table/pool-table.obj");
	//// va, vb and layout for an object
	VertexArray grassVA;
	VertexBuffer grassVB;
	VertexBufferLayout grassLayout;
	unsigned int grassTexture;

	prepareGrass(grassVA, grassVB, grassLayout, grassTexture);

	postProcessingColorBuffer = CreateFrameBufferTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	postProcessingRenderBuffer = CreateRenderBuffer(GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	postProcessingFrameBuffer = CreateFrameBuffer(postProcessingColorBuffer, postProcessingRenderBuffer);

	mirrorColorBuffer = CreateFrameBufferTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	mirrorRenderBuffer = CreateRenderBuffer(GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	mirrorFrameBuffer = CreateFrameBuffer(mirrorColorBuffer, mirrorRenderBuffer);

	VertexArray quadVA;
	VertexBuffer quadVB;
	VertexBufferLayout quadLayout;

	quadVA.Create();
	quadVA.Bind();
	quadVB.Create(quadVertices, sizeof(quadVertices));
	quadLayout.AddElement<float>(2);
	quadLayout.AddElement<float>(2);
	quadVA.AddVertexBuffer(quadVB, quadLayout);
	quadVA.UnBind();
	quadVB.UnBind();

	/* MIRROR MODEL SETUP */
	VertexArray mirrorVA;
	VertexBuffer mirrorVB;
	VertexBufferLayout mirrorLayout;

	mirrorVA.Create();
	mirrorVA.Bind();
	mirrorVB.Create(mirrorVertices, sizeof(mirrorVertices));
	mirrorLayout.AddElement<float>(2);
	mirrorLayout.AddElement<float>(2);
	mirrorVA.AddVertexBuffer(mirrorVB, mirrorLayout);
	mirrorVA.UnBind();
	mirrorVB.UnBind();

	unsigned int mirrorTexture = CreateTexture2D("mirror1.jpg");
	mirrorCamera.SetFOV(32.0);

	/* CUBEMAP STEUP */
	unsigned int skyboxTexture = CreateCubeMap(cubeFaces);
	VertexArray skyboxVA;
	VertexBuffer skyboxVB;
	VertexBufferLayout skyboxLayout;

	skyboxVA.Create();
	skyboxVA.Bind();
	skyboxVB.Create(skyboxVertices, sizeof(skyboxVertices));
	skyboxLayout.AddElement<float>(3);
	skyboxVA.AddVertexBuffer(skyboxVB, skyboxLayout);
	skyboxVA.UnBind();
	skyboxVB.UnBind();

	/* Uniform buffer objects */
	shader.bindUniformBlock("Matrices", 0);
	mirrorShader.bindUniformBlock("Matrices", 0);

	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW); // specify all the shared uniform blocks
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices,0, 2*sizeof(glm::mat4)); // bind ubo matrix to index 0 starting from offset 0 to the end of the size of the data inside it.


	Renderer renderer;

	camera.EnableMovement(window);

	int selectedMaterial = 0;
	materialPresets = ImportMaterials();
	DirectionalLight dirLight;
	dirLight.direction = glm::vec3(0.0f, -0.8f, 0.0f);
	PointLight pointLight;
	pointLight.position = glm::vec3(0.0f, 0.0f, -5.0f);

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
				ImGui::SliderFloat("Diffuse", &pointLight.diffuse, 0.0f, 2.0f);
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
		mirrorCamera.SetYaw(90.0); // look at the positive z axis

		glm::vec3 mirrorToCameraDirection = glm::normalize(mirrorCamera.GetPosition() - camera.GetPosition());
		glm::vec3 mirrorDirection = glm::reflect(mirrorToCameraDirection, mirrorCamera.GetFront());
		// Reflected render
		glBindFramebuffer(GL_FRAMEBUFFER, mirrorFrameBuffer);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		renderer.Clear(mirrorClearColors);
		//mirrorCamera.SetYaw(sin(glfwGetTime()) * 45 + 90);

		glm::mat4 mirrorProjection = glm::perspective(glm::radians(mirrorCamera.GetFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 mirrorView = mirrorCamera.LookAtDirection(mirrorDirection);
		glm::mat4 mirrorModel = glm::translate(glm::mat4(1.0), glm::vec3(0.0));
		//cube map start
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Bind();
		skyboxShader.setMat4("projection", mirrorProjection);
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(mirrorView)));
		skyboxShader.setInt("skybox", 0);
		skyboxVA.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		//cube map end

		shader.Bind();
		shader.setMat4("projection", mirrorProjection);
		shader.setMat4("view", mirrorView);
		shader.setMat4("model", mirrorModel);
		shader.setVec3("viewPos", mirrorCamera.GetPosition());
		shader.setVec3("dirLight.direction", dirLight.direction);
		shader.setVec3("dirLight.diffuse", dirLight.getDiffuseVector());
		shader.setVec3("dirLight.ambient", dirLight.getAmbientVector());
		shader.setVec3("dirLight.specular", dirLight.getSpecularVector());
		shader.setFloat("material.shininess", 32.0);
		

		renderer.Draw(macheteModel, shader);

		mirrorModel = glm::mat4(1.0);
		mirrorModel = glm::translate(mirrorModel, glm::vec3(0.0, 1.0, -4.0));
		shader.setMat4("model", mirrorModel);

		renderer.Draw(poolTable, shader);

		// Normal Render
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFrameBuffer);
		glEnable(GL_DEPTH_TEST);

		renderer.Clear(clearColors);
		camera.ApplySmoothMovement(deltaTime);

		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection;

		projection = glm::perspective(glm::radians(camera.GetFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0));

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		shader.Bind();
		shader.setMat4("model", model);
		shader.setVec3("viewPos", camera.GetPosition());
		//Directional Light : mainly revolves around the direction of the light, without regards to its position.
		shader.setVec3("dirLight.direction", dirLight.direction);
		shader.setVec3("dirLight.diffuse", dirLight.getDiffuseVector());
		shader.setVec3("dirLight.ambient", dirLight.getAmbientVector());
		shader.setVec3("dirLight.specular", dirLight.getSpecularVector());
		shader.setFloat("material.shininess", 32.0);

		renderer.Draw(macheteModel, shader);

		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0, 1.0, -4.0));
		shader.setMat4("model", model);

		renderer.Draw(poolTable, shader);
		//
		// Render Mirror
		//glDisable(GL_CULL_FACE);
		mirrorShader.Bind();
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, mirrorCamera.GetPosition());
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		mirrorShader.setMat4("model", model);
		mirrorShader.setInt("mirrorTexture", 0);
		mirrorShader.setInt("mirrorTexture1", 1);
		mirrorVA.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mirrorColorBuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mirrorTexture);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(mirrorVertices));

		// cube map start
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Bind();
		skyboxShader.setMat4("projection", projection);
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
		skyboxShader.setInt("skybox", 0);
		skyboxVA.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		// cube map end
		// QUAD RENDER
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		quadShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		quadShader.setInt("screenTexture", 0);
		quadVA.Bind();
		glBindTexture(GL_TEXTURE_2D, postProcessingColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		/*shader.setMat4("model", model);
		renderer.Draw(macheteModel, shader);*/
		quadVA.UnBind();

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
void prepareGrass(VertexArray& va, VertexBuffer& vb, VertexBufferLayout& layout, unsigned int& texId) {
	grassPos.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	grassPos.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	grassPos.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	grassPos.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	grassPos.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	va.Create();
	va.Bind();

	vb.Create(&grassVertices[0], sizeof(grassVertices));

	layout.AddElement<float>(3);
	layout.AddElement<float>(2);

	va.AddVertexBuffer(vb, layout);

	va.UnBind();
	vb.UnBind();

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("window.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
void drawGrass(VertexArray& va, unsigned int& texId, Shader& shader) {
	//shader.Bind();
	glActiveTexture(GL_TEXTURE0);
	shader.setInt("grassTexture", 0);
	glBindTexture(GL_TEXTURE_2D, texId);

	va.Bind();
	for (std::map<float, glm::vec3>::reverse_iterator it = sortedGrass.rbegin(); it != sortedGrass.rend(); ++it) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	va.UnBind();
}
std::map<float, glm::vec3> sortPositions(std::vector<glm::vec3> positions, Camera& camera) {
	std::map<float, glm::vec3> sorted;
	for (int i = 0; i < positions.size(); i++) {
		float distance = glm::length(camera.GetPosition() - positions[i]);
		sorted[distance] = positions[i];
	}
	return sorted;
}
unsigned int CreateFrameBuffer(unsigned int texColorBuffer, unsigned int renderBuffer) {
	unsigned int _id;
	glGenFramebuffers(1, &_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0); // set color buffer (texture).
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return _id;
};
unsigned int CreateFrameBufferTexture(int width, int height) {
	unsigned int _id;
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return _id;
};
unsigned int CreateRenderBuffer(unsigned int format, int width, int height) {
	unsigned int _id;
	glGenRenderbuffers(1, &_id);
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return _id;
};

unsigned int CreateTexture2D(const char* fileName) {
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texId;
}

unsigned int CreateCubeMap(std::vector<std::string> sides) {
	stbi_set_flip_vertically_on_load(false);
	unsigned int _id;
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	int width, height, nrChannels;
	unsigned char* data;

	for (int i = 0; i < sides.size(); i++) {
		data = stbi_load(sides[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << sides[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	stbi_set_flip_vertically_on_load(true);

	return _id;
}