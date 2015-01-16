#ifndef _CRAWLER_H_
#define _CRAWLER_H_

#include "Enemy.h"

#define CRAWLER_TEX_HEIGHT 128
#define CRAWLER_TEX_WIDTH 64

#define CRAWLER_MAX_SPEED 128
#define CRAWLER_ACCELERATION 64

#define CRAWLER_GRAVITY 400
#define CRAWLER_GRAVITY_MAX 30

class Crawler :
	public Enemy
{
public:
	Crawler(void);
	~Crawler(void);

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

#endif

