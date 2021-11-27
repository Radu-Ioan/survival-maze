#include "lab_m1/game/Camera.h"

using namespace implemented;

Camera::Camera()
{
	position    = glm::vec3(0, 2, 5);
	forward     = glm::vec3(0, 0, -1);
	up          = glm::vec3(0, 1, 0);
	right       = glm::vec3(1, 0, 0);
	distanceToTarget = 2;
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
	// Translates the camera using the `dir` vector computed from
	// `forward`. Movement will always keep the camera at the same
	// height. For example, if you rotate your head up/down, and then
	// walk forward, then you will still keep the same relative
	// distance (height) to the ground!
	glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	position += dir * distance;
}

void Camera::TranslateForward(float distance)
{
	// [DONE]: Translate the camera using the `forward` vector.
	// What's the difference between `TranslateForward()` and
	// `MoveForward()`?
	// glm::vec3 dir = glm::normalize(glm::vec3(forward.x, forward.y,
	//                                         forward.z));
	glm::vec3 dir = glm::normalize(forward);
	position += dir * distance;
}

void Camera::TranslateUpward(float distance)
{
	// [DONE]: Translate the camera using the `up` vector.
	glm::vec3 dir = glm::normalize(up);
	position += dir * distance;
}

void Camera::TranslateRight(float distance)
{
	// [DONE]: See instructions below. Read the entire thing!
	// You need to translate the camera using the `right` vector.
	// Usually, however, translation using camera's `right` vector
	// is not very useful, because if the camera is rotated around the
	// `forward` vector, then the translation on the `right` direction
	// will have an undesired effect, more precisely, the camera will
	// get closer or farther from the ground. The solution is to
	// actually use the projected `right` vector (projected onto the
	// ground plane), which makes more sense because we will keep the
	// same distance from the ground plane.
	glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
	position += distance * dir;
}

void Camera::RotateFirstPerson_OX(float angle)
{
	// [DONE]: Compute the new `forward` and `up` vectors.
	// Don't forget to normalize the vectors! Use `glm::rotate()`.
	// glm::vec3(1, 0, 0) -> directia lui OX
	glm::vec4 buffer;
	// glm::vec3 temp = glm::vec3(1, 0, 0);
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
	// [DONE]: Compute the new `forward`, `up` and `right`
	// vectors. Use `glm::rotate()`. Don't forget to normalize the
	// vectors!
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
	// [DONE]: Compute the new `right` and `up`. This time,
	// `forward` stays the same. Use `glm::rotate()`. Don't forget
	// to normalize the vectors!
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
	// [DONE]: Rotate the camera in third-person mode around
	// the OX axis. Use `distanceToTarget` as translation distance.
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OX(angle);
	TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OY(float angle)
{
	// [DONE]: Rotate the camera in third-person mode around
	// the OY axis.
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OY(angle);
	TranslateForward(-distanceToTarget);
}

void Camera::RotateThirdPerson_OZ(float angle)
{
	// [DONE]: Rotate the camera in third-person mode around
	// the OZ axis.
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
