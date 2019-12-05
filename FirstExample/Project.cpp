﻿//***************************************************************************
// Project.cpp by Jang Doosung, Junhyuk Kang, Junho Kim (C) 2018 All Rights Reserved.
//
// Project submission.
//
// Description:
//	Building Castle with primitives, texture, and light.
//  
//*****************************************************************************

using namespace std;
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "SoilLib/SOIL.h"
#include <vector>
#include "Object.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "Cube.h"

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint uniformModel = 0;
GLuint uniformView = 0;
GLuint uniformProj = 0;
GLuint uniformLightPos = 0;
GLuint uniformLightPos2 = 0;
GLuint uniformEyePos = 0;

GLuint iLeather_tex = 0;
GLuint iFence_tex = 0;
GLuint iWindow_tex = 0;

//float rotAngle = 0.0f;

// Horizontal and vertical ortho offsets.
//float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f;

int deltaTime, currentTime, lastTime = 0;
glm::mat4 view, projection;

int WindowWidth = 800;
int WindowHeight = 600;

glm::vec3 CameraPosition = glm::vec3(0.f, 0.f, 10);
float fCameraSpeed = 0.5f;


vector<Object*> vecObjects;



GLfloat pyramid_vertices[] =
{
	//Front
	0.f, 2.f, 0.f,		0.5, 0,		0, 0, 0,		//0
	-1.f, 0.f, 1.f,		0, 1,		0, 0, 0,
	1.f, 0.f, 1.f,		1, 1,		0, 0, 0,

	//right
	0.f, 2.f, 0.f,		0.5, 0,		0, 0, 0,		//3
	1.f, 0.f, 1.f,		0, 1,		0, 0, 0,
	1.f, 0.f, -1.f,		1, 1,		0, 0, 0,

	//back
	0.f, 2.f, 0.f,		0.5, 0,		0, 0, 0,		//6
	1.f, 0.f, -1.f,		0, 1,		0, 0, 0,
	-1.f, 0.f, -1.f,	1, 1,		0, 0, 0,

	//left
	0.f, 2.f, 0.f,		0.5, 0,		0, 0, 0,		//9
	-1.f, 0.f, -1.f,	0, 1,		0, 0, 0,
	-1.f, 0.f, 1.f,		1, 1,		0, 0, 0,

	//bottom
	-1.f, 0.f, 1.f,		0, 0,		0, 0, 0,		//12
	-1.f, 0.f, -1.f,	0, 1,		0, 0, 0,
	1.f, 0.f, -1.f,		1, 1,		0, 0, 0,
	1.f, 0.f, 1.f,		1, 0,		0, 0, 0
};

GLshort pyramid_indices[] =
{
	//Front
	0, 1, 2,

	//RIght
	3, 4, 5,

	//Back
	6, 7, 8,

	//left
	9, 10, 11,

	//bottom
	12, 13 ,14,
	12, 14, 15
};

bool bWireFrameMode = false;


struct Light
{
	GLfloat ambientStrength;

	glm::vec3 diffuseColor;
	GLfloat diffuseStrength;

	GLfloat specularStrength;
	GLfloat shininess;

	Light(GLfloat aStr, glm::vec3 dCol, GLfloat dStr, GLfloat sStr, GLfloat shin)
	{
		ambientStrength = aStr;

		diffuseColor = dCol;
		diffuseStrength = dStr;

		specularStrength = sStr;
		shininess = shin;
	}

};

struct PointLight : public Light
{
	glm::vec3 position;
	GLfloat constant, linear, exponent;	//Quadratic equation for attenuation

	PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat aStr, glm::vec3 dCol, GLfloat dStr, GLfloat sStr, GLfloat shin)
		:Light(aStr, dCol, dStr, sStr, shin)
	{
		position = pos;

		constant = con;
		linear = lin;
		exponent = exp;
	}
};

//50 range
PointLight pLight(glm::vec3(2.f, 2.f, -2.f), 1.f, 0.09, 0.032f,
				  0.2f, glm::vec3(1.f, 1.f, 1.f), 1.f, 1.f, 32.f);

PointLight pLight2(glm::vec3(-2.f, -2.f, 2.f), 1.f, 0.09, 0.032f,
				  0.2f, glm::vec3(1.f, 1.f, 1.f), 1.f, 1.f, 32.f);





