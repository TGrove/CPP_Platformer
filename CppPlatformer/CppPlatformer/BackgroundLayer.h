#ifndef _BACKGROUND_LAYER_H_
#define _BACKGROUND_LAYER_H_

#include <vector>
#include <string>
#include <SFML\System\Vector2.hpp>

class AssetManager;
class GameStateManager;

namespace sf{
	class Sprite;
	class RenderWindow;
	class Texture;
}
namespace tinyxml2{
	class XMLElement;
}

class BackgroundLayer
{
public:
	BackgroundLayer(std::string &texture, float scrollRatio);
	~BackgroundLayer(void);

	void Unload(void);
	void Load( sf::Vector2f leveldims );
	void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);
	void Scroll(sf::Vector2f scrollAmount);
	void Reset();

private:
	AssetManager *mAssetManager;

	std::vector<sf::Sprite *> mSprites;
	std::vector<std::string> mTextures;

	float mScrollSpeedRatio;

};

#endif

