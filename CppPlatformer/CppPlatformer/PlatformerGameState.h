#ifndef _PLATFORMER_GAME_STATE_H_
#define _PLATFORMER_GAME_STATE_H_

#include "gamestate.h"

#include <SFML\System.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>

#include <vector>
#include <string>

class TileLayer;
class BackgroundLayer;
class Cloud;

class GameEntity;
class Enemy;
class Hero;

class Camera;
class InputManager;
class AssetManager;
class GameStateManager;


namespace sf{
	class Text;
	class Font;
	class RenderWindow;
}

class PlatformerGameState:
	public GameState
{
public:

	~PlatformerGameState(void);

	virtual void Init(GameStateManager &manager);
	virtual void DeInit();

	virtual void LoadLevel(const std::string &levelfile);
	virtual void UnLoadLevel();

	virtual void Pause();
	virtual void Resume();

	//Moves the GameEntity in the level based on the magnitudes given
	//Does collision detection
	//Calls GameEntity.CollideWith(GameEntity *gameEnt)
	virtual void MoveEntity( sf::Vector2f movement, GameEntity *gameEnt );

	//Does a collision detection below the Entity
	virtual bool IsGrounded(GameEntity *ent);

	//Moves camera view (does paralaxing)
	virtual void Scroll(sf::Vector2f displacement);

	//Resets Hero and Enemy positions, resets camera and background layers
	virtual void ResetLevel();

	virtual void UpdatePhysics(GameStateManager &manager, float fixedDelta);
	virtual void UpdateState(GameStateManager &manager, float deltal);
	virtual void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);

	static PlatformerGameState* GetInstance(){
		static PlatformerGameState instance;
		return &instance;
	}

	virtual TileLayer * GetTileLayer(){
		return mLevelTileLayer;
	}

protected:
	PlatformerGameState(void);

private:
	TileLayer *mLevelTileLayer;
	std::vector<BackgroundLayer *> mBackgrounds;
	std::vector< Cloud *> mClouds;

	std::vector< Enemy * > mEnemies;
	Hero *mHero;

	Camera *mCamera;
	InputManager *mInputManager;
	AssetManager *mAssetManager;
};


#endif
