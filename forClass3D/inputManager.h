#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include <thread>
#include <fstream>

using namespace glm;

static const int MATRIX_SIZE = 3;
static const int CUBE_SIZE = 2;
static const float DELTA = 1.1;
int WALL_ROTATE_INDEX = 0;
int WALL_ROTATE_DIRECTION = 1;
float WALL_ROTATE_ANGLE = 90;
bool KEYBOARD_ACTIVE = true;
bool RECORD = true;

struct Record
{
	int axis;
	int wall;
	int direction;
	float angle;

	Record(int axis, int wall, int direction, float angle) : axis(axis), wall(wall), direction(direction), angle(angle) {}
};

Scene scene;
mat4 P;

bool isMixerReady;
std::thread mixerThread;

bool isSolverReady;
std::thread solverThread;

std::vector<Record*> recorder;

vec3 ***indices;
mat4 ***translates;
mat4 ***rotates;
mat4 ***rotates_anim;
float ***interpolations;
vec3 **angles;

inline void handleIndicesX_CW(int wall) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wall][j][i];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[wall][i][j] = tmp[MATRIX_SIZE - i - 1][j];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesX_CCW(int wall) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[wall][MATRIX_SIZE - i - 1][j];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[wall][i][j] = tmp[j][i];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesY_CW(int wall) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[j][wall][i];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wall][j] = tmp[i][MATRIX_SIZE - j - 1];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesY_CCW(int wall) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[i][wall][MATRIX_SIZE - j - 1];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][wall][j] = tmp[j][i];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesZ_CW(int wall) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[j][i][wall];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wall] = tmp[MATRIX_SIZE - i - 1][j];
		}
	}
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		delete(tmp[i]);
	}
	delete[](tmp);
}

inline void handleIndicesZ_CCW(int wall) {
	auto **tmp = new vec3*[MATRIX_SIZE];
	for (auto i = 0; i < MATRIX_SIZE; i++) {
		tmp[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			tmp[i][j] = indices[MATRIX_SIZE - i - 1][j][wall];
		}
	}

	for (auto i = 0; i < MATRIX_SIZE; i++) {
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			indices[i][j][wall] = tmp[j][i];
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


inline bool onRotateWallX(int wall) {
	std::cout << "Rotate wall [" << wall << "][y][z]" << std::endl;
	if (!verifyWallsVertical(1, 2)) {
		return false;
	}

	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[wall][i][j].x, y = indices[wall][i][j].y, z = indices[wall][i][j].z;
			if (interpolations[x][y][z] < 1){
				std::cout << "Wall is currently moving." << std::endl;
				return false;
			}
		}
	}

	std::cout << "added " << (WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE) << " to " << angles[0][wall].x << std::endl;
	angles[0][wall].x += (WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE);
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[wall][i][j].x, y = indices[wall][i][j].y, z = indices[wall][i][j].z;
			auto temp = glm::rotate(float(WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE), vec3(1.0f, 0.0f, 0.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
		}
	}

	int handleTimes = angles[0][wall].x / 90;
	angles[0][wall].x -= handleTimes * 90;
	handleTimes = abs(handleTimes);
	std::cout << "handle times: " << handleTimes << " new angle: " << angles[0][wall].x << std::endl;
	for (auto i = 0; i < handleTimes; i++) {
		WALL_ROTATE_DIRECTION == 1 ? handleIndicesX_CW(wall) : handleIndicesX_CCW(wall);
	}

	if (RECORD)
	{
		auto r = new Record(0, wall, WALL_ROTATE_DIRECTION, WALL_ROTATE_ANGLE);
		recorder.push_back(r);
	}
	return true;
}

