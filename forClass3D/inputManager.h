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
float WALL_ROTATE_ANGLE = 90;
int ROTATION_DIRECTION = 1;
boolean DEBUG = false;

mat4 P;
mat4 cubeRotateX;
mat4 cubeRotateX_anim;
float cubeRotateInterpolationX;
mat4 cubeRotateY;
mat4 cubeRotateY_anim;
float cubeRotateInterpolationY;

mat4 ***cubes;
mat4 ***translates;
mat4 ***rotates;
mat4 ***rotates_anim;
float ***interpolations;
vec3 ***indices;

vec3 ***angles;

void rotateCubeX(int direction) {
	if (cubeRotateInterpolationX < 1) {
		return;
	}

	mat4 temp = glm::rotate(float(direction * CUBE_ROTATE_ANGLE), vec3(1.0f, 0.0f, 0.0f));
	cubeRotateX_anim = cubeRotateX;
	cubeRotateX = temp * cubeRotateX;
	cubeRotateInterpolationX = 0;
}

void rotateCubeY(int direction) {
	if (cubeRotateInterpolationY < 1) {
		return;
	}

	mat4 temp = glm::rotate(float(direction * CUBE_ROTATE_ANGLE), vec3(0.0f, 1.0f, 0.0f));
	cubeRotateY_anim = cubeRotateY;
	cubeRotateY = temp * cubeRotateY;
	cubeRotateInterpolationY = 0;
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
			if (interpolations[x][y][z] < 1 || int(angles[x][y][z].y) % 90 != 0 || int(angles[x][y][z].z) % 90 != 0) {
				return;
			}
		}
	}

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[wallIndex][i][j].x, y = indices[wallIndex][i][j].y, z = indices[wallIndex][i][j].z;
			mat4 temp = glm::rotate(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(1.0f, 0.0f, 0.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
			
			angles[x][y][z] += vec3(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), 0.0, 0.0);
		}
	}
	int x = indices[wallIndex][0][0].x, y = indices[wallIndex][0][0].y, z = indices[wallIndex][0][0].z;
	int handleTimes = angles[x][y][z].x / 90;
	for (int i = 0; i < handleTimes; i++) {
		ROTATION_DIRECTION == 1 ? handleIndicesX_CW(wallIndex) : handleIndicesX_CCW(wallIndex);
	}
}

void rotateWallY(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wallIndex][j].x, y = indices[i][wallIndex][j].y, z = indices[i][wallIndex][j].z;
			if (interpolations[x][y][z] < 1 || int(angles[x][y][z].x) % 90 != 0 || int(angles[x][y][z].z) % 90 != 0) {
				return;
			}
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wallIndex][j].x, y = indices[i][wallIndex][j].y, z = indices[i][wallIndex][j].z;
			mat4 temp = glm::rotate(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 1.0f, 0.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
			angles[x][y][z] += vec3(0.0, float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), 0.0);
		}
	}
	int x = indices[0][wallIndex][0].x, y = indices[0][wallIndex][0].y, z = indices[0][wallIndex][0].z;
	int handleTimes = angles[x][y][z].y / 90;
	for (int i = 0; i < handleTimes; i++) {
		ROTATION_DIRECTION == 1 ? handleIndicesY_CW(wallIndex) : handleIndicesY_CCW(wallIndex);
	}
}

void rotateWallZ(int wallIndex) {
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wallIndex].x, y = indices[i][j][wallIndex].y, z = indices[i][j][wallIndex].z;
			if (interpolations[x][y][z] < 1 || int(angles[x][y][z].x) % 90 != 0 || int(angles[x][y][z].y) % 90 != 0) {
				return;
			}
		}
	}
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wallIndex].x, y = indices[i][j][wallIndex].y, z = indices[i][j][wallIndex].z;
			mat4 temp = glm::rotate(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 0.0f, 1.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
			angles[x][y][z] += vec3(0.0, 0.0, float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE));
		}
	}

	int x = indices[0][0][wallIndex].x, y = indices[0][0][wallIndex].y, z = indices[0][0][wallIndex].z;
	int handleTimes = angles[x][y][z].z / 90;
	for (int i = 0; i < handleTimes; i++) {
		ROTATION_DIRECTION == 1 ? handleIndicesZ_CW(wallIndex) : handleIndicesZ_CCW(wallIndex);
	}
}

void mixCube() {
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
	case GLFW_KEY_UP:
		rotateCubeX(1);
		break;
	case GLFW_KEY_DOWN:
		rotateCubeX(-1);
		break;
	case GLFW_KEY_RIGHT:
		rotateCubeY(1);
		break;
	case GLFW_KEY_LEFT:
		rotateCubeY(-1);
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
		ROTATION_DIRECTION *= -1;
		break;
	case GLFW_KEY_Z:
		WALL_ROTATE_ANGLE = std::fmax(1.40625, WALL_ROTATE_ANGLE / 2);
		break;
	case GLFW_KEY_A:
		WALL_ROTATE_ANGLE = std::fmin(180, WALL_ROTATE_ANGLE * 2);
		break;
	case GLFW_KEY_M:
		mixCube();
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:
		break;
	}
}




