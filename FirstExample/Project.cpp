//***************************************************************************
// Project.cpp by Doosung Jang, Junhyuk Kang, Junho Kim (C) 2018 All Rights Reserved.
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
#include "Fence.h"

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

GLuint uniformModel = 0;
GLuint uniformView = 0;
GLuint uniformProj = 0;
GLuint uniformEyePos = 0;
GLuint uniformShininess = 0;


int deltaTime, currentTime, lastTime = 0;
glm::mat4 projection;

int WindowWidth = 800;
int WindowHeight = 600;


vector<Object*> vecObjects;

bool bWireFrameMode = false;

Camera camera(glm::vec3(50.f, 10.f, 200.f), glm::vec3(0.f, 1.f, 0.f), 0.f, 0.f, 40.f, 0.5f);

float WatchTowerHeight = 26.5f;

float WallThickness = 4.f;

float StairThickness = 2.f;

float FootToTheGround = 18.7f;


struct Light
{
	GLfloat ambientStrength;

	glm::vec3 diffuseColor;
	GLfloat diffuseStrength;

	GLfloat specularStrength;

	Light(GLfloat aStr, glm::vec3 dCol, GLfloat dStr, GLfloat sStr)
	{
		ambientStrength = aStr;

		diffuseColor = dCol;
		diffuseStrength = dStr;

		specularStrength = sStr;
	}

};

struct DirLight : public Light
{
	glm::vec3 direction;

	DirLight(glm::vec3 dir, 
			GLfloat aStr, glm::vec3 dCol, GLfloat dStr, GLfloat sStr)
		:Light(aStr, dCol, dStr, sStr)
	{
		direction = dir;
	}
};

struct PointLight : public Light
{
	glm::vec3 position;
	GLfloat constant, linear, exponent;	//Quadratic equation for attenuation

	PointLight(glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat aStr, glm::vec3 dCol, GLfloat dStr, GLfloat sStr)
		:Light(aStr, dCol, dStr, sStr)
	{
		position = pos;

		constant = con;
		linear = lin;
		exponent = exp;
	}
};

//Directional Light
DirLight dirLight(glm::vec3(1.f, -1.f, -1.f),
				  0.2f, glm::vec3(1.f, 1.f, 1.f), 1.f, 1.f);

//50 range
PointLight pLight(glm::vec3(20.f, 7.f, 105.f), 1.f, 0.09, 0.032f,
				  0.2f, glm::vec3(1.f, 0.f, 0.f), 1.f, 1.f);

PointLight pLight2(glm::vec3(50.f, 7.f, 105.f), 1.f, 0.09, 0.032f,
				  0.2f, glm::vec3(0.f, 1.f, 0.f), 1.f, 1.f);

PointLight pLight3(glm::vec3(80.f, 7.f, 105.f), 1.f, 0.09, 0.032f,
	0.2f, glm::vec3(0.f, 0.f, 1.f), 1.f, 1.f);

