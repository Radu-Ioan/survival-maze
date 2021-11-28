#ifndef _FACTORY_H_
#define _FACTORY_H_

#pragma once

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace factory
{
	Mesh *createCube(const std::string &name, glm::vec3 color);
}

#endif
