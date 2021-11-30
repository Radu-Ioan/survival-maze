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
	position = {0.f, 0.0f, 0.f};
	u = 0.f;

	plane = factory::createCube("plane", colors.BLUE);
	body = factory::createCube("body", colors.YELLOW);
	leg = factory::createCube("leg", colors.PUCE);
	hand = factory::createCube("hand", colors.MAGENTA);
	head = factory::createCube("head", colors.PUCE);

	float shrink = 0.45f;
	bodyScale = {0.5f, 0.5f, 0.1f};
	bodyScale *= shrink;
	bodyTranslate = {0.f, 0.55f, 0.f};

	legScale = {0.1f, 0.25f, 0.2f};
	legScale *= shrink;
	leftLegTranslate = {-0.18f, 0.18f, 0.f};
	rightLegTranslate = {0.18f, 0.18f, 0.f};

	handScale = {0.1f, 0.35f, 0.2f};
	handScale *= shrink;
	leftHandTranslate = {-0.3f, 0.6f, 0.f};
	rightHandTranslate = {0.3f, 0.6f, 0.f};

	headScale = {0.2f, 0.2f, 0.2f};
	headScale *= shrink;
	headTranslate = {0.f, 0.9f, 0.f};
}


Game::~Game()
{
}


void Game::Init()
{
	camera = new implemented::Camera();
	// camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::vec3 cameraPos = this->position;
	cameraPos.y += 1.f;
	cameraPos.z += 2.f;
	camera->Set(cameraPos, this->position, {0.f, 1.f, 0.5f});

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
	/*{
		glm::mat4 modelMatrix = glm::mat4(1);
		// modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-5, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f),
			glm::vec3(0, 1, 0));

		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}
	// Render the camera target. This is useful for understanding where
	// the rotation point is, when moving in third-person camera mode.
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f));
		// RenderMesh(body, shaders["VertexNormal"], modelMatrix);
		RenderMesh(body, shaders["VertexColor"], modelMatrix);
	}*/
	DrawPlane(deltaTimeSeconds);
	DrawPlayer(deltaTimeSeconds);
}


void Game::FrameEnd()
{
	DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
	// DrawCoordinateSystem();
}

void Game::DrawPlayer(float deltaTimeSeconds)
{
	/* left leg */
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::translate(modelMatrix, leftLegTranslate);
	modelMatrix = glm::scale(modelMatrix, legScale);
	RenderMesh(leg, shaders["VertexColor"], modelMatrix);

	/* right leg */
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::translate(modelMatrix, rightLegTranslate);
	modelMatrix = glm::scale(modelMatrix, legScale);
	RenderMesh(leg, shaders["VertexColor"], modelMatrix);

	/* body */
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::translate(modelMatrix, bodyTranslate);
	modelMatrix = glm::scale(modelMatrix, bodyScale);
	RenderMesh(body, shaders["VertexColor"], modelMatrix);

	/* left hand */
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::translate(modelMatrix, leftHandTranslate);
	modelMatrix = glm::scale(modelMatrix, handScale);
	RenderMesh(hand, shaders["VertexColor"], modelMatrix);

	/* right hand */
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::translate(modelMatrix, rightHandTranslate);
	modelMatrix = glm::scale(modelMatrix, handScale);
	RenderMesh(hand, shaders["VertexColor"], modelMatrix);

	/* head */
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::translate(modelMatrix, headTranslate);
	modelMatrix = glm::scale(modelMatrix, headScale);
	RenderMesh(head, shaders["VertexColor"], modelMatrix);
}

