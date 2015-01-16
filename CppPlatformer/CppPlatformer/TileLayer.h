#ifndef _TILE_LAYER_H_
#define _TILE_LAYER_H_

#include <string>
#include <vector>
#include <SFML\System\Vector2.hpp>

class Animation;
class AssetManager;
class GameStateManager;


struct Tile{
	int mGID;
};

namespace tinyxml2{
	class XMLElement;
}

namespace sf{
	class Sprite;
	class RenderWindow;
}

class TileLayer
{
public:
	TileLayer(void);
	~TileLayer(void);

	void Unload(void);
	void Load( tinyxml2::XMLElement *mapRootElement, const std::string AssignedLayerName );

	void Update(GameStateManager &manager, float deltaTime);
	void UpdatePhysics(GameStateManager &manager, float fixedDeltaTime);
	void Draw(GameStateManager &manager, sf::RenderWindow &gameWindow);

	sf::Vector2f GetLevelDimensions();
	sf::Vector2f GetTileDimensions();
	//Retrive all tiles to do collision checks
	std::vector< std::vector<Tile*>> & GetTiles();

private:
	
	bool mLoaded;
	std::string mLayerName;
	int mTileWidth;
	int mTileHeight;
	
	AssetManager *mAssetManager;

	std::vector< sf::Sprite* > mTileSprites;
	std::vector< std::vector< Tile* > > mTiles;
	std::vector< Animation * > mAnimations;
};

#endif