void CreateTextures();
void CreateObjects();
void CreateWatchTower(float = 0.f, float = 0.f, float = 0.f);
void CreateSoldier(float X, float Y, float Z);
void CreateBlendObjects();
void CreateStair();
void CreateTurret();

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
	projection = glm::perspective(glm::radians(45.f), (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 1000.f);
	

	////////////////////////////Get uniform location////////////////////////////////////////////
	uniformModel = glGetUniformLocation(program, "model");
	uniformView = glGetUniformLocation(program, "view");
	uniformProj = glGetUniformLocation(program, "projection");
	uniformShininess = glGetUniformLocation(program, "Shininess");

	//Set Directional light
	glUniform1f(glGetUniformLocation(program, "dirLight.base.ambientStrength"), dirLight.ambientStrength);
	glUniform3f(glGetUniformLocation(program, "dirLight.base.diffuseColor"), dirLight.diffuseColor.x, dirLight.diffuseColor.y, dirLight.diffuseColor.z);
	glUniform1f(glGetUniformLocation(program, "dirLight.base.diffuseStrength"), dirLight.diffuseStrength);
	glUniform1f(glGetUniformLocation(program, "dirLight.base.specularStrength"), dirLight.specularStrength);

	glUniform3f(glGetUniformLocation(program, "dirLight.direction"), dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);

	//Set light1
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.ambientStrength"), pLight.ambientStrength);
	glUniform3f(glGetUniformLocation(program, "pLight[0].base.diffuseColor"), pLight.diffuseColor.x, pLight.diffuseColor.y, pLight.diffuseColor.z);
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.diffuseStrength"), pLight.diffuseStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[0].base.specularStrength"), pLight.specularStrength);

	glUniform3f(glGetUniformLocation(program, "pLight[0].position"), pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight[0].constant"), pLight.constant);
	glUniform1f(glGetUniformLocation(program, "pLight[0].linear"), pLight.linear);
	glUniform1f(glGetUniformLocation(program, "pLight[0].exponent"), pLight.exponent);

	//Set light2
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.ambientStrength"), pLight2.ambientStrength);
	glUniform3f(glGetUniformLocation(program, "pLight[1].base.diffuseColor"), pLight2.diffuseColor.x, pLight2.diffuseColor.y, pLight2.diffuseColor.z);
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.diffuseStrength"), pLight2.diffuseStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[1].base.specularStrength"), pLight2.specularStrength);

	glUniform3f(glGetUniformLocation(program, "pLight[1].position"), pLight2.position.x, pLight2.position.y, pLight2.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight[1].constant"), pLight2.constant);
	glUniform1f(glGetUniformLocation(program, "pLight[1].linear"), pLight2.linear);
	glUniform1f(glGetUniformLocation(program, "pLight[1].exponent"), pLight2.exponent);

	//Set light3
	glUniform1f(glGetUniformLocation(program, "pLight[2].base.ambientStrength"), pLight3.ambientStrength);
	glUniform3f(glGetUniformLocation(program, "pLight[2].base.diffuseColor"), pLight3.diffuseColor.x, pLight3.diffuseColor.y, pLight3.diffuseColor.z);
	glUniform1f(glGetUniformLocation(program, "pLight[2].base.diffuseStrength"), pLight3.diffuseStrength);
	glUniform1f(glGetUniformLocation(program, "pLight[2].base.specularStrength"), pLight3.specularStrength);

	glUniform3f(glGetUniformLocation(program, "pLight[2].position"), pLight3.position.x, pLight3.position.y, pLight3.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight[2].constant"), pLight3.constant);
	glUniform1f(glGetUniformLocation(program, "pLight[2].linear"), pLight3.linear);
	glUniform1f(glGetUniformLocation(program, "pLight[2].exponent"), pLight3.exponent);

	//Get Eye position location
	uniformEyePos = glGetUniformLocation(program, "eyePos");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////Create Meshes//////////////////////////////////////////
	//Create All meshes
	GeometryGenerator::GenerateMeshes(16);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////Create Textures//////////////////////////////////////////
	CreateTextures();	
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////Create Objects///////////////////////////////
	CreateObjects();
	CreateWatchTower(50.f, 90.f);
	CreateWatchTower(90.f, 50.f, 90.f);
	CreateWatchTower(50.f, 10.f);
	CreateWatchTower(10.f, 50.f, 90.f);

	//Front wall
	CreateSoldier(30.f, 19.f, 90.f);
	CreateSoldier(50.f, 19.f, 90.f);
	CreateSoldier(70.f, 19.f, 90.f);

	//Front door
	CreateSoldier(40.f, -2.f, 100.f);
	CreateSoldier(60.f, -2.f, 100.f);

	CreateStair();

	// Turret
	CreateTurret();

	//Objects using blend(should be always last)
	CreateBlendObjects();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





	//Enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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

	//Set eye position
	glm::vec3 cameraPos = camera.GetCameraPosition();
	glUniform3f(uniformEyePos, cameraPos.x, cameraPos.y, cameraPos.z);





	////////////////////////////////////Start Draw////////////////////////////////////////////
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	for(int i =0 ; i < vecObjects.size(); ++i)
		vecObjects[i]->Render();


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
	//glutMotionFunc(mouseMove); // Requires click to register.;



	glutMainLoop();


	clean();

}

void CreateTextures()
{
	TextureManager::CreateTexture("../Texture/Leather.jpg", "Leather", SOIL_LOAD_RGB);
	TextureManager::CreateTexture("../Texture/Fence.png", "Fence", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/Window.png", "Window", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/Grass.jpg", "Grass", SOIL_LOAD_RGB);
	TextureManager::CreateTexture("../Texture/Wall.jpg", "Wall", SOIL_LOAD_RGB);
	TextureManager::CreateTexture("../Texture/Wood.jpg", "Wood", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/RoofTile.jpg", "Giwa", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/Door.jpg", "Door", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/color.jpg", "color", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/armour.jpg", "armour", SOIL_LOAD_RGBA);
	TextureManager::CreateTexture("../Texture/Metal.jpg", "Metal", SOIL_LOAD_RGBA);
}

void CreateObjects()
{
	//Ground
	Object* pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PLANE));
	pObject1->SetTexture(TextureManager::GetTexture("Grass"));
	pObject1->SetPosition(-25.f, 0.f, -25.f);
	pObject1->SetScale(150.f, 1.f, 150.f);
	vecObjects.push_back(pObject1);

	//Front wall
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(50.f, 10.f, 90.f);
	pObject1->SetScale(38.f, 10.f, WallThickness);
	vecObjects.push_back(pObject1);
	
	//Right wall
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(90.f, 10.f, 50.f);
	pObject1->SetRotation(0.f, 1.f, 0.f, 90.f);
	pObject1->SetScale(42.f, 10.f, WallThickness);
	vecObjects.push_back(pObject1);
	
	//Left wall
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(10.f, 10.f, 50.f);
	pObject1->SetRotation(0.f, 1.f, 0.f, 90.f);
	pObject1->SetScale(42.f, 10.f, WallThickness);
	vecObjects.push_back(pObject1);
	
	//Back wall
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(50.f, 10.f, 10.f);
	pObject1->SetScale(38.f, 10.f, WallThickness);
	vecObjects.push_back(pObject1);
	
	//Castle Pillar1
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(10.f, 15.f, 90.f);
	pObject1->SetScale(5.f, 30.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar1's Roof
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CONE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(10.f, 30.f, 90.f);
	pObject1->SetScale(5.f, 10.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar2
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(90.f, 15.f, 90.f);
	pObject1->SetScale(5.f, 30.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar2's Roof
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CONE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(90.f, 30.f, 90.f);
	pObject1->SetScale(5.f, 10.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar3
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(90.f, 15.f, 10.f);
	pObject1->SetScale(5.f, 30.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar3's Roof
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CONE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(90.f, 30.f, 10.f);
	pObject1->SetScale(5.f, 10.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar4
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(10.f, 15.f, 10.f);
	pObject1->SetScale(5.f, 30.f, 5.f);
	vecObjects.push_back(pObject1);

	//Castle Pillar4's Roof
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CONE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(10.f, 30.f, 10.f);
	pObject1->SetScale(5.f, 10.f, 5.f);
	vecObjects.push_back(pObject1);

	
	//Door left
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_TRAPEZOID));
	pObject1->SetTexture(TextureManager::GetTexture("Door"));
	pObject1->SetPosition(40.f, 0.f, 94.f);
	pObject1->SetScale(5.f, 4.f, 3.f);
	vecObjects.push_back(pObject1);

	//Door right
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_TRAPEZOID));
	pObject1->SetTexture(TextureManager::GetTexture("Door"));
	pObject1->SetPosition(60.f, 0.f, 97.f);
	pObject1->SetScale(5.f, 4.f, 3.f);
	pObject1->SetRotation(0.f, 1.f, 0.f, 180.f);
	vecObjects.push_back(pObject1);


	//Pyramid
	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMID));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(30.f, 10.f, 26.f);
	pObject1->SetScale(3.f, 3.f, 3.f);
	vecObjects.push_back(pObject1);

	//PYramid kind of
	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(40.f, 10.f, 26.f);
	pObject1->SetScale(3.f, 3.f, 3.f);
	vecObjects.push_back(pObject1);


	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_SANDWICH));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(50.f, 10.f, 26.f);
	pObject1->SetScale(3.f, 3.f, 3.f);
	vecObjects.push_back(pObject1);

	
	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_RHOMBUS));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(60.f, 10.f, 26.f);
	pObject1->SetScale(3.f, 3.f, 3.f);
	vecObjects.push_back(pObject1);

	/*
	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_OCTAHEDRON));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(30.f, 10.f, 30.f);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_UPPERROOF));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(45.f, 10.f, 45.f);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_TRAPEZOID));
	pObject1->SetTexture(TextureManager::GetTexture("Door"));
	pObject1->SetPosition(60.f, 10.f, 60.f);
	vecObjects.push_back(pObject1);*/
}

