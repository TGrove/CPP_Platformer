#include "SplashScreenGameState.h"
#include "GameStateManager.h"
#include "InputManager.h"
#include "PlatformerGameState.h"

#include <SFML\Graphics.hpp>


SplashScreenGameState::SplashScreenGameState(void) :  mPressStartFadeRatio(1.0), mFading(true)
{
	mOpaque = true;
	mIsPaused = false;
	
	mGameTitle = new sf::Text();
	mPressStart = new sf::Text();
	mFont = new sf::Font();

	
}



SplashScreenGameState::~SplashScreenGameState(void)
{
	delete mGameTitle;
	delete mPressStart;
	delete mFont;

	mGameTitle = NULL;
	mPressStart = NULL;
	mFont = NULL;
}



void SplashScreenGameState::DeInit()
{

}



void SplashScreenGameState::Init(GameStateManager &manager)
{

	//Register Keys we will be checking
	mInputManager = InputManager::GetInstance();
	mInputManager->RegisterKey( sf::Keyboard::Return );
	//mInputManager->RegisterKey( sf::Keyboard::Key(58) );

	mIsPaused = false;

	//Load Font
	if( mFont->loadFromFile("..\\Assets\\Fonts\\calibri.ttf") ){
		//Init Game Name
		mGameTitle->setFont( (*mFont) );
		mGameTitle->setString("CPP Platformer");
		mGameTitle->setCharacterSize(60);
		mGameTitle->setColor(sf::Color::White);
		mGameTitle->setPosition( (float)(manager.GetWindowWidth() * 0.5f - mGameTitle->getLocalBounds().width * 0.5f ) , (float)(manager.GetWindowHeight() * 0.2f) );
		
		//Init Press Start
		mPressStart->setFont( (*mFont) );
		mPressStart->setString("Hit Enter To Start");
		mPressStart->setCharacterSize(40);
		mPressStart->setColor(sf::Color::White);
		mPressStart->setPosition( (float)(manager.GetWindowWidth() * 0.5f - mPressStart->getLocalBounds().width * 0.5f ) , (float)(manager.GetWindowHeight() * 0.6f) );
	}


}



void SplashScreenGameState::Pause()
{
	mIsPaused = true;
}



void SplashScreenGameState::Resume(){
	mIsPaused = false;
}



void SplashScreenGameState::UpdateState(GameStateManager &manager, float deltal)
{
	if( !mIsPaused ){
		//Check for Enter hit (released)
		if( mInputManager->KeyReleased(sf::Keyboard::Return) == true){
			//request a state change to PlatformerGameState
			manager.RequestStateChange(PlatformerGameState::GetInstance());
			Pause();
		}
	}

}



void SplashScreenGameState::UpdatePhysics(GameStateManager &manager, float fixedDelta)
{
	//Only update is not paused
	if( !mIsPaused ){
		
	//Flashing Text	
		//make "Hit Enter To Start" flash by decrementing it's alpha or colour to 0 or black
		float FadeSpeed = 0.75f;
		
		//fadeOUT
		if(mFading){
			mPressStartFadeRatio -= FadeSpeed * fixedDelta;
		}else{
		//fadeINfadeTime*delta
			mPressStartFadeRatio += FadeSpeed * fixedDelta;
		}

		if(mPressStartFadeRatio < 0){
			mPressStartFadeRatio = 0.0f;
			mFading = false;
		}else if(mPressStartFadeRatio > 1.0){
			mPressStartFadeRatio = 1.0f;
			mFading = true;
		}

	}//end if !paused
}



void SplashScreenGameState::Draw(GameStateManager &manager, sf::RenderWindow &gameWindow)
{
	if(mOpaque){
		gameWindow.clear(sf::Color::Black);
	}
	//Draw Game Title
	gameWindow.draw( (*mGameTitle) );

	//Draw Press Start
	////Apply White with an alpha value to fade something out
	mPressStart->setColor( sf::Color( 255, 255, 255, (sf::Uint8)(255*mPressStartFadeRatio) ) );
	gameWindow.draw( (*mPressStart) );
}
