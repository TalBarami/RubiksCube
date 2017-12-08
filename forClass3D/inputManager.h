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
glm::vec3 cubeAngle;
//Cube ****cubes;
boolean DEBUG = false;


void rotateCube(int direction, int angle) {
	cubeAngle[direction] = (int(cubeAngle[direction]) + angle) % 360;
}

void rotateWallX(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[wallIndex][i][j][0] = (int(angles[wallIndex][i][j][0]) + WALL_ROTATE_ANGLE) % 360;
		}
	}
}

void rotateWallY(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[i][wallIndex][j][1] = (int(angles[i][wallIndex][j][1]) + WALL_ROTATE_ANGLE) % 360;
		}
	}
}

void rotateWallZ(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[i][j][wallIndex][2] = (int(angles[i][j][wallIndex][2]) + WALL_ROTATE_ANGLE) % 360;
		}
	}
}

void handleIndicesRotationX(int wallIndex, int direction) {

}

void debug(int wallIndex, int direction, int angle) {
	if (!DEBUG)
	{
		return;
	}
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[wallIndex][i][j][direction] = (int(angles[wallIndex][i][j][direction]) + angle) % 360;
			std::cout << "angle is: " << angles[wallIndex][i][j].x << " " << angles[wallIndex][i][j].y << " " << angles[wallIndex][i][j].z << std::endl;
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
		rotateWallX(2);
		break;
	case GLFW_KEY_L:
		rotateWallX(0);
		break;
	case GLFW_KEY_U:
		rotateWallY(2);
		break;
	case GLFW_KEY_D:
		rotateWallY(0);
		break;
	case GLFW_KEY_B:
		rotateWallZ(2);
		break;
	case GLFW_KEY_F:
		rotateWallZ(0);
		break;
	case GLFW_KEY_SPACE:
		break;
	case GLFW_KEY_Z:
		//WALL_ROTATE_ANGLE = min(1, WALL_ROTATE_ANGLE / 2);
		WALL_ROTATE_ANGLE = std::fmax(1, WALL_ROTATE_ANGLE / 2);
		break;
	case GLFW_KEY_A:
		WALL_ROTATE_ANGLE = std::fmin(180, WALL_ROTATE_ANGLE * 2);
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:
		break;
	}
	/*case GLFW_KEY_A:
	debug(0, 0, 5);
	break;
	case GLFW_KEY_S:
	debug(0, 1, 5);
	break;
	case GLFW_KEY_D:
	debug(0, 2, 5);
	break;
	case GLFW_KEY_Z:
	debug(0, 0, -5);
	break;
	case GLFW_KEY_X:
	debug(0, 1, -5);
	break;
	case GLFW_KEY_C:
	debug(0, 2, -5);
	break;
	case GLFW_KEY_1:
	DEBUG = true;
	break;*/
}




