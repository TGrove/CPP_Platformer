#include "InputManager.h"

#include <iostream>

InputManager::InputManager(void)
{
	//map defaults
}


InputManager::~InputManager(void)
{
}

InputManager * InputManager::GetInstance(){
	static InputManager im;
	
	return &im;
}

//Frees all memory stored
//Frees all registered values in the Key map
void InputManager::CleanUp(){

	std::map<sf::Keyboard::Key, KeyState *>::iterator it;
	for (it = mKeyStates.begin(); it != mKeyStates.end(); it++){
		delete (it->second);
	}
	mKeyStates.clear();
	
}

void InputManager::UpdateKeys( sf::Event keyEvent ){
	KeyState *ks;

	switch( keyEvent.type ){
	//key Pressed
	case sf::Event::KeyPressed:
		
		if ( mKeyStates.find(keyEvent.key.code) != mKeyStates.end() ){ //Is it a mapped key?
			ks = mKeyStates[ keyEvent.key.code ];
			ks->mChangedThisFrame = true;
			ks->mIsDown = true;
//			std::cout << "InputManger Key: " << keyEvent.key.code << " was pressed." << std::endl;
		}
		break;
		
	//Key Released
	case sf::Event::KeyReleased:

		if ( mKeyStates.find(keyEvent.key.code) != mKeyStates.end() ){ //Is it a mapped key?
			ks = mKeyStates[ keyEvent.key.code ]; 
			ks->mChangedThisFrame = true;
			ks->mIsDown = false;
//			std::cout << "InputManger Key: " << keyEvent.key.code << " was released." << std::endl;
		}

		break;
	default:
		break;
	}
}

void InputManager::ResetForNewFrame(){
	
	//reset KeyMap stats
	for each( std::pair<sf::Keyboard::Key, KeyState*> key in mKeyStates ){
		key.second->mChangedThisFrame = false;
	}

	//reset Mouse stats
}

//Returns ture if the given key has been down for more than one frame
bool InputManager::KeyDown( sf::Keyboard::Key key ){

	if ( mKeyStates.find(key) == mKeyStates.end() ){
		return false; //Key is not being tracked
	}

	if( mKeyStates[key]->mIsDown == true ){
		return true;
	}
	return false;
}

//returns if the given key has been pressed this frame
bool InputManager::KeyPressed( sf::Keyboard::Key key ){

	if ( mKeyStates.find(key) == mKeyStates.end() ){
		return false; //Key is not being tracked
	}

	if( mKeyStates[key]->mIsDown == true && mKeyStates[key]->mChangedThisFrame == true){
		return true;
	}
	return false;
}

//returns if the key has been released this frame
bool InputManager::KeyReleased( sf::Keyboard::Key key ){
	
	if ( mKeyStates.find(key) == mKeyStates.end() ){
		return false; //Key is not being tracked
	}

	if( mKeyStates[key]->mIsDown == false && mKeyStates[key]->mChangedThisFrame == true){
		return true;
	}
	return false;
}

void InputManager::RegisterKey( sf::Keyboard::Key key ){

	if( mKeyStates[key] != NULL){
		std::cout << "InputManger Already Registered: " << key << std::endl;
	}else{
		mKeyStates[key] = new KeyState();
		std::cout << "InputManger Registered: " << key << std::endl;
	}

}