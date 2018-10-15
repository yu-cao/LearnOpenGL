#include "RegisterInput.h"

#include "Defines.h"

#include <GLFW/Glfw.h>
#include <Utility/InfoLambdaOp.h>
#include <Camera.h>

using namespace Ubpa;
using namespace LOGL;
using namespace Define;
using namespace std;

void RegisterInput::Run() {
	RegisterMouse();
	RegisterKey();
	PrintInfo();
}

void RegisterInput::RegisterMouse() {
	EventManager::GetInstance()->Register(EventManager::MOUSE_MOUVE, [](){
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get(str_MainCamera);
		auto xOffset = **Storage<float *>::GetInstance()->Get("mousePos_XOffset");
		auto yOffset = **Storage<float *>::GetInstance()->Get("mousePos_YOffset");
		mainCamera->ProcessMouseMovement(xOffset, yOffset);
	});
	EventManager::GetInstance()->Register(EventManager::MOUSE_SCROLL, []() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get(str_MainCamera);
		auto mouseScroll_YOffset = **Storage<float *>::GetInstance()->Get("mouseScroll_YOffset");
		mainCamera->ProcessMouseScroll(mouseScroll_YOffset);
	});
}

void RegisterInput::RegisterKey() {
	// Close Window
	auto closeWindowOp = new LambdaOp();
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_ESCAPE,
		[]() { Glfw::GetInstance()->CloseWindow(); });


	// Polygon Mode
	//------------ GLFW_KEY_1
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_1,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
	//------------ GLFW_KEY_2
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_2,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });


	// Texture Warp : GLFW_KEY_3 ~ GLFW_KEY_6
	for (size_t i = 0; i < 4; i++) {
		EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | (GLFW_KEY_3 + i), []() {
			auto textureID = *Storage<size_t>::GetInstance()->Get(str_ChangeTexture);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		});
	}

	// Projection
	//------------ GLFW_KEY_7
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_7, 
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get(str_MainCamera);
		mainCamera->SetPerspective();
	});
	//------------ GLFW_KEY_8
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_8, 
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get(str_MainCamera);
		mainCamera->SetOrtho();
	});


	// Move
	size_t moveKey[] = { GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E };
	for (size_t i = 0; i < 6; i++) {
		EventManager::GetInstance()->Register(EventManager::KEYBOARD | moveKey[i],
			[=]() {
			auto mainCamera = *Storage<Camera *>::GetInstance()->Get(str_MainCamera);
			auto deltaTime = **Storage<float *>::GetInstance()->Get(str_DeltaTime);
			mainCamera->ProcessKeyboard(Camera::ENUM_Movement(Camera::MOVE_FORWARD + i), deltaTime);
		});
	}
}

void RegisterInput::PrintInfo() {
	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press '3' to set TEXTURE_WRAP[REPEAT]" << endl
		<< "* 4. Press '4' to set TEXTURE_WRAP[MIRRORED_REPEAT]" << endl
		<< "* 5. Press '5' to set TEXTURE_WRAP[CLAMP_TO_EDGE]" << endl
		<< "* 6. Press '6' to set TEXTURE_WRAP[CLAMP_TO_BORDER]" << endl
		<< "* 7. Press '7' to set projection[perspective]" << endl
		<< "* 8. Press '8' to set projection[ortho]" << endl
		<< "* 9. Press 'ESC' to close exe" << endl << endl;
}