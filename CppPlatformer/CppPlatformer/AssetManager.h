//#pragma once

#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include <string>
//#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>

class AssetManager{

public:
	
	~AssetManager(void);
	//Pass String by reference
	sf::Texture* GetTexture(std::string& filename);

	static AssetManager * GetInstance();

private:
	std::map<std::string, sf::Texture*> mAssets;

	AssetManager(void);
	AssetManager(const AssetManager &am);
};



#endif