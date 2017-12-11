#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"

using namespace glm;

static const int MATRIX_SIZE = 3;
static const int CUBE_SIZE = 2;
static const float DELTA = 1.1;
float WALL_ROTATE_ANGLE = 90;
int ROTATION_DIRECTION = 1;
int WALL_ROTATE_INDEX = 0;
int SLEEP_BETWEEN_STEPS = 0;

bool KEYBOARD_ACTIVE = true;
int REMAINING_STEPS = 0;

Scene scene;
mat4 P;


vec3 ***indices;
mat4 ***translates;
mat4 ***rotates;
mat4 ***rotates_anim;
float ***interpolations;
vec3 **angles;

inline void handleIndicesX_CW(int wallIndex) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wallIndex][j][i];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[wallIndex][i][j] = tmp[MATRIX_SIZE - i - 1][j];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesX_CCW(int wallIndex) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wallIndex][MATRIX_SIZE - i - 1][j];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[wallIndex][i][j] = tmp[j][i];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesY_CW(int wallIndex) {
	vec3 **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[j][wallIndex][i];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wallIndex][j] = tmp[i][MATRIX_SIZE - j - 1];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesY_CCW(int wallIndex) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[i][wallIndex][MATRIX_SIZE - j - 1];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wallIndex][j] = tmp[j][i];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesZ_CW(int wallIndex) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[j][i][wallIndex];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wallIndex] = tmp[MATRIX_SIZE - i - 1][j];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesZ_CCW(int wallIndex) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[MATRIX_SIZE - i - 1][j][wallIndex];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wallIndex] = tmp[j][i];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline bool verifyWallsVertical(int wall1, int wall2) {
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		if (int(angles[wall1][i][wall1]) % 90 != 0 || int(angles[wall1][i][wall2]) % 90 != 0 ||
			int(angles[wall2][i][wall2]) % 90 != 0 || int(angles[wall2][i][wall1]) % 90 != 0) {
			std::cout << "Walls are not 90deg to each other." << std::endl;
			return false;
		}
	}
	return true;
}


inline void onRotateWallX(int wallIndex) {
	std::cout << "Rotate wall [" << wallIndex << "][y][z]" << std::endl;
	if (!verifyWallsVertical(1, 2)) {
		return;
	}

	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[wallIndex][i][j].x, y = indices[wallIndex][i][j].y, z = indices[wallIndex][i][j].z;
			if (interpolations[x][y][z] < 1){
				return;
			}
		}
	}

	std::cout << "added " << (ROTATION_DIRECTION * WALL_ROTATE_ANGLE) << " to " << angles[0][wallIndex].x << std::endl;
	angles[0][wallIndex].x += (ROTATION_DIRECTION * WALL_ROTATE_ANGLE);
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[wallIndex][i][j].x, y = indices[wallIndex][i][j].y, z = indices[wallIndex][i][j].z;
			auto temp = glm::rotate(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(1.0f, 0.0f, 0.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
		}
	}

	int handleTimes = angles[0][wallIndex].x / 90;
	angles[0][wallIndex].x -= handleTimes * 90;
	handleTimes = abs(handleTimes);
	std::cout << "handle times: " << handleTimes << " new angle: " << angles[0][wallIndex].x << std::endl;
	for (auto i = 0; i < handleTimes; i++) {
		ROTATION_DIRECTION == 1 ? handleIndicesX_CW(wallIndex) : handleIndicesX_CCW(wallIndex);
	}
	
}

