#ifndef _HERO_H_
#define _HERO_H_

#include "gameentity.h"
#include "GameStateManager.h"
#include "PlatformerGameState.h"
#include "Animation.h"

#define FIXED_DELTA (1.f / 60.f)
//#define GRAVITY_PIXELS 392
#define GRAVITY_PIXELS 400
//Capping max movement in the Y by 64px
#define GRAVITY_MAX 3200
//#define JUMP_FORCE_X 98
#define JUMP_FORCE_X 65

//#define JUMP_FORCE_Y 784
#define JUMP_FORCE_Y 320

//#define MAX_RUN_SPEED 128
#define MAX_RUN_SPEED 128


//#define ACCELERATION 128
#define ACCELERATION 64


#define ROBOT_WIDTH 64
#define ROBOT_HEIGHT 128

class InputManager;
class AssetManager;


enum ePlayerState{
	PS_INVAILD = -1,
	PS_IDLE = 0,
	PS_RUNNING = 1,
	PS_JUMPING_VERITCAL = 2,
	PS_JUMPING_DIRECTIONAL = 3,
	PS_FALLING = 4,
	PS_DEAD = 10
};


namespace sf{
	class Sprite;
	class RenderWindow;
	class Texture;
}

class Hero :
	public GameEntity
{
public:
	Hero(void);
	~Hero(void);

	void Load();
	void UnLoad();

	void SetState(ePlayerState newstate);
	
	ePlayerState GetState(){
		return mCurrentState;
	}

	virtual void SetFacing( eDirection newDir );

	virtual sf::Rect<float> GetBoundingBox();
	virtual sf::Vector2f GetDimensions();

	void Update(GameStateManager &manager, float deltaTime);
	void UpdatePhysics(GameStateManager &manager, float fixedDeltaTime);
	void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);

private:
	PlatformerGameState *mGateState;
	InputManager *mInputManager;

	int mGravity;
	
	int mJumpforce_x;
	int mJumpforce_y;
	
	float mAcceleration;
	float mMaxRunSpeed;
	float mCurrentRunSpeed;
	sf::Vector2f mCurrentMovementVector;
	sf::Vector2f mMoveDirection;
	ePlayerState mCurrentState;

	AssetManager *mAssetManager;

	sf::Sprite *maIdle;
	sf::Sprite *maRun;
	sf::Sprite *maJump;
	sf::Sprite *maDeath;

	Animation *maIdleAnimation;
	Animation *maRunAnimation;
	Animation *maJumpAnimation;
	Animation *maDeathAnimation;
};

#endif
