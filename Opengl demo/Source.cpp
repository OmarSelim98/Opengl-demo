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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void updateDeltaTime();
void setupCamera();

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
float brightness1 = 1.0f;
float brightness2 = 0.75f;

float clearColors[3] = { 0.0f,0.0f,0.0f };
float vertices[] = {
	//pos					 //color				//texcoords
	-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 0.0f,		0.0f, 1.0f
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
unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};
//Camera Config
float cameraSpeed = 2.5f;
const float mouseSensitivity = 0.1f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f; // make sure that the camera initially looks at the negative z-axis.
float cameraPitch = 0.0f;
float cameraLastX = (float)WINDOW_WIDTH / 2.0f;
float cameraLastY = (float)WINDOW_HEIGHT / 2.0f;
bool firstMouse = false;
float fov = 45.0f;
glm::vec3 direction;
// for texture
unsigned char* imgData;
unsigned int texture1, texture2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//Abort if glad hasn't loaded all opengl's functions pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture mouse.

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
		updateDeltaTime();
		setupCamera();
		processInput(window);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
			ImGui::ColorEdit3("Clear color", (float*)&clearColors); // Edit 3 floats representing a color
			//ImGui::SliderFloat("Brightness", &brightness1, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//ImGui::SliderFloat("ModelRotationZ", &rotation, -180.0f, 180.0);
			ImGui::Text("FOV : %.1f", fov);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		// Rendering commands start
		renderer.Clear(clearColors);
		cameraSpeed = 2.5f * deltaTime;

		

		// Model view Projection init
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection;

		projection = glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.25f, 0.0f));

		changeMixRatio(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.Bind();
		shader.setFloat("brightness", brightness1);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		//rotate -> translate -> scale (str)
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			renderer.Draw(va, 36);
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
void updateDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}
void setupCamera() {

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) // initially set to true
	{
		cameraLastX = xpos;
		cameraLastY = ypos;
		firstMouse = false;
	}

	// calculate new offsets
	float xOffset = xpos - cameraLastX;
	float yOffset = -(ypos - cameraLastY);

	//update last offsets
	cameraLastX = xpos;
	cameraLastY = ypos;

	//adjust offsets to a sensitivity factor
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	//update yaw (x/z) and pitch (y) values
	cameraYaw += xOffset;
	cameraPitch += yOffset;

	//constraints the pitch angle
	if (cameraPitch > 89.0f)
		cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)
		cameraPitch = -89.0f;

	//update direction vector
	direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	direction.y = sin(glm::radians(cameraPitch));
	direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	cameraFront = glm::normalize(direction);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos.x += cameraSpeed * cameraFront.x;
		cameraPos.z += cameraSpeed * cameraFront.z;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos.x -= cameraSpeed * cameraFront.x;
		cameraPos.z -= cameraSpeed * cameraFront.z;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
}