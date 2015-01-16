#include "AssetManager.h"


AssetManager::AssetManager(void){
	//load default texture, if one exists
}


AssetManager::~AssetManager(void){
	//unload all textures
		//iterate over the map
	for( std::map<std::string, sf::Texture*>::iterator it = mAssets.begin(); it!= mAssets.end(); it++){
		delete it->second;
	}
	mAssets.clear();
}

AssetManager * AssetManager::GetInstance(){
	static AssetManager instance;
	return &instance;
}


sf::Texture* AssetManager::GetTexture(std::string& filename){
//	mAssets.find(filename);

	//check for already loaded texture "filename"
	sf::Texture *tex = mAssets[filename];
	if( tex == NULL ){
		tex = new sf::Texture();
		//try and load it
		if (!tex->loadFromFile(filename)){
			delete tex;
			return NULL;
		}else{
			mAssets[filename] = tex;
		}
	}

	return tex;
}
