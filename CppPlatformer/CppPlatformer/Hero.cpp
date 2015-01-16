
//
// Update:
//    Handles player input 
// PhysicsUpdate
//    Handles movement, accleration, gravity. Based on state, advances animations
//
// Collisions
//    Are handles by Gamestate because it has the Tile Layer and Enemies list

#include "Hero.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "PlatformerGameState.h"

//Using alot of sfml
#include <SFML\Graphics.hpp>
#include <iostream>

Hero::Hero(void): mGravity(GRAVITY_PIXELS), mJumpforce_x(JUMP_FORCE_X), mJumpforce_y(JUMP_FORCE_Y)
{
	mHeight = ROBOT_HEIGHT;
	mWidth = ROBOT_WIDTH;
	mPosition = sf::Vector2f(0.0f,0.0f);
	mMoveDirection = sf::Vector2f(0.0f,0.0f);
	mCurrentRunSpeed = 0.0f;
	mCurrentMovementVector = sf::Vector2f(0.0f,0.0f);
	mCurrentState = PS_IDLE;
	mFacing = DIR_RIGHT;
}


Hero::~Hero(void)
{
}

void Hero::UnLoad(){
	delete maIdle;
	delete maIdleAnimation;
	
	delete maRun;
	delete maRunAnimation;

	delete maJump;
	delete maJumpAnimation;
	
	delete maDeath;
	delete maDeathAnimation;
}

