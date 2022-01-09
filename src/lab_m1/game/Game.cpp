#include "lab_m1/game/Game.h"
#include "lab_m1/game/transform2D.h"

#include <vector>
#include <string>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;


Game::Game()
{
	InitPlayerAttributes();
	InitEnemiesAttributes();
	plane = factory::createCube("plane", colors.BLUE);

	mazeHeight = mazeWidth = 5;
	maze = implemented::Maze(mazeHeight, mazeWidth);
	mazeObstacle = factory::createCube("maze-cube", colors.BLUE, colors.BLUE,
	                                   colors.BLUE, colors.BLUE, colors.RED,
	                                   colors.RED, colors.RED, colors.RED);

	bulletMesh = new Mesh("bulletMesh");
	bulletMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS,
	                               "primitives"), "sphere.obj");
	meshes[bulletMesh->GetMeshID()] = bulletMesh;
	bulletScale = 0.5f;

	logicSpace = LogicSpace(0, 0, 4, 4);
	timeRemaining = 80.f;

	digit0 = factory::createDigit0(colors.DARK_RED);

	firstCamera = false;
}


Game::~Game()
= default;

void Game::InitPlayerAttributes()
{
	position = {0.f, 0.0f, 0.f};
	u = 0.f;

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

	arrow = factory::createIndicator("arrow", colors.GREEN);

	life = lifeMax = 12;
}

void Game::InitEnemiesAttributes()
{
	enemyBodyMesh = factory::createCube("enemy-mesh", colors.DARK_RED);
	enemyHeadMesh = factory::createSolidHexagon("enemy-haed-mesh",
										colors.DARK_RED, colors.MAGENTA);

	float dist = 0.2f;
	float shrink = 0.5f;
	enemyBodyScale = {0.2f, 0.65f, 0.1f};
	enemyBodyScale *= shrink;
	enemyBodyTranslate = {dist, 0.2f + 0.05, 0.f};

	enemyLeftHandScale = {0.1f, 0.23f, 0.1f};
	enemyLeftHandScale *= shrink;
	enemyRightHandScale = enemyLeftHandScale;

	enemyLeftHandTranslate = {0.2f + dist, 0.23f, 0.f};
	enemyRightHandTranslate = {-0.2f + dist, 0.23f, 0.f};

	enemyHeadScale = {0.1f, 0.125f, 0.1f};
	enemyHeadTranslate = {dist, 0.5f, 0.f};

	enemyAngle = 0.f;
	enemyDeltaHeight = 0.f;
	enemyMaxDeltaHeight = 0.7f;

	// up
	enemySense = true;
	gameStart = true;

	timeTangentWithEnemy = 0;
	limitForNextLifeDecrease = 0.5f;
}

void Game::GenerateEnemies()
{
	int total = (int) emptyCells.size() / 4;

	for (int i = 0; i < total; i++) {
		int idx = maze.randrange((int) emptyCells.size());
		int z = emptyCells[idx].first;
		int x = emptyCells[idx].second;
		enemies.push_back({x, z});
		emptyCells.erase(emptyCells.begin() + idx);
		maze.grid[z][x] = ENEMY_CELL;
	}
	gameStart = false;
}

void Game::Init()
{
	maze.generate();
	maze.generate_entrances();

	this->position = {2 * maze.start.first + 1, 0.f,
					  2 * maze.start.second + 1};

	camera = new implemented::Camera();
	glm::vec3 cameraPos = this->position;
	float dy = 4.f;
	float dz = 8.f;
	cameraPos.y += dy;
	cameraPos.z -= dz;
	camera->Set(cameraPos, this->position, {0.f, 1.f, 0.5f});
	camera->distanceToTarget = (float) sqrt(dy * dy + dz * dz);

	projectionMatrix = glm::perspective(RADIANS(60),
			window->props.aspectRatio, 0.01f, 200.0f);
}


