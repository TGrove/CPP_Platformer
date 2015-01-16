#ifndef _ENEMEY_H_
#define _ENEMEY_H_

#include "gameentity.h"

class TileLayer;
class Animation;
class GameStateManager;
class PlatformerGameState;
class AssetManager;

enum eEnemyState{
	ES_IDLE = 0,
	ES_RUNNING = 1,
	ES_FALLING = 2,
	ES_DEAD = 5
};

namespace sf{
	class Sprite;
	class RenderWindow;
}

class Enemy :
	public GameEntity
{

public:
//	Enemy(void);
	virtual ~Enemy(void) = 0;
	
	virtual void Unload() = 0;
	virtual void Load() = 0;

	virtual sf::Rect<float> GetBoundingBox() =0;
	virtual sf::Vector2f GetDimensions() = 0;
	
	virtual eEnemyState GetCurrentState(){
		return mCurrentState;
	}

	virtual void SetState( eEnemyState newstate ) = 0;

	virtual void SetFacing( eDirection newDir ) = 0;

	virtual void Update(GameStateManager &manager, float deltaTime) = 0;
	virtual void UpdatePhysics(GameStateManager &manager, float fixedDeltaTime) = 0;
	virtual void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow) = 0;

protected:
	PlatformerGameState *mGateState;
	TileLayer *mLevelData;
	AssetManager *mAssetManager;

	eEnemyState mCurrentState;

	float  mCurrentRunSpeed;
	sf::Vector2f mMoveDirection;
	sf::Vector2f mCurrentMovementVector;
	sf::Vector2f mInitialPosition;
	
};

inline Enemy::~Enemy() {};

#endif
