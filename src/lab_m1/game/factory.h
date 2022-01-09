#ifndef _FACTORY_H_
#define _FACTORY_H_

#pragma once

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace factory
{
	Mesh *createCube(const std::string &name, glm::vec3 color);
	Mesh *createCube(const std::string &name, glm::vec3 v1Color,
	                 glm::vec3 v2Color, glm::vec3 v3Color,
	                 glm::vec3 v4Color, glm::vec3 v5Color,
	                 glm::vec3 v6Color, glm::vec3 v7Color,
	                 glm::vec3 v8Color);
	Mesh *createIndicator(const std::string &name, glm::vec3 color);
	Mesh *createSolidHexagon(const std::string &name, glm::vec3 color1, glm::vec3 color2);

	Mesh *createDigit0(glm::vec3 color);
	Mesh *createDigit1(glm::vec3 color);
	Mesh *createDigit2(glm::vec3 color);
	Mesh *createDigit3(glm::vec3 color);
	Mesh *createDigit4(glm::vec3 color);
	Mesh *createDigit5(glm::vec3 color);
	Mesh *createDigit6(glm::vec3 color);
	Mesh *createDigit7(glm::vec3 color);
	Mesh *createDigit8(glm::vec3 color);
	Mesh *createDigit9(glm::vec3 color);
}

#endif