void CreateWatchTower(float lx, float lz, float angle)
{
	Object* pObject2 = new Object(uniformModel, uniformShininess, 1.f);

	if (angle == 0)
	{
		float pillar_x = lx - 7.5f;
		float pillar_z = lz - 2.5f;

		for (int zcount = 0; zcount < 2; zcount++, pillar_z += 5.f)
		{
			for (int xcount = 0; xcount < 4; xcount++, pillar_x += 5.f)
			{
				if (pillar_x > lx + 7.5f)
				{
					pillar_x = lx - 7.5f;
				}
				//Pillar
				pObject2 = new Object(uniformModel, uniformShininess, 1.f);
				pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
				pObject2->SetTexture(TextureManager::GetTexture("Wood"));
				pObject2->SetPosition(pillar_x, WatchTowerHeight - 2.f, pillar_z);
				pObject2->SetScale(0.3f, 9.5f, 0.3f);
				vecObjects.push_back(pObject2);
			}
		}
	}
	else
	{
		float pillar_x = lx - 2.5f;
		float pillar_z = lz - 7.5f;

		for (int xcount = 0; xcount < 2; xcount++, pillar_x += 5.f)
		{
			for (int zcount = 0; zcount < 4; zcount++, pillar_z += 5.f)
			{
				if (pillar_z > lz + 7.5f)
				{
					pillar_z = lz - 7.5f;
				}
				//Pillar
				pObject2 = new Object(uniformModel, uniformShininess, 1.f);
				pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
				pObject2->SetTexture(TextureManager::GetTexture("Wood"));
				pObject2->SetPosition(pillar_x, WatchTowerHeight - 2.f, pillar_z);
				pObject2->SetScale(0.3f, 9.5f, 0.3f);
				vecObjects.push_back(pObject2);
			}
		}
	}

	pObject2 = new Object(uniformModel, uniformShininess, 1.f);
	pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject2->SetTexture(TextureManager::GetTexture("Giwa"));
	pObject2->SetPosition(lx, WatchTowerHeight + 3.5f, lz);
	pObject2->SetRotation(0.f, 1.f, 0.f, angle);
	pObject2->SetScale(10.f, 2.f, 5.f);
	vecObjects.push_back(pObject2);

	pObject2 = new Object(uniformModel, uniformShininess, 1.f);
	pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject2->SetTexture(TextureManager::GetTexture("Wood"));
	pObject2->SetPosition(lx, WatchTowerHeight + 5.3f, lz);
	pObject2->SetRotation(0.f, 1.f, 0.f, angle);
	pObject2->SetScale(5.f, 1.3f, 2.5f);
	vecObjects.push_back(pObject2);

	pObject2 = new Object(uniformModel, uniformShininess, 1.f);
	pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject2->SetTexture(TextureManager::GetTexture("Giwa"));
	pObject2->SetPosition(lx, WatchTowerHeight + 7.5f, lz);
	pObject2->SetRotation(0.f, 1.f, 0.f, angle);
	pObject2->SetScale(8.f, 1.3f, 4.5f);
	vecObjects.push_back(pObject2);

	pObject2 = new Object(uniformModel, uniformShininess, 1.f);
	pObject2->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_UPPERROOF));
	pObject2->SetTexture(TextureManager::GetTexture("Giwa"));
	pObject2->SetPosition(lx, WatchTowerHeight + 8.f, lz);
	pObject2->SetRotation(0.f, 1.f, 0.f, angle);
	pObject2->SetScale(2.02f, 2.f, 1.1f);
	vecObjects.push_back(pObject2);
}

