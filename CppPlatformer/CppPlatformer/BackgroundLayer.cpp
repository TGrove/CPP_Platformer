#include "BackgroundLayer.h"
#include "AssetManager.h"
#include "GameStateManager.h"
#include "Utility.h"

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <random>
#include <iostream>

BackgroundLayer::BackgroundLayer( std::string &texture,  float scrollRatio ) :
	mScrollSpeedRatio(scrollRatio)
{
	mAssetManager = AssetManager::GetInstance();
	
	//LoadAllTextures
	Utility::Split(texture, mTextures, ',');
	
}



BackgroundLayer::~BackgroundLayer(void)
{
	
}

void BackgroundLayer::Unload(){
	//Delete Sprites

	for each( sf::Sprite * sp in mSprites ){
		delete sp;
	}
	mSprites.clear();

	//unload textures from assetmanager
}

void BackgroundLayer::Load( sf::Vector2f leveldims ){
	srand ( (unsigned int)time(NULL) );

	std::vector<sf::Texture *> textures;

	//Get all avaliable textures
	for each ( std::string s in mTextures ){
		sf::Texture *tex = mAssetManager->GetTexture( s );
		if(tex != NULL){
			textures.push_back(tex);
			std::cout << "loaded " << s << std::endl;
		}else{
			std::cout << "Failed to load " << s << std::endl;
		}
	}

	// Check that we have textures
	if(textures.size() == 0){
		//can't make a background without textures
		return;
	}

	//Target size
	//Make up for the fact that we are scrolling backgroundlayers relative to the view
	sf::Vector2f targetSize = sf::Vector2f( leveldims.x / (1+ mScrollSpeedRatio), 600 ); //600 ~size of window)

	sf::Vector2f currentSize = sf::Vector2f(0.0f,0.0f);

	std::cout << "TargetSize: " << targetSize.x << " scrollSpeedRatio: " << mScrollSpeedRatio << std::endl; 


	//String together textures until we have reached targer size
	//Starting from 0,0 and goes to <= level_size * scroll_speed_ratio
	while(currentSize.x < targetSize.x){
		
		//grab a random texture from our list of availaibles
		int rIndex = rand() % (int)textures.size();


		sf::Sprite *sp = new sf::Sprite( *textures[rIndex] );
		
		//set flush to bottom of level
		currentSize.y = targetSize.y - textures[rIndex]->getSize().y ;

		sp->setPosition( currentSize );
		mSprites.push_back(sp);

		currentSize.x += textures[rIndex]->getSize().x;
		std::cout << "CurrentSize: " << currentSize.x << std::endl; 
	}

	std::cout << "bgl sprites: " << mSprites.size() << std::endl;

	//cleanup vector we made
	textures.clear();
}

void BackgroundLayer::Draw( GameStateManager &manager, sf::RenderWindow &gameWindow ){
	for each (sf::Sprite *sp in mSprites){
		//sf::Vector2f pos = sp->getPosition();
		//sp->setPosition( pos.x, gameWindow.getSize().y - (sp->getTexture()->getSize().y) ); 
		gameWindow.draw( *sp );
	}

}

void BackgroundLayer::Scroll(sf::Vector2f scrollAmount){

	//move each sprite in background strip
	for each (sf::Sprite *sp in mSprites){
		sf::Vector2f oldpos = sp->getPosition();
		sf::Vector2f newpos = sf::Vector2f( oldpos.x + (scrollAmount.x * (mScrollSpeedRatio) ), oldpos.y + scrollAmount.y);
		sp->setPosition( newpos );
	}

}

void BackgroundLayer::Reset(){
	if( mSprites.size() == 1 ){
		//Set to initial position
		sf::Vector2f oldpos = mSprites[0]->getPosition();
		sf::Vector2f newpos = sf::Vector2f( 0.0f , oldpos.y );
		mSprites[0]->setPosition( newpos );
	}
	else if( mSprites.size() > 1){
		//set first to initital position
		sf::Vector2f oldpos = mSprites[0]->getPosition();
		sf::Vector2f newpos = sf::Vector2f( 0.0f , oldpos.y );
		mSprites[0]->setPosition( newpos );

		//clamp the rest to the one before it
		for(int i = 1; i < (int)mSprites.size(); i++){
			oldpos = mSprites[i]->getPosition();
			newpos = sf::Vector2f( mSprites[i-1]->getPosition().x +  mSprites[i-1]->getLocalBounds().width , oldpos.y );
			mSprites[i]->setPosition( newpos );
		}

	}
}