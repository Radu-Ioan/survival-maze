#include "lab_m1/game/Game.h"

#include <vector>
#include <string>

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

	mazeHeight = mazeWidth = 5;
	maze = implemented::Maze(mazeHeight, mazeWidth);
	mazeObstacle = factory::createCube("maze-cube", colors.BLUE, colors.BLUE,
	                                   colors.BLUE, colors.BLUE, colors.RED,
	                                   colors.RED, colors.RED, colors.RED);
}


Game::~Game()
= default;


void Game::Init()
{
	maze.generate();
	maze.generate_entrances();
	this->position = {maze.start.first, 0.f, maze.start.second};

	camera = new implemented::Camera();
	glm::vec3 cameraPos = this->position;
	float dy = 4.f;
	float dz = 8.f;
	cameraPos.y += dy;
	cameraPos.z += dz;
	camera->Set(cameraPos, this->position, {0.f, 1.f, 0.5f});
	camera->distanceToTarget = (float) sqrt(dy * dy + dz * dz);

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
	DrawPlane(deltaTimeSeconds);
	DrawMaze(deltaTimeSeconds);
	DrawPlayer(deltaTimeSeconds);
}

void Game::FrameEnd()
{
	DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
	// DrawCoordinateSystem();
}

void Game::DrawMaze(float deltaTimeSeconds)
{
	glm::vec3 damp = {0.5f, 0.5f, 0.5f};
	glm::vec3 translate = {1, 1, 1};

	for (int row = 0; row < maze.H; row++) {
		for (int col = 0; col < maze.W; col++) {
			uint8_t field = maze.grid[row][col];
			if (field) {
				if (row != maze.start.second && col != maze.start.first
							&& row != maze.end.second
							&& col != maze.end.first) {
					glm::mat4 rendMatrix = glm::mat4(1);
					rendMatrix = glm::translate(rendMatrix,
												{2 * col, 0.f, 2 * row});
					rendMatrix = glm::translate(rendMatrix, translate);
//					rendMatrix = glm::translate(rendMatrix, damp);
//					rendMatrix = glm::scale(rendMatrix, damp);
					RenderMesh(mazeObstacle, shaders["VertexColor"],
							   rendMatrix);
				}
			}
		}
	}
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
//	glm::mat4 modelMatrix = glm::mat4(1);
//	modelMatrix = glm::scale(modelMatrix, {40.f, 0.01f, 40.f});
//	RenderMesh(plane, shaders["VertexColor"], modelMatrix);
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

	if (window->KeyHold(GLFW_KEY_W)) {
		camera->MoveForward(deltaTime * cameraSpeed);
		position = camera->GetTargetPosition();
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		camera->MoveForward(-deltaTime * cameraSpeed);
		position = camera->GetTargetPosition();
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

	/* voi scoate si if-urile astea */
//	if (window->KeyHold(GLFW_KEY_Q)) {
//		// [DONE]: Translate the camera downward
//		camera->TranslateUpward(-deltaTime * cameraSpeed);
//	}
//
//	if (window->KeyHold(GLFW_KEY_E)) {
//		// [DONE]: Translate the camera upward
//		camera->TranslateUpward(deltaTime * cameraSpeed);
//		position = camera->GetTargetPosition();
//	}
}

void deprecatedMovement()
{
	/* if w is pressed */
//		float v = u + PI/2;
//		float dx = position.x;
//		float dz = position.z;
//		position.x -= sin(u) * deltaTime * cameraSpeed;
//		position.z -= cos(u) * deltaTime * cameraSpeed;
//		dx = position.x - dx;
//		dz = position.z - dz;
//		camera->position.x += dx;
//		camera->position.z += dz;
//		camera->Set(camera->position, this->position, camera->up);
	/* if s is pressed */
//		float v = u + PI/2;
//		float dx = position.x;
//		float dz = position.z;
//		position.x += sin(u) * deltaTime * cameraSpeed;
//		position.z += cos(u) * deltaTime * cameraSpeed;
//		dx = position.x - dx;
//		dz = position.z - dz;
//		camera->position.x += dx;
//		camera->position.z += dz;
//		camera->Set(camera->position, this->position, camera->up);
}

void Game::OnKeyPress(int key, int mods)
{
	// Add key press event
}


void Game::OnKeyRelease(int key, int mods)
{
	// Add key release event
}

/**
 * Va fi scoasa aceasta functie.
 */
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