void Game::FrameStart()
{
	glClearColor(1, 1, 1, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Game::Update(float deltaTimeSeconds)
{
	DrawPlane(deltaTimeSeconds);
	DrawMaze(deltaTimeSeconds);
	DrawEnemies(deltaTimeSeconds);
	DrawPlayer(deltaTimeSeconds);
	DrawArrow(deltaTimeSeconds);
	DrawBullets(deltaTimeSeconds);
//	DrawTime(deltaTimeSeconds);
	DrawLife(deltaTimeSeconds);

	UpdateEnemiesCollision(deltaTimeSeconds);
}

void Game::FrameEnd()
{
	DrawCoordinateSystem();
}

void Game::DrawPlane(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, {maze.W, 0.f, maze.H});
	modelMatrix = glm::scale(modelMatrix, {maze.W, 0.01f, maze.H});
	RenderMesh(plane, shaders["VertexColor"], modelMatrix);
}

void Game::DrawMaze(float deltaTimeSeconds)
{
	glm::vec3 translator = {1, 0.3f, 1};
	glm::vec3 scaling = {1, 0.2f, 1};

	for (int row = 0; row < maze.H; row++) {
		for (int col = 0; col < maze.W; col++) {
			uint8_t field = maze.grid[row][col];
			if (field == WALL) {
				glm::mat4 rendMatrix = glm::mat4(1);
				rendMatrix = glm::translate(rendMatrix,
											{2 * col, 0.f, 2 * row});
				rendMatrix = glm::translate(rendMatrix, translator);

				// it works because on OY I don't translate anything,
				// and on OZ and OX I don't scale
				rendMatrix = glm::scale(rendMatrix, scaling);
				RenderMesh(mazeObstacle, shaders["VertexColor"],
						   rendMatrix);
			} else if (!(col == maze.start.first && row == maze.start.second)
			         && !(col == maze.end.first && row == maze.end.second)
					 && !(row == maze.start.first && col == maze.start.second)
					 && !(row == maze.end.first && col == maze.end.second)) {
				emptyCells.push_back({row, col});
			}
		}
	}
}

void Game::DrawEnemies(float deltaTimeSeconds)
{
	if (gameStart)
		GenerateEnemies();

	auto it = enemies.begin();
	while (it != enemies.end()) {
		float dx = 2 * (*it).first + 1.f;
		float dz = 2 * (*it).second + 1.f;

		/* body */
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix,
									 glm::vec3(dx, enemyDeltaHeight, dz));
		modelMatrix = glm::rotate(modelMatrix, enemyAngle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, enemyBodyTranslate);
		modelMatrix = glm::scale(modelMatrix, enemyBodyScale);
		RenderMesh(enemyBodyMesh, shaders["VertexColor"], modelMatrix);

		/* left hand */
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix,
									 glm::vec3(dx, enemyDeltaHeight, dz));
		modelMatrix = glm::rotate(modelMatrix, enemyAngle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, enemyLeftHandTranslate);
		modelMatrix = glm::scale(modelMatrix, enemyLeftHandScale);
		RenderMesh(enemyBodyMesh, shaders["VertexColor"], modelMatrix);

		/* right hand */
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix,
		                             glm::vec3(dx, enemyDeltaHeight, dz));
		modelMatrix = glm::rotate(modelMatrix, enemyAngle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, enemyRightHandTranslate);
		modelMatrix = glm::scale(modelMatrix, enemyRightHandScale);
		RenderMesh(enemyBodyMesh, shaders["VertexColor"], modelMatrix);

		/* head */
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(dx, enemyDeltaHeight, dz));
		modelMatrix = glm::rotate(modelMatrix, enemyAngle, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, enemyHeadTranslate);
		modelMatrix = glm::scale(modelMatrix, enemyHeadScale);
		RenderMesh(enemyHeadMesh, shaders["VertexColor"], modelMatrix);

		++it;
	}

	enemyAngle += deltaTimeSeconds;
	if (enemyAngle >= 2 * PI)
		enemyAngle -= 2 * PI;
	enemyDeltaHeight += enemySense ? deltaTimeSeconds : -deltaTimeSeconds;

	if (enemyDeltaHeight > enemyMaxDeltaHeight)
		enemySense = false;
	else if (enemyDeltaHeight <= 0)
		enemySense = true;
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

void Game::DrawArrow(float deltaTimeSeconds)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, {0, -0.2f, 0});
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, u, {0, 1, 0});
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
	RenderMesh(arrow, shaders["VertexColor"], modelMatrix);
}

