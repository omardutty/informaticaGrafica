#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <time.h>

#include "GL_framework.h"

#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

///////// fw decl

glm::vec3 seed = { 0,10,0 };
glm::vec4 seedR = { 0,10,0,1 };
bool pause = false;
float fakeGravity = 22;

float g61 = ((rand()%4) - 3) +20; 
float g62 = ((rand() % 4) - 3) + 20;
float g63 = ((rand() % 4) - 3) + 20;
glm::mat4 rotationMatrix;
bool e1 = true, e2 = false, e3 = false, e4 = false, e5 = false, e6 =false, e6b = false, e7 = false;
bool controlE1 = true;
bool controlE6 = true;
float r1x, r1y, r1z, r2x, r2y, r2z, r3x, r3y, r3z, r4x, r4y, r4z, r5x, r5y, r5z, r6x, r6y, r6z;
float randomz1, randomz2, randomz3;
glm::vec4 rColor;
static int cam = 0;
float W, H;
glm::mat4 escalar = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

namespace ImGui {
	void Render();
}
namespace Box {
	void setupCube();
	void cleanupCube();
	void drawCube();
}
namespace Axis {
	void setupAxis();
	void cleanupAxis();
	void drawAxis();
}
namespace Cube {
	void setupCube();
	void cleanupCube();
	void updateCube(const glm::mat4& transform);
	void drawCube();
}

namespace MyFirstShader {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise1 {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise3 {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise4 {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise5 {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise5Visual {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise6 {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}

namespace Exercise6b {
	void  myInitCode(void);
	GLuint myShaderCompile(void);

	void myCleanupCode(void);
	glm::vec3 tPos = { 0,0,0 };
	void myRenderCode(double currentTime);


}


void GUI() {
	bool show = true;
	ImGui::Begin("Shaders", &show, 0);

	// Do your GUI code here....
	{
		
		ImGui::RadioButton("Perspective", &cam,0);
		ImGui::RadioButton("Ortho", &cam,1);
		ImGui::Checkbox("Exercise 1", &e1);
		ImGui::Checkbox("Exercise 2", &e2);
		ImGui::Checkbox("Exercise 3", &e3);
		ImGui::Checkbox("Exercise 4", &e4);
		ImGui::Checkbox("Exercise 5", &e5);
		ImGui::Checkbox("Exercise 6", &e6);
		ImGui::Checkbox("Exercise 6b", &e6b);
		ImGui::Checkbox("Exercise 7", &e7);
	}
	// .........................

	ImGui::End();
}


////////////////

namespace RenderVars {
	const float FOV = glm::radians(65.f);
	const float zNear = 1.f;
	const float zFar = 50.f;

	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, -5.f, -15.f };
	float rota[2] = { 0.f, 0.f };
}
namespace RV = RenderVars;

void GLResize(int width, int height) {
	glViewport(0, 0, width, height);
	if (height != 0) RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);
	else RV::_projection = glm::perspective(RV::FOV, 0.f, RV::zNear, RV::zFar);
}

void GLmousecb(MouseEvent ev) {
	if (RV::prevMouse.waspressed && RV::prevMouse.button == ev.button) {
		float diffx = ev.posx - RV::prevMouse.lastx;
		float diffy = ev.posy - RV::prevMouse.lasty;
		switch (ev.button) {
		case MouseEvent::Button::Left: // ROTATE
			RV::rota[0] += diffx * 0.005f;
			RV::rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: // MOVE XY
			RV::panv[0] += diffx * 0.03f;
			RV::panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: // MOVE Z
			RV::panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	}
	else {
		RV::prevMouse.button = ev.button;
		RV::prevMouse.waspressed = true;
	}
	RV::prevMouse.lastx = ev.posx;
	RV::prevMouse.lasty = ev.posy;
	
	if (cam == 0) {
		RV::_projection = glm::perspective(RV::FOV, W / H, RV::zNear, RV::zFar);
	}
	else {
		RV::_projection = glm::ortho((float)-W / -50.f, (float)W / -50.f, (float)H / -50.f, (float)-H / -50.f, 0.1f, 100.f);
	}
}

void GLinit(int width, int height) {
	W = width;
	H = height;
	glViewport(0, 0, width, height);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);

	srand(time(NULL));
	// Setup shaders & geometry
	/*Box::setupCube();
	Axis::setupAxis();
	Cube::setupCube();*/


	Exercise1::myInitCode();
	Exercise3::myInitCode();
	Exercise4::myInitCode();
	Exercise5::myInitCode();
	Exercise5Visual::myInitCode();
	Exercise6::myInitCode();
	Exercise6b::myInitCode();
	MyFirstShader::myInitCode();






}

void GLcleanup() {
	/*Box::cleanupCube();
	Axis::cleanupAxis();
	Cube::cleanupCube();
	*/

	Exercise1::myCleanupCode();
	Exercise3::myCleanupCode();
	Exercise4::myCleanupCode(); 
	Exercise5::myCleanupCode();
	Exercise5Visual::myCleanupCode();
	Exercise6::myCleanupCode();
	Exercise6b::myCleanupCode();
	MyFirstShader::myCleanupCode();

}

void GLrender(double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RV::_modelView = glm::mat4(1.f);
	RV::_modelView = glm::translate(RV::_modelView, glm::vec3(RV::panv[0], RV::panv[1], RV::panv[2]));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[1], glm::vec3(1.f, 0.f, 0.f));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[0], glm::vec3(0.f, 1.f, 0.f));

	RV::_MVP = RV::_projection * RV::_modelView;

