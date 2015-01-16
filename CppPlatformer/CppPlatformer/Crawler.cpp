// 
//  Update:
//    TODO - Decide what direction to move
//
// UpdatePhys
//   Advances animations
//   TODO- moves the Enemes
//
// 
//  

#include "Crawler.h"
#include "Animation.h"
#include "AssetManager.h"
#include "PlatformerGameState.h"
#include "TileLayer.h"

#include <SFML\Graphics.hpp>

Crawler::Crawler(void)
{
	mHeight = CRAWLER_TEX_HEIGHT;
	mWidth = CRAWLER_TEX_WIDTH;
	mPosition = sf::Vector2f(0.0f,0.0f);

	mMoveDirection = sf::Vector2f(1.0f,0.0f); //set to move towards the right to start with
	mCurrentRunSpeed = 0.0f;
	mCurrentMovementVector = sf::Vector2f(0.0f,0.0f);
	mCurrentState = ES_IDLE;
	mFacing = DIR_LEFT;
}



Crawler::~Crawler(void)
{
}



void Crawler::Unload(){
	delete maIdle;
	delete maIdleAnimation;

	delete maRun;
	delete maRunAnimation;

	delete maDeath;
	delete maDeathAnimation;

}



void Crawler::Load(){
	mAssetManager = AssetManager::GetInstance();
	mGateState = PlatformerGameState::GetInstance();
	mLevelData = mGateState->GetTileLayer();

	sf::Texture *idleTex = mAssetManager->GetTexture(std::string("..\\Assets\\crawler_idle.png"));
	maIdle = new sf::Sprite( *idleTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, width, highet, number of frames in texture, frames per second 
	maIdleAnimation = new Animation( maIdle, mWidth, mHeight, 8, 8 ); //idle has 8 frames
	
	//run
	sf::Texture *runTex = mAssetManager->GetTexture(std::string("..\\Assets\\crawler_walk.png"));
	maRun = new sf::Sprite( *runTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, width, highet, number of frames in texture, frames per second 
	maRunAnimation = new Animation( maRun, mWidth, mHeight, 6, 6 ); //run has 6 frames
	
	
	//death
	sf::Texture *deathTex = mAssetManager->GetTexture(std::string("..\\Assets\\crawler_die.png"));
	maDeath = new sf::Sprite( *deathTex, sf::IntRect(0, 0, mWidth, mHeight) );
	//sprite, width, highet, number of frames in texture, frames per second 
	maDeathAnimation = new Animation( maDeath, mWidth, mHeight, 8, 8 ); //die has 8 frames
	maDeathAnimation->SetLooping(false);
}



sf::Rect<float> Crawler::GetBoundingBox(){
	sf::FloatRect bb;
	bb = sf::FloatRect( mPosition.x , mPosition.y, (float)mWidth, (float)mHeight );

	//shrink box to better fit a crawler
	bb.height = 40; //height of about 80px
	bb.top += 88; //offset of 88

	bb.width = 44; //texture is about 44px wide
	bb.left += 10; //texture has an edge of about 10px

	return bb;

}



sf::Vector2f Crawler::GetDimensions(){
	return sf::Vector2f( (float)mWidth, (float)mHeight );
}



void Crawler::Update(GameStateManager &manager, float deltaTime){
	mMoveDirection = sf::Vector2f(0.0f,0.0f);

	switch( mCurrentState ){
		case ES_IDLE:
			//stand there and look pretty
			break;

		case ES_RUNNING:{
			//decide which way to move

			//Grab tiles so we can look for ledges/walls/empty tiles infront of us to turn around
			std::vector< std::vector<Tile*>> world = mLevelData->GetTiles();

			sf::Vector2f tileDims = mLevelData->GetTileDimensions();
			sf::Rect<float> ebb = GetBoundingBox();

			//set BB to feet
			ebb.top += ebb.height - 1;

			int x;
			int y;

			y = (int)( ebb.top / tileDims.y );
			x = (int)( ebb.left / tileDims.x );

			switch (mFacing){
				case DIR_LEFT:
					//Check for Wall to the Left
					//Check if tile to the left is open air
					if( (x-1 >= 0)
						&& (world[y][x-1] != NULL)
					){
						//Wall, change direction
						SetFacing( DIR_RIGHT );
						mMoveDirection.x = (float)DIR_RIGHT;
					}else{
						mMoveDirection.x = (float)DIR_LEFT;
					}

					//Check for Edge
					//Check if tile to the left and down 1 is open air
					if( (x-1 >= 0) 
						&& (y+1 < mLevelData->GetLevelDimensions().y/tileDims.y ) 
						&& (world[y+1][x-1] == NULL) 
					){
						//Clif, change direction
						SetFacing( DIR_RIGHT );
						mMoveDirection.x = (float)DIR_RIGHT;
					}else{
						mMoveDirection.x = (float)DIR_LEFT;
					}

					break;

				case DIR_RIGHT:
					//Check for Wall to the Right
					//Check if tile to the Right is open air
					if( (x+1 < mLevelData->GetLevelDimensions().x/tileDims.x ) 
						&& (world[y][x+1] != NULL ) 
					){
						//Wall, change direction
						SetFacing( DIR_LEFT );
						mMoveDirection.x = (float)DIR_LEFT;
					}else{
						mMoveDirection.x = (float)DIR_RIGHT;
					}

					//Check for Edge
					//Check if tile to the left and down 1 is open air
					if( (x+1 < mLevelData->GetLevelDimensions().x/tileDims.x ) 
						&& (y+1 < mLevelData->GetLevelDimensions().y/tileDims.y ) 
						&& (world[y+1][x+1] == NULL) 
					){
						//Clif, change direction
						SetFacing( DIR_LEFT );
						mMoveDirection.x = (float)DIR_LEFT;
					}else{
						mMoveDirection.x = (float)DIR_RIGHT;
					}

					break;

			}
			break;
		}//end case running

		case ES_DEAD:
			break;

		default:
			break;
	}
}



void Crawler::UpdatePhysics(GameStateManager &manager, float fixedDeltaTime){
	bool grounded;

//update movement
	grounded = mGateState->IsGrounded( this );
	if( grounded ){
		//Check if we were in a bad state
		switch (mCurrentState){
			case ES_IDLE:
				mCurrentMovementVector.y = 0.0f;
				mCurrentMovementVector.x = 0.0f;
				break;

			case ES_RUNNING:
				mCurrentRunSpeed += CRAWLER_ACCELERATION * fixedDeltaTime;
				
				if( mCurrentRunSpeed > CRAWLER_MAX_SPEED ){
					mCurrentRunSpeed = CRAWLER_MAX_SPEED;
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
			case ES_DEAD:
				//stop gravity
				break;

			default:
				mCurrentMovementVector.y += CRAWLER_GRAVITY * fixedDeltaTime;
				//cap gravity
				if (mCurrentMovementVector.y  > CRAWLER_GRAVITY_MAX){
					mCurrentMovementVector.y = CRAWLER_GRAVITY_MAX;
				}
			break;
		}
		
	}

	//Request Movement from GameState
	mGateState->MoveEntity( mCurrentMovementVector, this );


	// **** really should be in a last update ***
	//update current state's animation
	switch( mCurrentState ){
		case ES_IDLE:
			maIdleAnimation->Animate(fixedDeltaTime);
			break;
		case ES_RUNNING:
			maRunAnimation->Animate(fixedDeltaTime);
			break;
		case ES_DEAD:
			maDeathAnimation->Animate(fixedDeltaTime);
			break;
	}
}






void Crawler::Draw(GameStateManager &manager, sf::RenderWindow &gameWindow){
	switch( mCurrentState ){
		case ES_IDLE:
			maIdle->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maIdle);
			break;

		case ES_RUNNING:
			maRun->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maRun);
			break;

		case ES_DEAD:
			maDeath->setPosition( mPosition.x, mPosition.y );
			gameWindow.draw(*maDeath);
			break;
	}

	//draw hitbox for debugging
	sf::Rect<float> bb = GetBoundingBox();
	
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(bb.width, bb.height));
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(1);
	rect.setFillColor( sf::Color::Transparent );
	rect.setPosition( bb.left, bb.top);

	gameWindow.draw(rect);
}





