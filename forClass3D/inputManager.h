#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtx/transform.hpp>
#include "Cube.h"

static const int MATRIX_SIZE = 3;
static const int CUBE_SIZE = 2;
static const float DELTA = 1.1;
static const int CUBE_ROTATE_ANGLE = 45;
int WALL_ROTATE_ANGLE = 90;

glm::mat4 P;
glm::mat4 ***cubes;
glm::vec3 ***angles;
//Cube ****cubes;



void rotateCube(int direction, int angle) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			for (int k = 0; k < MATRIX_SIZE; k++)
			{
				angles[i][j][k][direction] = (int(angles[i][j][k][direction]) + angle) % 360;
			}
		}
	}
}

void rotateWallX(int wallIndex, int direction, int angle) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[wallIndex][i][j][direction] = (int(angles[wallIndex][i][j][direction]) + angle) % 360;
		}
	}
}

void rotateWallY(int wallIndex, int direction, int angle) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[i][wallIndex][j][direction] = (int(angles[i][wallIndex][j][direction]) + angle) % 360;
		}
	}
}

void rotateWallZ(int wallIndex, int direction, int angle) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[i][j][wallIndex][direction] = (int(angles[i][j][wallIndex][direction]) + angle) % 360;
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
		rotateCube(0, CUBE_ROTATE_ANGLE);
		break;
	case GLFW_KEY_DOWN:
		rotateCube(0, -CUBE_ROTATE_ANGLE);
		break;
	case GLFW_KEY_RIGHT:
		rotateCube(1, CUBE_ROTATE_ANGLE);
		break;
	case GLFW_KEY_LEFT:
		rotateCube(1, -CUBE_ROTATE_ANGLE);
		break;
	case GLFW_KEY_R:
		rotateWallX(2, 0, WALL_ROTATE_ANGLE);
		break;
	case GLFW_KEY_L:
		rotateWallX(0, 0, WALL_ROTATE_ANGLE);
		break;
	case GLFW_KEY_U:
		rotateWallY(2, 1, WALL_ROTATE_ANGLE);
		break;
	case GLFW_KEY_D:
		rotateWallY(0, 1, WALL_ROTATE_ANGLE);
		break;
	case GLFW_KEY_B:
		rotateWallZ(2, 2, WALL_ROTATE_ANGLE);
		break;
	case GLFW_KEY_F:
		rotateWallZ(0, 2, WALL_ROTATE_ANGLE);
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




