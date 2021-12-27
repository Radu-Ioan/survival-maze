#include "components/simple_scene.h"

namespace m1
{
	struct Colors
	{
		glm::vec3 BLACK, DARK_GREY, WHITE, RED,
		GREEN, BLUE, PURPLE, YELLOW, DARK_RED,
		LIGHT_BLUE, MAGENTA, PUCE;
		Colors() : BLACK(0.f, 0.f, 0.f),
				DARK_GREY(0.1f, 0.1f, 0.1f),
				WHITE(1.f, 1.f, 1.f),
				RED(1.f, 0.f, 0.f),
				GREEN(0.f, 1.f, 0.f),
				BLUE(0.f, 0.f, 1.f),
				PURPLE(1.f, 0.f, 1.f),
				YELLOW(1.f, 1.f, 0.f),
				DARK_RED(0.6f, 0.0f, 0.0f),
				LIGHT_BLUE(0.6f, 1.f, 1.f),
				MAGENTA(1.f, 0.f, 1.f),
				PUCE(169.f / 255, 92.f / 255, 104.f / 255)
				{}
	};

	enum Direction
	{
		FORWARD, LEFT, BACK, RIGHT
	};
}