void CreateSoldier(float X, float Y, float Z)
{
	float rightHeight = Y - FootToTheGround;
	// 1st soldier
	//Head
	Object* pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("color"));
	pObject3->SetPosition(X, 25.2f +rightHeight, Z);
	pObject3->SetRotation(1.f, 0.f, 0.f, Z);
	pObject3->SetScale(0.5f, 0.5f, 0.5f);
	vecObjects.push_back(pObject3);

	//Shoulder
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X, 23.65f + rightHeight, Z);
	pObject3->SetScale(0.75f, 0.3f, 0.5f);
	vecObjects.push_back(pObject3);

	//body
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X, 23.7f+rightHeight, Z);
	pObject3->SetScale(0.75f, 0.7f, 0.48f);
	vecObjects.push_back(pObject3);

	//Pelvis
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X, 22.7f + rightHeight, Z);
	pObject3->SetRotation(1.f, 0.f, 0.f, 180.f);
	pObject3->SetScale(0.75f, 0.3f, 0.5f);
	vecObjects.push_back(pObject3);

	//Right Thigh
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X-0.5f, 22.2f + rightHeight, Z);
	pObject3->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject3);

	//Left Thigh
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X+0.5f, 22.2f + rightHeight, Z);
	pObject3->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject3);

	// Right calf
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X-0.5f, 20.7f + rightHeight, Z);
	pObject3->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject3);

	//Left calf
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X+0.5f, 20.7f + rightHeight, Z);
	pObject3->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject3);

	//Left arm
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X+0.9f, 23.55f + rightHeight, Z);
	pObject3->SetScale(0.2f, 1.8f, 0.2f);
	vecObjects.push_back(pObject3);

	//Right upper arm
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X-0.9f, 23.55f + rightHeight, Z);
	pObject3->SetScale(0.2f, 0.8f, 0.2f);
	vecObjects.push_back(pObject3);

	//Right lower arm
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("armour"));
	pObject3->SetPosition(X - 0.9f, 23.f + rightHeight, Z+0.5f);
	pObject3->SetRotation(1.f, 0.f, 0.f, Z);
	pObject3->SetScale(0.2f, 1.f, 0.2f);
	vecObjects.push_back(pObject3);

	//spear body
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject3->SetTexture(TextureManager::GetTexture("Wood"));
	pObject3->SetPosition(X - 0.9f, 23.f + rightHeight, Z+1.f);
	pObject3->SetScale(0.05f, 5.f, 0.05f);
	vecObjects.push_back(pObject3);

	//spear tip
	pObject3 = new Object(uniformModel, uniformShininess, 1.f);
	pObject3->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_OCTAHEDRON));
	pObject3->SetTexture(TextureManager::GetTexture("Metal"));
	pObject3->SetPosition(X - 0.9f, 25.2f + rightHeight, Z+1.f);
	pObject3->SetScale(0.1f, 0.2f, 0.1f);
	vecObjects.push_back(pObject3);
}

