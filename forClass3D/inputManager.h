#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"

using namespace glm;

static const int MATRIX_SIZE = 3;
static const int CUBE_SIZE = 2;
static const float DELTA = 1.1;
static const int CUBE_ROTATE_ANGLE = 45;
int WALL_ROTATE_ANGLE = 90;
int ROTATION_DIRECTION = 1;
boolean DEBUG = false;

mat4 P;
vec3 cubeAngle;

mat4 ***cubes;
mat4 ***translates;
mat4 ***rotates;
mat4 ***rotates_anim;
vec3 ***indices;

vec3 ***angles;


vec3 toMove = vec3(0);
bool asd = false;

void printPoint(vec3 p) {
	std::cout << "(" << p.x << "," << p.y << "," << p.z << ")    ";
}

void rotateCube(int direction, int angle) {
	cubeAngle[direction] = (int(cubeAngle[direction]) + angle) % 360;
}

void handleIndicesX_CW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wallIndex][j][i];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[wallIndex][i][j] = tmp[MATRIX_SIZE - i - 1][j];
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		free(tmp[i]);
	}
	free(tmp);
}

void handleIndicesX_CCW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wallIndex][MATRIX_SIZE - i - 1][j];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[wallIndex][i][j] = tmp[j][i];
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		free(tmp[i]);
	}
	free(tmp);
}

void handleIndicesY_CW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[j][wallIndex][i];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wallIndex][j] = tmp[i][MATRIX_SIZE - j - 1];
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		free(tmp[i]);
	}
	free(tmp);
}

void handleIndicesY_CCW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[i][wallIndex][MATRIX_SIZE - j - 1];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wallIndex][j] = tmp[j][i];
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		free(tmp[i]);
	}
	free(tmp);
}

void handleIndicesZ_CW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[j][i][wallIndex];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wallIndex] = tmp[MATRIX_SIZE - i - 1][j];
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		free(tmp[i]);
	}
	free(tmp);
}

void handleIndicesZ_CCW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[MATRIX_SIZE - i - 1][j][wallIndex];
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wallIndex] = tmp[j][i];
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++) {
		free(tmp[i]);
	}
	free(tmp);
}

void rotateWallX(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[wallIndex][i][j].x, y = indices[wallIndex][i][j].y, z = indices[wallIndex][i][j].z;
			rotates[x][y][z] = glm::rotate(rotates[x][y][z], float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(1.0f, 0.0f, 0.0f));
			angles[x][y][z] += vec3(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), 0.0, 0.0);
		}
	}
	ROTATION_DIRECTION == 1 ? handleIndicesX_CW(wallIndex) : handleIndicesX_CCW(wallIndex);
}

void rotateWallY(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wallIndex][j].x, y = indices[i][wallIndex][j].y, z = indices[i][wallIndex][j].z;
			rotates[x][y][z] = glm::rotate(rotates[x][y][z], float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 1.0f, 0.0f));
			angles[x][y][z] += vec3(0.0, float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), 0.0);
		}
	}
	ROTATION_DIRECTION == 1 ? handleIndicesY_CW(wallIndex) : handleIndicesY_CCW(wallIndex);
}

void rotateWallZ(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wallIndex].x, y = indices[i][j][wallIndex].y, z = indices[i][j][wallIndex].z;
			rotates[x][y][z] = glm::rotate(rotates[x][y][z], float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 0.0f, 1.0f));
			angles[x][y][z] += vec3(0.0, 0.0, float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE));
		}
	}
	ROTATION_DIRECTION == 1 ? handleIndicesZ_CW(wallIndex) : handleIndicesZ_CCW(wallIndex);
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
		printPoint(indices[0][0][0]);
		asd = !asd;
		break;
	case GLFW_KEY_W:
		toMove = indices[0][2][0];
		printPoint(indices[0][2][0]);
		asd = true;
		break;
	case GLFW_KEY_E:
		toMove = indices[2][0][0];
		printPoint(indices[2][0][0]);
		asd = true;
		break;
	case GLFW_KEY_R:
		toMove = indices[2][2][0];
		printPoint(indices[2][2][0]);
		asd = true;
		break;
	case GLFW_KEY_A:
		toMove = indices[0][0][2];
		printPoint(indices[0][0][2]);
		asd = true;
		break;
	case GLFW_KEY_S:
		toMove = indices[0][2][2];
		printPoint(indices[0][2][2]);
		asd = true;
		break;
	case GLFW_KEY_1:
		for (int i = MATRIX_SIZE - 1; i >= 0; i--) {
			for (int j = MATRIX_SIZE - 1; j >= 0; j--) {
				printPoint(indices[j][i][0]);
			}
			std::cout << std::endl;
		}
		break;
	case GLFW_KEY_2:
		for (int i = MATRIX_SIZE - 1; i >= 0; i--) {
			for (int j = MATRIX_SIZE - 1; j >= 0; j--) {
				printPoint(indices[j][i][1]);
			}
			std::cout << std::endl;
		}
		break;
	case GLFW_KEY_3:
		for (int i = MATRIX_SIZE - 1; i >= 0; i--) {
			for (int j = MATRIX_SIZE - 1; j >= 0; j--) {
				printPoint(indices[j][i][2]);
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
	/*case GLFW_KEY_Z:
		WALL_ROTATE_ANGLE = std::fmax(1, WALL_ROTATE_ANGLE / 2);
		break;
	case GLFW_KEY_A:
		WALL_ROTATE_ANGLE = std::fmin(180, WALL_ROTATE_ANGLE * 2);
		break;*/
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