void Game::DrawBullets(float deltaTimeSeconds)
{
	float speed = 4;
	list<DynamicData>::iterator it = bullets.begin();

	while (it != bullets.end()) {
		auto &bullet = *it;
		bullet.pos += bullet.dir * speed * deltaTimeSeconds;
		int marginX = ((int) floor(bullet.pos.x)) / 2;
		int marginZ = ((int) floor(bullet.pos.z)) / 2;
		if (marginX >= 0 && marginX < maze.W && marginZ >= 0
					&& marginZ < maze.H && maze.grid[marginZ][marginX]) {
			it = bullets.erase(it);
			if (maze.grid[marginZ][marginX] == ENEMY_CELL) {
				for (auto i = enemies.begin(); i != enemies.end(); ++i) {
					auto item = *i;
					int enemyX = item.first;
					int enemyZ = item.second;
					if (enemyX == marginX && enemyZ == marginZ) {
						enemies.erase(i);
						maze.grid[marginZ][marginX] = EMPTY_CELL;
						break;
					}
				}
			}
		} else {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, bullet.pos);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(bulletScale));
			RenderMesh(bulletMesh, shaders["VertexColor"], modelMatrix);
			it++;
		}
	}
}

void Game::DrawTime(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();

	auto viewSpace = ViewportSpace(0, resolution.y - resolution.y / 8,
	                               resolution.x / 5, resolution.y / 16);
	SetViewportArea(viewSpace, glm::vec3(0.5f), true);

	auto visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
	auto modelMatrix = visMatrix * transform2D::Translate(0, 0);
	RenderMesh2D(digit0, shaders["VertexColor"], modelMatrix);

	timeRemaining -= deltaTimeSeconds;
}

void Game::DrawLife(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();
	int xStart = 4;
	int yStart = resolution.y - resolution.y / 16;
	int width = resolution.x / 5;
	int height = resolution.y / 16;

	auto lifeFrame = ViewportSpace(xStart, yStart, width, height);
	SetViewportArea(lifeFrame, colors.BLACK, true);

	int thickness = 4;
	int displayUnit = width / lifeMax;

	if (width - 2 * thickness - displayUnit * (lifeMax - life) > 0) {
		auto lifeContent = ViewportSpace(xStart + thickness,
		                                 yStart + thickness,
		                                 width - 2 * thickness
										    - displayUnit * (lifeMax - life),
		                                 resolution.y / 16 - 2 * thickness);
		SetViewportArea(lifeContent, colors.CYAN, true);
	} else {
		cout << "GAME OVER" << endl;
	}
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

bool Game::AllowMove(float deltaTime, float cameraSpeed, Direction direction)
{
	glm::vec3 playerForward{sin(u), 0, cos(u)};
	playerForward = glm::normalize(playerForward);

	float radius = 0.3f;
	// the next position of the player based on the direction given;
	// we check then if this new position makes a collision with a wall
	glm::vec3 dest;

	switch (direction)
	{
		case FORWARD:
			dest = this->position + playerForward * deltaTime * cameraSpeed;
			break;
		case BACK:
			dest = this->position - playerForward * deltaTime * cameraSpeed;
			break;
		default:
			return false;
	}

	glm::vec3 upMargin = dest + glm::vec3(0, 0, radius);
	glm::vec3 leftMargin = dest + glm::vec3(radius, 0, 0);
	glm::vec3 rightMargin = dest + glm::vec3(-radius, 0, 0);
	glm::vec3 downMargin = dest + glm::vec3(0, 0, -radius);

	int upMarginX = ((int) floor(upMargin.x)) / 2;
	int upMarginZ = ((int) floor(upMargin.z)) / 2;
	int leftMarginX = ((int) floor(leftMargin.x)) / 2;
	int leftMarginZ = ((int) floor(leftMargin.z)) / 2;
	int rightMarginX = ((int) floor(rightMargin.x)) / 2;
	int rightMarginZ = ((int) floor(rightMargin.z)) / 2;
	int downMarginX = ((int) floor(downMargin.x)) / 2;
	int downMarginZ = ((int) floor(downMargin.z)) / 2;

	int wall = 1;
	bool notAllowed = (upMarginZ < 0 || upMarginX < 0
				|| rightMarginZ < 0 || rightMarginX < 0
				|| leftMarginZ < 0 || leftMarginX < 0
				|| downMarginZ < 0 || downMarginX < 0
				|| upMarginZ >= maze.H || upMarginX >= maze.W
				|| rightMarginZ >= maze.H || rightMarginX >= maze.W
				|| leftMarginZ >= maze.H || leftMarginX >= maze.W
				|| downMarginZ >= maze.H || downMarginX >= maze.W
				|| maze.grid[upMarginZ][upMarginX] == wall
				|| maze.grid[rightMarginZ][rightMarginX] == wall
				|| maze.grid[leftMarginZ][leftMarginX] == wall
				|| maze.grid[downMarginZ][downMarginX] == wall);
	bool allowed = !notAllowed;

	return allowed;
}

void Game::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
		firstCamera = true;

	if (window->KeyHold(GLFW_KEY_W)) {
		if (AllowMove(deltaTime, cameraSpeed, FORWARD)) {
			position.x += sin(u) * deltaTime * cameraSpeed;
			position.z += cos(u) * deltaTime * cameraSpeed;

			SetThirdCamera();
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		if (AllowMove(deltaTime, cameraSpeed, BACK)) {
			position.x -= sin(u) * deltaTime * cameraSpeed;
			position.z -= cos(u) * deltaTime * cameraSpeed;

			SetThirdCamera();
		}
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
}

void Game::UpdateEnemiesCollision(float deltaTimeSeconds)
{
	int x = ((int) floor(position.x)) / 2;
	int z = ((int) floor(position.z)) / 2;
	if (maze.grid[z][x] == ENEMY_CELL) {
		if (timeTangentWithEnemy == 0)
			--life;

		timeTangentWithEnemy += deltaTimeSeconds;
		if (timeTangentWithEnemy >= limitForNextLifeDecrease)
			timeTangentWithEnemy = 0;
	}
}

void Game::SetFirstCamera()
{
	float unit = 1.5f;
	glm::vec3 cameraPos = this->position;
	float dx = sin(u) * unit;
	float dz = cos(u) * unit;
	cameraPos.x += dx;
	cameraPos.z += dz;
	cameraPos.y += 0.5f;

	glm::vec3 center = cameraPos;
	center += glm::vec3(sin(u) * 2 * unit, 0, cos(u) * 2 * unit);
	camera->Set(cameraPos, center, {0, 1, 0});
	camera->distanceToTarget = unit;
}

void Game::SetThirdCamera()
{
	glm::vec3 cameraPos = this->position;
	float dy = 4.f;
	float dz = 8.f;
	cameraPos.y += dy;
	cameraPos.z -= dz;
	camera->Set(cameraPos, this->position, {0.f, 1.f, 0.5f});
	camera->distanceToTarget = (float) sqrt(dy * dy + dz * dz);

	camera->RotateThirdPerson_OY(u);

	projectionMatrix = glm::perspective(RADIANS(60),
						window->props.aspectRatio, 0.01f, 200.0f);
}

void Game::SetViewportArea(const ViewportSpace &viewSpace,
						   glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float) viewSpace.x,
									  (float) (viewSpace.x + viewSpace.width),
									  (float) viewSpace.y,
									  (float) (viewSpace.y + viewSpace.height),
									  0.1f, 400);
	GetSceneCamera()->Update();
}

