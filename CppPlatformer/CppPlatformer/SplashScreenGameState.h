#ifndef _SPLASH_SCREEN_GAME_STATE_H_
#define _SPLASH_SCREEN_GAME_STATE_H_

#include "gamestate.h"

class InputManager;

namespace sf{
	class Text;
	class Font;
}

class SplashScreenGameState :
	public GameState
{
public:

	~SplashScreenGameState(void);

	virtual void Init(GameStateManager &manager);
	virtual void DeInit();

	virtual void Pause();
	virtual void Resume();

	virtual void UpdatePhysics(GameStateManager &manager, float fixedDelta);
	virtual void UpdateState(GameStateManager &manager, float deltal);
	virtual void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);

	static SplashScreenGameState* GetInstance(){
		static SplashScreenGameState instance;
		return &instance;
	}

protected:
	SplashScreenGameState(void);

private:
	sf::Text *mGameTitle;
	sf::Text *mPressStart;
	sf::Font *mFont;

	InputManager *mInputManager;

	float mPressStartFadeRatio;
	bool mFading;
};

#endif