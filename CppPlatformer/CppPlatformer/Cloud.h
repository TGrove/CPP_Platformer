#ifndef _CLOUD_H_
#define _CLOUD_H_

class GameStateManager;

namespace sf{
	class Sprite;
	class RenderWindow;
}

class Cloud
{
public:
	Cloud(void);
	~Cloud(void);
	void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);
};


#endif