//Resets the movement vector 
//Resets CurrentState's animation if it is set to play once
void Crawler::SetState( eEnemyState newstate ){
	mCurrentState = newstate;

	switch( mCurrentState ){

		case ES_IDLE:
			mCurrentMovementVector = sf::Vector2f(0,0);
			mCurrentRunSpeed = 0;
			break;

		case ES_RUNNING:
			maRunAnimation->Reset();
			mCurrentMovementVector = sf::Vector2f(0,0);
			break;

		case ES_DEAD:
			mCurrentMovementVector = sf::Vector2f(0,0);
			mCurrentRunSpeed = 0;
			maDeathAnimation->Reset();
			break;
	}
}






//Flips Sprites by mirroring them about the Y axis (Y axis set to middle of texture =/- offsets)
//Reminders: texture is 64px wide and his actual image is about 44px wide
void Crawler::SetFacing( eDirection newDir ){
	
	//Switching from right to left
	if( mFacing == DIR_RIGHT && newDir == DIR_LEFT ){
		
		//Flip all sprites
		maIdle->setOrigin(0 , 0);
		maIdle->scale(-1,1);

		maRun->setOrigin( 0 , 0 );
		maRun->scale(-1,1);

		maDeath->setOrigin(0  , 0);
		maDeath->scale(-1,1);
	}

	//Switching from left to right
	else if( mFacing == DIR_LEFT && newDir == DIR_RIGHT  ){
		//Flip all sprites
		maIdle->setOrigin(64 , 0);
		maIdle->scale(-1,1);
		
		maRun->setOrigin( 64, 0 );
		maRun->scale(-1,1);

		maDeath->setOrigin(64 , 0);
		maDeath->scale(-1,1);

	}

	else{
		//facing direction already, do nothing
	}

	mFacing = newDir;
}
