#include "PlatformerGameState.h"
#include "SplashScreenGameState.h"
#include "Camera.h"
#include "GameStateManager.h"
#include "TileLayer.h"
#include "BackgroundLayer.h"
#include "Cloud.h"
#include "Utility.h"
#include "Hero.h"
#include "Crawler.h"
#include "Leader.h"

#include "InputManager.h"
#include "AssetManager.h"

#include <SFML\Graphics.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Rect.hpp>

#include "tinyxml2.h"
#include <iostream>
#include <string>



PlatformerGameState::PlatformerGameState(void) : mCamera(new Camera())
{
	mOpaque = true;
	mIsPaused = false;

}



PlatformerGameState::~PlatformerGameState(void)
{
	delete mCamera;
}



void PlatformerGameState::Init(GameStateManager &manager)
{
	mInputManager = InputManager::GetInstance();
	mInputManager->RegisterKey(sf::Keyboard::Escape); //to splash
	mInputManager->RegisterKey(sf::Keyboard::Return); //reset

	//load level 1
	LoadLevel("..\\Levels\\Level1.tmx");

	//load player
	mHero = new Hero();
	mHero->Load();

	//spawn hero into level
	//drop him in at top left corner
	mHero->SetPosition( sf::Vector2f(0, 0) );
	mHero->SetState( PS_IDLE );

	//make a camera
	mCamera->Init(sf::FloatRect(0 ,0.f, (float)manager.GetWindowWidth(), (float)manager.GetWindowHeight()));
	mCamera->MoveCamera( sf::Vector2f( mHero->GetPosition().x - (float)manager.GetWindowWidth()/2, 0.0f ) );
	Scroll( sf::Vector2f( mHero->GetPosition().x - (float)manager.GetWindowWidth()/2, 0.0f ) );
}



void PlatformerGameState::DeInit()
{
	//Delete Level
	UnLoadLevel();

	//Delete Hero
	mHero->UnLoad();

	//Delete Enimies
	//TODO

	delete mHero;
}



void PlatformerGameState::UnLoadLevel(){

	//Clear tiles
	mLevelTileLayer->Unload();
	delete mLevelTileLayer;

	//clear backgrounds
	for(unsigned int i = 0; i < mBackgrounds.size(); i++){
		mBackgrounds[i]->Unload();
		delete mBackgrounds[i];
	}
	mBackgrounds.clear();

	//clear Enemies
	for ( auto ge : mEnemies ){
		ge->Unload();
		delete ge;
	}
	mEnemies.clear();
}



void PlatformerGameState::LoadLevel( const std::string &levelfile ){
	using namespace tinyxml2;
	XMLDocument doc;

	if( doc.LoadFile(levelfile.c_str()) != 0 ){
		std::cout << "Failed to open level.tmx" << std::endl;
	}
	XMLElement * mapElement = doc.FirstChildElement("map");

//store all property elements
	// Property Name, PropertyValues
	std::map<std::string, std::string> props;

	XMLElement *mapPropertyElement = mapElement->FirstChildElement("properties")->FirstChildElement("property");
	while(mapPropertyElement != NULL){
		std::string name(mapPropertyElement->Attribute("name"));
		std::string value(mapPropertyElement->Attribute("value"));
		props[name] = value;
		mapPropertyElement = mapPropertyElement->NextSiblingElement("property");
	}

//Load background layers
	// Check to see if we have backgrounds
	// backgrounds are seporated by ';'
	// varaibles seporated by ':'
	// textures are seporated by ','
	// Variables:
	//		layer number = draw order
	//		ratio = scroll speed relative to camera scroll speed
	//		textures = list of possible textures for the layer
	// 
	// Ex:// the gradient and foreground only
	// "0:0:Gradient.png;1:0.6:Mountains_Fore_1.png,Mountains_Fore_2.png,Mountains_Fore_3.png"
	if( props.find("backgrounds") != props.end() ){
		std::vector<std::string> backgrounds;
		//Split to find how many backgrounds we have
		Utility::Split( props["backgrounds"], backgrounds, ';');

		int numBGL = (int)backgrounds.size();
		mBackgrounds.resize( numBGL, NULL );

		//Parse each individual layer
		//for each( std::string background in backgrounds ){
		for ( auto background : backgrounds ){
			BackgroundLayer *bgl;
			std::vector<std::string> variables;
			Utility::Split( background, variables, ':' );

			//make sure we have all he variables
			if (variables.size() != 3){
				std::cout << "Bad layer variables, format = \"layer#:ratio:texture1.png,textureN.png\" inputgiven: " << background << std::endl;
				continue;
			}

			// 0 = layer#, 1 = ratio, 2 = texture list
			bgl = new BackgroundLayer( variables[2], Utility::Convert<float>(variables[1]) );
			int layerNum = Utility::Convert<int>(variables[0]);

			mBackgrounds[layerNum] = bgl;
		}//end individual layer
	}//end background layers

//clouds
	//TODO

//Load Tiles	
	XMLElement *layerElement = mapElement->FirstChildElement("layer");
	while(layerElement != NULL){
		//parse the element and check name, etc..
		std::string layerName(layerElement->Attribute("name"));

		if(layerName == "Tiles"){
			mLevelTileLayer = new TileLayer();
			mLevelTileLayer->Load( mapElement, "Tiles" );

		}else if(layerName == "Enemies"){
			//load enemies
		}

		layerElement = layerElement->NextSiblingElement("layer");
	}

//generate backgrounds
	sf::Vector2f levelsize;
	levelsize = mLevelTileLayer->GetLevelDimensions();
	for ( auto bgl : mBackgrounds){
		bgl->Load( levelsize );
	}

//Load enemies
	//HACKED IN
	mEnemies.push_back( new Crawler() );
	mEnemies[0]->Load();
	mEnemies[0]->SetPosition( sf::Vector2f( 896, 384) ); //magic spawn points
	mEnemies[0]->SetState(ES_IDLE);

	mEnemies.push_back( new Leader() );
	mEnemies[1]->Load();
	mEnemies[1]->SetPosition( sf::Vector2f( 1536, 224) ); //magic spawn points
	mEnemies[1]->SetState(ES_IDLE);

	mEnemies.push_back( new Leader() );
	mEnemies[2]->Load();
	mEnemies[2]->SetPosition( sf::Vector2f( 480, 384) ); //magic spawn points
	mEnemies[2]->SetState(ES_RUNNING);

	mEnemies.push_back( new Crawler() );
	mEnemies[3]->Load();
	mEnemies[3]->SetPosition( sf::Vector2f( 1664, 384) ); //magic spawn points
	mEnemies[3]->SetState(ES_RUNNING);
}