	// render code
	/*Box::drawCube();
	Axis::drawAxis();
	Cube::drawCube();*/

	
	//EX1
	if (e1) {
			if (controlE1) {
				r1x = (rand() % 15) - 7;
				r1y = (rand() % 15) - 7;
				r1z = -(rand() % 5);
				r2x = (rand() % 15) - 7;
				r2y = (rand() % 15) - 7;
				r2z = -(rand() % 5);
				r3x = (rand() % 15) - 7;
				r3y = (rand() % 15) - 7;
				r3z = -(rand() % 5);
				r4x = (rand() % 15) - 7;
				r4y = (rand() % 15) - 7;
				r4z = -(rand() % 5);
				r5x = (rand() % 15) - 7;
				r5y = (rand() % 15) - 7;
				r5z = -(rand() % 5);
				r6x = (rand() % 15) - 7;
				r6y = (rand() % 15) - 7;
				r6z = -(rand() % 5);
				controlE1 = false;
			}
			seed = { r1x,r1y,r1z };
			Exercise1::myRenderCode(currentTime);
			seed = { r2x,r2y,r2z };
			Exercise1::myRenderCode(currentTime);
			seed = { r3x,r3y,r3z };
			Exercise1::myRenderCode(currentTime);
			seed = { r4x,r4y,r4z };
			Exercise1::myRenderCode(currentTime);
			seed = { r5x,r5y,r5z };
			Exercise1::myRenderCode(currentTime);
			seed = { r6x,r6y,r6z };
			Exercise1::myRenderCode(currentTime);
		}
	else {
			controlE1 = true;
		}
	//EX2
	if (e2) {
			seed = { 0,0,0 };
			rotationMatrix = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 0,2 * sqrt(3),0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 0,4 * sqrt(3),0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,0,0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,2 * sqrt(3),0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,4 * sqrt(3),0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,0,0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,2 * sqrt(3),0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,4 * sqrt(3),0 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,sqrt(3),2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,3 * sqrt(3),2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 0,sqrt(3),2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 0,3 * sqrt(3),2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,sqrt(3),2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,3 * sqrt(3),2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,sqrt(3),-2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { -3,3 * sqrt(3),-2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 0,sqrt(3),-2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 0,3 * sqrt(3),-2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,sqrt(3),-2 };
			MyFirstShader::myRenderCode(currentTime);
			seed = { 3,3 * sqrt(3),-2 };
			MyFirstShader::myRenderCode(currentTime);
		}
	//EX3
	if (e3) {
		if (cam == 0) {
			escalar = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { -4,fakeGravity,0,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { -2,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 0,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),0,cos(currentTime),0,0,1,0,0,-cos(currentTime),0,sin(currentTime),0,0,0,0,1 };
			seedR = { 2,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),-cos(currentTime),0,0,cos(currentTime),sin(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 4,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			fakeGravity -= 0.5;
			if (fakeGravity < -18) {
				fakeGravity = 22;
			}
		}
		else {
			escalar = { 4,0,0,0,0,4,0,0,0,0,4,0,0,0,0,1 };
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { -1,fakeGravity,0,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { -0.5,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 0,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),0,cos(currentTime),0,0,1,0,0,-cos(currentTime),0,sin(currentTime),0,0,0,0,1 };
			seedR = { 0.5,fakeGravity,0 ,1 };
			Exercise3::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),-cos(currentTime),0,0,cos(currentTime),sin(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 1,fakeGravity,0,1 };
			Exercise3::myRenderCode(currentTime);
			fakeGravity -= 0.1;
			if (fakeGravity < -5) {
				fakeGravity = 5;
			}
		}
	}
	//EX4
	if (e4) {
		if (cam == 0) {
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { -8,fakeGravity,0,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { -4,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 0,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),0,cos(currentTime),0,0,1,0,0,-cos(currentTime),0,sin(currentTime),0,0,0,0,1 };
			seedR = { 4,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),-cos(currentTime),0,0,cos(currentTime),sin(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 8,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			fakeGravity -= 0.5;
			if (fakeGravity < -18) {
				fakeGravity = 22;
			}
		}
		else {
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { -1,fakeGravity,0,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { -0.5,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 0,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),0,cos(currentTime),0,0,1,0,0,-cos(currentTime),0,sin(currentTime),0,0,0,0,1 };
			seedR = { 0.5,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),-cos(currentTime),0,0,cos(currentTime),sin(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 1,fakeGravity,0 ,1 };
			Exercise4::myRenderCode(currentTime);
			fakeGravity -= 0.1;
			if (fakeGravity < -5) {
				fakeGravity = 5;
			}
		}
	}
	//EX5
	if (e5) {
			rotationMatrix = { sin(currentTime),-cos(currentTime),0,0,cos(currentTime),sin(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { -12,fakeGravity,0,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { -8,fakeGravity,0,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { -4,fakeGravity,0 ,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 0,fakeGravity,0 ,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),0,cos(currentTime),0,0,1,0,0,-cos(currentTime),0,sin(currentTime),0,0,0,0,1 };
			seedR = { 4,fakeGravity,0 ,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			rotationMatrix = { sin(currentTime),-cos(currentTime),0,0,cos(currentTime),sin(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { 8,fakeGravity,0 ,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { 12,fakeGravity,0,1 };
			Exercise5::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			fakeGravity -= 0.8;
			if (fakeGravity < -10) {
				fakeGravity = 22;
			}
		}
	//EX6
	if (e6) {
		seed = { 0,0,0 };
		rotationMatrix = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
		Exercise6::myRenderCode(currentTime);
		seed = { 0,2 * sqrt(3),0 };
		Exercise6::myRenderCode(currentTime);
		seed = { 0,4 * sqrt(3),0 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,0,0 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,2 * sqrt(3),0 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,4 * sqrt(3),0 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,0,0 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,2 * sqrt(3),0 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,4 * sqrt(3),0 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,sqrt(3),2 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,3 * sqrt(3),2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 0,sqrt(3),2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 0,3 * sqrt(3),2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,sqrt(3),2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,3 * sqrt(3),2 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,sqrt(3),-2 };
		Exercise6::myRenderCode(currentTime);
		seed = { -3,3 * sqrt(3),-2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 0,sqrt(3),-2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 0,3 * sqrt(3),-2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,sqrt(3),-2 };
		Exercise6::myRenderCode(currentTime);
		seed = { 3,3 * sqrt(3),-2 };
		Exercise6::myRenderCode(currentTime);
	}
	//EX6b
	if (e6b) {
		if (controlE6) {
			randomz1 = (rand() % 2) - 1;
			randomz2 = (rand() % 2) - 1;
			randomz3 = (rand() % 2) - 1;
			controlE6 = false;
		}
		if (cam == 0) {
			
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { 0,g61,randomz1,1 };
			Exercise6b::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { 3,g62,randomz2 ,1 };
			Exercise6b::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { -3,g63,randomz3 ,1 };
			Exercise6b::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);

			g61 -= ((rand()%2)-1 )+0.1;
			g62 -= ((rand() % 2) - 1) + 0.1;
			g63 -= ((rand() % 2) - 1) + 0.1;

			if (g61 < -18) {
				g61 = ((rand() % 4) - 3) + 20;
			}
			if (g62 < -18) {
				g62 = ((rand() % 4) - 3) + 20;
			}
			if (g63 < -18){
				g63 = ((rand() % 4) - 3) + 20;
			}
		}
		else {
			rotationMatrix = { 1,0,0,0,0,cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1 };
			seedR = { 0,g61,randomz1,1 };
			Exercise6b::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			
			rotationMatrix = { cos(currentTime),0,sin(currentTime),0,0,1,0,0,-sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
			seedR = { 0.2,g62,randomz2 ,1 };
			Exercise6b::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			
			rotationMatrix = { cos(currentTime),-sin(currentTime),0,0,sin(currentTime),cos(currentTime),0,0,0,0,1,0,0,0,0,1 };
			seedR = { -0.2,g63,randomz3 ,1 };
			Exercise6b::myRenderCode(currentTime);
			Exercise5Visual::myRenderCode(currentTime);
			
			g61 -= 0.1;
			g62 -= 0.15;
			g63 -= 0.2;

			if (g61 < -18) {
				g61 = ((rand() % 4) - 3) + 20;
			}
			if (g62 < -18) {
				g62 = ((rand() % 4) - 3) + 20;
			}
			if (g63 < -18) {
				g63 = ((rand() % 4) - 3) + 20;
			}
		}
	}
	//EX7
	if (e7) {
	}

	ImGui::Render();
}


//////////////////////////////////// COMPILE AND LINK
GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name = "") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetShaderInfoLog(shader, res, &res, buff);
		fprintf(stderr, "Error Shader %s: %s", name, buff);
		delete[] buff;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
void linkProgram(GLuint program) {
	glLinkProgram(program);
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}
/////////////////my first shader

namespace MyFirstShader {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{ "#version 330 \n\
			uniform mat4 mvp;\n\
			uniform vec4 seed;\n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 72) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[6] = vec4[6](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1));\n\
				\n\
//CARA 1\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices[i]+seed);\n\
gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
//CARA 2\n\
				const vec4 vertices2[6]= vec4[6](vec4(-0.5,sqrt(3), 0.5, 1.0),\n\
										vec4(-1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),0.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices2[i]+seed);\n\
gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 3\n\
				const vec4 vertices3[6]= vec4[6](vec4(0.5, sqrt(3), 0.5, 1.0),\n\
										vec4(1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices3[i]+seed);\n\
gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 4\n\
				const vec4 vertices4[6]= vec4[6](vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices4[i]+seed);\n\
gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA SUPERIOR\n\
		const vec4 vertices9[4]= vec4[4](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5, sqrt(3),0.5, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5, sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*(vertices9[i]+seed);\n\
gl_PrimitiveID = 8;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 5\n\
				const vec4 vertices5[6]= vec4[6](vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices5[i]+seed);\n\
gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 6\n\
				const vec4 vertices6[6]= vec4[6](vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices6[i]+seed);\n\
gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 7\n\
				const vec4 vertices7[6]= vec4[6](vec4(0.5,0,1.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices7[i]+seed);\n\
gl_PrimitiveID = 6;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 8\n\
				const vec4 vertices8[6]= vec4[6](vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*(vertices8[i]+seed);\n\
gl_PrimitiveID = 7;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA INFERIOR\n\
				const vec4 vertices10[4]= vec4[4](vec4(-0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(0.5, -sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*(vertices10[i]+seed);\n\
gl_PrimitiveID = 9;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 1\n\
				const vec4 vertices11[4]= vec4[4](vec4(-1.5,0,0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-0.5,0,1.5,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*(vertices11[i]+seed);\n\
gl_PrimitiveID = 10;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 2\n\
				const vec4 vertices12[4]= vec4[4](vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5,0,1.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*(vertices12[i]+seed);\n\
gl_PrimitiveID = 11;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 3\n\
				const vec4 vertices13[4]= vec4[4](vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5,0,-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*(vertices13[i]+seed);\n\
gl_PrimitiveID = 12;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
////CARA LATERAL 4\n\
				const vec4 vertices14[4]= vec4[4](vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*(vertices14[i]+seed);\n\
gl_PrimitiveID = 13;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};

		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[14] = vec4[14](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,0,1),vec4(0,1,0,1),vec4(1,0,0,1),vec4(1,1,0,1),vec4(0,0,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1));\n\
		color = colors[gl_PrimitiveID];\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glm::mat4 tMatrix = { cos(currentTime),0,-sin(currentTime),0,0,1,0,0,sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seed.x, (GLfloat)seed.y, (GLfloat)seed.z,1.0);
		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


	}


}

namespace Exercise1 {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{
			"#version 330 \n\
			uniform mat4 rotation;\n\
			uniform vec3 seed;\n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 24) out;\n\
			void main()\n\
			{\n\
				vec4 vertices[4] = vec4[4](vec4(seed.x + 0.25, seed.y - 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y + 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y - 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y + 0.25, seed.z + 0.25, 1.0));\n\
				\n\
				//CARA 1\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = rotation*vertices[i]+gl_in[0].gl_Position;\n\
					gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
				//CARA 2\n\
				vec4 vertices2[4]= vec4[4](vec4(seed.x + 0.25, seed.y + 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y + 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y + 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y + 0.25, seed.z - 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = rotation*vertices2[i]+gl_in[0].gl_Position;\n\
					gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 3\n\
				 vec4 vertices3[4]= vec4[4](vec4(seed.x - 0.25, seed.y - 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y + 0.25, seed.z + 0.25, 1.0),\n\
								 				 vec4(seed.x - 0.25, seed.y - 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y + 0.25, seed.z - 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = rotation*vertices3[i]+gl_in[0].gl_Position;\n\
					gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 4\n\
				 vec4 vertices4[4]= vec4[4](vec4(seed.x - 0.25, seed.y - 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y + 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y - 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y + 0.25, seed.z - 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = rotation*vertices4[i]+gl_in[0].gl_Position;\n\
					gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 5\n\
				 vec4 vertices5[4]= vec4[4](vec4(seed.x - 0.25, seed.y - 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x - 0.25, seed.y - 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y - 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y - 0.25, seed.z - 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = rotation*vertices5[i]+gl_in[0].gl_Position;\n\
					gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 6\n\
				 vec4 vertices6[4]= vec4[4](vec4(seed.x + 0.25, seed.y - 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y + 0.25, seed.z - 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y - 0.25, seed.z + 0.25, 1.0),\n\
												 vec4(seed.x + 0.25, seed.y + 0.25, seed.z + 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = rotation*vertices6[i]+gl_in[0].gl_Position;\n\
					gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};
		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[6] = vec4[6](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,1,1),vec4(0,1,1,1),vec4(1,1,0,1));\n\
		color = colors[gl_PrimitiveID];\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glm::mat4 tMatrix = { cos(currentTime),0,-sin(currentTime),0,0,1,0,0,sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
		GLint locationID = glGetUniformLocation(myRenderProgram, "rotation");
		glUniform3f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seed.x, (GLfloat)seed.y, (GLfloat)seed.z);
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


	}


}

namespace Exercise3 {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{
			"#version 330 \n\
			uniform mat4 rotation;\n\
			uniform mat4 mvp;\n\
			uniform mat4 esc;\n\
			uniform vec4 seed;\n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 24) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[4] = vec4[4](vec4(0.25, -0.25, 0.25, 1.0),\n\
										vec4(0.25, 0.25, 0.25, 1.0),\n\
										vec4(-0.25, -0.25, 0.25, 1.0),\n\
										vec4(-0.25, 0.25, 0.25, 1.0));\n\
				\n\
				//CARA 1\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*esc*vertices[i]+gl_in[0].gl_Position+seed;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
				//CARA 2\n\
				const vec4 vertices2[4]= vec4[4](vec4(0.25, 0.25, 0.25, 1.0),\n\
										vec4(0.25, 0.25, -0.25, 1.0),\n\
										vec4(-0.25, 0.25, 0.25, 1.0),\n\
										vec4(-0.25, 0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*esc*vertices2[i]+gl_in[0].gl_Position+seed;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 3\n\
				const vec4 vertices3[4]= vec4[4](vec4(-0.25, -0.25, 0.25, 1.0),\n\
										vec4(-0.25, 0.25, 0.25, 1.0),\n\
										vec4(-0.25, -0.25, -0.25, 1.0),\n\
										vec4(-0.25, 0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*esc*vertices3[i]+gl_in[0].gl_Position+seed;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 4\n\
				const vec4 vertices4[4]= vec4[4](vec4(-0.25, -0.25, -0.25, 1.0),\n\
										vec4(-0.25, 0.25, -0.25, 1.0),\n\
										vec4(0.25, -0.25, -0.25, 1.0),\n\
										vec4(0.25, 0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*esc*vertices4[i]+gl_in[0].gl_Position+seed;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 5\n\
				const vec4 vertices5[4]= vec4[4](vec4(-0.25, -0.25, 0.25, 1.0),\n\
										vec4(-0.25, -0.25, -0.25, 1.0),\n\
										vec4(0.25, -0.25, 0.25, 1.0),\n\
										vec4(0.25, -0.25, -0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*esc*vertices5[i]+gl_in[0].gl_Position+seed;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				//CARA 6\n\
				const vec4 vertices6[4]= vec4[4](vec4(0.25, -0.25, -0.25, 1.0),\n\
										vec4(0.25, 0.25, -0.25, 1.0),\n\
										vec4(0.25, -0.25, 0.25, 1.0),\n\
										vec4(0.25, 0.25, 0.25, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*esc*vertices6[i]+gl_in[0].gl_Position+seed;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};
		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[6] = vec4[6](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,0,1),vec4(0,1,0,1),vec4(1,0,0,1));\n\
		color = colors[0];\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seedR.x, (GLfloat)seedR.y, (GLfloat)seedR.z, (GLfloat)seedR.w);

		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));

		locationID = glGetUniformLocation(myRenderProgram, "rotation");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

		locationID = glGetUniformLocation(myRenderProgram, "esc");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(escalar));

		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


	}


}

namespace Exercise4 {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{ "#version 330 \n\
			uniform mat4 mvp;\n\
			uniform mat4 rotation;\n\
			uniform vec4 seed;\n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 72) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[6] = vec4[6](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1));\n\
				\n\
//CARA 1\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices[i]+seed;\n\
gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
//CARA 2\n\
				const vec4 vertices2[6]= vec4[6](vec4(-0.5,sqrt(3), 0.5, 1.0),\n\
										vec4(-1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),0.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices2[i]+seed;\n\
gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 3\n\
				const vec4 vertices3[6]= vec4[6](vec4(0.5, sqrt(3), 0.5, 1.0),\n\
										vec4(1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices3[i]+seed;\n\
gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 4\n\
				const vec4 vertices4[6]= vec4[6](vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices4[i]+seed;\n\
gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA SUPERIOR\n\
		const vec4 vertices9[4]= vec4[4](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5, sqrt(3),0.5, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5, sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices9[i]+seed;\n\
gl_PrimitiveID = 8;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 5\n\
				const vec4 vertices5[6]= vec4[6](vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices5[i]+seed;\n\
gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 6\n\
				const vec4 vertices6[6]= vec4[6](vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices6[i]+seed;\n\
gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 7\n\
				const vec4 vertices7[6]= vec4[6](vec4(0.5,0,1.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices7[i]+seed;\n\
gl_PrimitiveID = 6;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 8\n\
				const vec4 vertices8[6]= vec4[6](vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices8[i]+seed;\n\
gl_PrimitiveID = 7;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA INFERIOR\n\
				const vec4 vertices10[4]= vec4[4](vec4(-0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(0.5, -sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices10[i]+seed;\n\
gl_PrimitiveID = 9;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 1\n\
				const vec4 vertices11[4]= vec4[4](vec4(-1.5,0,0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-0.5,0,1.5,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices11[i]+seed;\n\
gl_PrimitiveID = 10;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 2\n\
				const vec4 vertices12[4]= vec4[4](vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5,0,1.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices12[i]+seed;\n\
gl_PrimitiveID = 11;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 3\n\
				const vec4 vertices13[4]= vec4[4](vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5,0,-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices13[i]+seed;\n\
gl_PrimitiveID = 12;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
////CARA LATERAL 4\n\
				const vec4 vertices14[4]= vec4[4](vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices14[i]+seed;\n\
gl_PrimitiveID = 13;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};

		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[14] = vec4[14](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,0,1),vec4(0,1,0,1),vec4(1,0,0,1),vec4(1,1,0,1),vec4(0,0,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1));\n\
		color = colors[gl_PrimitiveID];\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seedR.x, (GLfloat)seedR.y, (GLfloat)seedR.z, (GLfloat)seedR.w);

		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));

		locationID = glGetUniformLocation(myRenderProgram, "rotation");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		
		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


	}


}

namespace Exercise5 {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{
			"#version 330 \n\
			uniform mat4 mvp;\n\
			uniform mat4 rotation;\n\
			uniform vec4 seed;\n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 72) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[6] = vec4[6](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1));\n\
				\n\
//CARA 1\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices[i]+seed;\n\
gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
//CARA 2\n\
				const vec4 vertices2[6]= vec4[6](vec4(-0.5,sqrt(3), 0.5, 1.0),\n\
										vec4(-1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),0.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices2[i]+seed;\n\
gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 3\n\
				const vec4 vertices3[6]= vec4[6](vec4(0.5, sqrt(3), 0.5, 1.0),\n\
										vec4(1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices3[i]+seed;\n\
gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 4\n\
				const vec4 vertices4[6]= vec4[6](vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices4[i]+seed;\n\
gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA SUPERIOR\n\
		const vec4 vertices9[4]= vec4[4](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5, sqrt(3),0.5, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5, sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices9[i]+seed;\n\
gl_PrimitiveID = 8;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 5\n\
				const vec4 vertices5[6]= vec4[6](vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices5[i]+seed;\n\
gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 6\n\
				const vec4 vertices6[6]= vec4[6](vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices6[i]+seed;\n\
gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 7\n\
				const vec4 vertices7[6]= vec4[6](vec4(0.5,0,1.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices7[i]+seed;\n\
gl_PrimitiveID = 6;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 8\n\
				const vec4 vertices8[6]= vec4[6](vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices8[i]+seed;\n\
gl_PrimitiveID = 7;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA INFERIOR\n\
				const vec4 vertices10[4]= vec4[4](vec4(-0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(0.5, -sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices10[i]+seed;\n\
gl_PrimitiveID = 9;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 1\n\
				const vec4 vertices11[4]= vec4[4](vec4(-1.5,0,0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-0.5,0,1.5,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices11[i]+seed;\n\
gl_PrimitiveID = 10;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 2\n\
				const vec4 vertices12[4]= vec4[4](vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5,0,1.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices12[i]+seed;\n\
gl_PrimitiveID = 11;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 3\n\
				const vec4 vertices13[4]= vec4[4](vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5,0,-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices13[i]+seed;\n\
gl_PrimitiveID = 12;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
////CARA LATERAL 4\n\
				const vec4 vertices14[4]= vec4[4](vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices14[i]+seed;\n\
gl_PrimitiveID = 13;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};
		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		uniform float time;\n\
		\n\
		void main() {\n\
	   color = vec4(0.5 + 0.5 * sin(3*time),1,0.5 + 0.5 * sin(3*time),1); \n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seedR.x, (GLfloat)seedR.y, (GLfloat)seedR.z, (GLfloat)seedR.w);

		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));

		locationID = glGetUniformLocation(myRenderProgram, "rotation");

		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		float green = 0.5f + 0.5f*sin(3.f*currentTime);
		glUniform4f(glGetUniformLocation(myRenderProgram, "color"), 0.f, 0.f, 0.f, 0.f);

		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seedR.x, (GLfloat)seedR.y, (GLfloat)seedR.z, (GLfloat)seedR.w);

		glUniform1f(glGetUniformLocation(myRenderProgram, "time"), (GLfloat)currentTime);

		glDrawArrays(GL_TRIANGLES, 0, 3);


	}


}

namespace Exercise5Visual {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{ "#version 330 \n\
			uniform mat4 mvp;\n\
			uniform mat4 rot;\n\
			uniform vec4 seed;\n\
			layout(lines) in;\n\
			layout(line_strip, max_vertices = 72) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[7] = vec4[7](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,sqrt(3),0.5,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5, sqrt(3), -0.5, 1.0));\n\
				\n\
//CARA 1\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices[i]+seed;\n\
gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
//CARA 2\n\
				const vec4 vertices2[7]= vec4[7](vec4(-0.5,sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,sqrt(3),0.5,1),\n\
										vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0),\n\
										vec4(-0.5, 0, 1.5, 1.0),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-0.5,sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices2[i]+seed;\n\
gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 3\n\
				const vec4 vertices3[7]= vec4[7](vec4(0.5, sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5, sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices3[i]+seed;\n\
gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 4\n\
				const vec4 vertices4[7]= vec4[7](vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(-0.5,sqrt(3),-0.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(0.5,sqrt(3),-0.5,1));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices4[i]+seed;\n\
gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 5\n\
				const vec4 vertices5[7]= vec4[7](vec4(-0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5, -sqrt(3), -0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices5[i]+seed;\n\
gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 6\n\
				const vec4 vertices6[7]= vec4[7](vec4(-0.5,-sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,-sqrt(3),0.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0),\n\
										vec4(-0.5, 0, 1.5, 1.0),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-0.5,-sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices6[i]+seed;\n\
gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 7\n\
				const vec4 vertices7[7]= vec4[7](vec4(0.5, -sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1, -sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5, -sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices7[i]+seed;\n\
gl_PrimitiveID = 6;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 8\n\
				const vec4 vertices8[7]= vec4[7](vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*rot*vertices8[i]+seed;\n\
gl_PrimitiveID = 7;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};

		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[14] = vec4[14](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,0,1),vec4(0,1,0,1),vec4(1,0,0,1),vec4(1,1,0,1),vec4(0,0,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1));\n\
		color = vec4(0,0,0,1);\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glm::mat4 tMatrix = { cos(currentTime),0,-sin(currentTime),0,0,1,0,0,sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		locationID = glGetUniformLocation(myRenderProgram, "rot");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seedR.x, (GLfloat)seedR.y, (GLfloat)seedR.z, 1.0);
		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_LINE_STRIP, 0, 2);


	}


}

namespace Exercise6 {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{ "#version 330 \n\
			uniform mat4 mvp;\n\
			uniform vec4 seed;\n\
			layout(lines) in;\n\
			layout(line_strip, max_vertices = 72) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[7] = vec4[7](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,sqrt(3),0.5,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5, sqrt(3), -0.5, 1.0));\n\
				\n\
//CARA 1\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices[i]+seed);\n\
gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
//CARA 2\n\
				const vec4 vertices2[7]= vec4[7](vec4(-0.5,sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,sqrt(3),0.5,1),\n\
										vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0),\n\
										vec4(-0.5, 0, 1.5, 1.0),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-0.5,sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices2[i]+seed);\n\
gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 3\n\
				const vec4 vertices3[7]= vec4[7](vec4(0.5, sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5, sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices3[i]+seed);\n\
gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 4\n\
				const vec4 vertices4[7]= vec4[7](vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(-0.5,sqrt(3),-0.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(0.5,sqrt(3),-0.5,1));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices4[i]+seed);\n\
gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 5\n\
				const vec4 vertices5[7]= vec4[7](vec4(-0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5, -sqrt(3), -0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices5[i]+seed);\n\
gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 6\n\
				const vec4 vertices6[7]= vec4[7](vec4(-0.5,-sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,-sqrt(3),0.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0),\n\
										vec4(-0.5, 0, 1.5, 1.0),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-0.5,-sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices6[i]+seed);\n\
gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 7\n\
				const vec4 vertices7[7]= vec4[7](vec4(0.5, -sqrt(3), 0.5, 1.0),\n\
										vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1, -sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5, -sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices7[i]+seed);\n\
gl_PrimitiveID = 6;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 8\n\
				const vec4 vertices8[7]= vec4[7](vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<7; i++)\n\
				{\n\
					gl_Position = mvp*(vertices8[i]+seed);\n\
gl_PrimitiveID = 7;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};

		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[14] = vec4[14](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,0,1),vec4(0,1,0,1),vec4(1,0,0,1),vec4(1,1,0,1),vec4(0,0,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1));\n\
		color = vec4(0,1,0,1);\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glm::mat4 tMatrix = { cos(currentTime),0,-sin(currentTime),0,0,1,0,0,sin(currentTime),0,cos(currentTime),0,0,0,0,1 };
		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seed.x, (GLfloat)seed.y, (GLfloat)seed.z, 1.0);
		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_LINE_STRIP, 0, 2);


	}


}

namespace Exercise6b {
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec3 Pos;

	void myCleanupCode(void) {
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
	GLuint myShaderCompile(void) {
		static const GLchar * vertex_shader_source[] =
		{
			"#version 330\n\
		\n\
		void main() {\n\
		const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25,0.5, 1.0),\n\
									   vec4(0.25, 0.25, 0.5, 1.0),\n\
										vec4(-0.25, -0.25, 0.5, 1.0));\n\
		gl_Position = vertices[gl_VertexID];\n\
		}"
		};

		static const GLchar* geom_shader_source[] =
		{ "#version 330 \n\
			uniform mat4 mvp;\n\
			uniform mat4 rotation;\n\
			uniform vec4 seed;\n\
			layout(triangles) in;\n\
			layout(triangle_strip, max_vertices = 72) out;\n\
			void main()\n\
			{\n\
				const vec4 vertices[6] = vec4[6](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-1.5,0,0.5,1));\n\
				\n\
//CARA 1\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices[i]+seed;\n\
gl_PrimitiveID = 0;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
				\n\
//CARA 2\n\
				const vec4 vertices2[6]= vec4[6](vec4(-0.5,sqrt(3), 0.5, 1.0),\n\
										vec4(-1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),0.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5, 0, 1.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices2[i]+seed;\n\
gl_PrimitiveID = 1;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 3\n\
				const vec4 vertices3[6]= vec4[6](vec4(0.5, sqrt(3), 0.5, 1.0),\n\
										vec4(1, sqrt(3)/2, 1, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5, 0, -0.5, 1.0));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices3[i]+seed;\n\
gl_PrimitiveID = 2;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 4\n\
				const vec4 vertices4[6]= vec4[6](vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1),\n\
										vec4(-0.5,0,-1.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices4[i]+seed;\n\
gl_PrimitiveID = 3;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA SUPERIOR\n\
		const vec4 vertices9[4]= vec4[4](vec4(-0.5, sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5, sqrt(3),0.5, 1.0),\n\
										vec4(0.5,sqrt(3),-0.5,1),\n\
										vec4(0.5, sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices9[i]+seed;\n\
gl_PrimitiveID = 8;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 5\n\
				const vec4 vertices5[6]= vec4[6](vec4(-0.5,0,-1.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices5[i]+seed;\n\
gl_PrimitiveID = 4;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 6\n\
				const vec4 vertices6[6]= vec4[6](vec4(1.5, 0, -0.5, 1.0),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),-0.5,1),\n\
										vec4(0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices6[i]+seed;\n\
gl_PrimitiveID = 5;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 7\n\
				const vec4 vertices7[6]= vec4[6](vec4(0.5,0,1.5,1),\n\
										vec4(-0.5,0,1.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices7[i]+seed;\n\
gl_PrimitiveID = 6;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA 8\n\
				const vec4 vertices8[6]= vec4[6](vec4(-1.5,0,0.5,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(-0.5,-sqrt(3),-0.5,1));\n\
				for (int i = 0; i<6; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices8[i]+seed;\n\
gl_PrimitiveID = 7;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA TAPA INFERIOR\n\
				const vec4 vertices10[4]= vec4[4](vec4(-0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(0.5, -sqrt(3), -0.5, 1.0),\n\
										vec4(-0.5,-sqrt(3),0.5,1),\n\
										vec4(0.5, -sqrt(3), 0.5, 1.0));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices10[i]+seed;\n\
gl_PrimitiveID = 9;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 1\n\
				const vec4 vertices11[4]= vec4[4](vec4(-1.5,0,0.5,1),\n\
										vec4(-1,-sqrt(3)/2,1,1),\n\
										vec4(-1,sqrt(3)/2,1,1),\n\
										vec4(-0.5,0,1.5,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices11[i]+seed;\n\
gl_PrimitiveID = 10;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 2\n\
				const vec4 vertices12[4]= vec4[4](vec4(1,sqrt(3)/2,1,1),\n\
										vec4(0.5,0,1.5,1),\n\
										vec4(1.5,0,0.5,1),\n\
										vec4(1,-sqrt(3)/2,1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices12[i]+seed;\n\
gl_PrimitiveID = 11;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
//CARA LATERAL 3\n\
				const vec4 vertices13[4]= vec4[4](vec4(1,sqrt(3)/2,-1,1),\n\
										vec4(1.5,0,-0.5,1),\n\
										vec4(0.5,0,-1.5,1),\n\
										vec4(1,-sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices13[i]+seed;\n\
gl_PrimitiveID = 12;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
////CARA LATERAL 4\n\
				const vec4 vertices14[4]= vec4[4](vec4(-1,-sqrt(3)/2,-1,1),\n\
										vec4(-1.5,0,-0.5,1),\n\
										vec4(-0.5,0,-1.5,1),\n\
										vec4(-1,sqrt(3)/2,-1,1));\n\
				for (int i = 0; i<4; i++)\n\
				{\n\
					gl_Position = mvp*rotation*vertices14[i]+seed;\n\
gl_PrimitiveID = 13;\n\
					EmitVertex();\n\
				}\n\
				EndPrimitive();\n\
			}"
		};

		static const GLchar * fragment_shader_source[] =
		{
			"#version 330\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
const vec4 colors[14] = vec4[14](vec4(1,0,0,1),vec4(0,1,0,1),vec4(0,0,1,1),vec4(1,1,0,1),vec4(0,1,0,1),vec4(1,0,0,1),vec4(1,1,0,1),vec4(0,0,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1),vec4(1,1,1,1));\n\
		color = vec4(0,1,0,1);\n\
		}"
		};



		GLuint geom_shader;
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);


		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geom_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}


	void  myInitCode(void) {

		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);


	}


	void myRenderCode(double currentTime) {

		glUseProgram(myRenderProgram);
		glUniform4f(glGetUniformLocation(myRenderProgram, "seed"), (GLfloat)seedR.x, (GLfloat)seedR.y, (GLfloat)seedR.z, (GLfloat)seedR.w);

		GLint locationID = glGetUniformLocation(myRenderProgram, "mvp");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(RV::_MVP));

		locationID = glGetUniformLocation(myRenderProgram, "rotation");
		glUniformMatrix4fv(locationID, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

		glUniform1f(glGetUniformLocation(myRenderProgram, "color"), 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


	}


}

////////////////////////////////////////////////// BOX
namespace Box {
	GLuint cubeVao;
	GLuint cubeVbo[2];
	GLuint cubeShaders[2];
	GLuint cubeProgram;

	float cubeVerts[] = {
		// -5,0,-5 -- 5, 10, 5
		-5.f,  0.f, -5.f,
		5.f,  0.f, -5.f,
		5.f,  0.f,  5.f,
		-5.f,  0.f,  5.f,
		-5.f, 10.f, -5.f,
		5.f, 10.f, -5.f,
		5.f, 10.f,  5.f,
		-5.f, 10.f,  5.f,
	};
	GLubyte cubeIdx[] = {
		1, 0, 2, 3, // Floor - TriangleStrip
		0, 1, 5, 4, // Wall - Lines
		1, 2, 6, 5, // Wall - Lines
		2, 3, 7, 6, // Wall - Lines
		3, 0, 4, 7  // Wall - Lines
	};

	const char* vertShader_xform =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* fragShader_flatColor =
		"#version 330\n\
out vec4 out_Color;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = color;\n\
}";

	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(2, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 20, cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(vertShader_xform, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(fragShader_flatColor, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		linkProgram(cubeProgram);
	}
	void cleanupCube() {
		glDeleteBuffers(2, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);
	}
	void drawCube() {
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		// FLOOR
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.6f, 0.6f, 0.6f, 1.f);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
		// WALLS
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.f, 0.f, 0.f, 1.f);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 4));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 8));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 12));
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(sizeof(GLubyte) * 16));

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// AXIS
namespace Axis {
	GLuint AxisVao;
	GLuint AxisVbo[3];
	GLuint AxisShader[2];
	GLuint AxisProgram;

	float AxisVerts[] = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0
	};
	float AxisColors[] = {
		1.0, 0.0, 0.0, 1.0,
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		0.0, 0.0, 1.0, 1.0
	};
	GLubyte AxisIdx[] = {
		0, 1,
		2, 3,
		4, 5
	};
	const char* Axis_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
in vec4 in_Color;\n\
out vec4 vert_color;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	vert_color = in_Color;\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* Axis_fragShader =
		"#version 330\n\
in vec4 vert_color;\n\
out vec4 out_Color;\n\
void main() {\n\
	out_Color = vert_color;\n\
}";

	void setupAxis() {
		glGenVertexArrays(1, &AxisVao);
		glBindVertexArray(AxisVao);
		glGenBuffers(3, AxisVbo);

		glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, AxisVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, AxisColors, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AxisVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, AxisIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		AxisShader[0] = compileShader(Axis_vertShader, GL_VERTEX_SHADER, "AxisVert");
		AxisShader[1] = compileShader(Axis_fragShader, GL_FRAGMENT_SHADER, "AxisFrag");

		AxisProgram = glCreateProgram();
		glAttachShader(AxisProgram, AxisShader[0]);
		glAttachShader(AxisProgram, AxisShader[1]);
		glBindAttribLocation(AxisProgram, 0, "in_Position");
		glBindAttribLocation(AxisProgram, 1, "in_Color");
		linkProgram(AxisProgram);
	}
	void cleanupAxis() {
		glDeleteBuffers(3, AxisVbo);
		glDeleteVertexArrays(1, &AxisVao);

		glDeleteProgram(AxisProgram);
		glDeleteShader(AxisShader[0]);
		glDeleteShader(AxisShader[1]);
	}
	void drawAxis() {
		glBindVertexArray(AxisVao);
		glUseProgram(AxisProgram);
		glUniformMatrix4fv(glGetUniformLocation(AxisProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// SPHERE
namespace Sphere {
	GLuint sphereVao;
	GLuint sphereVbo;
	GLuint sphereShaders[3];
	GLuint sphereProgram;
	float radius;

	const char* sphere_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mv_Mat;\n\
void main() {\n\
	gl_Position = mv_Mat * vec4(in_Position, 1.0);\n\
}";
	const char* sphere_geomShader =
		"#version 330\n\
layout(points) in;\n\
layout(triangle_strip, max_vertices = 4) out;\n\
out vec4 eyePos;\n\
out vec4 centerEyePos;\n\
uniform mat4 projMat;\n\
uniform float radius;\n\
vec4 nu_verts[4];\n\
void main() {\n\
	vec3 n = normalize(-gl_in[0].gl_Position.xyz);\n\
	vec3 up = vec3(0.0, 1.0, 0.0);\n\
	vec3 u = normalize(cross(up, n));\n\
	vec3 v = normalize(cross(n, u));\n\
	nu_verts[0] = vec4(-radius*u - radius*v, 0.0); \n\
	nu_verts[1] = vec4( radius*u - radius*v, 0.0); \n\
	nu_verts[2] = vec4(-radius*u + radius*v, 0.0); \n\
	nu_verts[3] = vec4( radius*u + radius*v, 0.0); \n\
	centerEyePos = gl_in[0].gl_Position;\n\
	for (int i = 0; i < 4; ++i) {\n\
		eyePos = (gl_in[0].gl_Position + nu_verts[i]);\n\
		gl_Position = projMat * eyePos;\n\
		EmitVertex();\n\
	}\n\
	EndPrimitive();\n\
}";
	const char* sphere_fragShader_flatColor =
		"#version 330\n\
in vec4 eyePos;\n\
in vec4 centerEyePos;\n\
out vec4 out_Color;\n\
uniform mat4 projMat;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
uniform float radius;\n\
void main() {\n\
	vec4 diff = eyePos - centerEyePos;\n\
	float distSq2C = dot(diff, diff);\n\
	if (distSq2C > (radius*radius)) discard;\n\
	float h = sqrt(radius*radius - distSq2C);\n\
	vec4 nuEyePos = vec4(eyePos.xy, eyePos.z + h, 1.0);\n\
	vec4 nuPos = projMat * nuEyePos;\n\
	gl_FragDepth = ((nuPos.z / nuPos.w) + 1) * 0.5;\n\
	vec3 normal = normalize(nuEyePos - centerEyePos).xyz;\n\
	out_Color = vec4(color.xyz * dot(normal, (mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)).xyz) + color.xyz * 0.3, 1.0 );\n\
}";

	bool shadersCreated = false;
	void createSphereShaderAndProgram() {
		if (shadersCreated) return;

		sphereShaders[0] = compileShader(sphere_vertShader, GL_VERTEX_SHADER, "sphereVert");
		sphereShaders[1] = compileShader(sphere_geomShader, GL_GEOMETRY_SHADER, "sphereGeom");
		sphereShaders[2] = compileShader(sphere_fragShader_flatColor, GL_FRAGMENT_SHADER, "sphereFrag");

		sphereProgram = glCreateProgram();
		glAttachShader(sphereProgram, sphereShaders[0]);
		glAttachShader(sphereProgram, sphereShaders[1]);
		glAttachShader(sphereProgram, sphereShaders[2]);
		glBindAttribLocation(sphereProgram, 0, "in_Position");
		linkProgram(sphereProgram);

		shadersCreated = true;
	}
	void cleanupSphereShaderAndProgram() {
		if (!shadersCreated) return;
		glDeleteProgram(sphereProgram);
		glDeleteShader(sphereShaders[0]);
		glDeleteShader(sphereShaders[1]);
		glDeleteShader(sphereShaders[2]);
		shadersCreated = false;
	}

	void setupSphere(glm::vec3 pos, float radius) {
		Sphere::radius = radius;
		glGenVertexArrays(1, &sphereVao);
		glBindVertexArray(sphereVao);
		glGenBuffers(1, &sphereVbo);

		glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &pos, GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		createSphereShaderAndProgram();
	}
	void cleanupSphere() {
		glDeleteBuffers(1, &sphereVbo);
		glDeleteVertexArrays(1, &sphereVao);

		cleanupSphereShaderAndProgram();
	}
	void updateSphere(glm::vec3 pos, float radius) {
		glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
		float* buff = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		buff[0] = pos.x;
		buff[1] = pos.y;
		buff[2] = pos.z;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Sphere::radius = radius;
	}
	void drawSphere() {
		glBindVertexArray(sphereVao);
		glUseProgram(sphereProgram);
		glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RV::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "projMat"), 1, GL_FALSE, glm::value_ptr(RV::_projection));
		glUniform4f(glGetUniformLocation(sphereProgram, "color"), 0.6f, 0.1f, 0.1f, 1.f);
		glUniform1f(glGetUniformLocation(sphereProgram, "radius"), Sphere::radius);
		glDrawArrays(GL_POINTS, 0, 1);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// CAPSULE
namespace Capsule {
	GLuint capsuleVao;
	GLuint capsuleVbo[2];
	GLuint capsuleShader[3];
	GLuint capsuleProgram;
	float radius;

	const char* capsule_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mv_Mat;\n\
void main() {\n\
	gl_Position = mv_Mat * vec4(in_Position, 1.0);\n\
}";
	const char* capsule_geomShader =
		"#version 330\n\
layout(lines) in; \n\
layout(triangle_strip, max_vertices = 14) out;\n\
out vec3 eyePos;\n\
out vec3 capPoints[2];\n\
uniform mat4 projMat;\n\
uniform float radius;\n\
vec3 boxVerts[8];\n\
int boxIdx[14];\n\
void main(){\n\
	vec3 A = gl_in[0].gl_Position.xyz;\n\
	vec3 B = gl_in[1].gl_Position.xyz;\n\
	if(gl_in[1].gl_Position.x < gl_in[0].gl_Position.x) {\n\
		A = gl_in[1].gl_Position.xyz;\n\
		B = gl_in[0].gl_Position.xyz;\n\
	}\n\
	vec3 u = vec3(0.0, 1.0, 0.0);\n\
	if (abs(dot(u, normalize(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz))) - 1.0 < 1e-6) {\n\
		if(gl_in[1].gl_Position.y > gl_in[0].gl_Position.y) {\n\
			A = gl_in[1].gl_Position.xyz;\n\
			B = gl_in[0].gl_Position.xyz;\n\
		}\n\
		u = vec3(1.0, 0.0, 0.0);\n\
	}\n\
	vec3 Am = normalize(A - B); \n\
	vec3 Bp = -Am;\n\
	vec3 v = normalize(cross(Am, u)) * radius;\n\
	u = normalize(cross(v, Am)) * radius;\n\
	Am *= radius;\n\
	Bp *= radius;\n\
	boxVerts[0] = A + Am - u - v;\n\
	boxVerts[1] = A + Am + u - v;\n\
	boxVerts[2] = A + Am + u + v;\n\
	boxVerts[3] = A + Am - u + v;\n\
	boxVerts[4] = B + Bp - u - v;\n\
	boxVerts[5] = B + Bp + u - v;\n\
	boxVerts[6] = B + Bp + u + v;\n\
	boxVerts[7] = B + Bp - u + v;\n\
	boxIdx = int[](0, 3, 4, 7, 6, 3, 2, 1, 6, 5, 4, 1, 0, 3);\n\
	capPoints[0] = A;\n\
	capPoints[1] = B;\n\
	for (int i = 0; i<14; ++i) {\n\
		eyePos = boxVerts[boxIdx[i]];\n\
		gl_Position = projMat * vec4(boxVerts[boxIdx[i]], 1.0);\n\
		EmitVertex();\n\
	}\n\
	EndPrimitive();\n\
}";
	const char* capsule_fragShader_flatColor =
		"#version 330\n\
in vec3 eyePos;\n\
in vec3 capPoints[2];\n\
out vec4 out_Color;\n\
uniform mat4 projMat;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
uniform float radius;\n\
const int lin_steps = 30;\n\
const int bin_steps = 5;\n\
vec3 closestPointInSegment(vec3 p, vec3 a, vec3 b) {\n\
	vec3 pa = p - a, ba = b - a;\n\
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);\n\
	return a + ba*h;\n\
}\n\
void main() {\n\
	vec3 viewDir = normalize(eyePos);\n\
	float step = radius / 5.0;\n\
	vec3 nuPB = eyePos;\n\
	int i = 0;\n\
	for(i = 0; i < lin_steps; ++i) {\n\
		nuPB = eyePos + viewDir*step*i;\n\
		vec3 C = closestPointInSegment(nuPB, capPoints[0], capPoints[1]);\n\
		float dist = length(C - nuPB) - radius;\n\
		if(dist < 0.0) break;\n\
	}\n\
	if(i==lin_steps) discard;\n\
	vec3 nuPA = nuPB - viewDir*step;\n\
	vec3 C;\n\
	for(i = 0; i < bin_steps; ++i) {\n\
		vec3 nuPC = nuPA + (nuPB - nuPA)*0.5; \n\
		C = closestPointInSegment(nuPC, capPoints[0], capPoints[1]); \n\
		float dist = length(C - nuPC) - radius; \n\
		if(dist > 0.0) nuPA = nuPC; \n\
		else nuPB = nuPC; \n\
	}\n\
	vec4 nuPos = projMat * vec4(nuPA, 1.0);\n\
	gl_FragDepth = ((nuPos.z / nuPos.w) + 1) * 0.5;\n\
	vec3 normal = normalize(nuPA - C);\n\
	out_Color = vec4(color.xyz * dot(normal, (mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)).xyz) + color.xyz * 0.3, 1.0 );\n\
}";

	void setupCapsule(glm::vec3 posA, glm::vec3 posB, float radius) {
		Capsule::radius = radius;
		glGenVertexArrays(1, &capsuleVao);
		glBindVertexArray(capsuleVao);
		glGenBuffers(2, capsuleVbo);

		float capsuleVerts[] = {
			posA.x, posA.y, posA.z,
			posB.x, posB.y, posB.z
		};
		GLubyte capsuleIdx[] = {
			0, 1
		};

		glBindBuffer(GL_ARRAY_BUFFER, capsuleVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, capsuleVerts, GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, capsuleVbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 2, capsuleIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		capsuleShader[0] = compileShader(capsule_vertShader, GL_VERTEX_SHADER, "capsuleVert");
		capsuleShader[1] = compileShader(capsule_geomShader, GL_GEOMETRY_SHADER, "capsuleGeom");
		capsuleShader[2] = compileShader(capsule_fragShader_flatColor, GL_FRAGMENT_SHADER, "capsuleFrag");

		capsuleProgram = glCreateProgram();
		glAttachShader(capsuleProgram, capsuleShader[0]);
		glAttachShader(capsuleProgram, capsuleShader[1]);
		glAttachShader(capsuleProgram, capsuleShader[2]);
		glBindAttribLocation(capsuleProgram, 0, "in_Position");
		linkProgram(capsuleProgram);
	}
	void cleanupCapsule() {
		glDeleteBuffers(2, capsuleVbo);
		glDeleteVertexArrays(1, &capsuleVao);

		glDeleteProgram(capsuleProgram);
		glDeleteShader(capsuleShader[0]);
		glDeleteShader(capsuleShader[1]);
		glDeleteShader(capsuleShader[2]);
	}
	void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius) {
		float vertPos[] = { posA.x, posA.y, posA.z, posB.z, posB.y, posB.z };
		glBindBuffer(GL_ARRAY_BUFFER, capsuleVbo[0]);
		float* buff = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		buff[0] = posA.x; buff[1] = posA.y; buff[2] = posA.z;
		buff[3] = posB.x; buff[4] = posB.y; buff[5] = posB.z;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Capsule::radius = radius;
	}
	void drawCapsule() {
		glBindVertexArray(capsuleVao);
		glUseProgram(capsuleProgram);
		glUniformMatrix4fv(glGetUniformLocation(capsuleProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(capsuleProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RV::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(capsuleProgram, "projMat"), 1, GL_FALSE, glm::value_ptr(RV::_projection));
		glUniform4fv(glGetUniformLocation(capsuleProgram, "camPoint"), 1, &RV::_cameraPoint[0]);
		glUniform4f(glGetUniformLocation(capsuleProgram, "color"), 0.1f, 0.6f, 0.1f, 1.f);
		glUniform1f(glGetUniformLocation(capsuleProgram, "radius"), Capsule::radius);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// PARTICLES
// Same rendering as Sphere (reusing shaders)
namespace LilSpheres {
	GLuint particlesVao;
	GLuint particlesVbo;
	float radius;
	int numparticles;
	extern const int maxParticles = SHRT_MAX;

	void setupParticles(int numTotalParticles, float radius) {
		assert(numTotalParticles > 0);
		assert(numTotalParticles <= SHRT_MAX);
		numparticles = numTotalParticles;
		LilSpheres::radius = radius;

		glGenVertexArrays(1, &particlesVao);
		glBindVertexArray(particlesVao);
		glGenBuffers(1, &particlesVbo);

		glBindBuffer(GL_ARRAY_BUFFER, particlesVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numparticles, 0, GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		Sphere::createSphereShaderAndProgram();
	}
	void cleanupParticles() {
		glDeleteVertexArrays(1, &particlesVao);
		glDeleteBuffers(1, &particlesVbo);

		Sphere::cleanupSphereShaderAndProgram();
	}
	void updateParticles(int startIdx, int count, float* array_data) {
		glBindBuffer(GL_ARRAY_BUFFER, particlesVbo);
		float* buff = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		buff = &buff[3 * startIdx];
		for (int i = 0; i < 3 * count; ++i) {
			buff[i] = array_data[i];
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void drawParticles(int startIdx, int count) {
		glBindVertexArray(particlesVao);
		glUseProgram(Sphere::sphereProgram);
		glUniformMatrix4fv(glGetUniformLocation(Sphere::sphereProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniformMatrix4fv(glGetUniformLocation(Sphere::sphereProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RV::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(Sphere::sphereProgram, "projMat"), 1, GL_FALSE, glm::value_ptr(RV::_projection));
		glUniform4f(glGetUniformLocation(Sphere::sphereProgram, "color"), 0.1f, 0.1f, 0.6f, 1.f);
		glUniform1f(glGetUniformLocation(Sphere::sphereProgram, "radius"), LilSpheres::radius);
		glDrawArrays(GL_POINTS, startIdx, count);

		glUseProgram(0);
		glBindVertexArray(0);
	}
}

////////////////////////////////////////////////// CLOTH
namespace ClothMesh {
	GLuint clothVao;
	GLuint clothVbo[2];
	GLuint clothShaders[2];
	GLuint clothProgram;
	extern const int numCols = 14;
	extern const int numRows = 18;
	extern const int numVerts = numRows * numCols;
	int numVirtualVerts;

	const char* cloth_vertShader =
		"#version 330\n\
in vec3 in_Position;\n\
uniform mat4 mvpMat;\n\
void main() {\n\
	gl_Position = mvpMat * vec4(in_Position, 1.0);\n\
}";
	const char* cloth_fragShader =
		"#version 330\n\
uniform vec4 color;\n\
out vec4 out_Color;\n\
void main() {\n\
	out_Color = color;\n\
}";

	void setupClothMesh() {
		glGenVertexArrays(1, &clothVao);
		glBindVertexArray(clothVao);
		glGenBuffers(2, clothVbo);

		glBindBuffer(GL_ARRAY_BUFFER, clothVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * numVerts, 0, GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glPrimitiveRestartIndex(UCHAR_MAX);
		constexpr int facesVertsIdx = 5 * (numCols - 1) * (numRows - 1);
		GLubyte facesIdx[facesVertsIdx] = { 0 };
		for (int i = 0; i < (numRows - 1); ++i) {
			for (int j = 0; j < (numCols - 1); ++j) {
				facesIdx[5 * (i*(numCols - 1) + j) + 0] = i * numCols + j;
				facesIdx[5 * (i*(numCols - 1) + j) + 1] = (i + 1)*numCols + j;
				facesIdx[5 * (i*(numCols - 1) + j) + 2] = (i + 1)*numCols + (j + 1);
				facesIdx[5 * (i*(numCols - 1) + j) + 3] = i * numCols + (j + 1);
				facesIdx[5 * (i*(numCols - 1) + j) + 4] = UCHAR_MAX;
			}
		}
		numVirtualVerts = facesVertsIdx;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clothVbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*numVirtualVerts, facesIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		clothShaders[0] = compileShader(cloth_vertShader, GL_VERTEX_SHADER, "clothVert");
		clothShaders[1] = compileShader(cloth_fragShader, GL_FRAGMENT_SHADER, "clothFrag");

		clothProgram = glCreateProgram();
		glAttachShader(clothProgram, clothShaders[0]);
		glAttachShader(clothProgram, clothShaders[1]);
		glBindAttribLocation(clothProgram, 0, "in_Position");
		linkProgram(clothProgram);
	}
	void cleanupClothMesh() {
		glDeleteBuffers(2, clothVbo);
		glDeleteVertexArrays(1, &clothVao);

		glDeleteProgram(clothProgram);
		glDeleteShader(clothShaders[0]);
		glDeleteShader(clothShaders[1]);
	}
	void updateClothMesh(float *array_data) {
		glBindBuffer(GL_ARRAY_BUFFER, clothVbo[0]);
		float* buff = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < 3 * numVerts; ++i) {
			buff[i] = array_data[i];
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void drawClothMesh() {
		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(clothVao);
		glUseProgram(clothProgram);
		glUniformMatrix4fv(glGetUniformLocation(clothProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glUniform4f(glGetUniformLocation(clothProgram, "color"), 0.1f, 1.f, 1.f, 0.f);
		glDrawElements(GL_LINE_LOOP, numVirtualVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}
}

////////////////////////////////////////////////// CUBE
namespace Cube {
	GLuint cubeVao;
	GLuint cubeVbo[3];
	GLuint cubeShaders[2];
	GLuint cubeProgram;
	glm::mat4 objMat = glm::mat4(1.f);

	extern const float halfW = 0.5f;
	int numVerts = 24 + 6; // 4 vertex/face * 6 faces + 6 PRIMITIVE RESTART

						   //   4---------7
						   //  /|        /|
						   // / |       / |
						   //5---------6  |
						   //|  0------|--3
						   //| /       | /
						   //|/        |/
						   //1---------2
	glm::vec3 verts[] = {
		glm::vec3(-halfW, -halfW, -halfW),
		glm::vec3(-halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW, -halfW),
		glm::vec3(-halfW,  halfW, -halfW),
		glm::vec3(-halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW, -halfW)
	};
	glm::vec3 norms[] = {
		glm::vec3(0.f, -1.f,  0.f),
		glm::vec3(0.f,  1.f,  0.f),
		glm::vec3(-1.f,  0.f,  0.f),
		glm::vec3(1.f,  0.f,  0.f),
		glm::vec3(0.f,  0.f, -1.f),
		glm::vec3(0.f,  0.f,  1.f)
	};

	glm::vec3 cubeVerts[] = {
		verts[1], verts[0], verts[2], verts[3],
		verts[5], verts[6], verts[4], verts[7],
		verts[1], verts[5], verts[0], verts[4],
		verts[2], verts[3], verts[6], verts[7],
		verts[0], verts[4], verts[3], verts[7],
		verts[1], verts[2], verts[5], verts[6]
	};
	glm::vec3 cubeNorms[] = {
		norms[0], norms[0], norms[0], norms[0],
		norms[1], norms[1], norms[1], norms[1],
		norms[2], norms[2], norms[2], norms[2],
		norms[3], norms[3], norms[3], norms[3],
		norms[4], norms[4], norms[4], norms[4],
		norms[5], norms[5], norms[5], norms[5]
	};
	GLubyte cubeIdx[] = {
		0, 1, 2, 3, UCHAR_MAX,
		4, 5, 6, 7, UCHAR_MAX,
		8, 9, 10, 11, UCHAR_MAX,
		12, 13, 14, 15, UCHAR_MAX,
		16, 17, 18, 19, UCHAR_MAX,
		20, 21, 22, 23, UCHAR_MAX
	};




	const char* cube_vertShader =
		"#version 330\n\
	in vec3 in_Position;\n\
	in vec3 in_Normal;\n\
	out vec4 vert_Normal;\n\
	uniform mat4 objMat;\n\
	uniform mat4 mv_Mat;\n\
	uniform mat4 mvpMat;\n\
	void main() {\n\
		gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);\n\
		vert_Normal = mv_Mat * objMat * vec4(in_Normal, 0.0);\n\
	}";


	const char* cube_fragShader =
		"#version 330\n\
in vec4 vert_Normal;\n\
out vec4 out_Color;\n\
uniform mat4 mv_Mat;\n\
uniform vec4 color;\n\
void main() {\n\
	out_Color = vec4(color.xyz * dot(vert_Normal, mv_Mat*vec4(0.0, 1.0, 0.0, 0.0)) + color.xyz * 0.3, 1.0 );\n\
}";
	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(3, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNorms), cubeNorms, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glPrimitiveRestartIndex(UCHAR_MAX);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(cube_vertShader, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(cube_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		glBindAttribLocation(cubeProgram, 1, "in_Normal");
		linkProgram(cubeProgram);
	}
	void cleanupCube() {
		glDeleteBuffers(3, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);
	}
	void updateCube(const glm::mat4& transform) {
		objMat = transform;
	}
	void drawCube() {
		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), 0.1f, 1.f, 1.f, 0.f);
		glDrawElements(GL_TRIANGLE_STRIP, numVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}


}


////////////////////////////////////////// MY FIRST SHADER

//namespace MyFirstShader {
//
//
//	// 1.  declare shader
//	static const GLchar * vertex_shader_source[] =
//	{
//		"#version 330\n\
//		\n\
//		void main(){\n\
//		gl_Position = vec4(0.0,0.0,0.5,1.0);\n\
//		}"
//	};
//
//	static const GLchar * fragment_shader_source[] =
//	{
//		"#version 330\n\
//		\n\
//		out vec4 color; \n\
//		\n\
//		void main(){\n\
//		color = vec4(0.0,0.8,1.0,1.0);\n\
//		}"
//	};
//
//
//
//
//	//2. compile and link shader
//	GLuint myShaderCompile(void) {
//		GLuint vertex_shader;
//		GLuint fragment_shader;
//		GLuint program;
//