inline bool onRotateWallY(int wall) {
	std::cout << "Rotate wall [x][" << wall << "][z]" << std::endl;
	if (!verifyWallsVertical(0, 2)) {
		return false;
	}

	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wall][j].x, y = indices[i][wall][j].y, z = indices[i][wall][j].z;
			if (interpolations[x][y][z] < 1 ){
				std::cout << "Wall is currently moving." << std::endl;
				return false;
			}
		}
	}

	std::cout << "added " << (WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE) << " to " << angles[1][wall].y << std::endl;
	angles[1][wall].y += (WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE);
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][wall][j].x, y = indices[i][wall][j].y, z = indices[i][wall][j].z;
			auto temp = glm::rotate(float(WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 1.0f, 0.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
		}
	}

	int handleTimes = angles[1][wall].y / 90;
	angles[1][wall].y -= handleTimes * 90;
	handleTimes = abs(handleTimes);
	std::cout << "handle times: " << handleTimes << " new angle: " << angles[1][wall].y << std::endl;
	for (auto i = 0; i < handleTimes; i++) {
		WALL_ROTATE_DIRECTION == 1 ? handleIndicesY_CW(wall) : handleIndicesY_CCW(wall);
	}

	if (RECORD)
	{
		auto r = new Record(1, wall, WALL_ROTATE_DIRECTION, WALL_ROTATE_ANGLE);
		recorder.push_back(r);
	}
	return true;
}

inline bool onRotateWallZ(int wall) {
	std::cout << "Rotate wall [x][y][" << wall << "]"  << std::endl;
	if (!verifyWallsVertical(0, 1)) {
		return false;
	}

	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wall].x, y = indices[i][j][wall].y, z = indices[i][j][wall].z;
			if (interpolations[x][y][z] < 1){
				std::cout << "Wall is currently moving." << std::endl;
				return false;
			}
		}
	}
	std::cout << "added " << (WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE) << " to " << angles[2][wall].z << std::endl;
	angles[2][wall].z += (WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE);
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			int x = indices[i][j][wall].x, y = indices[i][j][wall].y, z = indices[i][j][wall].z;
			auto temp = glm::rotate(float(WALL_ROTATE_DIRECTION * WALL_ROTATE_ANGLE), vec3(0.0f, 0.0f, 1.0f));
			rotates_anim[x][y][z] = rotates[x][y][z];
			rotates[x][y][z] = temp * rotates[x][y][z];
			interpolations[x][y][z] = 0;
		}
	}

	int handleTimes = angles[2][wall].z / 90;
	angles[2][wall].z -= handleTimes * 90;
	handleTimes = abs(handleTimes);
	std::cout << "handle times: " << handleTimes << " new angle: " << angles[2][wall].z << std::endl;
	for (auto i = 0; i < handleTimes; i++) {
		WALL_ROTATE_DIRECTION == 1 ? handleIndicesZ_CW(wall) : handleIndicesZ_CCW(wall);
	}

	if(RECORD)
	{
		auto r = new Record(2, wall, WALL_ROTATE_DIRECTION, WALL_ROTATE_ANGLE);
		recorder.push_back(r);
	}
	return true;
}

