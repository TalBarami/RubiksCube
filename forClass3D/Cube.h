#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Cube
{
public:
	Cube(glm::mat4 lookAt, glm::vec3 position);
	Cube();
	~Cube();

	void rotateCube(float angle, glm::vec3 direction);

	glm::mat4 refresh();
	glm::mat4 normal() const;

private:
	glm::mat4 cubeState;
	glm::mat4 rotate;
	glm::mat4 P;

	glm::vec3 indices;
};

