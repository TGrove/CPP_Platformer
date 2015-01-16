#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

class GameStateManager;

namespace sf{
	class RenderWindow;
}

class GameState{

public:
//	GameState(void);
//	~GameState(void);

	virtual void Init(GameStateManager &manager ) = 0;
	virtual void DeInit() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void UpdatePhysics(GameStateManager &manager, float fixedDelta) = 0;
	virtual void UpdateState(GameStateManager &manager, float deltal) = 0;
	virtual void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow) = 0;

	bool isOpaque(){ return mOpaque; }

private:

protected:
	bool mOpaque;
	bool mIsPaused;
};


#endif
