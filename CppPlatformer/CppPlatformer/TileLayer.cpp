#include "TileLayer.h"
#include "tinyxml2.h"
#include "AssetManager.h"
#include "Animation.h"
#include "GameStateManager.h"

#include <SFML\Graphics.hpp>

TileLayer::TileLayer()
{

}

TileLayer::~TileLayer()
{

}

void TileLayer::Unload()
{
	//free tiles
	for (unsigned int y = 0; y < mTiles.size() ; y++){
		for(unsigned int x = 0; x < mTiles[y].size(); x++){
			delete mTiles[y][x];
		}
	}

	//free tile sprites
	for(unsigned int i = 0; i < mTileSprites.size(); i++){
		delete mTileSprites[i];
	}
	mTileSprites.clear();

	//free animations
	for(unsigned int i = 0; i < mAnimations.size(); i++){
		delete mAnimations[i];
	}
	mAnimations.clear();
}


void TileLayer::Load( tinyxml2::XMLElement *mapRootElement, const std::string AssignedLayerName )
{
	using namespace tinyxml2;

	//Dimensions of level
	int width, height, tileWidth, tileHeight;
	width = mapRootElement->IntAttribute("width");
	height = mapRootElement->IntAttribute("height");
	tileWidth = mapRootElement->IntAttribute("tilewidth");
	tileHeight = mapRootElement->IntAttribute("tileheight");
	
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;
	mLayerName = AssignedLayerName;

	//Assign the vector tiles and prefill it
	mTiles.resize( height, std::vector<Tile*>(width, NULL) );
	
	//get the name of the TileSheet
	XMLElement *tileSetElement = mapRootElement->FirstChildElement("tileset");
	int gid = tileSetElement->IntAttribute("firstgid");

	//tile sprite sheet
	XMLElement *tileSetImageElement = tileSetElement->FirstChildElement("image");
	std::string textureName(tileSetImageElement->Attribute("source"));

	mAssetManager = AssetManager::GetInstance();

	//Load TileSheet
	std::string path = "..\\Levels\\";
	path += textureName;
	sf::Texture *tileSetElements = mAssetManager->GetTexture(path);

	//Create new vector of sprites from TileSheet
	mTileSprites.push_back(NULL);
	//How many tiles in sheet x/y
	int tilesInSheetHorizontally = tileSetElements->getSize().x / tileWidth;
	int tilesInSheetVertically = tileSetElements->getSize().y / tileHeight;

	//Go over TileSheet based on Tile dimentions
	for( int y = 0; y < tilesInSheetVertically; y++ ){
		for( int x = 0; x < tilesInSheetHorizontally; x++ ){
			//Cut out the sprite's rectangle on the TileSheet
			sf::Sprite *sprite = new sf::Sprite( *tileSetElements, sf::IntRect(x*tileWidth, y*tileHeight, tileWidth, tileHeight) );
			mTileSprites.push_back(sprite);
		}//x
	}//y

	//parse through all properties and handle them
	XMLElement *tilesetTileElement = tileSetElement->FirstChildElement("tile");

	while(tilesetTileElement != NULL){
		int id = tilesetTileElement->IntAttribute("id");

		std::map<std::string, std::string> props;
		XMLElement *tilesetTilePropertyElement = tilesetTileElement->FirstChildElement("properties")->FirstChildElement("property");
		//store all property elements
		while(tilesetTilePropertyElement != NULL){
			std::string name(tilesetTilePropertyElement->Attribute("name"));
			std::string value(tilesetTilePropertyElement->Attribute("value"));
			props[name] = value;
			tilesetTilePropertyElement = tilesetTilePropertyElement->NextSiblingElement("property");

		}
		//check if this is an animated thing, if it is, create animation and store it
		if(props["type"] == "animated"){
			//create the new animation object
			Animation *a = new Animation(mTileSprites[id + gid], props);
			//store it
			mAnimations.push_back(a);
		}

		tilesetTileElement = tilesetTileElement->NextSiblingElement("tile");
	}
	//iterate through all the layers until we find the one we were assigned
	XMLElement *layerElement = mapRootElement->FirstChildElement("layer");
	while(layerElement != NULL){
		//parse the element and check name, etc..
		std::string layerName(layerElement->Attribute("name"));
		if(layerName == AssignedLayerName){
			XMLElement *tileElement = layerElement->FirstChildElement("data")->FirstChildElement("tile");
			int tileCount = 0;
			while(tileElement != NULL){
				int gid = tileElement->IntAttribute("gid");
				//if( gid != 0 ){
					Tile *t = new Tile();
					t->mGID = gid;
					mTiles[ tileCount / width ][ tileCount % width ] = t;
				//}else{

				//}
				tileCount++;
				//once we are done parsing, ge the next element
				tileElement = tileElement->NextSiblingElement("tile");
			}
		}
		layerElement = layerElement->NextSiblingElement("layer");
	}
	
}

void TileLayer::Update(GameStateManager &manager, float deltaTime)
{
	
}
void TileLayer::UpdatePhysics(GameStateManager &manager, float fixedDeltaTime)
{
	//animate spheres
	for(unsigned int i = 0; i < mAnimations.size(); i++){
		mAnimations[i]->Animate(fixedDeltaTime);
	}
}


void TileLayer::Draw(GameStateManager &manager, sf::RenderWindow &gameWindow)
{

	Tile *tile;

	for (unsigned int y = 0; y < mTiles.size() ; y++){
		for(unsigned int x = 0; x < mTiles[y].size(); x++){
			if(mTiles[y][x] != NULL && mTiles[y][x]->mGID != 0){
				tile = mTiles[y][x];
				//move sprite
				mTileSprites[tile->mGID]->setPosition( (float)(x*mTileWidth) , (float)(y*mTileHeight) );
				//stamp it
				gameWindow.draw( (*mTileSprites[tile->mGID]) );
			}
		}
	}

}

sf::Vector2f TileLayer::GetLevelDimensions(){
	sf::Vector2f dim;

	dim = sf::Vector2f( (float)((int)mTiles[0].size() * mTileWidth) , (float)((int)mTiles.size() * mTileHeight) );

	return dim;
}

sf::Vector2f TileLayer::GetTileDimensions(){
	return sf::Vector2f((float)mTileWidth, (float)mTileHeight);
}

std::vector< std::vector<Tile*>> & TileLayer::GetTiles(){
	return mTiles;
}