void Hero::Load(){
	mInputManager = InputManager::GetInstance();

	mInputManager->RegisterKey(sf::Keyboard::A); //left
	mInputManager->RegisterKey(sf::Keyboard::D); //right
	mInputManager->RegisterKey(sf::Keyboard::S); //drop through floors
	mInputManager->RegisterKey(sf::Keyboard::Space); //jump

	mAssetManager = AssetManager::GetInstance();
	mGateState = PlatformerGameState::GetInstance();

	//idle
	sf::Texture *idleTex = mAssetManager->GetTexture(std::string("..\\Assets\\robot_idle_debug.png"));
	maIdle = new sf::Sprite( *idleTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, robot width, robot highet, number of frames in texture, frames per second 
	maIdleAnimation = new Animation( maIdle, mWidth, mHeight, 7, 7 ); //idle has 7 frames
	
	//run
	sf::Texture *runTex = mAssetManager->GetTexture(std::string("..\\Assets\\robot_run_debug.png"));
	maRun = new sf::Sprite( *runTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, robot width, robot highet, number of frames in texture, frames per second 
	maRunAnimation = new Animation( maRun, mWidth, mHeight, 6, 6 ); //run has 6 frames
	
	
	//jump
	sf::Texture *jumpTex = mAssetManager->GetTexture(std::string("..\\Assets\\robot_jump_debug.png"));
	maJump = new sf::Sprite( *jumpTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, robot width, robot highet, number of frames in texture, frames per second 
	maJumpAnimation = new Animation( maJump, mWidth, mHeight, 8, 8 ); //jump has 8 frames
	maJumpAnimation->SetLooping(false);
	
	//death
	sf::Texture *deathTex = mAssetManager->GetTexture(std::string("..\\Assets\\robot_die.png"));
	maDeath = new sf::Sprite( *deathTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, robot width, robot highet, number of frames in texture, frames per second 
	maDeathAnimation = new Animation( maDeath, mWidth, mHeight, 8, 8 ); //die has 8 frames
	maDeathAnimation->SetLooping(false);

}



void Hero::Update(GameStateManager &manager, float deltaTime){
	mMoveDirection = sf::Vector2f(0.0f,0.0f);

	switch( mCurrentState ){
		case PS_INVAILD:
			break;

		case PS_IDLE:
			//Move: A,D, Space
			//A = left
			if( mInputManager->KeyDown(sf::Keyboard::A) ){
				mMoveDirection.x -= 1.0f;
				SetState( PS_RUNNING );
				SetFacing( DIR_LEFT );
			}
			//D = right
			else if( mInputManager->KeyDown(sf::Keyboard::D) ){
				mMoveDirection.x += 1.0f;
				SetState( PS_RUNNING );
				SetFacing( DIR_RIGHT );
			}
			//Space = verticle jump
			//Key Pressed to prevent bunnyhopping
			else if( mInputManager->KeyPressed(sf::Keyboard::Space) ){
				SetState( PS_JUMPING_VERITCAL );
				mCurrentMovementVector = sf::Vector2f( 0.0f, -JUMP_FORCE_Y * FIXED_DELTA ); // -ForceY because up is towards 0
				mGateState->MoveEntity( mCurrentMovementVector, this );
			}
			break;

		case PS_RUNNING:
			//Move: A,D, Space
			//A = left
			if( mInputManager->KeyDown(sf::Keyboard::A) ){
				mMoveDirection.x -= 1.0f;
				SetFacing(DIR_LEFT);
			}
			//D = right
			else if( mInputManager->KeyDown(sf::Keyboard::D) ){
				mMoveDirection.x += 1.0f;
				SetFacing(DIR_RIGHT);
			}
			
			//No new input, we have stopped moving
			//note, holding both A and D will stop us as movedirection.x will == 0
			if(	mMoveDirection.x == 0 ){
				SetState(PS_IDLE);
			}

			//Space = directional jump
			//Key Pressed to prevent bunnyhopping
			if( mInputManager->KeyPressed(sf::Keyboard::Space) ){
				SetState( PS_JUMPING_DIRECTIONAL );
				mCurrentMovementVector = sf::Vector2f( JUMP_FORCE_X * FIXED_DELTA * mMoveDirection.x, -JUMP_FORCE_Y * FIXED_DELTA  ); // -ForceY because up is towards 0
				mGateState->MoveEntity( mCurrentMovementVector, this );
			}
			break;

		case PS_JUMPING_VERITCAL:
			//no movement allowed, let physics take over

			//TODO -put in a double jump

			break;

		case PS_JUMPING_DIRECTIONAL:
			//no movement allowed, let physics take over

			//TODO -put in a double jump
			break;

		case PS_DEAD:
			//no movement
			//waiting on reset
			break;
		case PS_FALLING:
			break;
	}

}

//Takes care of Gravity
//Accelerate player when running
//Advance Animation frames
void Hero::UpdatePhysics(GameStateManager &manager, float fixedDeltaTime){
	bool grounded = false;


	//Check if Hero is grounded
	grounded = mGateState->IsGrounded( this );
	if( grounded && mCurrentMovementVector.y >= 0){
		//Check if we were in a bad state
		switch (mCurrentState){
			case PS_IDLE:
				mCurrentMovementVector.y = 0.0f;
				mCurrentMovementVector.x = 0.0f;
				break;
			case PS_JUMPING_VERITCAL:
				
				//Gravity is moving us downwards, and we have hit the ground
				if( mCurrentMovementVector.y >= 0 ){
					SetState(PS_IDLE);
				}

				//Moving veloctiy going upwards, but grounded, means we have gone through a platform on our way up 
				//(or are just starting our jump, either way we have upward velocity that needs to play out) 
				else{
					//So, keep going up
				}
		
				break;

			case PS_JUMPING_DIRECTIONAL:
				
				//Gravity is moving us downwards, and we have hit the ground
				if( mCurrentMovementVector.y >= 0 ){
					SetState(PS_RUNNING);
				}

				//Moving veloctiy going upwards, but grounded, means we have gone through a platform on our way up
				//(or are just starting our jump, either way we have upward velocity that needs to play out) 
				else{
					//So, keep going up
				}

				
				break;

			case PS_RUNNING:
				mCurrentRunSpeed += ACCELERATION * fixedDeltaTime;
				
				if( mCurrentRunSpeed > MAX_RUN_SPEED ){
					mCurrentRunSpeed = MAX_RUN_SPEED;
				}
				mCurrentMovementVector.x = mCurrentRunSpeed * mMoveDirection.x * fixedDeltaTime;
				mCurrentMovementVector.y = 0;

				//std::cout << "current speed: " << mCurrentRunSpeed << std::endl;
				break;
		}
	}
	//Hero is not grounded, apply gravity
	else{
		switch (mCurrentState){
			case PS_DEAD:
				//stop gravity
				break;

			default:
				mCurrentMovementVector.y += GRAVITY_PIXELS * fixedDeltaTime * fixedDeltaTime;
				//cap gravity
				if (mCurrentMovementVector.y  > GRAVITY_MAX){
					mCurrentMovementVector.y = GRAVITY_MAX;
				}
				break;
		}
		
	}



	//Request Movement from GameState
	mGateState->MoveEntity( mCurrentMovementVector, this );


	//update current state's animation
	switch( mCurrentState ){
		case PS_INVAILD:
			maIdleAnimation->Animate(fixedDeltaTime);
			break;
		case PS_IDLE:
			maIdleAnimation->Animate(fixedDeltaTime);
			break;
		case PS_RUNNING:
			maRunAnimation->Animate(fixedDeltaTime);
			break;
		case PS_JUMPING_VERITCAL:
			maJumpAnimation->Animate(fixedDeltaTime);
			break;
		case PS_JUMPING_DIRECTIONAL:
			maJumpAnimation->Animate(fixedDeltaTime);
			break;
		case PS_DEAD:
			maDeathAnimation->Animate(fixedDeltaTime);
			break;
	}
}

//Draw Hero
//Picks animation based on currentPlayerState
void Hero::Draw(GameStateManager &manager, sf::RenderWindow &gameWindow){
	sf::RectangleShape textrect;
	sf::RectangleShape bbrect;


	//draw hitbox -- for debugging
	sf::Rect<float> bb = GetBoundingBox();
	bbrect.setSize(sf::Vector2f(bb.width, bb.height));
	bbrect.setOutlineThickness(1);
	bbrect.setFillColor( sf::Color::Transparent );
	bbrect.setPosition( bb.left, bb.top);

	//draw texturebox -- for debugging
	textrect.setSize(sf::Vector2f( (float)mWidth, (float)mHeight ));
	textrect.setOutlineColor(sf::Color::Red);
	textrect.setOutlineThickness(1);
	textrect.setFillColor( sf::Color::Transparent );
	textrect.setPosition(  mPosition.x, mPosition.y );
	gameWindow.draw(textrect);

	switch( mCurrentState ){
		case PS_INVAILD:
			maIdle->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maIdle);

			bbrect.setOutlineColor(sf::Color::Red);
			gameWindow.draw(bbrect);
			break;

		case PS_IDLE:
			maIdle->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maIdle);

			bbrect.setOutlineColor(sf::Color::Red);
			gameWindow.draw(bbrect);
			break;

		case PS_RUNNING:
			maRun->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maRun);

			bbrect.setOutlineColor(sf::Color::Green);
			gameWindow.draw(bbrect);
			break;

		case PS_JUMPING_VERITCAL:
			maJump->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maJump);

			bbrect.setOutlineColor(sf::Color::Blue);
			gameWindow.draw(bbrect);
			break;

		case PS_JUMPING_DIRECTIONAL:
			maJump->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maJump);

			bbrect.setOutlineColor(sf::Color::Magenta);
			gameWindow.draw(bbrect);
			break;

		case PS_DEAD:
			maDeath->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maDeath);

			bbrect.setOutlineColor(sf::Color::Yellow);
			gameWindow.draw(bbrect);
			break;
	}

	
}