inline void onRotateWallY(int wallIndex) {
	std::cout << "Rotate wall [x][" << wallIndex << "][z]" << std::endl;
	if (!verifyWallsVertical(0, 2)) {
		return;
	}

	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wallIndex][j].x, y = indices[i][wallIndex][j].y, z = indices[i][wallIndex][j].z;
			if (interpolations[x][y][z] < 1 ){
				return;
			}
		}
	}

	std::cout << "added " << (ROTATION_DIRECTION * WALL_ROTATE_ANGLE) << " to " << angles[1][wallIndex].y << std::endl;
	angles[1][wallIndex].y += (ROTATION_DIRECTION * WALL_ROTATE_ANGLE);
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wallIndex][j].x, y = indices[i][wallIndex][j].y, z = indices[i][wallIndex][j].z;
			auto temp = glm::rotate(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 1.0f, 0.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
		}
	}

	int handleTimes = angles[1][wallIndex].y / 90;
	angles[1][wallIndex].y -= handleTimes * 90;
	handleTimes = abs(handleTimes);
	std::cout << "handle times: " << handleTimes << " new angle: " << angles[1][wallIndex].y << std::endl;
	for (auto i = 0; i < handleTimes; i++) {
		ROTATION_DIRECTION == 1 ? handleIndicesY_CW(wallIndex) : handleIndicesY_CCW(wallIndex);
	}
}

inline void onRotateWallZ(int wallIndex) {
	std::cout << "Rotate wall [x][y][" << wallIndex << "]"  << std::endl;
	if (!verifyWallsVertical(0, 1)) {
		return;
	}

	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wallIndex].x, y = indices[i][j][wallIndex].y, z = indices[i][j][wallIndex].z;
			if (interpolations[x][y][z] < 1){
				return;
			}
		}
	}
	std::cout << "added " << (ROTATION_DIRECTION * WALL_ROTATE_ANGLE) << " to " << angles[2][wallIndex].z << std::endl;
	angles[2][wallIndex].z += (ROTATION_DIRECTION * WALL_ROTATE_ANGLE);
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wallIndex].x, y = indices[i][j][wallIndex].y, z = indices[i][j][wallIndex].z;
			auto temp = glm::rotate(float(ROTATION_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 0.0f, 1.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
		}
	}

	int handleTimes = angles[2][wallIndex].z / 90;
	angles[2][wallIndex].z -= handleTimes * 90;
	handleTimes = abs(handleTimes);
	std::cout << "handle times: " << handleTimes << " new angle: " << angles[2][wallIndex].z << std::endl;
	for (auto i = 0; i < handleTimes; i++) {
		ROTATION_DIRECTION == 1 ? handleIndicesZ_CW(wallIndex) : handleIndicesZ_CCW(wallIndex);
	}
	
}

inline void onFlipRotateDirectionClick()
{
	std::cout << "Flip rotate direction." << std::endl;
	ROTATION_DIRECTION *= -1;
}

inline void onRotateIndexClick(int index) {
	if (MATRIX_SIZE > index) {
		std::cout << "Rotate index changed from " << WALL_ROTATE_INDEX << " to " << index << std::endl;
		WALL_ROTATE_INDEX = index;
	}
}

inline void onAngleDivisionClick()
{
	std::cout << "Divide rotation angle by 2." << std::endl;
	WALL_ROTATE_ANGLE = std::fmax(1.40625, WALL_ROTATE_ANGLE / 2);
	std::cout << "New rotation angle: " << WALL_ROTATE_ANGLE << std::endl;
}

inline void onAngleMultiplyClick()
{
	std::cout << "Multiply rotation angle by 2." << std::endl;
	WALL_ROTATE_ANGLE = std::fmin(180, WALL_ROTATE_ANGLE * 2);
	std::cout << "New rotation angle: " << WALL_ROTATE_ANGLE << std::endl;
}

inline void shuffleCube() {
	std::cout << "Shuffle." << std::endl;
	KEYBOARD_ACTIVE = false;
	REMAINING_STEPS = rand() % 41 + 10;
	SLEEP_BETWEEN_STEPS = 1;

	auto totalSteps = rand() % 41 + 10; // 10 - 50 random moves;
	int direction, wallIndex, action;

	for (auto i = 0; i < totalSteps; i++)
	{
		direction = rand() % 2;
		wallIndex = rand() % MATRIX_SIZE;
		action = rand() % 3;

		if(direction == 1)
		{
			onFlipRotateDirectionClick();
		}

		onRotateIndexClick(wallIndex);

		switch(action)
		{
		case 0:
			onRotateWallX(wallIndex);
			break;
		case 1:
			onRotateWallY(wallIndex);
			break;
		case 2:
			onRotateWallZ(wallIndex);
			break;
		default:
			break;
		}
		Sleep(1000);
	}
}

