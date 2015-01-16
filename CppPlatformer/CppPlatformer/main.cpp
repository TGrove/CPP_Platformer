#include "GameStateManager.h"
#include "GameState.h"
#include "SplashScreenGameState.h"
#include "InputManager.h"

#include <iostream>
//#include <vld.h>

int main(int argc, char *argv[]){
	std::cout << "Loading game" << std::endl;

	//load assests
		//keys into input manager
		//textures into asset manager
		//fonts into asset manager

	GameStateManager gm;
	gm.Init(800, 600, "Game State Managment Game");

	GameState *splash = SplashScreenGameState::GetInstance() ;

	gm.PushState(splash);

	int returnCode = gm.Loop();

	InputManager::GetInstance()->CleanUp();

	gm.DeInit();
	return returnCode;
}