void Game::DrawPlane(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::scale(modelMatrix, {40.f, 0.01f, 40.f});
	// RenderMesh(plane, shaders["VertexColor"], modelMatrix);
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
	float epsilon = 0.01f;

	if (window->KeyHold(GLFW_KEY_W)) {
		float v = u + PI/2;
		float dx = position.x;
		float dz = position.z;
		// if (sin(v) >= 0 && cos(v) <= epsilon) {
		// 	position.x += sin(v) * deltaTime * cameraSpeed;
		// 	position.z += cos(v) * deltaTime * cameraSpeed;
		// } else if (sin(v) <= 0 && cos(v) <= 0) {
		// 	position.x += sin(v) * deltaTime * cameraSpeed;
		// 	position.z -= cos(v) * deltaTime * cameraSpeed;
		// } else if (sin(v) <= 0 && cos(v) >= epsilon) {
		// 	position.x -= sin(v) * deltaTime * cameraSpeed;
		// 	position.z -= cos(v) * deltaTime * cameraSpeed;
		// } else if (sin(v) >= 0 && cos(v) >= 0) {
		// 	position.x -= sin(v) * deltaTime * cameraSpeed;
		// 	position.z += cos(v) * deltaTime * cameraSpeed;
		// }

		// position.x -= sin(v) * deltaTime * cameraSpeed;
		// position.z -= cos(v) * deltaTime * cameraSpeed;
		position.x -= sin(u) * deltaTime * cameraSpeed;
		position.z -= cos(u) * deltaTime * cameraSpeed;
		dx = position.x - dx;
		dz = position.z - dz;
		camera->position.x += dx;
		camera->position.z += dz;
		camera->Set(camera->position, this->position, camera->up);
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		float v = u + PI/2;
		float dx = position.x;
		float dz = position.z;
		// if (sin(v) >= 0 && cos(v) <= epsilon) {
		// 	position.x -= sin(v) * deltaTime * cameraSpeed;
		// 	position.z -= cos(v) * deltaTime * cameraSpeed;
		// } else if (sin(v) <= 0 && cos(v) <= epsilon) {
		// 	position.x -= sin(v) * deltaTime * cameraSpeed;
		// 	position.z += cos(v) * deltaTime * cameraSpeed;
		// } else if (sin(v) <= 0 && cos(v) >= epsilon) {
		// 	position.x += sin(v) * deltaTime * cameraSpeed;
		// 	position.z += cos(v) * deltaTime * cameraSpeed;
		// } else if (sin(v) >= 0 && cos(v) >= epsilon) {
		// 	position.x += sin(v) * deltaTime * cameraSpeed;
		// 	position.z -= cos(v) * deltaTime * cameraSpeed;
		// }
		// position.x += sin(v) * deltaTime * cameraSpeed;
		// position.z += cos(v) * deltaTime * cameraSpeed;

		position.x += sin(u) * deltaTime * cameraSpeed;
		position.z += cos(u) * deltaTime * cameraSpeed;
		dx = position.x - dx;
		dz = position.z - dz;
		camera->position.x += dx;
		camera->position.z += dz;
		camera->Set(camera->position, this->position, camera->up);
	}

	
	if (window->KeyHold(GLFW_KEY_A)) {
		float du = u;
		u += deltaTime * cameraSpeed;
		du = u - du;
		if (u < -2 * PI)
			u += 2 * PI;
		camera->RotateThirdPerson_OY(du);
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		float du = u;
		u -= deltaTime * cameraSpeed;
		du = u - du;
		if (u > 2 * PI)
			u -= 2 * PI;
		camera->RotateThirdPerson_OY(du);
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


void Game::OnKeyPress(int key, int mods)
{
	// Add key press event
}


void Game::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
	float sensivityOX = 0.001f;
	float sensivityOY = 0.001f;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		// [DONE]: Rotate the camera in third-person mode around
		// OX and OY using `deltaX` and `deltaY`. Use the sensitivity
		// variables for setting up the rotation speed.
		camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
		camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
	}
}

void oldMouseMove()
{
	// if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	// {
	// 	float sensivityOX = 0.001f;
	// 	float sensivityOY = 0.001f;

	// 	if (window->GetSpecialKeyState() == 0) {
	// 		// [DONE]: Rotate the camera in first-person mode around
	// 		// OX and OY using `deltaX` and `deltaY`. Use the sensitivity
	// 		// variables for setting up the rotation speed.
	// 		camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
	// 		camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
	// 	}

	// 	if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
	// 		// [DONE]: Rotate the camera in third-person mode around
	// 		// OX and OY using `deltaX` and `deltaY`. Use the sensitivity
	// 		// variables for setting up the rotation speed.
	// 		camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
	// 		camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
	// 	}
	// }
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