//Resets the movement vector 
//Resets CurrentState's animation if it is set to play once
void Hero::SetState(ePlayerState newstate){
	mCurrentState = newstate;

	switch( mCurrentState ){
		case PS_INVAILD:
			mCurrentMovementVector = sf::Vector2f(0,0);
			mCurrentRunSpeed = 0;
			std::cout << "invalid state" << std::endl;
			break;

		case PS_IDLE:
			mCurrentMovementVector = sf::Vector2f(0,0);
			mCurrentRunSpeed = 0;
			std::cout << "idle state" << std::endl;
			break;

		case PS_RUNNING:
			maRunAnimation->Reset();
			mCurrentMovementVector = sf::Vector2f(0,0);
			std::cout << "running state" << std::endl;
			break;

		case PS_JUMPING_VERITCAL:
			maJumpAnimation->Reset();
			std::cout << "verticaljump state" << std::endl;
			break;

		case PS_JUMPING_DIRECTIONAL:
			maJumpAnimation->Reset();
			std::cout << "directionaljump state" << std::endl;
			break;

		case PS_DEAD:
			mCurrentMovementVector = sf::Vector2f(0,0);
			mCurrentRunSpeed = 0;
			maDeathAnimation->Reset();
			std::cout << "dead state" << std::endl;
			break;
	}
}




//Flips Sprites by mirroring them about the Y axis (Y axis set to middle of texture =/- offsets)
//Reminders: hero texture is 64px wide and his actual image is about 30px wide
void Hero::SetFacing( eDirection newDir ){
	
	//Switching from right to left
	if( mFacing == DIR_RIGHT && newDir == DIR_LEFT ){
		
		//Flip all sprites
		maIdle->setOrigin(64/2 + 30 , 0);
		maIdle->scale(-1,1);
		//maIdle->setOrigin(0 , 0);

		maJump->setOrigin(64/2 + 30 , 0);
		maJump->scale(-1,1);
		//maJump->setOrigin(0 , 0);
		
		maRun->setOrigin( 64/2 + 30, 0 );
		maRun->scale(-1,1);
		//maRun->setOrigin(0 , 0);

		maDeath->setOrigin(64/2 + 30, 0);
		maDeath->scale(-1,1);
	//	maDeath->setOrigin(0 , 0);
	}

	//Switching from left to right
	else if( mFacing == DIR_LEFT && newDir == DIR_RIGHT  ){
		//Flip all sprites
		maIdle->setOrigin(64/2 - 30 , 0);
		maIdle->scale(-1,1);
		//maIdle->setOrigin(0 , 0);

		maJump->setOrigin(64/2 - 30 , 0);
		maJump->scale(-1,1);
		//maJump->setOrigin(0 , 0);
		
		maRun->setOrigin( 64/2 - 30, 0 );
		maRun->scale(-1,1);
		//maRun->setOrigin(0 , 0);

		maDeath->setOrigin(64/2 - 30, 0);
		maDeath->scale(-1,1);
	//	maDeath->setOrigin(0 , 0);
	}

	else{
		//facing direction already, do nothing
	}

	mFacing = newDir;
}



sf::Rect<float> Hero::GetBoundingBox(){
	sf::FloatRect bb;
	bb = sf::FloatRect( mPosition.x , mPosition.y, (float)mWidth, (float)mHeight );

	//shrink box to better fit a hero
	//Shrink hero bb to better reflect his pixel art
	bb.top += 48.0f; //offset
	bb.height = 80.0f; //texture is about 88px tall

	bb.left += 19.0f; //padding of about 19
	bb.width = 30.0f;//width of about 30px

	return bb;
}



sf::Vector2f Hero::GetDimensions(){
	return sf::Vector2f( (float)mWidth, (float)mHeight );
}

