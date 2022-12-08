#include "Camera.h"

Camera::Camera(glm::vec3 position, float windowWidth, float windowHeight) :m_Pos(position) {
	m_LastX = windowWidth / 2.0f;
	m_LastY = windowHeight / 2.0f;
	m_Direction = m_Front;
	updateCameraVectors();
}
void Camera::EnableMovement(GLFWwindow* window) {
	m_CanMove = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture mouse.
}
void Camera::DisableMovement(GLFWwindow* window) {
	m_CanMove = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //capture mouse.
}
void Camera::OnCursorPositionChange(GLFWwindow* window, double xpos, double ypos) {
	if (m_CanMove) {

		if (m_FirstMouse) // initially set to true
		{
			m_LastX = xpos;
			m_LastY = ypos;
			m_FirstMouse = false;
		}

		// calculate new offsets
		float xOffset = xpos - m_LastX;
		float yOffset = -(ypos - m_LastY);

		//update last offsets
		m_LastX = xpos;
		m_LastY = ypos;

		//adjust offsets to a sensitivity factor
		xOffset *= m_Sensitivity;
		yOffset *= m_Sensitivity;

		//update yaw (x/z) and pitch (y) values
		m_Yaw += xOffset;
		m_Pitch += yOffset;

		//constraints the pitch angle
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		//update direction vector
		updateCameraVectors();
	}
}
void Camera::OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
	m_FOV -= (float)yoffset;
	if (m_FOV < 1.0f)
		m_FOV = 1.0f;
	if (m_FOV > 45.0f)
		m_FOV = 45.0f;
};
void Camera::ApplySmoothMovement(float deltaTime) {
	if (m_CanMove) {
		m_SmoothVelocity = m_Speed * deltaTime;
	}
};
glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
}

void Camera::updateCameraVectors() {
	m_Direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Direction.y = sin(glm::radians(m_Pitch));
	m_Direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(m_Direction);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::StepForward() {
	m_Pos += m_SmoothVelocity * m_Front;
}
void Camera::StepBackward() {
	m_Pos -= m_SmoothVelocity * m_Front;
}
void Camera::StepRight() {
	m_Pos += m_SmoothVelocity * m_Right;
}
void Camera::StepLeft() {
	m_Pos -= m_SmoothVelocity * m_Right;
}
