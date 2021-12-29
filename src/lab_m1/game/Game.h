#pragma once

#include "components/simple_scene.h"
#include "lab_m1/game/Camera.h"
#include "lab_m1/game/Maze.h"
#include "lab_m1/game/constants.h"
#include "lab_m1/game/factory.h"

#include <list>

namespace m1
{
	struct DynamicData
	{
		DynamicData(glm::vec3 pos, glm::vec3 dir, float u)
					: pos(pos), dir(dir), u(u) {}
		// the position at a certain moment
		glm::vec3 pos;
		// the direction of movement
		glm::vec3 dir;
		// the rotation angle
		float u;
	};

	class Game : public gfxc::SimpleScene
	{
	public:
		Game();
		~Game();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void DrawPlane(float deltaTimeSeconds);
		void DrawMaze(float deltaTimeSeconds);
		void DrawEnemies(float deltaTimeSeconds);
		void DrawPlayer(float deltaTimeSeconds);
		void DrawArrow(float deltaTimeSeconds);
		void DrawBullets(float deltaTimeSeconds);

		/* for collisions */
		bool allowMove(float deltaTime, float cameraSpeed,
					   Direction direction);

		void RenderMesh(Mesh *mesh, Shader *shader,
			const glm::mat4 &modelMatrix) override;


		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
			override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
			override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
			override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
			override;
		void OnWindowResize(int width, int height) override;

	protected:
		implemented::Camera *camera;
		glm::mat4 projectionMatrix;

		/* constants */
		Colors colors;
		const float PI = 3.1415f;

		Mesh *plane;
		/* player variables */
		glm::vec3 position;
		// rotation angle
		float u;
		Mesh *body, *leg, *hand, *head;
		glm::vec3 legScale, bodyScale, handScale, headScale;
		glm::vec3 leftLegTranslate, rightLegTranslate;
		glm::vec3 leftHandTranslate, rightHandTranslate;
		glm::vec3 bodyTranslate, headTranslate;

		/* maze attributes */
		int mazeWidth, mazeHeight;
		implemented::Maze maze;
		Mesh *mazeObstacle;
		Mesh *arrow;
		// used to know where to emplace enemies
		std::vector<std::pair<int, int>> emptyCells;

		/* for enemies */
		Mesh *enemy;
		std::vector<DynamicData> enemies;

		/* for bullets */
		Mesh *bulletMesh;
		float bulletScale;
		std::list<DynamicData> bullets;

		// for a short debug on collisions
		float quick_time_buffer = 0;
		const float time_quick_limit = 2.f;
	};
}