void CreateBlendObjects()
{
	//Back Fence
	for (int i = 0; i < 5; ++i)
	{
		Object* pObject = new Fence(uniformModel, uniformShininess, 1.f);
		pObject->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
		pObject->SetTexture(TextureManager::GetTexture("Fence"));
		pObject->SetPosition(-10.f + (i * 30), 4.f, -23.f);
		pObject->SetRotation(0.f, 1.f, 0.f, 0.f);
		pObject->SetScale(15.f, 4.f, 2.f);
		vecObjects.push_back(pObject);
	}

	//Left Fence
	for (int i = 0; i < 5; ++i)
	{
		Object* pObject = new Fence(uniformModel, uniformShininess, 1.f);
		pObject->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
		pObject->SetTexture(TextureManager::GetTexture("Fence"));
		pObject->SetPosition(-23.f, 4.f, -10.f + (i * 30));
		pObject->SetRotation(0.f, 1.f, 0.f, 90.f);
		pObject->SetScale(15.f, 4.f, 2.f);
		vecObjects.push_back(pObject);
	}

	//Right Fence
	for (int i = 0; i < 5; ++i)
	{
		Object* pObject = new Fence(uniformModel, uniformShininess, 1.f);
		pObject->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
		pObject->SetTexture(TextureManager::GetTexture("Fence"));
		pObject->SetPosition(123.f, 4.f, -10.f + (i * 30));
		pObject->SetRotation(0.f, 1.f, 0.f, 90.f);
		pObject->SetScale(15.f, 4.f, 2.f);
		vecObjects.push_back(pObject);
	}

	//Front Fence
	for (int i = 0; i < 5; ++i)
	{
		Object* pObject = new Fence(uniformModel, uniformShininess, 1.f);
		pObject->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
		pObject->SetTexture(TextureManager::GetTexture("Fence"));
		pObject->SetPosition(-10.f + (i * 30), 4.f, 123.f);
		pObject->SetRotation(0.f, 1.f, 0.f, 0.f);
		pObject->SetScale(15.f, 4.f, 2.f);
		vecObjects.push_back(pObject);
	}
}