inline void onFlipRotateDirectionClick()
{
	std::cout << "Flip rotate direction." << std::endl;
	WALL_ROTATE_DIRECTION *= -1;
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

inline void shuffleCube()
{
	std::cout << "Shuffle." << std::endl;
	std::srand(time(nullptr));
	int direction, wallIndex, action;
	auto totalSteps = rand() % (10 * MATRIX_SIZE) + 20;
	auto restBetweenSteps = 500;
	std::string record = "";
	while(WALL_ROTATE_ANGLE != 90)
	{
		WALL_ROTATE_ANGLE < 90 ? onAngleMultiplyClick() : onAngleDivisionClick();
	}
	for (auto i = 0; i < totalSteps; i++)
	{
		direction = std::rand() % 2;
		wallIndex = std::rand() % MATRIX_SIZE;
		action = std::rand() % 3;
		std::cout << "Rotate wall [" << action << "][" << wallIndex << "] dir=" << direction << std::endl;
		if (direction == 1)
		{
			onFlipRotateDirectionClick();
		}

		onRotateIndexClick(wallIndex);

		switch (action)
		{
		case 0:
			while (!onRotateWallX(wallIndex))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(restBetweenSteps));
			}
			break;
		case 1:
			while (!onRotateWallY(wallIndex))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(restBetweenSteps));
			}
			break;
		case 2:
			while (!onRotateWallZ(wallIndex))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(restBetweenSteps));
			}
			break;
		default:
			break;
		}
	}

	std::cout << "Total recorded: " << recorder.size() << ", Total steps: " << totalSteps << std::endl;
	int start = recorder.size() - 1;
	int end = recorder.size() - totalSteps;
	for (auto i = start; i >= end; i--)
	{
		std::cout << "Record number: " << i << std::endl;
		auto r = recorder.at(i);
		record += "AXIS:" + std::to_string(r->axis) +
			";  WALL:" + std::to_string(r->wall) +
			";  DIRECTION:" + std::to_string(r->direction) +
			";  ANGLE:" + std::to_string(r->angle) + "\n";
	}
	record += "TOTAL_STEPS=" + std::to_string(totalSteps) + "\n";
	std::cout << "Done mixing, recorded steps:" << std::endl;
	std::cout << record << std::endl;
	std::ofstream out("mixer_result.txt");
	out << record;
	out.close();
	KEYBOARD_ACTIVE = true;
}

inline void startMix()
{
	KEYBOARD_ACTIVE = false;
	isMixerReady = false;
	mixerThread = std::thread(&shuffleCube);
	mixerThread.detach();
}

inline void solveCube()
{
	std::cout << "Solve cube." << std::endl;
	auto restBetweenSteps = 500;
	auto totalSteps = 0;
	std::string record;
	while(!recorder.empty())
	{
		auto r = recorder.back();
		std::cout << "AXIS=" << r->axis << "  WALL=" << r->wall << "   ANGLE=" << r->angle << "   DIRECTION=" << r->direction;
		if(r->angle != WALL_ROTATE_ANGLE)
		{
			r->angle < WALL_ROTATE_ANGLE ? onAngleDivisionClick() : onAngleMultiplyClick();
		}

		if(r->direction == WALL_ROTATE_DIRECTION)
		{
			onFlipRotateDirectionClick();
		}

		if(r->wall != WALL_ROTATE_INDEX)
		{
			onRotateIndexClick(r->wall);
		}

		switch (r->axis)
		{
		case 0:
			while (!onRotateWallX(r->wall))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(restBetweenSteps));
			}
			break;
		case 1:
			while (!onRotateWallY(r->wall))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(restBetweenSteps));
			}
			break;
		case 2:
			while (!onRotateWallZ(r->wall))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(restBetweenSteps));
			}
			break;
		default:
			break;
		}
		record += "AXIS:" + std::to_string(r->axis) +
			";  WALL:" + std::to_string(r->wall) +
			";  DIRECTION:" + std::to_string(WALL_ROTATE_DIRECTION) +
			";  ANGLE:" + std::to_string(r->angle) + "\n";
		recorder.pop_back();
		totalSteps++;
	}
	record += "TOTAL_STEPS=" + std::to_string(totalSteps) + "\n";
	std::cout << "Done solving, recorded steps:" << std::endl;
	std::cout << record << std::endl;
	std::ofstream out("solver_result.txt");
	out << record;
	out.close();

	KEYBOARD_ACTIVE = true;
	RECORD = true;
}

inline void startSolve()
{
	KEYBOARD_ACTIVE = false;
	RECORD = false;
	isSolverReady = false;
	solverThread = std::thread(&solveCube);
	solverThread.detach();
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
		isMixerReady = true;
		break;
	case GLFW_KEY_S:
		isSolverReady = true;
		break;
	case GLFW_KEY_ESCAPE:
		std::cout << "Exit program." << std::endl;
		for (auto i = 0; i < MATRIX_SIZE; i++) {
			for (auto j = 0; j < MATRIX_SIZE; j++) {
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
		for (auto i = 0; i < 3; i++) {
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




