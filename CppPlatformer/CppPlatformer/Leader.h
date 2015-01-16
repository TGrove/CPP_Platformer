#ifndef _LEADER_H_
#define _LEADER_H_H

#include "Enemy.h"

#define LEADER_TEX_HEIGHT 128
#define LEADER_TEX_WIDTH 64

#define LEADER_TEX_HEIGHT 128
#define LEADER_TEX_WIDTH 64

#define LEADER_MAX_SPEED 64
#define LEADER_ACCELERATION 32

#define LEADER_GRAVITY 400
#define LEADER_GRAVITY_MAX 30

class Leader :
	public Enemy
{
public:
	Leader(void);
	~Leader(void);

	virtual void Unload();
	virtual void Load();

	virtual sf::Rect<float> GetBoundingBox();
	virtual sf::Vector2f GetDimensions();

	virtual void SetState( eEnemyState newstate );
	virtual void SetFacing( eDirection newDir );

	virtual void Update(GameStateManager &manager, float deltaTime);
	virtual void UpdatePhysics(GameStateManager &manager, float fixedDeltaTime);
	virtual void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);

private:
	sf::Sprite *maIdle;
	sf::Sprite *maRun;
	sf::Sprite *maDeath;

	Animation *maIdleAnimation;
	Animation *maRunAnimation;
	Animation *maDeathAnimation;
};

#endif;
