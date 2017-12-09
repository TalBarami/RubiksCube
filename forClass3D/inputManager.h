#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtx/transform.hpp>
#include "Cube.h"

static const int MATRIX_SIZE = 3;
static const int CUBE_SIZE = 2;
static const float DELTA = 1.1;
static const int CUBE_ROTATE_ANGLE = 45;
int WALL_ROTATE_ANGLE = 90;
int ROTATION_DIRECTION = 1;
boolean DEBUG = false;

glm::mat4 P;
glm::vec3 cubeAngle;

glm::mat4 ***cubes;
glm::vec3 ***indices;
glm::mat4 ***indices2;
glm::vec3 ***angles;

glm::vec3 toMove = glm::vec3(0);
bool asd = false;

void printPoint(glm::vec3 p) {
	std::cout << "(" << p.x << "," << p.y << "," << p.z << ")    ";
}

void rotateCube(int direction, int angle) {
	cubeAngle[direction] = (int(cubeAngle[direction]) + angle) % 360;
}

void handleIndicesX_CW(int wallIndex) {
	glm::vec3 **tmp = new glm::vec3*[MATRIX_SIZE];
	glm::vec3 **tmp2 = new glm::vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new glm::vec3[MATRIX_SIZE];
		tmp2[i] = new glm::vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wallIndex][i][j];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp2[i][j] = tmp[j][i];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[wallIndex][i][j] = tmp2[MATRIX_SIZE - i - 1][j];
		}
	}
}

void handleIndicesY_CW(int wallIndex) {
	glm::vec3 **tmp = new glm::vec3*[MATRIX_SIZE];
	glm::vec3 **tmp2 = new glm::vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new glm::vec3[MATRIX_SIZE];
		tmp2[i] = new glm::vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[i][wallIndex][j];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp2[i][j] = tmp[j][i];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wallIndex][j] = tmp2[i][MATRIX_SIZE - j - 1];
		}
	}
}

void handleIndicesZ_CW(int wallIndex) {
	glm::vec3 **tmp = new glm::vec3*[MATRIX_SIZE];
	glm::vec3 **tmp2 = new glm::vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new glm::vec3[MATRIX_SIZE];
		tmp2[i] = new glm::vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[i][j][wallIndex];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp2[i][j] = tmp[j][i];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wallIndex] = tmp2[MATRIX_SIZE - i - 1][j];
		}
	}
}

void rotateWallX(int wallIndex) {
	handleIndicesX_CW(wallIndex);
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[wallIndex][i][j][0] = (int(angles[wallIndex][i][j][0]) + (ROTATION_DIRECTION * WALL_ROTATE_ANGLE)) % 360;
		}
	}
}

void rotateWallY(int wallIndex) {
	handleIndicesY_CW(wallIndex);
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[i][wallIndex][j][1] = (int(angles[i][wallIndex][j][1]) + (ROTATION_DIRECTION * WALL_ROTATE_ANGLE)) % 360;
		}
	}
}

void rotateWallZ(int wallIndex) {
	handleIndicesZ_CW(wallIndex);
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			angles[i][j][wallIndex][2] = (int(angles[i][j][wallIndex][2]) + (ROTATION_DIRECTION * WALL_ROTATE_ANGLE)) % 360;
		}
	}
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
	case GLFW_KEY_BACKSPACE:
		asd = false;
		break;
	case GLFW_KEY_Q:
		toMove = indices[0][0][0];
		asd = true;
		break;
	case GLFW_KEY_W:
		toMove = indices[0][2][0];
		asd = true;
		break;
	case GLFW_KEY_E:
		toMove = indices[2][0][0];
		asd = true;
		break;
	case GLFW_KEY_R:
		toMove = indices[2][2][0];
		asd = true;
		break;
	case GLFW_KEY_1:
		for (int i = MATRIX_SIZE - 1; i >= 0; i--) {
			for (int j = MATRIX_SIZE - 1; j >= 0; j--) {
				printPoint(indices[i][j][0]);
			}
			std::cout << std::endl;
		}
		break;
	case GLFW_KEY_2:
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				printPoint(indices[i][j][1]);
			}
			std::cout << std::endl;
		}
		break;
	case GLFW_KEY_3:
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				printPoint(indices[i][j][2]);
			}
			std::cout << std::endl;
		}
		break;
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
	/*case GLFW_KEY_R:
		rotateWallX(2);
		break;*/
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
		ROTATION_DIRECTION *= -1;
		break;
	case GLFW_KEY_Z:
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




