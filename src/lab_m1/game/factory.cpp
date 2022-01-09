#include "factory.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

#include <iostream>
#include <vector>

static Mesh *CreateMesh(const std::string &name,
	const std::vector<VertexFormat> &vertices,
	const std::vector<unsigned int> &indices);

Mesh *factory::createCube(const std::string &name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1, -1,  1), color),
		VertexFormat(glm::vec3(1, -1, 1), color),
		VertexFormat(glm::vec3(1, -1, -1), color),
		VertexFormat(glm::vec3(-1, -1, -1), color),

		VertexFormat(glm::vec3(-1, 1, 1), color),
		VertexFormat(glm::vec3(1, 1, 1), color),
		VertexFormat(glm::vec3(1, 1, -1), color),
		VertexFormat(glm::vec3(-1, 1, -1), color),
	};

	std::vector<unsigned int> indices
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		4, 5, 6,
		4, 6, 7,
		3, 2, 6,
		3, 6, 7,
		0, 3, 7,
		0, 7, 4
	};

	return CreateMesh(name, vertices, indices);
}

Mesh *factory::createCube(const std::string &name, glm::vec3 v1Color,
						  glm::vec3 v2Color, glm::vec3 v3Color,
						  glm::vec3 v4Color, glm::vec3 v5Color,
						  glm::vec3 v6Color, glm::vec3 v7Color,
						  glm::vec3 v8Color)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1, -1,  1), v1Color),
		VertexFormat(glm::vec3(1, -1, 1), v2Color),
		VertexFormat(glm::vec3(1, -1, -1), v3Color),
		VertexFormat(glm::vec3(-1, -1, -1), v4Color),
		VertexFormat(glm::vec3(-1, 1, 1), v5Color),
		VertexFormat(glm::vec3(1, 1, 1), v6Color),
		VertexFormat(glm::vec3(1, 1, -1), v7Color),
		VertexFormat(glm::vec3(-1, 1, -1), v8Color),
	};

	std::vector<unsigned int> indices
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		4, 5, 6,
		4, 6, 7,
		3, 2, 6,
		3, 6, 7,
		0, 3, 7,
		0, 7, 4
	};

	return CreateMesh(name, vertices, indices);
}

Mesh *factory::createIndicator(const std::string &name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(1, 0.5f, -1), color),
		VertexFormat(glm::vec3(-1, 0.5f, -1), color),
		VertexFormat(glm::vec3(0, 0.5f, 2), color)
	};

	std::vector<unsigned int> indices
	{
		0, 1, 2
	};

	return CreateMesh(name, vertices, indices);
}

Mesh *factory::createSolidHexagon(const std::string &name, glm::vec3 color1, glm::vec3 color2)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1, 0, 1), color1),
		VertexFormat(glm::vec3(-0.5f, -1, 1), color1),
		VertexFormat(glm::vec3(0.5f, -1, 1), color2),
		VertexFormat(glm::vec3(1, 0, 1), color1),
		VertexFormat(glm::vec3(0.5f, 1, 1), color1),
		VertexFormat(glm::vec3(-0.5f, 1, 1), color2),

		VertexFormat(glm::vec3(-1, 0, -1), color1),
		VertexFormat(glm::vec3(-0.5f, -1, -1), color2),
		VertexFormat(glm::vec3(0.5f, -1, -1), color2),
		VertexFormat(glm::vec3(1, 0, -1), color1),
		VertexFormat(glm::vec3(0.5f, 1, -1), color2),
		VertexFormat(glm::vec3(-0.5f, 1, -1), color2),
	};

	std::vector<unsigned int> indices
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,

		0, 5, 11,
		0, 11, 6,
		0, 6, 7,
		0, 7, 1,
		4, 10, 11,
		4, 11, 5,
		4, 9, 3,
		4, 10, 9,
		2, 3, 9,
		2, 9, 8,
		2, 8, 7,
		2, 7, 1,

		6, 7, 8,
		6, 8, 9,
		6, 9, 10,
		6, 10, 11,
	};

	return CreateMesh(name, vertices, indices);
}

Mesh *factory::createDigit0(glm::vec3 color)
{
	std::vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0.f, 0.2f, 0), color),
		VertexFormat(glm::vec3(0.2f, 0.f,0), color),
		VertexFormat(glm::vec3(0.4f, 0.f, 0), color),
		VertexFormat(glm::vec3(0.6f, 0.2f, 0), color),
		VertexFormat(glm::vec3(0.6f, 1.f, 0), color),
		VertexFormat(glm::vec3(0.4f, 1.2f, 0), color),
		VertexFormat(glm::vec3(0.2f, 1.2f, 0), color),
		VertexFormat(glm::vec3(0.f, 1.f, 0), color)
	};

	std::vector<unsigned int> indices
	{
		0, 1, 2, 3, 4, 5, 6, 7
	};

	Mesh *digit = new Mesh("0");
	digit->SetDrawMode(GL_LINE_LOOP);
	digit->InitFromData(vertices, indices);
	return digit;
}

Mesh *factory::createDigit1(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit2(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit3(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit4(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit5(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit6(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit7(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit8(glm::vec3 color)
{
	return NULL;
}

Mesh *factory::createDigit9(glm::vec3 color)
{
	return NULL;
}


static Mesh *CreateMesh(const std::string &name,
	const std::vector<VertexFormat> &vertices,
	const std::vector<unsigned int> &indices)
{
	unsigned int VAO = 0;
	// Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO = 0;
	// Create the VBO and bind it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(),
		&vertices[0], GL_STATIC_DRAW);

	unsigned int IBO = 0;
	// Create the IBO and bind it
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
		&indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section demonstrates how the GPU vertex shader program
	// receives data. It will be learned later, when GLSL shaders will be
	// introduced. For the moment, just think that each property value from
	// our vertex format needs to be sent to a certain channel, in order to
	// know how to receive it in the GLSL vertex shader.

	// Set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// Set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
		(void*)(sizeof(glm::vec3)));

	// Set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
		(void*)(2 * sizeof(glm::vec3)));

	// Set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
		(void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	if (GetOpenGLError() == GL_INVALID_OPERATION)
	{
		std::cout << "\t[NOTE] : For students : DON'T PANIC! This error should go "
			<< "away when completing the tasks." << std::endl;
		std::cout << "\t[NOTE] : For developers : This happens because OpenGL core "
			<< "spec >=3.1 forbids null VAOs." << std::endl;
	}

	// Mesh information is saved into a Mesh object
	Mesh *mesh = new Mesh(name);
	mesh->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	return mesh;
}
