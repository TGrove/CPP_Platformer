#ifndef _GAME_STATE_MANAGER_H_
#define _GAME_STATE_MANAGER_H_

//We want our includes to be as light as possible because alot of things will be including it
#include <string>
#include <vector>

class GameState;
class InputManager;
namespace sf {
	class RenderWindow;
}


class GameStateManager
{
public:
	GameStateManager(void);
	virtual ~GameStateManager(void);

	void Init( unsigned int width, unsigned int height, std::string const &windowTitle);
	void DeInit();

	//main gameay loop, only call after Init()
	int Loop();

	void RequestStateChange(GameState* newState);
	void ChangeState(GameState* newState);
	void PushState(GameState* newState);
	void PopState();

	int GetWindowWidth();
	int GetWindowHeight();

private:
	std::vector< GameState *> mGameStates;
	sf::RenderWindow* mGameWindow;
	bool mGameIsRunning;
	bool mRequestedStateChange;
	GameState *mRequestedState;

	InputManager *mInputManager;

	void UpdatePhysics(float fixedDelta);
	void UpdateGameState(float delta);
	void Draw();


};

#endif