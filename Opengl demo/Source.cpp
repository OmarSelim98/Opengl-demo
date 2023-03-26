

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightCasters.h"
#include "shader_class.h"
#include "Camera.h"
//#include "model.h"
#include "Renderer.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int Create2DTexture(std::string pathName);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f,0.0f, 5.0f), SCR_WIDTH, SCR_HEIGHT);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Sphere {
	glm::vec3 position;
	glm::vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};
struct PBRPointLight {
	glm::vec3 position;
	glm::vec3 color;
	float intensity = 1.0f;
	glm::vec3 GetColor() {
		return intensity * color;
	}
};
const int LIGHTS_COUNT = 4;
const int spehresCount = 5;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	//Setup CPU Spheres and Lights
	Model sphereModel("sphere.obj");

	PBRPointLight lights[4];
	lights[0].color = { 1.0f , 1.0f, 1.0f };
	lights[1].color = { 0.5f , 1.0f, 1.0f };
	lights[2].color = { 1.0f , 4.5f, 1.0f };
	lights[3].color = { 1.0f , 1.0f, 0.5f };

	lights[0].intensity = 2.0f;
	lights[1].intensity = 3.0f;
	lights[2].intensity = 1.0f;
	lights[3].intensity = 2.5f;


	lights[0].position = { 4.0f , 4.0f, 1.0f }; // top-right
	lights[1].position = { -4.0f , 4.0f, 1.0f }; // top -left
	lights[2].position = { 4.0f , -4.0f, 1.0f }; // bottom-right
	lights[3].position = { -4.0f , -4.0f, 1.0f }; // bottom-left

	Sphere spheres[spehresCount];
	//srand(glfwGetTime());
	for (int i = 0; i < spehresCount; i++) {
		spheres[i].position = glm::vec3((i- spehresCount/2)*2.25f, 0.0f, 0.0f);
		spheres[i].albedo = glm::vec3((rand()%10)/10.0f, (rand() % 20) / 20.0f, 1.0f);
		spheres[i].metallic = (float) 1.0/(i+1);
		spheres[i].roughness = 0.4f;
		spheres[i].ao = 0.1f;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
	//renderer.EnableAntialiasing();
	// build and compile shaders
	// -------------------------
	//Shader mainShader("normal_model_shader.vert", "model_shader.frag");
	//Shader instancesShader("instanced_model_shader.vert", "model_shader.frag");
	Shader PBRShader("pbr_shader.vert", "pbr_shader.frag");

	// models setup
	// -------------------------
	float clearColor[3] = { 0.5f,0.5f,0.7f };
	// End 
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		camera.ApplySmoothMovement(deltaTime);

		renderer.Clear();
		glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 500.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0);

		PBRShader.Bind();
		PBRShader.setMat4("projection", projection);
		PBRShader.setMat4("view", view);
		PBRShader.setVec3("viewPos", camera.GetPosition());

		for (int i = 0; i < LIGHTS_COUNT; i++) {
			PBRShader.setVec3("light["+std::to_string(i)+"].position", lights[i].position);
			PBRShader.setVec3("light[" + std::to_string(i) + "].color", lights[i].GetColor());
		}

		for (int i = 0; i < spehresCount; i++) {
			PBRShader.setVec3("albedo",spheres[i].albedo);
			PBRShader.setFloat("metallic", spheres[i].metallic);
			PBRShader.setFloat("roughness", spheres[i].roughness);
			PBRShader.setFloat("ao", spheres[i].ao);

			model = glm::translate(glm::mat4(1.0f), spheres[i].position);

			PBRShader.setMat4("model", model);

			renderer.Draw(sphereModel, PBRShader);
		}


		//renderer.EndAntialaising();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.StepForward();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.StepBackward();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.StepLeft();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.StepRight();

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.OnCursorPositionChange(window, xposIn, yposIn);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.OnScroll(window, xoffset, yoffset);
}

unsigned int Create2DTexture(std::string pathName) {
	unsigned int _id;
	glGenTextures(1, &_id);
	int w, h, nrComponents;
	unsigned char* texData = stbi_load(pathName.c_str(), &w, &h, &nrComponents, 0);
	if (texData) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "ERROR::STBI::(" << pathName << ") couldn't be loaded." << std::endl;
	}
	stbi_image_free(texData);
	return _id;
}