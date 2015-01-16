#include "GameStateManager.h"
#include "GameState.h"
#include "InputManager.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

#include <string>
#include <iostream>

GameStateManager::GameStateManager(void) :  
	mGameIsRunning(true), mRequestedStateChange(false), mRequestedState(NULL)
{	
	mGameWindow = new sf::RenderWindow();
	mInputManager = InputManager::GetInstance();
}



GameStateManager::~GameStateManager(void)
{
	delete mGameWindow;
}



void GameStateManager::Init( unsigned int width, unsigned int height, std::string const &windowTitle)
{
	
	mGameWindow->create( sf::VideoMode(width, height), windowTitle );
	mGameWindow->setVisible(true);

	mInputManager = InputManager::GetInstance();

	mInputManager->RegisterKey(sf::Keyboard::Escape);
	//mInputManager->RegisterKey( sf::Keyboard::Key(36) );

}



//Clean up
void GameStateManager::DeInit()
{
	//clean up render window
	mGameWindow->close();

	//clean up states in the stack
	for(unsigned int i = 0; i < mGameStates.size(); i++ ){
		PopState();
	}
	mGameStates.clear();
}



void GameStateManager::RequestStateChange(GameState* newState){
	mRequestedStateChange = true;
	mRequestedState = newState;
}



void GameStateManager::ChangeState(GameState* newState)
{
	if(newState == NULL){
		std::cout << "GameStateManager::ChangeState: State is NULL" << std::endl;
		return;
	}

	//Pop
	if( !mGameStates.empty() ){
		mGameStates.back()->Pause();
		mGameStates.back()->DeInit();
		mGameStates.pop_back();
	}
	
	//Pause the current topstate if one exists (after the pop)
	if( !mGameStates.empty() ){
		mGameStates.back()->Pause();
	}

	//Push the new state onto the stack
	newState->Init( (*this) );
	newState->Resume();
	mGameStates.push_back(newState);

}



void GameStateManager::PushState(GameState* newState)
{
	//Check if pointer is valid
	if(newState == NULL){
		std::cout << "GameStateManager::PushState; State is NULL" << std::endl;
		return;
	}
	//Pause the current topstate if one exists
	if( !mGameStates.empty() ){
		mGameStates.back()->Pause();
	}
	//Push the new state onto the stack
	newState->Init( (*this) );
	mGameStates.push_back(newState);
}



void GameStateManager::PopState()
{
	//Check if there is at least one state, pop the top off the stack
	if( !mGameStates.empty() ){
		mGameStates.back()->Pause();
		mGameStates.back()->DeInit();
		mGameStates.pop_back();
	}
	//if there is still a state in the stack, resume it
	if( !mGameStates.empty() ){
		mGameStates.back()->Resume();
	}
}



int GameStateManager::GetWindowWidth(){
	return mGameWindow->getSize().x;
}



int GameStateManager::GetWindowHeight(){
	return mGameWindow->getSize().y;
}



int GameStateManager::Loop()
{
	//Create a clock, and set the game time and real world time to 0;
	sf::Clock clock;
	float gameTime = 0.f;
	float worldTime = 0.f;
	float lastFrame = clock.getElapsedTime().asSeconds();
	float deltaTime = 0.0f;
	//Calculate the fixed delta. Is this a good place to store this number?
	float fixedDelta = 1.f / 60.f;

	while(mGameIsRunning){
		//reset KeyStates from last frame
		mInputManager->ResetForNewFrame();

		//Process events from the window
		sf::Event anEvent;
		while( mGameWindow->pollEvent( anEvent ) ){
			switch( anEvent.type ){
			case sf::Event::Closed:
					mGameIsRunning = false;
					break;
				case sf::Event::GainedFocus:
					break;
				case sf::Event::LostFocus:
					break;
				case sf::Event::Resized:
					break;
				case sf::Event::KeyPressed:
					mInputManager->UpdateKeys(anEvent);
					break;
				case sf::Event::KeyReleased:
					mInputManager->UpdateKeys(anEvent);

					break;
				default:
					break;
			}//end switch anEvent
		}//end while polling

		//Check Close Game
		if ( mInputManager->KeyReleased(sf::Keyboard::Escape) ){
		//if ( mInputManager->KeyReleased(sf::Keyboard::Key(36)) ){
			mGameIsRunning = false;
		}

		//Gameplay update and draw section of loop
        //Only draw if the world has been updated on this tick.
        bool drawNeeded = false;

		//Normal Update
		//time since last Frame
		worldTime = clock.getElapsedTime().asSeconds();
		deltaTime = worldTime - lastFrame; 
		UpdateGameState( deltaTime );

		//Physics Update
        ////Updates happen when the elapsed realworld time has exceeded the elapsed game time.
        while( (worldTime = clock.getElapsedTime().asSeconds()) > gameTime ) {
            drawNeeded = true;//Trigger a draw. 
            UpdatePhysics(fixedDelta); //Update the game
            gameTime += fixedDelta;//Increment the in-game time.
        }

		//check for a statechange request
		if(mRequestedStateChange == true){
			//change to new state
			ChangeState(mRequestedState);
			mRequestedStateChange = false;
			mRequestedState = NULL;
		}

        //Check if a draw is needed. Draws happen only if the world was updated 1 or more
        //time in the current tick. There is no need to redraw the game if nothing changed.
        if(drawNeeded) {
           	this->Draw();
        }

		lastFrame = clock.getElapsedTime().asSeconds();

	}//end while running

	return 0;
}

//Update all GameStates
//States that are paused may or may not update themselves
void GameStateManager::UpdateGameState(float delta){

	if( !mGameStates.empty() ){
		//Send an update to all states. States that are paused may or may not update themselves
		std::vector< GameState *>::iterator it;
		for(it = mGameStates.begin(); it != mGameStates.end(); it++){
			(*it)->UpdateState( (*this) , delta );
		}	
	}
	
}

void GameStateManager::UpdatePhysics(float fixedDelta){
	if( !mGameStates.empty() ){
		//Send an update to all states. States that are paused may or may not update themselves
		std::vector< GameState *>::iterator it;
		for(it = mGameStates.begin(); it != mGameStates.end(); it++){
 			(*it)->UpdatePhysics( (*this) , fixedDelta );
		}	
	}
}


void GameStateManager::Draw(){
	//Look down the stack for the first Opaque window (or last state if all opaque)
	////// HACK: for now draw the top //////

	mGameWindow->clear();
	mGameWindow->setView( mGameWindow->getDefaultView() );

	if( !mGameStates.empty() ){
		mGameStates.back()->Draw( (*this) , (*mGameWindow) );
	}

	mGameWindow->display();

}