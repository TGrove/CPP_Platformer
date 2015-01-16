#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include <string>

#include <SFML\Graphics\Rect.hpp>
#include <SFML\System\Vector2.hpp>

class GameStateManager;

namespace sf{
	class RenderWindow;
}

enum eDirection{
	DIR_LEFT = -1,
	DIR_NOTHING = 0,
	DIR_RIGHT = 1
};

class GameEntity
{
public:
//	GameEntity(void);
	virtual ~GameEntity(void);

	virtual void SetPosition( sf::Vector2f newpos );
	virtual sf::Vector2f GetPosition();

	virtual void SetVelocity( sf::Vector2f newVel );
	virtual sf::Vector2f GetVelocity();

	virtual void SetAcceleration( sf::Vector2f newAccl );
	virtual sf::Vector2f GetAcceleration();

	virtual sf::Rect<float> GetBoundingBox() =0;
	virtual sf::Vector2f GetDimensions() = 0;

	virtual void Update(GameStateManager &manager, float deltaTime) = 0;
	virtual void UpdatePhysics(GameStateManager &manager, float fixedDeltaTime) = 0;
	virtual void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow) = 0;

	virtual eDirection Facing();
	virtual void SetFacing( eDirection newDir ) = 0;

private:

protected:
	
	int mHeight;
	int mWidth;
	eDirection mFacing;

	sf::Vector2f mPosition;
	sf::Vector2f mVelocity;
	sf::Vector2f mAcceleraton;

};


#endif
