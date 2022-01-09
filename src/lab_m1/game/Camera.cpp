#include "lab_m1/game/Camera.h"

using namespace implemented;

Camera::Camera()
{
	position    = glm::vec3(0, 2, 5);
	forward     = glm::vec3(0, 0, -1);
	up          = glm::vec3(0, 1, 0);
	right       = glm::vec3(1, 0, 0);
	distanceToTarget = 4;
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &center,
		const glm::vec3 &up)
{
	Set(position, center, up);
}

Camera::~Camera()
{ }

// Update camera
void Camera::Set(const glm::vec3 &position, const glm::vec3 &center,
	const glm::vec3 &up)
{
	this->position = position;
	forward     = glm::normalize(center - position);
	right       = glm::cross(forward, up);
	this->up    = glm::cross(right, forward);
}

void Camera::MoveForward(float distance)
{
	glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	position += dir * distance;
}

void Camera::TranslateForward(float distance)
{
	glm::vec3 dir = glm::normalize(forward);
	position += dir * distance;
}

void Camera::TranslateUpward(float distance)
{
	glm::vec3 dir = glm::normalize(up);
	position += dir * distance;
}

void Camera::TranslateRight(float distance)
{
	glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
	position += distance * dir;
}

void Camera::RotateFirstPerson_OX(float angle)
{
	glm::vec4 buffer;
	glm::vec3 temp = right;
	buffer = glm::rotate(glm::mat4(1.0f), angle, temp)
				* glm::vec4(forward, 1);
	forward = glm::normalize(glm::vec3(buffer));
	buffer = glm::rotate(glm::mat4(1.0f), angle, temp)
				* glm::vec4(up, 1);
	up = glm::normalize(glm::vec3(buffer));
}

void Camera::RotateFirstPerson_OY(float angle)
{
	glm::vec4 buffer;
	buffer = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
				* glm::vec4(forward, 1);
	forward = glm::normalize(glm::vec3(buffer));
	buffer = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
				* glm::vec4(up, 1);
	up = glm::normalize(glm::vec3(buffer));
	right = glm::cross(forward, up);
}

void Camera::RotateFirstPerson_OZ(float angle)
{
	glm::vec4 buffer;
	buffer = glm::rotate(glm::mat4(1.0f), angle, forward)
				* glm::vec4(right, 1);
	right = glm::normalize(glm::vec3(buffer));
	buffer = glm::rotate(glm::mat4(1.0f), angle, forward)
				* glm::vec4(up, 1);
	up = glm::normalize(glm::vec3(buffer));
}

void Camera::RotateThirdPerson_OX(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OX(angle);
	TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OY(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OY(angle);
	TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OZ(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OZ(angle);
	TranslateForward(-distanceToTarget);
}

glm::mat4 Camera::GetViewMatrix()
{
	// Returns the view matrix
	return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetTargetPosition()
{
	return position + forward * distanceToTarget;
}
