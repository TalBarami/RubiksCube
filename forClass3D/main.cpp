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

	unsigned int indices[] = {0, 1, 2,
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


    Mesh cubeMesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
	//Mesh cube("./res/meshes/testBoxNoUV.obj");
	Shader shader("./res/shaders/basicShader");
	
	vec3 pos = -vec3(0, 0, 3 * MATRIX_SIZE * CUBE_SIZE);
	vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	P = glm::perspective(60.0f, float(DISPLAY_WIDTH) / float(DISPLAY_HEIGHT), 0.1f, 100.0f);
	//M = glm::rotate(45.0f, vec3(1, 1, 1));
	P = P * glm::lookAt(pos, pos + forward, up);

	
	// Cubes:
	cubes = new mat4**[MATRIX_SIZE];
	angles = new vec3**[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		cubes[i] = new mat4*[MATRIX_SIZE];
		angles[i] = new vec3*[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			cubes[i][j] = new mat4[MATRIX_SIZE];
			angles[i][j] = new vec3[MATRIX_SIZE];
			for (int k = 0; k < MATRIX_SIZE; k++)
			{
				cubes[i][j][k] = mat4(1);
				angles[i][j][k] = vec3(0);
			}
		}
	}

	/*cubes = new Cube***[MATRIX_SIZE];
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		cubes[i] = new Cube**[MATRIX_SIZE];
		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			cubes[i][j] = new Cube*[MATRIX_SIZE];
			for (int k = 0; k < MATRIX_SIZE; k++)
			{
				cubes[i][j][k] = new Cube(P, vec3(i, j, k));
			}
		}
	}*/

	glm::mat4 rotates, translates;
	glfwSetKeyCallback(display.m_window, key_callback);

	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(3);
		shader.Bind();
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		for (int i = 0; i < MATRIX_SIZE; i++)
		{
			for (int j = 0; j < MATRIX_SIZE; j++)
			{
				for (int k = 0; k < MATRIX_SIZE; k++)
				{
					translates = mat4(1);
					translates = glm::translate(translates,
						(vec3(float(i), float(j), float(k)) - vec3(MATRIX_SIZE / 2)) * float(CUBE_SIZE) * DELTA);

					rotates = mat4(1);
					rotates = glm::rotate(rotates, cubeAngle.x, vec3(1, 0, 0));
					rotates = glm::rotate(rotates, cubeAngle.y, vec3(0, 1, 0));
					rotates = glm::rotate(rotates, cubeAngle.z, vec3(0, 0, 1));
					rotates = glm::rotate(rotates, angles[i][j][k].x, vec3(1, 0, 0));
					rotates = glm::rotate(rotates, angles[i][j][k].y, vec3(0, 1, 0));
					rotates = glm::rotate(rotates, angles[i][j][k].z, vec3(0, 0, 1));

					cubes[i][j][k] = P * rotates;
					cubes[i][j][k] = glm::translate(cubes[i][j][k],
						(vec3(float(i), float(j), float(k)) - vec3(MATRIX_SIZE / 2)) * float(CUBE_SIZE) * DELTA);

					shader.Update(cubes[i][j][k], rotates);
					cubeMesh.Draw();
				}
			}
		}

		display.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}




