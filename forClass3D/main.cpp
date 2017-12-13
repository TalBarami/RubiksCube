#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <thread>

using namespace glm;

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 800;

int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 0.5, 0.5)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 0.5, 0.5)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 0.5, 0.5)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 0.5 , 0.5)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(0.5, 0.5, 0.5)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(0.5, 0.5, 0.5)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(0.5, 0.5, 0.5)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(0.5, 0.5, 0.5))
	};

	unsigned int vIndices[] = {0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };


    Mesh cubeMesh(vertices, sizeof(vertices)/sizeof(vertices[0]), vIndices, sizeof(vIndices)/sizeof(vIndices[0]));
	//Mesh cube("./res/meshes/testBoxNoUV.obj");
	Shader shader("./res/shaders/basicShader");
	
	auto pos = -vec3(0, 0, 3 * MATRIX_SIZE * CUBE_SIZE);
	auto forward = glm::vec3(0.0f, 0.0f, 1.0f);
	auto up = glm::vec3(0.0f, 1.0f, 0.0f);
	P = glm::perspective(60.0f, float(DISPLAY_WIDTH) / float(DISPLAY_HEIGHT), 0.1f, 100.0f);
	P = P * glm::lookAt(pos, pos + forward, up);

	scene = Scene();
	translates = new mat4**[MATRIX_SIZE];
	rotates = new mat4**[MATRIX_SIZE];
	rotates_anim = new mat4**[MATRIX_SIZE];
	indices = new vec3**[MATRIX_SIZE];
	interpolations = new float**[MATRIX_SIZE];
	
	for (auto i = 0; i < MATRIX_SIZE; i++)
	{
		translates[i] = new mat4*[MATRIX_SIZE];
		rotates[i] = new mat4*[MATRIX_SIZE];
		rotates_anim[i] = new mat4*[MATRIX_SIZE];
		indices[i] = new vec3*[MATRIX_SIZE];
		interpolations[i] = new float*[MATRIX_SIZE];

		for (auto j = 0; j < MATRIX_SIZE; j++)
		{
			translates[i][j] = new mat4[MATRIX_SIZE];
			rotates[i][j] = new mat4[MATRIX_SIZE];
			rotates_anim[i][j] = new mat4[MATRIX_SIZE];
			indices[i][j] = new vec3[MATRIX_SIZE];
			interpolations[i][j] = new float[MATRIX_SIZE];

			for (auto k = 0; k < MATRIX_SIZE; k++)
			{
				float relative;
				if (MATRIX_SIZE % 2 == 0) {
					relative = (MATRIX_SIZE - 1.0f) / 2.0f;
				}
				else {
					relative = int(MATRIX_SIZE / 2);
				}
				translates[i][j][k] = glm::translate(mat4(1),
					(vec3(float(i), float(j), float(k)) - vec3(relative)) * float(CUBE_SIZE) * DELTA);
				rotates[i][j][k] = mat4(1);
				rotates_anim[i][j][k] = mat4(1);
				interpolations[i][j][k] = 1;
				indices[i][j][k] = vec3(i, j, k);
			}
		}
	}

	angles = new vec3*[3];
	for (auto i = 0; i < 3; i++) {
		angles[i] = new vec3[MATRIX_SIZE];
		for (auto j = 0; j < MATRIX_SIZE; j++) {
			angles[i][j] = vec3(0);
		}
	}

	glm::mat4 M, MVP;
	int x, y, z;

	scene.rotateCubeX(-1);
	scene.rotateCubeY(-1);

	glfwSetKeyCallback(display.m_window, key_callback);
	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(3);
		shader.Bind();
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);

		auto localRotateX = scene.animateX();
		auto localRotateY = scene.animateY();

		for (auto i = 0; i < MATRIX_SIZE; i++)
		{
			for (auto j = 0; j < MATRIX_SIZE; j++)
			{
				for (auto k = 0; k < MATRIX_SIZE; k++)
				{
					x = indices[i][j][k].x;
					y = indices[i][j][k].y;
					z = indices[i][j][k].z;
					
					auto translate = translates[x][y][z];
					auto rotate = glm::interpolate(rotates_anim[x][y][z], rotates[x][y][z], interpolations[x][y][z]);
					if (interpolations[x][y][z] < 1) {
						interpolations[x][y][z] += 0.05;
					}
					else {
						interpolations[x][y][z] = 1;
					}

					M = localRotateX * localRotateY * rotate * translate;
					MVP = P * M;
					shader.Update(MVP, M);
					cubeMesh.Draw();
				}
			}
		}

		display.SwapBuffers();
		glfwPollEvents();

		if (isMixerReady)
		{
			startMix();
		}

		if(isSolverReady)
		{
			startSolve();
		}
	}

	return 0;
}

void rubiksCube()
{
	
}



