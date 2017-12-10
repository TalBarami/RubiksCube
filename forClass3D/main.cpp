#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Cube.h"

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
	
	vec3 pos = -vec3(0, 0, 3 * MATRIX_SIZE * CUBE_SIZE);
	vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	P = glm::perspective(60.0f, float(DISPLAY_WIDTH) / float(DISPLAY_HEIGHT), 0.1f, 100.0f);
	P = P * glm::lookAt(pos, pos + forward, up);

	// Cubes:
	cubes = new mat4**[MATRIX_SIZE];
	translates = new mat4**[MATRIX_SIZE];
	rotates = new mat4**[MATRIX_SIZE];
	indices = new vec3**[MATRIX_SIZE];

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		cubes[i] = new mat4*[MATRIX_SIZE];
		translates[i] = new mat4*[MATRIX_SIZE];
		rotates[i] = new mat4*[MATRIX_SIZE];
		indices[i] = new vec3*[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			cubes[i][j] = new mat4[MATRIX_SIZE];
			translates[i][j] = new mat4[MATRIX_SIZE];
			rotates[i][j] = new mat4[MATRIX_SIZE];
			indices[i][j] = new vec3[MATRIX_SIZE];
			for (int k = 0; k < MATRIX_SIZE; k++)
			{
				cubes[i][j][k] = mat4(1);
				translates[i][j][k] = glm::translate(mat4(1),
					(vec3(float(i), float(j), float(k)) - vec3(MATRIX_SIZE / 2)) * float(CUBE_SIZE) * DELTA);
				rotates[i][j][k] = mat4(1);
				indices[i][j][k] = vec3(i, j, k);
			}
		}
	}

	glm::mat4 cubeRotate, rotate, translate;
	glfwSetKeyCallback(display.m_window, key_callback);
	int x, y, z;
	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(3);
		shader.Bind();
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);

		cubeRotate = mat4(1);
		cubeRotate = glm::rotate(cubeRotate, cubeAngle.x, vec3(1, 0, 0));
		cubeRotate = glm::rotate(cubeRotate, cubeAngle.y, vec3(0, 1, 0));
		cubeRotate = glm::rotate(cubeRotate, cubeAngle.z, vec3(0, 0, 1));

		for (int i = 0; i < MATRIX_SIZE; i++)
		{
			for (int j = 0; j < MATRIX_SIZE; j++)
			{
				for (int k = 0; k < MATRIX_SIZE; k++)
				{
					x = indices[i][j][k].x;
					y = indices[i][j][k].y;
					z = indices[i][j][k].z;
					
					if (asd & x == toMove.x && y == toMove.y && z == toMove.z) {
						translate = glm::translate(mat4(1), vec3(0, 0, 0));
					}
					else {
						translate = translates[x][y][z];
					}

					rotate = rotates[x][y][z];

					cubes[x][y][z] = P * cubeRotate * rotate * translate;

					shader.Update(cubes[x][y][z], cubeRotate * rotate * translate);
					cubeMesh.Draw();
				}
			}
		}

		display.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}