void PlatformerGameState::ResetLevel(){
	std::cout << "Reseting Level" << std::endl;

	//reset hero
	sf::Vector2f oldpos = mHero->GetPosition();
	mHero->SetPosition( sf::Vector2f(0.0f,0.0f) );
	mHero->SetState( PS_IDLE );

	//reset enemies
	for (auto e : mEnemies ){
		//if they were dead or moving, reset em
		e->SetState(ES_IDLE);
		//reset positions
		//TODO they dont move yet so i cant even do that
	}
//****HACKED IN****
	//reset all my AI position
	mEnemies[0]->SetPosition( sf::Vector2f( 896, 384) ); //magic number
	mEnemies[0]->SetState(ES_IDLE);

	mEnemies[1]->SetPosition( sf::Vector2f( 1536, 224) ); //magic number
	mEnemies[1]->SetState(ES_IDLE);

	mEnemies[2]->SetPosition( sf::Vector2f( 480, 384) ); //magic number
	mEnemies[2]->SetState(ES_RUNNING);

	mEnemies[3]->SetPosition( sf::Vector2f( 1664, 384) ); //magic number
	mEnemies[3]->SetState(ES_RUNNING);
//END HACKED IN

	//reset scroll
	sf::Vector2f movement = sf::Vector2f(0.0,0.0) - oldpos;
	movement.y = 0;

	//background layers
	Scroll( movement );

	//camera
	mCamera->MoveCamera( movement );
}



void PlatformerGameState::Pause()
{
	mIsPaused = true;
}



void PlatformerGameState::Resume(){
	mIsPaused = false;
}



void PlatformerGameState::UpdateState(GameStateManager &manager, float delta)
{
	if( !mIsPaused ){
		//Handle input

		//Enter - Reset Level
		if( mInputManager->KeyPressed(sf::Keyboard::Return) ){
			ResetLevel();
		}

		//Call update on wolrd, player and enemies

		//TileLayer Update (doesnt do anything)
		//mLevelTileLayer->Update( manager, delta);


		//Player update (movement)
		if(mHero != NULL){
			mHero->Update(manager, delta);
		}

		//Enemy update (movement)
		for ( auto ge : mEnemies ){
			//ge->Update(manager, delta);
		}
	}
}



void PlatformerGameState::UpdatePhysics(GameStateManager &manager, float fixedDeltaTime)
{
	//TileLayer update 
	if(mLevelTileLayer != NULL ){
		mLevelTileLayer->UpdatePhysics(manager, fixedDeltaTime);
	}

	//Player physics update 
	if(mHero != NULL){
		//mHero->UpdatePhysics(manager, fixedDeltaTime );
	}

	//AI physics update
	for ( auto ge : mEnemies ){
		//ge->UpdatePhysics(manager, fixedDeltaTime);
	}

	//Camera update
	if( mCamera != NULL){
		//mCamera->MoveCamera( sf::Vector2f( mHero->GetPosition().x - mCamera->GetView().getCenter().x, 0.0f) );
	}
}


//Rectangle1 intersection vs Rectangle 2
//returns true/false and Overlap rectangle
bool AABBintersect(sf::FloatRect r1, sf::FloatRect r2, sf::FloatRect &overlap){
	//intersection true/false
	if( r1.left < r2.left + r2.width &&
		r1.left + r1.width > r2.left &&
		r1.top < r2.top + r2.height &&
		r1.top + r1.height > r2.height )
	{
		//overlap
		overlap.left = std::max( r1.left, r2.left ); 
		overlap.top = std::max( r1.top, r2.top );

		overlap.width = std::min( r1.left + r1.width, r2.left + r2.width ) - overlap.left;
		overlap.height = std::min( r1.top + r1.height, r2.top + r2.height ) - overlap.top;

		return true;
	}

	return false;
}



