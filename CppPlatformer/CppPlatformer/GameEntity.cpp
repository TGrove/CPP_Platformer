#include "GameEntity.h"



GameEntity::~GameEntity(){
}



eDirection GameEntity::Facing(){
	return mFacing;
}


void GameEntity::SetPosition( sf::Vector2f newpos ){
	mPosition = newpos;
}

sf::Vector2f GameEntity::GetPosition(){
	return mPosition;
}

void GameEntity::SetVelocity( sf::Vector2f newVel ){
	mVelocity = newVel;
}

sf::Vector2f GameEntity::GetVelocity(){
	return mVelocity;
}

void GameEntity::SetAcceleration( sf::Vector2f newAccl ){
	mAcceleraton = newAccl;
}

sf::Vector2f GameEntity::GetAcceleration(){
	return mAcceleraton;
}