glm::mat3 Game::VisualizationTransf2D(const LogicSpace &logicSpace,
									  const ViewportSpace &viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
			sx, 0.0f, tx,
			0.0f, sy, ty,
			0.0f, 0.0f, 1.0f));
}

glm::mat3 Game::VisualizationTransf2DUnif(const LogicSpace &logicSpace,
                                    const ViewportSpace &viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x
			+ (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y
			+ (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
			smin, 0.0f, tx,
			0.0f, smin, ty,
			0.0f, 0.0f, 1.0f));
}

void Game::OnKeyPress(int key, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (key == GLFW_KEY_SPACE) {
			auto dir = glm::vec3(0, 0, 1);
			glm::vec4 buffer = glm::rotate(glm::mat4(1.f), u, {0, 1, 0})
			                   * glm::vec4(dir, 1);
			dir = {buffer.x, buffer.y, buffer.z};

			float distanceFromPlayerAtLaunch = 0.2f;
			glm::vec3 initPos = position + dir * distanceFromPlayerAtLaunch;
			initPos.y = 0.5f;
			bullets.push_front(DynamicData(initPos, dir, u));
		}
	}
}


void Game::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (window->KeyHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			auto dir = glm::vec3(0, 0, 1);
			glm::vec4 buffer = glm::rotate(glm::mat4(1.f), u, {0, 1, 0})
			                   * glm::vec4(dir, 1);
			dir = {buffer.x, buffer.y, buffer.z};

			float distanceFromPlayerAtLaunch = 0.2f;
			glm::vec3 initPos = position + dir * distanceFromPlayerAtLaunch;
			initPos.y = 0.5f;
			bullets.push_front(DynamicData(initPos, dir, u));
		}
	}
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