inline void shuffleStep()
{
	
}

inline void solveCube() {
	std::cout << "Solve cube." << std::endl;

}

inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(!KEYBOARD_ACTIVE || action != GLFW_PRESS && action != GLFW_REPEAT)
	{
		return;
	}
	std::cout << "key_callback with: key=" << key << " scancode=" << scancode << " action=" << action << " mods=" << mods << std::endl;
	switch (key)
	{
	case GLFW_KEY_1:
		onRotateIndexClick(0);
		break;
	case GLFW_KEY_2:
		onRotateIndexClick(1);
		break;
	case GLFW_KEY_3:
		onRotateIndexClick(2);
		break;
	case GLFW_KEY_4:
		onRotateIndexClick(3);
		break;
	case GLFW_KEY_5:
		onRotateIndexClick(4);
		break;
	case GLFW_KEY_6:
		onRotateIndexClick(5);
		break;
	case GLFW_KEY_7:
		onRotateIndexClick(6);
		break;
	case GLFW_KEY_8:
		onRotateIndexClick(7);
		break;
	case GLFW_KEY_9:
		onRotateIndexClick(8);
		break;
	case GLFW_KEY_UP:
		std::cout << "Rotate cube up." << std::endl;
		scene.rotateCubeX(1);
		break;
	case GLFW_KEY_DOWN:
		std::cout << "Rotate cube down." << std::endl;
		scene.rotateCubeX(-1);
		break;
	case GLFW_KEY_RIGHT:
		std::cout << "Rotate cube right." << std::endl;
		scene.rotateCubeY(1);
		break;
	case GLFW_KEY_LEFT:
		std::cout << "Rotate cube left." << std::endl;
		scene.rotateCubeY(-1);
		break;
	case GLFW_KEY_R:
		onRotateWallX(MATRIX_SIZE - WALL_ROTATE_INDEX - 1);
		break;
	case GLFW_KEY_L:
		onRotateWallX(WALL_ROTATE_INDEX);
		break;
	case GLFW_KEY_U:
		onRotateWallY(MATRIX_SIZE - WALL_ROTATE_INDEX - 1);
		break;
	case GLFW_KEY_D:
		onRotateWallY(WALL_ROTATE_INDEX);
		break;
	case GLFW_KEY_B:
		onRotateWallZ(MATRIX_SIZE - WALL_ROTATE_INDEX - 1);
		break;
	case GLFW_KEY_F:
		onRotateWallZ(WALL_ROTATE_INDEX);
		break;
	case GLFW_KEY_SPACE:
		onFlipRotateDirectionClick();
		break;
	case GLFW_KEY_Z:
		onAngleDivisionClick();
		break;
	case GLFW_KEY_A:
		onAngleMultiplyClick();
		break;
	case GLFW_KEY_M:
		shuffleCube();
		break;
	case GLFW_KEY_S:
		solveCube();
		break;
	case GLFW_KEY_ESCAPE:
		std::cout << "Exit program." << std::endl;
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				delete(translates[i][j]);
				delete(rotates[i][j]);
				delete(rotates_anim[i][j]);
				delete(indices[i][j]);
				delete(interpolations[i][j]);
			}
			delete(translates[i]);
			delete(rotates[i]);
			delete(rotates_anim[i]);
			delete(indices[i]);
			delete(interpolations[i]);
		}
		for (int i = 0; i < 3; i++) {
			delete(angles[i]);
		}
		delete(translates);
		delete(rotates);
		delete(rotates_anim);
		delete(indices);
		delete(interpolations);
		delete(angles);

		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:
		std::cout << "Unsupported key." << std::endl;
		break;
	}
}