//Moves the GameEntity in the level based on the magnitudes given
//Does collision detection
//
void PlatformerGameState::MoveEntity( sf::Vector2f movement, GameEntity *gameEnt ){
	
	//Project AABB along movement vector
	sf::FloatRect newPos = gameEnt->GetBoundingBox();
	newPos.left += movement.x;
	newPos.top += movement.y;

	//Check vs Tiles
	//Calculate sweep area
	sf::Vector2f tileDims = mLevelTileLayer->GetLevelDimensions();
	
	int x = (int)floor( newPos.left / tileDims.x );
	int y = (int)floor( newPos.top / tileDims.y );
	int x2 = (int)ceil( (newPos.left + newPos.width) / tileDims.x );
	int y2 = (int)ceil( (newPos.top + newPos.height) / tileDims.y );

	std::vector< std::vector<Tile*>> world = mLevelTileLayer->GetTiles();
	
	for( y ; y < y2 && y < world.size() ; y++ ){
		for( x ; x < x2 && x < world[y].size() ; x++ ){
			//Null tile and Air check
			if( world[y][x] == NULL || world[y][x]->mGID == 0 ){
				continue;
			}

			//Platform Check
			if( world[y][x]->mGID == 16){
				//ignore for now
			}
			//Anythign else that isnt air
			else{
				//Intersection Test
				sf::FloatRect tileAABB = sf::FloatRect( x * tileDims.x, y * tileDims.y, tileDims.x, tileDims.y );
				sf::FloatRect overlap = sf::FloatRect();

				if( AABBintersect( newPos, tileAABB, overlap ) == true ){
					//pushback
					//movement.x = 0;
					//movement.y = 0;
				}
			}
		}
	}
	

	sf::Vector2f moveTo = sf::Vector2f( newPos.left, newPos.top );
	//sf::Vector2f moveTo = gameEnt->GetPosition();
	//moveTo.x += movement.x;
	//moveTo.y += movement.y;

	gameEnt->SetPosition( moveTo );
}



//Check if we are grounded
//Do a collision check on rightbelow the GameEntity
//Creates a bounding box below the bounding box (5px)
//Calculates what tiles this new bounding box is over and does an intersection test
bool PlatformerGameState::IsGrounded( GameEntity *ent){

	//Get tiles
	std::vector< std::vector<Tile*>> world = mLevelTileLayer->GetTiles();

	//Create a bounding box for the GameEntity's feet
	sf::FloatRect wbb;
	sf::FloatRect pbb = ent->GetBoundingBox(); //in world coordinates

	//Set the bounding box to be at his feet
	pbb.top += pbb.height;
	//Add set the height to how far we want to check under him for collisions
	pbb.height = 5; //Magic number: we check px below him for ground

	int x,y;
	sf::Vector2f tileDims = mLevelTileLayer->GetTileDimensions();

	//Calculate section of tiles that need to be checked
	y = (int)( pbb.top / tileDims.y );
	x = (int)( pbb.left / tileDims.x );

	float posx = x * tileDims.x;

	while ( posx <= pbb.left + pbb.width ){
		//anythign but open air
		//Bounds check for Tile access
		if (( y >= 0 && y < mLevelTileLayer->GetLevelDimensions().y / tileDims.y ) &&
			( x >= 0 && x < mLevelTileLayer->GetLevelDimensions().x / tileDims.x))
		{
			if( world[y][x] != NULL && world[y][x]->mGID != 0 ){
				wbb = sf::FloatRect( x*tileDims.x, y*tileDims.y , tileDims.x, tileDims.y );
				//are we grounded?
				if(wbb.intersects(pbb)){
					return true;
				}
			}
			//advance to next tile
			x++;
			posx = x * tileDims.x;

		}else{
			//advance to next tile
			x++;
			posx = x * tileDims.x;
		}
	}
	return false;
}



void PlatformerGameState::Scroll(sf::Vector2f displacement){
	//scroll backgrounds
	for  ( auto bgl : mBackgrounds ){
		bgl->Scroll( displacement );
	}

	//scroll clouds
}



void PlatformerGameState::Draw(GameStateManager &manager, sf::RenderWindow &gameWindow)
{
	if(mOpaque){
		gameWindow.clear(sf::Color::Black);
	}

	gameWindow.setView(mCamera->GetView());

	//draw background layers
	for(unsigned int i = 0; i < mBackgrounds.size(); i++){
		mBackgrounds[i]->Draw(manager, gameWindow);
	}

	//draw clounds
	for(unsigned int i = 0; i < mClouds.size(); i++){
		mClouds[i]->Draw(manager, gameWindow);
	}

	//draw tile layer
	mLevelTileLayer->Draw(manager, gameWindow);
	

	//draw entities
	for ( auto ge : mEnemies ){
		ge->Draw(manager, gameWindow);
	}

	//draw player
	mHero->Draw(manager, gameWindow);
}

