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
#include <glm/gtc/type_ptr.hpp>
#include "SoilLib/SOIL.h"
#include <vector>
#include "Camera.h"
#include "TextureManager.h"
#include "Object.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "Cube.h"
#include "Pyramid.h"

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

//float rotAngle = 0.0f;

// Horizontal and vertical ortho offsets.
//float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f;

int deltaTime, currentTime, lastTime = 0;
glm::mat4 projection;

int WindowWidth = 800;
int WindowHeight = 600;


vector<Object*> vecObjects;

bool bWireFrameMode = false;

Camera camera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 1.f, 0.f), 0.f, 0.f, 7.f, 0.5f);


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

	//Set Projection Matrix
	projection = glm::perspective(glm::radians(45.f), (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.f);
	

	////////////////////////////Get uniform location////////////////////////////////////////////
	uniformModel = glGetUniformLocation(program, "model");
	uniformView = glGetUniformLocation(program, "view");
	uniformProj = glGetUniformLocation(program, "projection");

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

	//Get Eye position location
	uniformEyePos = glGetUniformLocation(program, "eyePos");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////Create Meshes//////////////////////////////////////////
	//Create All meshes
	GeometryGenerator::GenerateMeshes();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////Create Textures//////////////////////////////////////////
	TextureManager::CreateTexture("Leather.jpg", "Leather", SOIL_LOAD_RGB);
	TextureManager::CreateTexture("Fence.png", "Fence", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("Window.png", "Window", SOIL_LOAD_RGBA);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//Create Objects
	Object* pObject1 = new Object(uniformModel);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(4.f, 0.f, 0.f);
	vecObjects.push_back(pObject1);

	Object* pObject2 = new Object(uniformModel);
	pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMID));
	pObject2->SetTexture(TextureManager::GetTexture("Leather"));
	pObject2->SetPosition(4.f, 1.f, 0.f);
	vecObjects.push_back(pObject2);

	Object* pObject3 = new Object(uniformModel);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject3->SetTexture(TextureManager::GetTexture("Leather"));
	pObject3->SetPosition(0.f, 2.f, 0.f);
	vecObjects.push_back(pObject3);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	//Set blend option
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Antialiasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
}

void display(void)
{
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	//Update objects
	for (int i = 0; i < vecObjects.size(); ++i)
		vecObjects[i]->Update();
	
	//Set projection and view matrix in shader
	glUniformMatrix4fv(uniformProj, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

	//Set light position
	glUniform3f(uniformLightPos, pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform3f(uniformLightPos2, pLight2.position.x, pLight2.position.y, pLight2.position.z);

	//Set eye position
	glm::vec3 cameraPos = camera.GetCameraPosition();
	glUniform3f(uniformEyePos, cameraPos.x, cameraPos.y, cameraPos.z);





	////////////////////////////////////Start Draw////////////////////////////////////////////
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	//Camera Keyboard Control
	camera.KeyControl(key, deltaTime / 1000.f);




	switch(key)
	{
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
	if (camera.GetMouseFirstMoved())
	{
		camera.SetLastMouseX(x);
		camera.SetLastMouseY(y);
		camera.SetMouseFirstMoved(false);
		return;
	}

	camera.MouseControl(camera.GetLastMouseX() - x, camera.GetLastMouseY() - y);

	camera.SetLastMouseX(x);
	camera.SetLastMouseY(y);

	//if (x != WindowWidth * 0.5 || y != WindowHeight * 0.5)
	//{
	//	glutWarpPointer(WindowWidth * 0.5, WindowHeight * 0.5);
	//}
}

void mouseDown(int btn, int state, int x, int y)
{
	cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;
}

void clean()
{
	//Delete objects
	for (int i = 0; i < vecObjects.size(); ++i)
	{
		vecObjects[i]->Clear();
		delete vecObjects[i];
	}


	//Delete texture
	TextureManager::DestroyTexture();

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
