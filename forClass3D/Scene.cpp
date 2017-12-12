#include "Scene.h"



Scene::Scene()
{
	sceneRotateInterpolationX = 1;
	sceneRotateInterpolationY = 1;
	sceneRotateX = glm::mat4(1);
	sceneRotateX_anim = glm::mat4(1);
	sceneRotateY = glm::mat4(1);
	sceneRotateY_anim = glm::mat4(1);
}

void Scene::rotateCubeX(int direction) {
	if (sceneRotateInterpolationX < 1) {
		return;
	}

	glm::mat4 temp = glm::rotate(float(direction * CUBE_ROTATE_ANGLE), glm::vec3(1.0f, 0.0f, 0.0f));
	sceneRotateX_anim = sceneRotateX;
	sceneRotateX = temp * sceneRotateX;
	sceneRotateInterpolationX = 0;
}

void Scene::rotateCubeY(int direction) {
	if (sceneRotateInterpolationY < 1) {
		return;
	}

	glm::mat4 temp = glm::rotate(float(direction * CUBE_ROTATE_ANGLE), glm::vec3(0.0f, 1.0f, 0.0f));
	sceneRotateY_anim = sceneRotateY;
	sceneRotateY = temp * sceneRotateY;
	sceneRotateInterpolationY = 0;
}

glm::mat4 Scene::animateX()
{
	auto result = glm::interpolate(sceneRotateX_anim, sceneRotateX, sceneRotateInterpolationX);
	if (sceneRotateInterpolationX < 1) {
		sceneRotateInterpolationX += 0.05;
	} else
	{
		sceneRotateInterpolationX = 1;
	}

	return result;
}

glm::mat4 Scene::animateY()
{
	auto result = glm::interpolate(sceneRotateY_anim, sceneRotateY, sceneRotateInterpolationY);
	if (sceneRotateInterpolationY < 1) {
		sceneRotateInterpolationY += 0.05;
	}
	else
	{
		sceneRotateInterpolationY = 1;
	}

	return result;
}


Scene::~Scene()
{
}
