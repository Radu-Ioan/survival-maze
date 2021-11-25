#pragma once

#include "components/simple_scene.h"
#include "lab_m1/game/lab_camera.h"


namespace m1
{
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
		bool renderCameraTarget;

		// [DONE]: If you need any other class variables, define them here.
		float fov, zNear, zFar;
		float left, right, bottom, top;
		// true for perspective
		bool perspectiveProj;
	};
}   // namespace m1
