#include "lab_m1/game/Game.h"

#include <vector>
#include <string>
#include <iostream>

/* de la lab 2 */
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;


Game::Game()
{
}


Game::~Game()
{
}


void Game::Init()
{
	renderCameraTarget = false;

	camera = new implemented::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
			"primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
			"primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// [DONE]: After you implement the changing of the projection
	// parameters, remove hardcodings of these parameters
	projectionMatrix = glm::perspective(RADIANS(60),
			window->props.aspectRatio, 0.01f, 200.0f);
}


void Game::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Game::Update(float deltaTimeSeconds)
{
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f),
			glm::vec3(0, 1, 0));

		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}
	// Render the camera target. This is useful for understanding where
	// the rotation point is, when moving in third-person camera mode.
	// if (renderCameraTarget)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
}


void Game::FrameEnd()
{
	DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
	// DrawCoordinateSystem();
}

void Game::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// Render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE,
			glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE,
			glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE,
			glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Game::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{

		if (window->KeyHold(GLFW_KEY_W)) {
			// [DONE]: Translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// [DONE]: Translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// [DONE]: Translate the camera backward
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// [DONE]: Translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// [DONE]: Translate the camera downward
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// [DONE]: Translate the camera upward
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}
	}
}


void Game::OnKeyPress(int key, int mods)
{
	// Add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
}


void Game::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// [DONE]: Rotate the camera in first-person mode around
			// OX and OY using `deltaX` and `deltaY`. Use the sensitivity
			// variables for setting up the rotation speed.
			camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
		}

		if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// [DONE]: Rotate the camera in third-person mode around
			// OX and OY using `deltaX` and `deltaY`. Use the sensitivity
			// variables for setting up the rotation speed.
			camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
			camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
		}
	}
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Game::OnWindowResize(int width, int height)
{
}
