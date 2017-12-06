#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtx/transform.hpp>
#include "Cube.h"

static const int MATRIX_SIZE = 3;
static const int CUBE_SIZE = 2;
static const float DELTA = 1.1;
static const int CUBE_ROTATE_ANGLE = 45;

glm::mat4 P;
glm::mat4 ***cubes;
glm::mat4 ***rotatesX;
glm::mat4 ***rotatesY;
//Cube ****cubes;

int rotateWallAngle = 90;


void rotateCube(glm::mat4 ***rotates, float angle,  glm::vec3 direction) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			for (int k = 0; k < MATRIX_SIZE; k++)
			{
				rotates[i][j][k] = glm::rotate(rotates[i][j][k], angle, direction);
			}
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action != GLFW_PRESS && action != GLFW_REPEAT)
	{
		std::cout << "action was: " << action << std::endl;
		return;
	}
	std::cout << "key_callback with: key=" << key << " scancode=" << scancode << " action=" << action << " mods=" << mods << std::endl;

	switch (key)
	{
	// Cube rotate:
	case GLFW_KEY_UP:
		rotateCube(rotatesX, CUBE_ROTATE_ANGLE, glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_DOWN:
		rotateCube(rotatesX, CUBE_ROTATE_ANGLE, glm::vec3(-1.0f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_RIGHT:
		rotateCube(rotatesY, CUBE_ROTATE_ANGLE, glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case GLFW_KEY_LEFT:
		rotateCube(rotatesY, CUBE_ROTATE_ANGLE, glm::vec3(0.0f, -1.0f, 0.0f));
		break;
	case GLFW_KEY_R:
		break;
	case GLFW_KEY_L:
		break;
	case GLFW_KEY_U:
		break;
	case GLFW_KEY_D:
		break;
	case GLFW_KEY_B:
		break;
	case GLFW_KEY_F:
		break;
	case GLFW_KEY_SPACE:
		break;
	case GLFW_KEY_Z:
		break;
	case GLFW_KEY_A:
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:
		break;
	}

}




