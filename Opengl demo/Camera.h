#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:
	GLFWwindow* m_GlWindow;
	bool m_FirstMouse = true;
	bool m_CanMove = false;
	float m_Yaw = -45.0f; // make sure that the camera initially looks at the negative z-axis.
	float m_Pitch = 0.0f;
	float m_LastX;//= (float)WINDOW_WIDTH / 2.0f;
	float m_LastY;//= (float)WINDOW_HEIGHT / 2.0f;
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Right;
	const glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Pos;
	glm::vec3 m_Direction;
	float m_Sensitivity = 0.1f;
	float m_Speed = 4.5f;
	float m_SmoothVelocity = 2.5f;
	float m_FOV = 45.0f;
	void updateCameraVectors();
public:
	Camera(glm::vec3 position, float windowWidth, float windowHeight);
	void EnableMovement(GLFWwindow* window);
	void DisableMovement(GLFWwindow* window);
	void OnCursorPositionChange(GLFWwindow* window, double xpos, double ypos);
	void OnScroll(GLFWwindow* window, double xoffset, double yoffset);
	void ApplySmoothMovement(float deltaTime);
	glm::mat4 GetViewMatrix();
	glm::mat4 LookAtDirection(glm::vec3 direction);
	void StepForward();
	void StepBackward();
	void StepRight();
	void StepLeft();

	inline void setSpeed(float speed) { m_Speed = speed; };

	inline void SetYaw(float newVal) { m_Yaw = newVal; updateCameraVectors(); };
	inline void SetFOV(float newVal) { m_FOV = newVal; };
	inline bool IsMovementEnabled() const { return m_CanMove; };
	inline float GetFOV() const { return m_FOV; };
	inline glm::vec3 GetPosition() const { return m_Pos; };
	inline glm::vec3 GetFront() const { return m_Front; };
};