void init(void)
{
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};
	
	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	uniformModel = glGetUniformLocation(program, "model");
	uniformView = glGetUniformLocation(program, "view");
	uniformProj = glGetUniformLocation(program, "projection");

	// Perspective arameters : Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	// Ortho parameters: left, right, bottom, top, nearVal, farVal
	//projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // In world coordinates
	projection = glm::perspective(glm::radians(45.f), (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.f);
	
	// Camera matrix
	view = glm::lookAt
	(
		CameraPosition,		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);



	//Set light1
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.ambientStrength"), pLight.ambientStrength);
	glUniform3f(glGetUniformLocation(program, "pLight[0].base.diffuseColor"), pLight.diffuseColor.x, pLight.diffuseColor.y, pLight.diffuseColor.z);
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.diffuseStrength"), pLight.diffuseStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.specularStrength"), pLight.specularStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.shininess"), pLight.shininess);

	glUniform3f(glGetUniformLocation(program, "pLight[0].position"), pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight[0].constant"), pLight.constant);
	glUniform1f(glGetUniformLocation(program, "pLight[0].linear"), pLight.linear);
	glUniform1f(glGetUniformLocation(program, "pLight[0].exponent"), pLight.exponent);

	uniformLightPos = glGetUniformLocation(program, "pLight[0].position");

	//Set light2
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.ambientStrength"), pLight2.ambientStrength);
	glUniform3f(glGetUniformLocation(program, "pLight[1].base.diffuseColor"), pLight2.diffuseColor.x, pLight2.diffuseColor.y, pLight2.diffuseColor.z);
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.diffuseStrength"), pLight2.diffuseStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.specularStrength"), pLight2.specularStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.shininess"), pLight2.shininess);

	glUniform3f(glGetUniformLocation(program, "pLight[1].position"), pLight2.position.x, pLight2.position.y, pLight2.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight[1].constant"), pLight2.constant);
	glUniform1f(glGetUniformLocation(program, "pLight[1].linear"), pLight2.linear);
	glUniform1f(glGetUniformLocation(program, "pLight[1].exponent"), pLight2.exponent);

	uniformLightPos2 = glGetUniformLocation(program, "pLight[1].position");



	uniformEyePos = glGetUniformLocation(program, "eyePos");


	//Create All meshes
	GeometryGenerator::GenerateMeshes();

	//Create Objects
	Object* pObject1 = new Object(uniformModel);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetPosition(4, 0.f, 0.f);
	vecObjects.push_back(pObject1);


	//////////////////////////////////Create Pyramid////////////////////////////////////////////////
	//iNumOfPyramidIndices = sizeof(pyramid_indices) / sizeof(GLshort);
	//iNumOfPyramidVertices = sizeof(pyramid_vertices) / (sizeof(GLfloat) * iVertexLength);
	//calcAverageNormals(pyramid_indices, iNumOfPyramidIndices, pyramid_vertices, iNumOfPyramidVertices, iVertexLength, 5);


	//glGenVertexArrays(1, &pyramid_vao);
	//glBindVertexArray(pyramid_vao);

	//	glGenBuffers(1, &pyramid_ibo);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_ibo);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

	//	glGenBuffers(1, &pyramid_points_vbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, pyramid_points_vbo);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pyramid_vertices[0])* iVertexLength, 0);
	//	glEnableVertexAttribArray(0);

	//	//tell location1 is for tex coordinate
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(pyramid_vertices[0])* iVertexLength, (void*)(sizeof(pyramid_vertices[0]) * 3));
	//	glEnableVertexAttribArray(1);

	//	//tell location2 is for normal
	//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(pyramid_vertices[0])* iVertexLength, (void*)(sizeof(pyramid_vertices[0]) * 5));
	//	glEnableVertexAttribArray(2);
	////////////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////Create Texture////////////////////////////////////////////////
	GLint width, height;
	unsigned char* image = SOIL_load_image("Leather.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
	{
		printf("Error: image not found\n");
	}
	//glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &iLeather_tex);
	glBindTexture(GL_TEXTURE_2D, iLeather_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);





	image = SOIL_load_image("Fence.png", &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == nullptr)
	{
		printf("Error: image not found\n");
	}
	glGenTextures(1, &iFence_tex);
	glBindTexture(GL_TEXTURE_2D, iFence_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	int a = 0;

	image = SOIL_load_image("Window.png", &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == nullptr)
	{
		printf("Error: image not found\n");
	}
	glGenTextures(1, &iWindow_tex);
	glBindTexture(GL_TEXTURE_2D, iWindow_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);


	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	////////////////////////////////////////////////////////////////////////////////////////////////




	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	//Set blend option
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Antialiasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
}

//---------------------------------------------------------------------
//
// display
//

void display(void)
{
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	//Camera Update
	view = glm::lookAt
	(
		CameraPosition,		// Camera pos in World Space
		glm::vec3(0, 0, 0),		// and looks at the origin
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);

	for (int i = 0; i < vecObjects.size(); ++i)
		vecObjects[i]->Update();
	
	//Set projection and view matrix in shader
	glUniformMatrix4fv(uniformProj, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, &view[0][0]);
	

	//Set light position
	glUniform3f(uniformLightPos, pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform3f(uniformLightPos2, pLight2.position.x, pLight2.position.y, pLight2.position.z);

	//Set eye position
	glUniform3f(uniformEyePos, CameraPosition.x, CameraPosition.y, CameraPosition.z);




	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	



	//Draw Plane
	glBindTexture(GL_TEXTURE_2D, iLeather_tex);
	static float fAngle = 0.f;
	//fAngle += 1.f;
	//if (fAngle >= 360.f)
	//	fAngle -= 360.f;
	for(int i =0 ; i < vecObjects.size(); ++i)
		vecObjects[i]->Render();	
	

	/*glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, iFence_tex);
	glBindVertexArray(vao);
	transformObject(glm::vec3(1.2f, 1.2f, 1.2f), Y_AXIS, fAngle, glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, iNumOfCubeIndices, GL_UNSIGNED_SHORT, 0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);


	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, iWindow_tex);
	glBindVertexArray(pyramid_vao);
	transformObject(glm::vec3(1.f, 1.f, 1.f), Y_AXIS, 0, glm::vec3(0.0f, -1.0f, 3.0f));
	glDrawElements(GL_TRIANGLES, iNumOfPyramidIndices, GL_UNSIGNED_SHORT, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);*/



	glutSwapBuffers(); // Instead of double buffering.
}

void idle()
{
	
	//glutPostRedisplay();
}

void timer(int id)
{ 
	glutPostRedisplay();
	glutTimerFunc(33, timer, 0); 
}

void keyDown(unsigned char key, int x, int y)
{
	// Orthographic.
	switch(key)
	{
		case 'w':
			CameraPosition.z -= fCameraSpeed;
			//osV -= scrollSpd;
			break;
		case 's':
			CameraPosition.z += fCameraSpeed;
			//osV += scrollSpd;
			break;
		case 'a':
			CameraPosition.x -= fCameraSpeed;
			//osH += scrollSpd;
			break;
		case 'd':
			CameraPosition.x += fCameraSpeed;
			//osH -= scrollSpd;
			break;
		case 'r':
			CameraPosition.y += fCameraSpeed;
			break;
		case 'f':
			CameraPosition.y -= fCameraSpeed;
			break;

		case 'i':
			pLight.position.z -= fCameraSpeed;
			break;
		case 'k':
			pLight.position.z += fCameraSpeed;
			break;
		case 'j':
			pLight.position.x -= fCameraSpeed;
			break;
		case 'l':
			pLight.position.x += fCameraSpeed;
			break;
		case 'p':
			pLight.position.y += fCameraSpeed;
			break;
		case ';':
			pLight.position.y -= fCameraSpeed;
			break;




		case 'q':
			bWireFrameMode ? bWireFrameMode = false : bWireFrameMode = true;
			if (bWireFrameMode)
			{
				//Wireframe mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	// Empty for now.
}

void mouseMove(int x, int y)
{
	//cout << "Mouse pos: " << x << "," << y << endl;
}

void mouseDown(int btn, int state, int x, int y)
{
	cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;
}

void clean()
{
	glDeleteTextures(1, &iLeather_tex);
	glDeleteTextures(1, &iFence_tex);
	glDeleteTextures(1, &iWindow_tex);

	//Delete objects
	for (int i = 0; i < vecObjects.size(); ++i)
	{
		vecObjects[i]->Clear();
		delete vecObjects[i];
	}

	//Delete meshes
	GeometryGenerator::DestroyMeshes();
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("Jang, Doosung, 101175013");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();

	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseDown);
	glutPassiveMotionFunc(mouseMove); // or...
	//glutMotionFunc(mouseMove); // Requires click to register.

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);



	glutMainLoop();


	clean();

}
