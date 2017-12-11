#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

class Scene
{
public:
	static const int CUBE_ROTATE_ANGLE = 45;

	Scene();
	~Scene();
	
	void rotateCubeX(int direction);
	void rotateCubeY(int direction);
	glm::mat4 animateX();
	glm::mat4 animateY();

	glm::mat4 sceneRotateX;
	glm::mat4 sceneRotateX_anim;
	float sceneRotateInterpolationX;
	glm::mat4 sceneRotateY;
	glm::mat4 sceneRotateY_anim;
	float sceneRotateInterpolationY;
};