void CreateStair()
{
	Object* pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(57.f, 1.f, 84.f);
	pObject1->SetScale(1.5f, 1.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(60.f, 2.f, 84.f);
	pObject1->SetScale(1.5f, 2.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(63.f, 3.f, 84.f);
	pObject1->SetScale(1.5f, 3.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(66.f, 4.f, 84.f);
	pObject1->SetScale(1.5f, 4.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(69.f, 5.f, 84.f);
	pObject1->SetScale(1.5f, 5.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(72.f, 6.f, 84.f);
	pObject1->SetScale(1.5f, 6.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(75.f, 7.f, 84.f);
	pObject1->SetScale(1.5f, 7.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(78.f, 8.f, 84.f);
	pObject1->SetScale(1.5f, 8.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(81.f, 9.f, 84.f);
	pObject1->SetScale(1.5f, 9.f, StairThickness);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Wall"));
	pObject1->SetPosition(84.f, 10.f, 84.f);
	pObject1->SetScale(1.5f, 10.f, StairThickness);
	vecObjects.push_back(pObject1);

	
}

void CreateTurret()
{
	// turret
	Object* pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(50.f, 10.f, 50.f);
	pObject1->SetScale(1.f, 5.f, 1.f);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(50.f, 3.5f, 50.f);
	pObject1->SetScale(4.f, 4.f, 4.f);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Leather"));
	pObject1->SetPosition(50.f, 15.f, 48.f);
	pObject1->SetScale(1.f, 7.f, 1.f);
	pObject1->SetRotation(1.f, 0.f, 0.f, 135);
	vecObjects.push_back(pObject1);

	// arrows
	for (int i = 0; i < 3; ++i)
	{
		pObject1 = new Object(uniformModel, uniformShininess, 1.f);
		pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
		pObject1->SetTexture(TextureManager::GetTexture("Wood"));
		pObject1->SetPosition(49.f + i, 18.f, 47.f);
		pObject1->SetScale(0.05f, 5.f, 0.05f);
		pObject1->SetRotation(1.f, 0.f, 0.f, 135);
		vecObjects.push_back(pObject1);

		pObject1 = new Object(uniformModel, uniformShininess, 1.f);
		pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_OCTAHEDRON));
		pObject1->SetTexture(TextureManager::GetTexture("Metal"));
		pObject1->SetPosition(49.f + i, 20.f, 45.f);
		pObject1->SetScale(0.1f, 0.2f, 0.1f);
		pObject1->SetRotation(1.f, 0.f, 0.f, 135);
		vecObjects.push_back(pObject1);
	}

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Giwa"));
	pObject1->SetPosition(50.f, 16.f, 49.f);
	pObject1->SetScale(0.1f, 5.f, 0.1f);
	pObject1->SetRotation(0.f, 0.f, 1.f, 90);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Giwa"));
	pObject1->SetPosition(47.5f, 17.f, 47.f);
	pObject1->SetScale(0.2f, 3.5f, 0.2f);
	pObject1->SetRotation(1.f, 0.f, 1.f, 115);
	vecObjects.push_back(pObject1);

	pObject1 = new Object(uniformModel, uniformShininess, 8.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("Giwa"));
	pObject1->SetPosition(52.5f, 17.f, 47.f);
	pObject1->SetScale(0.2f, 3.5f, 0.2f);
	pObject1->SetRotation(1.f, 0.f, -1.f, -60);
	vecObjects.push_back(pObject1);

	// solider on the turret
	//Head
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("color"));
	pObject1->SetPosition(49.8f, 20.2f, 50.f);
	pObject1->SetRotation(1.f, 0.f, 0.f, 90.f);
	pObject1->SetScale(0.5f, 0.5f, 0.5f);
	vecObjects.push_back(pObject1);

	//Shoulder
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(49.8f, 18.65f, 50.f);
	pObject1->SetScale(0.75f, 0.3f, 0.5f);
	vecObjects.push_back(pObject1);

	//body
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CUBE));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(49.8f, 18.7f, 50.f);
	pObject1->SetScale(0.75f, 0.7f, 0.48f);
	vecObjects.push_back(pObject1);

	//Pelvis
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_PYRAMIDKINDOF));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(49.8f, 17.7f, 50.f);
	pObject1->SetRotation(1.f, 0.f, 0.f, 180.f);
	pObject1->SetScale(0.75f, 0.3f, 0.5f);
	vecObjects.push_back(pObject1);

	//Right Thigh
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(49.3f, 17.2f, 50.f);
	pObject1->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject1);

	//Left Thigh
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(50.3f, 17.2f, 50.f);
	pObject1->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject1);

	// Right calf
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(49.3f, 15.7f, 50.f);
	pObject1->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject1);

	//Left calf
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(50.3f, 15.7f, 50.f);
	pObject1->SetScale(0.35f, 1.5f, 0.35f);
	vecObjects.push_back(pObject1);

	//Left arm
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(50.7f, 18.55f, 50.f);
	pObject1->SetScale(0.2f, 1.8f, 0.2f);
	vecObjects.push_back(pObject1);

	//Right upper arm
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(48.9f, 18.55f, 50.f);
	pObject1->SetScale(0.2f, 0.8f, 0.2f);
	vecObjects.push_back(pObject1);

	//Right lower arm
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("armour"));
	pObject1->SetPosition(48.9f, 18.f, 50.5f);
	pObject1->SetRotation(1.f, 0.f, 0.f, 90.f);
	pObject1->SetScale(0.2f, 1.f, 0.2f);
	vecObjects.push_back(pObject1);

	//spear body
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_CYLINDER));
	pObject1->SetTexture(TextureManager::GetTexture("Wood"));
	pObject1->SetPosition(48.9f, 18.f, 51.f);
	pObject1->SetScale(0.05f, 5.f, 0.05f);
	vecObjects.push_back(pObject1);

	//spear tip
	pObject1 = new Object(uniformModel, uniformShininess, 1.f);
	pObject1->SetMesh(GeometryGenerator::GetMesh(GeometryGenerator::EMeshList::MESH_OCTAHEDRON));
	pObject1->SetTexture(TextureManager::GetTexture("Metal"));
	pObject1->SetPosition(48.9f, 20.2f, 51.f);
	pObject1->SetScale(0.1f, 0.2f, 0.1f);
	vecObjects.push_back(pObject1);
	
	
}